#ifndef __GN_INPUT_GNINPUT_H__
#define __GN_INPUT_GNINPUT_H__
// *****************************************************************************
/// \file
/// \brief   Main header of input module
/// \author  chenlee (2005.10.24)
// *****************************************************************************

#include "GNbase.h"

///
/// Global input instance
///
#define gInput (::GN::input::Input::sGetInstance())

///
/// Pointer of global input instance
///
#define gInputPtr (::GN::input::Input::sGetInstancePtr())

namespace GN {
///
/// Namespace for input module
///
namespace input {
/// define keycode
#define GNINPUT_DEFINE_KEYCODE(name, dikey, scancode, vkeycode, xkeysym) name,

///
/// keycode type
///
struct KeyCode {
    enum Enum {
        ///
        /// 空码 ( normally indicate a error )
        ///
        NONE = 0,

#include "input/keyCodeMeta.h"

        ///
        /// number of available keycodes
        ///
        NUM_KEYS,

        ///
        /// first mouse button
        ///
        FIRST_MOUSE_BUTTON = MOUSEBTN_0,

        ///
        /// last mouse button
        ///
        LAST_MOUSE_BUTTON = MOUSEBTN_7,

        ///
        /// first xb360 button
        ///
        FIRST_XB360_BUTTON = XB360_UP,

        ///
        /// last xb360 button
        ///
        LAST_XB360_BUTTON = XB360_Y,
    };

    GN_DEFINE_ENUM_CLASS_HELPERS(KeyCode, Enum);
};

#undef GNINPUT_DEFINE_KEYCODE

///
/// axis type
///
struct Axis {
    enum Enum {
        NONE,          ///< ...
        MOUSE_X,       ///< mouse X
        MOUSE_Y,       ///< mouse Y
        MOUSE_WHEEL_0, ///< mouse wheel 0
        MOUSE_WHEEL_1, ///< mouse wheel 1

        XB360_LEFT_TRIGGER,  ///< xb360 left trigger
        XB360_RIGHT_TRIGGER, ///< xb360 right trigger
        XB360_THUMB_LX,      ///< xb360 left thumb X
        XB360_THUMB_LY,      ///< xb360 left thumb Y
        XB360_THUMB_RX,      ///< xb360 right thumb X
        XB360_THUMB_RY,      ///< xb360 right thumb Y

        NUM_AXISES, ///< number of axises.

        /// \name some aliases
        //@{
        MOUSE_FIRST = MOUSE_X,
        MOUSE_LAST  = MOUSE_WHEEL_1,
        XB360_FIRST = XB360_LEFT_TRIGGER,
        XB360_LAST  = XB360_THUMB_RY,
        //@}
    };

    GN_DEFINE_ENUM_CLASS_HELPERS(Axis, Enum);
};

// TODO: joystick support

///
/// 定义按键的状态, used by struct KeyEvent
///
struct KeyState {
    enum Enum {
        DOWN   = 1 << 0,
        LCTRL  = 1 << 1,
        RCTRL  = 1 << 2,
        LALT   = 1 << 3,
        RALT   = 1 << 4,
        LSHIFT = 1 << 5,
        RSHIFT = 1 << 6,
    };

    GN_DEFINE_ENUM_CLASS_HELPERS(KeyState, Enum);
};

//@{
// These are provided for easy construction of usual key event constants
#define GNINPUT_KEYDOWN(keycode) (keycode | (::GN::input::KeyState::DOWN << 8))
#define GNINPUT_KEYUP(keycode)   (keycode)
//@}

///
/// Key status structure
///
union KeyStatus {
    uint8 u8; ///< Key status as unsigned integer
    sint8 i8; ///< Key status as signed integer

    struct {
        bool down   : 1; ///< key down
        bool lctrl  : 1; ///< left ctrl down
        bool rctrl  : 1; ///< right ctrl down
        bool lalt   : 1; ///< left alt down
        bool ralt   : 1; ///< right alt down
        bool lshift : 1; ///< left shift down
        bool rshift : 1; ///< right shift down
    };

    ///
    /// Equality
    ///
    bool operator==(const KeyStatus & rhs) const { return u8 == rhs.u8; }

    ///
    /// Equality
    ///
    bool operator!=(const KeyStatus & rhs) const { return u8 != rhs.u8; }

    /// \name Misc functions
    //@{
    bool ctrlDown() const { return lctrl && rctrl; }
    bool altDown() const { return lalt && ralt; }
    bool shiftDown() const { return lshift && rshift; }
    //@}
};

///
/// key event structure
///
/// 一个keyevent_s实际上就是一个uint16_t类型的整数，
/// 其低8位是key code，高8位是key state
///
union KeyEvent {
    /// key event as unsigned 16bit integer
    uint16 u16;

    /// key event as signed 16bit integer
    sint16 i16;

    /// structured key states
    struct {
        uint8     code;   ///< Key code
        KeyStatus status; ///< Key status
    };

    /// \name constructor(s)
    //@{
    KeyEvent() {}
    KeyEvent(const KeyEvent & k): u16(k.u16) {}
    KeyEvent(KeyCode kc, KeyStatus ks): code(static_cast<uint8>(kc)), status(ks) { GN_ASSERT(kc < KeyCode::NUM_KEYS); }
    //@}

    ///
    /// Equality
    ///
    bool operator==(const KeyEvent & rhs) const { return u16 == rhs.u16; }

    ///
    /// Equality
    ///
    bool operator!=(const KeyEvent & rhs) const { return u16 != rhs.u16; }

    //@}

    ///
    /// set key data
    ///
    const KeyEvent & set(KeyCode kc, KeyStatus ks) {
        GN_ASSERT(kc < KeyCode::NUM_KEYS);
        code   = static_cast<uint8>(kc);
        status = ks;
        return *this;
    }
};

///
/// main interface of input module (singleton)
///
struct Input : public NoCopy {
    // ************************************************************************
    //      Singleton
    // ************************************************************************

    //@{

private:
    static GN_API Input * msInstancePtr; ///< 指向singleton的实例

protected:
    ///
    /// Constructor
    ///
    Input() {
        GN_ASSERT(0 == msInstancePtr);
        msInstancePtr = this;
    }

public:
    ///
    /// Destructor
    ///
    virtual ~Input() {
        GN_ASSERT(msInstancePtr);
        msInstancePtr = 0;
    }

    ///
    /// Get the instance
    ///
    static Input & sGetInstance() {
        GN_ASSERT(msInstancePtr);
        return *msInstancePtr;
    }

    ///
    /// Get the instance pointer (might be NULL)
    ///
    static Input * sGetInstancePtr() { return msInstancePtr; }

    // ************************************************************************
    //      input sinals
    // ************************************************************************

    //@{

    ///
    /// triggered when keyboard, mouse button, joystick button was pressed/released.
    ///
    Signal1<void, KeyEvent> sigKeyPress;

    ///
    /// triggered when a valid unicode character was generated by user input
    ///
    Signal1<void, wchar_t> sigCharPress;

    ///
    /// triggered when mouse/wheel/joystick moving/scrolling
    ///
    /// \note  参数2中返回的是相对位移
    ///
    Signal2<void, Axis, int> sigAxisMove;

    //@}

    // ************************************************************************
    //      main interface functions
    // ************************************************************************

    ///
    /// Must call this function before you can retrieve input events from this interface.
    ///
    /// \Note parameters displayHandle is only used on X11 platform.
    ///
    virtual bool attachToWindow(intptr_t displayHandle, intptr_t windowHandle) = 0;

    ///
    /// 获取并处理最新的输入事件
    ///
    virtual void processInputEvents() = 0;

    ///
    /// get the latest key event from event queue, then pop it out of event queue.
    ///
    /// Return a empty event, if there's no more events in event queue.
    ///
    /// Note: the input device will buffer the most recent 32 key events.
    ///
    virtual KeyEvent popLastKeyEvent() = 0;

    ///
    /// Return keyboard status indexed by KeyCode.
    ///
    virtual const KeyStatus * getKeyboardStatus() const = 0;

    ///
    /// Get status of specific key
    ///
    const KeyStatus & getKeyStatus(KeyCode k) const {
        GN_ASSERT(0 <= k && k < KeyCode::NUM_KEYS);
        return getKeyboardStatus()[k];
    }

    ///
    /// Return absolute value of all axises.
    ///
    /// \note
    /// - Mouse positions are based left-up corner of the attached window.
    ///
    virtual const int * getAxisStatus() const = 0;

    ///
    /// 得到当前鼠标的位置（相对窗口的左上角）
    ///
    virtual void getMousePosition(int & x, int & y) const = 0;
};

///
/// Input API type
///
struct InputAPI {
    enum Enum {
        NATIVE,   ///< OS native API (Win32, X11, XInput)
        DINPUT,   ///< DirectInput
        FAKE,     ///< Fake
        NUM_APIs, ///< Number of available input APIs.
    };

    const char * toString() const {
        static const char * TABLE[] = {
            "NATIVE",
            "DINPUT",
            "FAKE",
        };

        const Enum & e = *this;
        if (0 <= e && e <= NUM_APIs)
            return TABLE[e];
        else
            return "INVALID_INPUT_API";
    }

    GN_DEFINE_ENUM_CLASS_HELPERS(InputAPI, Enum);
};

///
/// initialize of input system (shutdown previous input system automatically)
///
GN_API bool initializeInputSystem(InputAPI = InputAPI::NATIVE);

///
/// shutdown input system
///
GN_API void shutdownInputSystem();

///
/// convert string to keycode
///
/// \return 失败时返回 KeyCode::NONE
///
inline KeyCode str2kc(const char *);

///
/// convert keycode to string
///
/// \return  失败时返回空字串（注意，是空字串""，不是NULL，
///          你可以用 GN::str::isEmpty() 来检测返回值
///
inline const char * keyCode2String(int);

} // namespace input
} // namespace GN

#include "input/inputUtils.inl"

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_INPUT_GNINPUT_H__
