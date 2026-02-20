#include "vk-backbuffer.h"
#include "vk-resource-tracker.h"

static GN::Logger * sLogger = GN::getLogger("GN.rdg.vk");

namespace GN::rdg {

namespace {

vk::UniqueSurfaceKHR createSurfaceFromWindow(vk::Instance instance, GN::win::Window * win) {
    if (!win) return {};
#if GN_WINPC
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
    mGpuContext.set(static_cast<GpuContextVulkan *>(params.context.get()));
    mDescriptor = params.descriptor;

    auto         ctxVk = static_cast<GpuContextVulkan *>(params.context.get());
    const auto & inst  = ctxVk->instance();
    const auto & dev   = ctxVk->device();

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

Action::ExecutionResult BackbufferVulkan::prepare(SubmissionImpl & submission) {
    if (!mSwapchain.valid()) GN_UNLIKELY {
            GN_ERROR(sLogger)("BackbufferVulkan::prepare: swapchain not initialized");
            return Action::ExecutionResult::FAILED;
        }
    // Retrieve or create the FrameExecutionContext for this submission
    auto & ctx = submission.ensureExecutionContext<FrameExecutionContextVulkan>();
    if (ctx.bb2frame.find(this->sequence) != ctx.bb2frame.end()) GN_UNLIKELY {
            GN_WARN(sLogger)("BackbufferVulkan::prepare: already prepared. Redundant call is ignored.");
            return Action::ExecutionResult::WARNING;
        }
    // Call beginFrame and store the mapping from 'this' backbuffer to the frame pointer
    GN_VERBOSE(sLogger)("BackbufferVulkan::prepare: beginFrame");
    auto frame = mSwapchain->beginFrame();
    if (!frame) GN_UNLIKELY {
            GN_ERROR(sLogger)("BackbufferVulkan::prepare: beginFrame failed");
            return Action::ExecutionResult::FAILED;
        }
    ctx.bb2frame[this->sequence] = frame;

    // Call resource tracker to set the intial state of the backbuffer image.
    auto & rt = submission.ensureExecutionContext<ResourceTrackerVulkan>(
        ResourceTrackerVulkan::ConstructParameters {.submission = submission, .gpu = mGpuContext.castTo<GpuContextVulkan>()});
    // set initial state to undefined, meaning backbuffer content has been discarded at the beginning of the frame.
    auto state = ResourceTrackerVulkan::ImageState {vk::ImageLayout::eUndefined, vk::AccessFlagBits::eNone, vk::PipelineStageFlagBits::eBottomOfPipe};
    rt.setImageState(frame->backbuffer().image->handle(), 0, 0, state);

    // done
    return Action::ExecutionResult::PASSED;
}

Action::ExecutionResult BackbufferVulkan::present(SubmissionImpl & submission) {
    if (!mSwapchain.valid()) GN_UNLIKELY {
            GN_ERROR(sLogger)("BackbufferVulkan::present: swapchain not initialized");
            return Action::ExecutionResult::FAILED;
        }

    // Retrieve or create the FrameExecutionContext for this submission
    auto & ctx       = submission.ensureExecutionContext<FrameExecutionContextVulkan>();
    auto   frameIter = ctx.bb2frame.find(this->sequence);
    if (frameIter == ctx.bb2frame.end()) GN_UNLIKELY {
            GN_ERROR(sLogger)("BackbufferVulkan::present: frame is not prepared yet. Call prepare() first.");
            return Action::ExecutionResult::FAILED;
        }

    // Call resource tracker to get the final state of the backbuffer image.
    auto rt = submission.getExecutionContext<ResourceTrackerVulkan>();
    if (!rt) GN_UNLIKELY {
            GN_ERROR(sLogger)("BackbufferVulkan::present: ResourceTrackerVulkan not found. This is most likely due to rendering to a unprepared backbuffer.");
            return Action::ExecutionResult::FAILED;
        }
    auto frame = frameIter->second;
    auto state = rt->queryImageState(frame->backbuffer().image->handle(), 0, 0);
    if (!state) GN_UNLIKELY {
            GN_ERROR(sLogger)("BackbufferVulkan::present: backbuffer state not found. This is most likely due to rendering to a unprepared backbuffer.");
            return Action::ExecutionResult::FAILED;
        }

    // This frame is about to end. So we need to remove this frame from the frame buffer manager.
    ctx.bb2frame.erase(this->sequence);

    // Call present to end the frame.
    GN_VERBOSE(sLogger)("BackbufferVulkan::present: present frame");
    auto newState = mSwapchain->present(
        rapid_vulkan::Swapchain::PresentParameters(rapid_vulkan::Swapchain::BackbufferStatus {state->curr.layout, state->curr.access, state->curr.stages}));
    rt->setImageState(frame->backbuffer().image->handle(), 0, 0, {newState.layout, newState.access, newState.stages});

    // done
    return Action::ExecutionResult::PASSED;
}

// =============================================================================
// createVulkanBackbuffer - API-specific factory
// =============================================================================

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
