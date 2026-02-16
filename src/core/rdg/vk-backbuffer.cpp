#if defined(_WIN32)
    #ifndef NOMINMAX
        #define NOMINMAX 1
    #endif
    #define VK_USE_PLATFORM_WIN32_KHR 1
#endif

#include "vk-backbuffer.h"

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

void BackbufferVulkan::prepare() {
    if (mSwapchain.valid()) mSwapchain->beginFrame();
}

void BackbufferVulkan::present() {
    if (mSwapchain.valid()) {
        rapid_vulkan::Swapchain::PresentParameters pp;
        mSwapchain->present(pp);
    }
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
