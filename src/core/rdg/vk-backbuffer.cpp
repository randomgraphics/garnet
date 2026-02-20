#include "pch.h"
#include "vk-backbuffer.h"

static GN::Logger * sLogger = GN::getLogger("GN.rdg.vk");

namespace GN::rdg {

namespace {

vk::UniqueSurfaceKHR createSurfaceFromWindow(vk::Instance instance, GN::win::Window * win) {
    if (!win) return {};
#if GN_MSWIN
    vk::Win32SurfaceCreateInfoKHR info = {};
    info.hinstance                     = reinterpret_cast<HINSTANCE>(win->getModuleHandle());
    info.hwnd                          = reinterpret_cast<HWND>(win->getWindowHandle());
    return instance.createWin32SurfaceKHRUnique(info);
#elif GN_LINUX
    vk::WaylandSurfaceCreateInfoKHR info = {};
    info.display                         = reinterpret_cast<wl_display *>(win->getDisplayHandle());
    info.surface                         = reinterpret_cast<wl_surface *>(win->getWindowHandle());
    return instance.createWaylandSurfaceKHRUnique(info);
#else
    (void) instance;
    (void) win;
    GN_ERROR(sLogger)("createSurfaceFromWindow: not implemented for this platform");
    return {};
#endif
}

} // namespace

// =============================================================================
// BackbufferVulkan - constructor and init
// =============================================================================

BackbufferVulkan::BackbufferVulkan(ArtifactDatabase & db, const StrA & name): BackbufferCommon(db, name) {
    if (0 == sequence) { GN_ERROR(sLogger)("BackbufferVulkan::BackbufferVulkan: duplicate type+name, name='{}'", name); }
}

BackbufferVulkan::~BackbufferVulkan() { GN_INFO(sLogger)("Destorying Vulkan backbuffer, name='{}'", name); }

bool BackbufferVulkan::init(const Backbuffer::CreateParameters & params) {
    if (0 == sequence) return false;

    // store GPU context and descriptor.
    mGpuContext = params.context.castTo<GpuContextVulkan>();
    mDescriptor = params.descriptor;

    const auto & inst = mGpuContext->instance();
    const auto & dev  = mGpuContext->device();

    size_t w = mDescriptor.width;
    size_t h = mDescriptor.height;
    if (mDescriptor.win && (w == 0 || h == 0)) {
        auto sz = mDescriptor.win->getClientSize();
        if (w == 0) w = sz.x;
        if (h == 0) h = sz.y;
    }
    if (w == 0 || h == 0) {
        GN_ERROR(sLogger)("BackbufferVulkan::init: invalid dimensions {}x{}, name='{}'", w, h, name);
        return false;
    }

    vk::SurfaceKHR surfaceHandle = {};
    if (mDescriptor.win) {
        mSurface = createSurfaceFromWindow(inst.handle(), mDescriptor.win);
        if (mSurface) surfaceHandle = mSurface.get();
    }

    rapid_vulkan::Swapchain::ConstructParameters scp;
    scp.setDevice(dev);
    scp.setDimensions(w, h);
    if (surfaceHandle) scp.setSurface(surfaceHandle);
    scp.depthStencilFormat.mode = rapid_vulkan::Swapchain::DepthStencilFormat::DISABLED; // do not automatically create depth buffer

    try {
        mSwapchain = rapid_vulkan::Ref<rapid_vulkan::Swapchain>::make(scp);
    } catch (const std::exception & e) {
        GN_ERROR(sLogger)("BackbufferVulkan::init: Swapchain creation failed: {}, name='{}'", e.what(), name);
        return false;
    }

    return true;
}

gfx::img::Image BackbufferVulkan::readback() const {
    // TODO: read back the backbuffer content into an image
    GN_ERROR(sLogger)("BackbufferVulkan::readback: not implemented");
    return gfx::img::Image();
}

Action::ExecutionResult BackbufferVulkan::prepare(SubmissionImpl &) {
    if (!mSwapchain.valid()) GN_UNLIKELY {
            GN_ERROR(sLogger)("BackbufferVulkan::prepare: swapchain not initialized");
            return Action::ExecutionResult::FAILED;
        }
    // Check if the backbuffer is already prepared.
    if (mActiveFrame) GN_UNLIKELY {
            GN_VERBOSE(sLogger)("BackbufferVulkan::prepare: already prepared. Redundant call is ignored.");
            return Action::ExecutionResult::WARNING;
        }

    // Call beginFrame and store the frame pointer.
    GN_VERBOSE(sLogger)("BackbufferVulkan::prepare: beginFrame");
    mActiveFrame = mSwapchain->beginFrame();
    if (!mActiveFrame) GN_UNLIKELY {
            GN_ERROR(sLogger)("BackbufferVulkan::prepare: beginFrame failed");
            return Action::ExecutionResult::FAILED;
        }

    // Update backbuffer state (transition needed when frame is new).
    trackImageState(TextureVulkan::ImageState::UNDEFINED());

    // Update the pending semaphores.
    mPendingSemaphores.clear();
    mPendingSemaphores.append(mActiveFrame->imageAvailable());

    // done
    return Action::ExecutionResult::PASSED;
}

Action::ExecutionResult BackbufferVulkan::present(SubmissionImpl &) {
    if (!mSwapchain.valid()) GN_UNLIKELY {
            GN_ERROR(sLogger)("BackbufferVulkan::present: swapchain not initialized");
            return Action::ExecutionResult::FAILED;
        }

    // Check if the backbuffer is already prepared. If not, ignore this present call.
    if (!mActiveFrame) GN_UNLIKELY {
            GN_VERBOSE(sLogger)("BackbufferVulkan::present: ignore. The backbuffer is not prepared yet.");
            return Action::ExecutionResult::WARNING;
        }

    // Call present() and update the image state to post-present layout.
    GN_VERBOSE(sLogger)("BackbufferVulkan::present: present frame");
    auto pp = rapid_vulkan::Swapchain::PresentParameters(
        rapid_vulkan::Swapchain::BackbufferStatus {mBackbufferState.curr.layout, mBackbufferState.curr.access, mBackbufferState.curr.stages});
    pp.setRenderFinished(vk::ArrayProxy<vk::Semaphore>((uint32_t) mPendingSemaphores.size(), mPendingSemaphores.data()));
    auto newStatus = mSwapchain->present(pp);
    trackImageState({newStatus.layout, newStatus.access, newStatus.stages});

    // We are done. Close the frame
    mActiveFrame = nullptr;
    return Action::ExecutionResult::PASSED;
}

// =============================================================================
// createVulkanBackbuffer - API-specific factory
// =============================================================================

bool BackbufferVulkan::trackImageState(const TextureVulkan::ImageState & newState, TextureVulkan::ImageStateTransitionFlags flags) {
    if (mBackbufferState.curr == newState) return false;
    mBackbufferState.transitTo(newState, flags);
    return true;
}

AutoRef<Backbuffer> createVulkanBackbuffer(ArtifactDatabase & db, const StrA & name, const Backbuffer::CreateParameters & params) {
    auto * p = new BackbufferVulkan(db, name);
    if (p->sequence == 0) {
        GN_ERROR(sLogger)("createVulkanBackbuffer: duplicate type+name, name='{}'", name);
        delete p;
        return {};
    }
    if (!p->init(params)) {
        delete p;
        return {};
    }
    return AutoRef<Backbuffer>(p);
}

} // namespace GN::rdg
