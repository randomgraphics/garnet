#ifndef __GN_WIN_WINDOW_GLFW_H__
#define __GN_WIN_WINDOW_GLFW_H__
// *****************************************************************************
/// \file
/// \brief   Window class using GLFW (cross-platform).
/// \author  Garnet
// *****************************************************************************

#if HAS_GLFW

struct GLFWwindow;
struct GLFWmonitor;
    #include <unordered_map>

    #if HAS_VULKAN
        #include <vulkan/vulkan.h>
    #endif
    #include <GLFW/glfw3.h>

namespace GN {
namespace win {
///
/// Window implementation using GLFW.
///
class WindowGlfw : public Window, public StdClass {
    GN_DECLARE_STDCLASS(WindowGlfw, StdClass);

    // ********************************
    // ctor/dtor
    // ********************************
public:
    WindowGlfw() { clear(); }
    ~WindowGlfw() override { quit(); }

    // ********************************
    // from StdClass
    // ********************************
public:
    bool init(const WindowCreateParameters &);
    bool init(const WindowAttachingParameters &);
    void quit() override;

private:
    void clear() {
        mWindow  = nullptr;
        mMonitor = nullptr;
        mClosing = false;
    #if HAS_VULKAN
        mVulkanSurfaces.clear();
    #endif
    }

    // ********************************
    // from Window
    // ********************************
public:
    intptr_t          getDisplayHandle() const override;
    intptr_t          getMonitorHandle() const override;
    intptr_t          getWindowHandle() const override;
    intptr_t          getModuleHandle() const override;
    intptr_t          getVulkanSurfaceHandle(intptr_t vulkanInstanceHandle) const override;
    Vector2<uint32_t> getClientSize() const override;
    void              show() override;
    void              hide() override;
    void              minimize() override;
    void              moveTo(int x, int y) override;
    void              setClientSize(size_t clientWidth, size_t clientHeight) override;
    bool              runUntilNoNewEvents(bool blockCallerWhenMinimized) override;

    // ********************************
    // private
    // ********************************
private:
    GLFWwindow *  mWindow;
    GLFWmonitor * mMonitor;
    bool          mClosing;
    bool          mOwned; ///< true if we created the window; false if attached (unsupported with GLFW)

    #if HAS_VULKAN
    struct VulkanSurfaceEntry {
        VkSurfaceKHR            surface    = {}; ///< created by glfwCreateWindowSurface
        PFN_vkDestroySurfaceKHR pfnDestroy = {}; ///< from glfwGetInstanceProcAddress; used in quit()
    };
    mutable std::unordered_map<intptr_t, VulkanSurfaceEntry> mVulkanSurfaces; ///< instance (intptr_t) -> entry; window owns, destroyed in quit()
    #endif

    bool createWindow(const WindowCreateParameters & wcp);
};
} // namespace win
} // namespace GN

#endif // HAS_GLFW

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_WIN_WINDOW_GLFW_H__
