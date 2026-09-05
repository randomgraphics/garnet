#include "pch.h"
#if HAS_GLFW
    #include "window-glfw.h"

static GN::Logger * sLogger = GN::getLogger("GN.win.GLFW");

static GN::win::KeyCode sTranslateGlfwKey(int key) {
    using GN::win::KeyCode;
    switch (key) {
    case GLFW_KEY_ESCAPE:
        return KeyCode::ESCAPE;
    case GLFW_KEY_BACKSPACE:
        return KeyCode::BACKSPACE;
    case GLFW_KEY_TAB:
        return KeyCode::TAB;
    case GLFW_KEY_CAPS_LOCK:
        return KeyCode::CAPSLOCK;
    case GLFW_KEY_ENTER:
        return KeyCode::RETURN;
    case GLFW_KEY_SPACE:
        return KeyCode::SPACEBAR;
    case GLFW_KEY_LEFT_SHIFT:
        return KeyCode::LSHIFT;
    case GLFW_KEY_RIGHT_SHIFT:
        return KeyCode::RSHIFT;
    case GLFW_KEY_LEFT_CONTROL:
        return KeyCode::LCTRL;
    case GLFW_KEY_RIGHT_CONTROL:
        return KeyCode::RCTRL;
    case GLFW_KEY_LEFT_ALT:
        return KeyCode::LALT;
    case GLFW_KEY_RIGHT_ALT:
        return KeyCode::RALT;
    case GLFW_KEY_PAGE_UP:
        return KeyCode::PAGEUP;
    case GLFW_KEY_PAGE_DOWN:
        return KeyCode::PAGEDOWN;
    case GLFW_KEY_END:
        return KeyCode::END;
    case GLFW_KEY_HOME:
        return KeyCode::HOME;
    case GLFW_KEY_INSERT:
        return KeyCode::INSERT;
    case GLFW_KEY_DELETE:
        return KeyCode::_DELETE;
    case GLFW_KEY_LEFT:
        return KeyCode::LEFT;
    case GLFW_KEY_UP:
        return KeyCode::UP;
    case GLFW_KEY_RIGHT:
        return KeyCode::RIGHT;
    case GLFW_KEY_DOWN:
        return KeyCode::DOWN;
    case GLFW_KEY_MINUS:
        return KeyCode::MINUS;
    case GLFW_KEY_EQUAL:
        return KeyCode::EQUALS;
    case GLFW_KEY_LEFT_BRACKET:
        return KeyCode::LBRACKET;
    case GLFW_KEY_RIGHT_BRACKET:
        return KeyCode::RBRACKET;
    case GLFW_KEY_SEMICOLON:
        return KeyCode::SEMICOLON;
    case GLFW_KEY_APOSTROPHE:
        return KeyCode::APOSTROPHE;
    case GLFW_KEY_GRAVE_ACCENT:
        return KeyCode::GRAVE;
    case GLFW_KEY_BACKSLASH:
        return KeyCode::BACKSLASH;
    case GLFW_KEY_COMMA:
        return KeyCode::COMMA;
    case GLFW_KEY_PERIOD:
        return KeyCode::PERIOD;
    case GLFW_KEY_SLASH:
        return KeyCode::SLASH;
    case GLFW_KEY_KP_MULTIPLY:
        return KeyCode::NUMPAD_MULTIPLY;
    case GLFW_KEY_KP_ADD:
        return KeyCode::NUMPAD_ADD;
    case GLFW_KEY_KP_SUBTRACT:
        return KeyCode::NUMPAD_SUBTRACT;
    case GLFW_KEY_KP_DECIMAL:
        return KeyCode::NUMPAD_DECIMAL;
    case GLFW_KEY_KP_DIVIDE:
        return KeyCode::NUMPAD_DIVIDE;
    case GLFW_KEY_KP_ENTER:
        return KeyCode::NUMPAD_ENTER;
    case GLFW_KEY_NUM_LOCK:
        return KeyCode::NUMPAD_NUMLOCK;
    case GLFW_KEY_PRINT_SCREEN:
        return KeyCode::SYSRQ;
    case GLFW_KEY_SCROLL_LOCK:
        return KeyCode::SCROLL;
    case GLFW_KEY_PAUSE:
        return KeyCode::PAUSE;
    default:
        break;
    }
    if (GLFW_KEY_0 <= key && key <= GLFW_KEY_9) return (KeyCode) ((int) KeyCode::_0 + key - GLFW_KEY_0);
    if (GLFW_KEY_A <= key && key <= GLFW_KEY_Z) return (KeyCode) ((int) KeyCode::A + key - GLFW_KEY_A);
    if (GLFW_KEY_KP_0 <= key && key <= GLFW_KEY_KP_9) return (KeyCode) ((int) KeyCode::NUMPAD_0 + key - GLFW_KEY_KP_0);
    if (GLFW_KEY_F1 <= key && key <= GLFW_KEY_F12) return (KeyCode) ((int) KeyCode::F1 + key - GLFW_KEY_F1);
    return KeyCode::NONE;
}

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
    GN_ERROR(sLogger, "attachToExistingWindow is not supported with the GLFW backend.");
    return failure();

    GN_UNGUARD;
}

void GN::win::WindowGlfw::quit() {
    GN_GUARD;

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

intptr_t GN::win::WindowGlfw::getMonitorHandle() const { return (intptr_t) mMonitor; }

intptr_t GN::win::WindowGlfw::getWindowHandle() const { return (intptr_t) mWindow; }

intptr_t GN::win::WindowGlfw::createVulkanSurfaceHandle(intptr_t vulkanInstanceHandle) const {
    #if GN_BUILD_HAS_VULKAN
    if (!mWindow) {
        GN_ERROR(sLogger, "createVulkanSurfaceHandle: window not created");
        return 0;
    }
    if (!vulkanInstanceHandle) return 0;
    VkSurfaceKHR surface = VK_NULL_HANDLE;
    VkResult     err     = glfwCreateWindowSurface((VkInstance) vulkanInstanceHandle, mWindow, nullptr, &surface);
    if (err != VK_SUCCESS || surface == VK_NULL_HANDLE) {
        GN_ERROR(sLogger, "createVulkanSurfaceHandle: glfwCreateWindowSurface failed");
        return 0;
    }
    return (intptr_t) (void *) surface;
    #else
    (void) vulkanInstanceHandle;
    GN_ERROR(sLogger, "createVulkanSurfaceHandle: Vulkan not supported in this build");
    return 0;
    #endif
}

void GN::win::WindowGlfw::destroyVulkanSurfaceHandle(intptr_t vulkanInstanceHandle, intptr_t vulkanSurfaceHandle) const {
    #if GN_BUILD_HAS_VULKAN
    if (!vulkanInstanceHandle || !vulkanSurfaceHandle) return;
    // The Vulkan loader is not linked; resolve the (platform-independent) destroy entry point
    // through GLFW's loader, the same one createVulkanSurfaceHandle() used to create it.
    auto pfn = (PFN_vkDestroySurfaceKHR) glfwGetInstanceProcAddress((VkInstance) vulkanInstanceHandle, "vkDestroySurfaceKHR");
    if (!pfn) {
        GN_ERROR(sLogger, "destroyVulkanSurfaceHandle: vkDestroySurfaceKHR not available; surface leaked");
        return;
    }
    pfn((VkInstance) vulkanInstanceHandle, (VkSurfaceKHR) (void *) vulkanSurfaceHandle, nullptr);
    #else
    (void) vulkanInstanceHandle;
    (void) vulkanSurfaceHandle;
    #endif
}

intptr_t GN::win::WindowGlfw::getModuleHandle() const { return (intptr_t) 1; }

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
        GN_ERROR(sLogger, "Parent window is not supported with GLFW.");
        return false;
    }

    // TODO: do it only once.
    if (glfwInit() != GLFW_TRUE) {
        GN_ERROR(sLogger, "glfwInit failed.");
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
    mWindow            = glfwCreateWindow(width, height, title, mon, nullptr);
    if (!mWindow) {
        GN_ERROR(sLogger, "glfwCreateWindow failed.");
        glfwTerminate();
        return false;
    }

    mClosing = false;
    glfwSetWindowUserPointer(mWindow, this);
    glfwSetWindowCloseCallback(mWindow, [](GLFWwindow * w) {
        void * p = glfwGetWindowUserPointer(w);
        if (p) static_cast<WindowGlfw *>(p)->mClosing = true;
    });
    glfwSetKeyCallback(mWindow, [](GLFWwindow * w, int key, int, int action, int) {
        if (GLFW_REPEAT == action) return;
        auto *        self = static_cast<WindowGlfw *>(glfwGetWindowUserPointer(w));
        const KeyCode code = sTranslateGlfwKey(key);
        if (self && KeyCode::NONE != code) self->notifyKeyPress(code, GLFW_PRESS == action);
    });
    glfwSetCharCallback(mWindow, [](GLFWwindow * w, unsigned int codepoint) {
        auto * self = static_cast<WindowGlfw *>(glfwGetWindowUserPointer(w));
        if (self) self->notifyCharPress((wchar_t) codepoint);
    });
    glfwSetMouseButtonCallback(mWindow, [](GLFWwindow * w, int button, int action, int) {
        auto * self = static_cast<WindowGlfw *>(glfwGetWindowUserPointer(w));
        if (!self || button < GLFW_MOUSE_BUTTON_1 || button > GLFW_MOUSE_BUTTON_8) return;
        self->notifyKeyPress((KeyCode) ((int) KeyCode::MOUSEBTN_0 + button - GLFW_MOUSE_BUTTON_1), GLFW_PRESS == action);
    });
    glfwSetCursorPosCallback(mWindow, [](GLFWwindow * w, double x, double y) {
        auto * self = static_cast<WindowGlfw *>(glfwGetWindowUserPointer(w));
        if (self) self->updateMousePosition((int) x, (int) y);
    });
    glfwSetScrollCallback(mWindow, [](GLFWwindow * w, double x, double y) {
        auto * self = static_cast<WindowGlfw *>(glfwGetWindowUserPointer(w));
        if (!self) return;
        self->notifyAxisMove(Axis::MOUSE_WHEEL_0, (int) (y * 120.0));
        self->notifyAxisMove(Axis::MOUSE_WHEEL_1, (int) (x * 120.0));
    });

    double mouseX = 0, mouseY = 0;
    glfwGetCursorPos(mWindow, &mouseX, &mouseY);
    updateMousePosition((int) mouseX, (int) mouseY, false);

    mMonitor = glfwGetWindowMonitor(mWindow);
    if (!mMonitor) mMonitor = glfwGetPrimaryMonitor();

    return true;

    GN_UNGUARD;
}

#endif // HAS_GLFW
