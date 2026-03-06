#include "pch.h"
#if HAS_GLFW
    #include "window-glfw.h"

static GN::Logger * sLogger = GN::getLogger("GN.win.GLFW");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

bool GN::win::WindowGlfw::init(const WindowCreateParameters & wcp) {
    GN_GUARD;

    GN_STDCLASS_INIT();

    if (!createWindow(wcp)) return failure();

    mOwned = true;
    return success();

    GN_UNGUARD;
}

bool GN::win::WindowGlfw::init(const WindowAttachingParameters &) {
    GN_GUARD;

    GN_STDCLASS_INIT();
    GN_ERROR(sLogger)("attachToExistingWindow is not supported with the GLFW backend.");
    return failure();

    GN_UNGUARD;
}

void GN::win::WindowGlfw::quit() {
    GN_GUARD;

#if HAS_VULKAN
    for (const auto & kv : mVulkanSurfaces) {
        if (kv.second.pfnDestroy && kv.second.surface)
            kv.second.pfnDestroy((VkInstance) kv.first, kv.second.surface, nullptr);
    }
    mVulkanSurfaces.clear();
#endif

    if (mWindow && mOwned) {
        glfwDestroyWindow(mWindow);
        mWindow = nullptr;
    }
    mMonitor = nullptr;

    // todo: terminates on process exit.
    glfwTerminate();

    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// Window interface
// *****************************************************************************

intptr_t GN::win::WindowGlfw::getDisplayHandle() const {
    return 0; // GLFW does not expose display handle; 0 means default.
}

intptr_t GN::win::WindowGlfw::getMonitorHandle() const {
    return (intptr_t) mMonitor;
}

intptr_t GN::win::WindowGlfw::getWindowHandle() const {
    return (intptr_t) mWindow;
}

intptr_t GN::win::WindowGlfw::getVulkanSurfaceHandle(intptr_t vulkanInstanceHandle) const {
#if HAS_VULKAN
    if (!mWindow) {
        GN_ERROR(sLogger)("getVulkanSurfaceHandle: window not created");
        return 0;
    }
    if (!vulkanInstanceHandle) return 0;
    auto it = mVulkanSurfaces.find(vulkanInstanceHandle);
    if (it != mVulkanSurfaces.end())
        return (intptr_t) (void *) it->second.surface;
    VkSurfaceKHR surface = VK_NULL_HANDLE;
    VkResult     err     = glfwCreateWindowSurface((VkInstance) vulkanInstanceHandle, mWindow, nullptr, &surface);
    if (err != VK_SUCCESS || surface == VK_NULL_HANDLE) {
        GN_ERROR(sLogger)("getVulkanSurfaceHandle: glfwCreateWindowSurface failed");
        return 0;
    }
    auto pfn = (PFN_vkDestroySurfaceKHR) glfwGetInstanceProcAddress((VkInstance) vulkanInstanceHandle, "vkDestroySurfaceKHR");
    if (!pfn) {
        GN_ERROR(sLogger)("getVulkanSurfaceHandle: vkDestroySurfaceKHR not available");
        return 0; // surface created above would leak; caller gets no handle
    }
    mVulkanSurfaces[vulkanInstanceHandle] = VulkanSurfaceEntry {surface, pfn};
    return (intptr_t) (void *) surface;
#else
    (void) vulkanInstanceHandle;
    GN_ERROR(sLogger)("getVulkanSurfaceHandle: Vulkan not supported in this build");
    return 0;
#endif
}

intptr_t GN::win::WindowGlfw::getModuleHandle() const {
    return (intptr_t) 1;
}

GN::Vector2<uint32_t> GN::win::WindowGlfw::getClientSize() const {
    Vector2<uint32_t> sz(0, 0);
    if (!mWindow) return sz;
    int w = 0, h = 0;
    glfwGetFramebufferSize(mWindow, &w, &h);
    sz.x = (uint32_t) (w > 0 ? w : 0);
    sz.y = (uint32_t) (h > 0 ? h : 0);
    return sz;
}

void GN::win::WindowGlfw::show() {
    if (mWindow) glfwShowWindow(mWindow);
}

void GN::win::WindowGlfw::hide() {
    if (mWindow) glfwHideWindow(mWindow);
}

void GN::win::WindowGlfw::minimize() {
    if (mWindow) glfwIconifyWindow(mWindow);
}

void GN::win::WindowGlfw::moveTo(int x, int y) {
    if (mWindow) glfwSetWindowPos(mWindow, x, y);
}

void GN::win::WindowGlfw::setClientSize(size_t w, size_t h) {
    if (mWindow) glfwSetWindowSize(mWindow, (int) w, (int) h);
}

bool GN::win::WindowGlfw::runUntilNoNewEvents(bool blockCallerWhenMinimized) {
    GN_GUARD_SLOW;
    if (!mWindow) return false;
    if (blockCallerWhenMinimized && glfwGetWindowAttrib(mWindow, GLFW_ICONIFIED))
        glfwWaitEvents();
    else
        glfwPollEvents();
    return !mClosing && !glfwWindowShouldClose(mWindow);
    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// Private
// *****************************************************************************

bool GN::win::WindowGlfw::createWindow(const WindowCreateParameters & wcp) {
    GN_GUARD;

    if (wcp.parent) {
        GN_ERROR(sLogger)("Parent window is not supported with GLFW.");
        return false;
    }

    // TODO: do it only once.
    if (glfwInit() != GLFW_TRUE) {
        GN_ERROR(sLogger)("glfwInit failed.");
        return false;
    }

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_DECORATED, wcp.hasBorder ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_FLOATING, wcp.topMost ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // no OpenGL context; we use Vulkan/D3D etc.

    int width  = (int) (wcp.clientWidth > 0 ? wcp.clientWidth : 640);
    int height = (int) (wcp.clientHeight > 0 ? wcp.clientHeight : 480);
    if (width <= 0) width = 640;
    if (height <= 0) height = 480;

    GLFWmonitor * mon = nullptr;
    if (wcp.monitor)
        mon = (GLFWmonitor *) wcp.monitor;
    else if (!wcp.hasBorder && !wcp.hasTitleBar)
        mon = glfwGetPrimaryMonitor();

    const char * title = wcp.caption.empty() ? "Garnet" : wcp.caption.c_str();
    mWindow = glfwCreateWindow(width, height, title, mon, nullptr);
    if (!mWindow) {
        GN_ERROR(sLogger)("glfwCreateWindow failed.");
        glfwTerminate();
        return false;
    }

    mClosing = false;
    glfwSetWindowUserPointer(mWindow, this);
    glfwSetWindowCloseCallback(mWindow, [](GLFWwindow * w) {
        void * p = glfwGetWindowUserPointer(w);
        if (p) static_cast<WindowGlfw *>(p)->mClosing = true;
    });

    mMonitor = glfwGetWindowMonitor(mWindow);
    if (!mMonitor) mMonitor = glfwGetPrimaryMonitor();

    return true;

    GN_UNGUARD;
}

#endif // HAS_GLFW
