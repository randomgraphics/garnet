#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX 1
#endif
#define VK_USE_PLATFORM_WIN32_KHR 1
#endif

#include "vk-backbuffer.h"
#if defined(_WIN32)
#include <vulkan/vulkan_win32.h>
#endif

static GN::Logger * sLogger = GN::getLogger("GN.rdg.vk");

namespace GN::rdg {

namespace {

vk::UniqueSurfaceKHR createSurfaceFromWindow(vk::Instance instance, GN::win::Window * win) {
#if GN_WINPC
    if (!win) return {};
    VkSurfaceKHR surf = {};
    VkWin32SurfaceCreateInfoKHR info = {};
    info.sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    info.hinstance = reinterpret_cast<HINSTANCE>(win->getModuleHandle());
    info.hwnd      = reinterpret_cast<HWND>(win->getWindowHandle());
    VkResult r     = vkCreateWin32SurfaceKHR(static_cast<VkInstance>(instance), &info, nullptr, &surf);
    if (r != VK_SUCCESS) return {};
    return vk::UniqueSurfaceKHR(
        vk::SurfaceKHR(surf),
        vk::ObjectDestroy(instance, nullptr, VULKAN_HPP_DEFAULT_DISPATCHER));
#else
    (void)instance;
    (void)win;
    GN_ERROR(sLogger)("createSurfaceFromWindow: not implemented for this platform");
    return {};
#endif
}

} // namespace

// =============================================================================
// BackbufferVulkan - constructor and init
// =============================================================================

BackbufferVulkan::BackbufferVulkan(ArtifactDatabase & db, const StrA & name)
    : Backbuffer(db, Backbuffer::TYPE, name) {
    if (0 == sequence) {
        GN_ERROR(sLogger)("BackbufferVulkan::BackbufferVulkan: duplicate type+name, name='{}'", name);
    }
}

bool BackbufferVulkan::init(const Backbuffer::CreateParameters & params) {
    if (0 == sequence) return false;

    mDescriptor = params.descriptor;
    auto * ctxVk = static_cast<GpuContextVulkan *>(params.context.get());
    rapid_vulkan::Device * dev = ctxVk->device();
    const rapid_vulkan::GlobalInfo * gi = ctxVk->globalInfo();
    if (!dev || !gi) {
        GN_ERROR(sLogger)("BackbufferVulkan::init: context has no Vulkan device/global info, name='{}'", name);
        return false;
    }

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
#if GN_WINPC
    if (mDescriptor.win) {
        mSurface = createSurfaceFromWindow(gi->instance, mDescriptor.win);
        if (mSurface) surfaceHandle = mSurface.get();
    }
#endif

    rapid_vulkan::Swapchain::ConstructParameters scp;
    scp.setDevice(*dev);
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
