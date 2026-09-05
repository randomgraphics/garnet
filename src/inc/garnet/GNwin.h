#ifndef __GN_WIN_GNWIN_H__
#define __GN_WIN_GNWIN_H__
// *****************************************************************************
/// \file
/// \brief   public interface of WIN module
/// \author  chenlee (2006.2.22)
// *****************************************************************************

#include "GNbase.h"

namespace GN {
///
/// Namespace for window module
///
namespace win {

/// Keyboard keys and mouse buttons reported by a Window.
#define GNWIN_DEFINE_KEYCODE(name, dikey, scancode, vkeycode, xkeysym) name,
enum class KeyCode : uint8_t {
    NONE = 0,
#include "win/key-code-meta.h"
    NUM_KEYS,
    FIRST_MOUSE_BUTTON = MOUSEBTN_0,
    LAST_MOUSE_BUTTON  = MOUSEBTN_7,
    FIRST_XB360_BUTTON = XB360_UP,
    LAST_XB360_BUTTON  = XB360_Y,
};
#undef GNWIN_DEFINE_KEYCODE

/// Window-relative pointer and wheel axes, plus retained controller axes.
enum class Axis : int {
    NONE = 0,
    MOUSE_X,
    MOUSE_Y,
    MOUSE_WHEEL_0,
    MOUSE_WHEEL_1,
    XB360_LEFT_TRIGGER,
    XB360_RIGHT_TRIGGER,
    XB360_THUMB_LX,
    XB360_THUMB_LY,
    XB360_THUMB_RX,
    XB360_THUMB_RY,
    NUM_AXISES,
    MOUSE_FIRST = MOUSE_X,
    MOUSE_LAST  = MOUSE_WHEEL_1,
    XB360_FIRST = XB360_LEFT_TRIGGER,
    XB360_LAST  = XB360_THUMB_RY,
};

/// State captured along with a key or mouse-button transition.
union KeyStatus {
    uint8_t u8 = 0;
    int8_t  i8;
    struct {
        bool down   : 1;
        bool lctrl  : 1;
        bool rctrl  : 1;
        bool lalt   : 1;
        bool ralt   : 1;
        bool lshift : 1;
        bool rshift : 1;
    };

    bool operator==(const KeyStatus & rhs) const { return u8 == rhs.u8; }
    bool operator!=(const KeyStatus & rhs) const { return u8 != rhs.u8; }
    bool ctrlDown() const { return lctrl || rctrl; }
    bool altDown() const { return lalt || ralt; }
    bool shiftDown() const { return lshift || rshift; }
};

/// A key or mouse-button transition and the modifier state at that transition.
struct KeyEvent {
    KeyCode   key    = KeyCode::NONE;
    KeyStatus status = {};

    KeyEvent() = default;
    KeyEvent(KeyCode keyCode, KeyStatus keyStatus): key(keyCode), status(keyStatus) {}
    KeyCode  code() const { return key; }
    explicit operator bool() const { return KeyCode::NONE != key; }
    bool     operator==(const KeyEvent &) const = default;
};

/// Convert a key name to its key code, or return KeyCode::NONE.
inline KeyCode str2kc(const char *);

/// Return the stable name of a key code, or an empty string for an invalid code.
inline const char * keyCode2String(int);

///
/// General window class
///
class GN_API Window : public NoCopy {
public:
    virtual ~Window() = default;

    // ********************************
    // public functions
    // ********************************
public:
    /// \name window properties
    //@{

    virtual intptr_t          getDisplayHandle() const = 0;
    virtual intptr_t          getMonitorHandle() const = 0;
    virtual intptr_t          getWindowHandle() const  = 0;
    virtual intptr_t          getModuleHandle() const  = 0;
    virtual Vector2<uint32_t> getClientSize() const    = 0;

    /// Creates a new VkSurfaceKHR for the given Vulkan instance (\p vulkanInstanceHandle is
    /// VkInstance cast to intptr_t) and returns it as intptr_t. The window keeps no reference
    /// to the result: ownership passes to the caller, who must pass it back to
    /// destroyVulkanSurfaceHandle() before the instance is destroyed. Returns 0 on failure or
    /// if unsupported. Each call creates a distinct surface.
    virtual intptr_t createVulkanSurfaceHandle(intptr_t vulkanInstanceHandle) const = 0;

    /// Destroys a VkSurfaceKHR previously returned by createVulkanSurfaceHandle() for the same
    /// Vulkan instance. Must be called after any swapchain using the surface is destroyed and
    /// while the instance is still alive. No-op if either handle is 0.
    virtual void destroyVulkanSurfaceHandle(intptr_t vulkanInstanceHandle, intptr_t vulkanSurfaceHandle) const = 0;

    //@}

    /// \name window manipulation
    //@{
    virtual void show()                                                 = 0;
    virtual void hide()                                                 = 0;
    virtual void minimize()                                             = 0;
    virtual void moveTo(int x, int y)                                   = 0;
    virtual void setClientSize(size_t clientWidth, size_t clientHeight) = 0;
    //@}

    /// \name window events
    //@{

    ///
    /// Run the application while there's events in event queue.
    ///
    /// Returns false when the Window system received request to quit the application.
    ///
    virtual bool runUntilNoNewEvents(bool blockCallerWhenMinimized = false) = 0;

    //@}

    /// Return the signal emitted for keyboard and mouse-button transitions.
    virtual Signal<void(KeyEvent)> & keyPressSignal() = 0;

    /// Return the signal emitted for Unicode text entered while this window has input focus.
    virtual Signal<void(wchar_t)> & charPressSignal() = 0;

    /// Return the signal emitted when a pointer or wheel axis changes.
    virtual Signal<void(Axis, int)> & axisMoveSignal() = 0;

    /// Pop the oldest buffered key transition, or return an empty event.
    virtual KeyEvent popLastKeyEvent() = 0;

    /// Return all key and mouse-button states, indexed by KeyCode.
    virtual const KeyStatus * getKeyboardStatus() const = 0;

    /// Return the state of one key or mouse button.
    virtual const KeyStatus & getKeyStatus(KeyCode key) const = 0;

    /// Return all absolute axis values, indexed by Axis.
    virtual const int * getAxisStatus() const = 0;

    /// Return the current pointer position relative to the client area's upper-left corner.
    virtual void getMousePosition(int & x, int & y) const = 0;
};

///
/// Window creation parameters
///
struct WindowCreateParameters {
    std::string caption;              ///< window title text.
    size_t      clientWidth  = 0;     ///< client width. 0 means current display width.
    size_t      clientHeight = 0;     ///< client height. 0 means current display height.
    intptr_t    display      = 0;     ///< display handle. X windows only. 0 means using default display handle.
    intptr_t    monitor      = 0;     ///< Monitor handle. 0 means default monitor.
    intptr_t    parent       = 0;     ///< Parent window.
    bool        hasBorder    = true;  ///< has border or not
    bool        hasTitleBar  = true;  ///< has title bar or not
    bool        topMost      = false; ///< top-most(always on top) or not
    bool        closebox     = true;  ///< close box is enabled.
};

struct WindowAttachingParameters {
    intptr_t display; ///< display handle. X windows only. Ignored on other platforms.
    intptr_t window;  ///< the external window handle.
};

///
/// Default parameters to create main application window.
///
/// border, title, no parent, default size
///
extern GN_API const WindowCreateParameters WCP_APPLICATION_WINDOW;

///
/// Default parameters to create main render window.
///
/// border, title, no parent, 640x480
///
extern GN_API const WindowCreateParameters WCP_WINDOWED_RENDER_WINDOW;

///
/// Default parameters to create main render window.
///
/// no border, no title, no parent, topmost, 640x480
///
extern GN_API const WindowCreateParameters WCP_FULLSCREEN_RENDER_WINDOW;

///
/// Create window instance
///
GN_API Window * createWindow(const WindowCreateParameters &);

///
/// Create a wrapper class on top of existing native window handle.
///
GN_API Window * attachToExistingWindow(const WindowAttachingParameters &);
} // namespace win
} // namespace GN

#include "win/input-utils.inl"

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_WIN_GNWIN_H__
