#ifndef __GN_INPUT_GNINPUT_H__
#define __GN_INPUT_GNINPUT_H__
// *****************************************************************************
//! \file    GNinput.h
//! \brief   Main header of input module
//! \author  chenlee (2005.10.24)
// *****************************************************************************

#include "GNbase.h"

//!
//! Global input instance
//!
#define gInput (::GN::input::Input::sGetInstance())

//!
//! Pointer of global input instance
//!
#define gInputPtr (::GN::input::Input::sGetInstancePtr())

namespace GN
{
    //!
    //! Namespace for input module
    //!
    namespace input
    {
        //! define keycode
        #define GNINPUT_DEFINE_KEYCODE( name, dikey, scancode, vkeycode, xkeysym ) name,

        //!
        //! keycode type
        //!
        enum KeyCode
        {
            //!
            //! 空码 ( normally indicate a error )
            //!
            KEY_NONE = 0,

            #include "input/keyCodeMeta.h"

            //!
            //! number of available keycodes
            //!
            NUM_KEYS,

            //!
            //! first mouse button
            //!
            KEY_MOUSEBTN_FIRST = KEY_MOUSEBTN_0,

            //!
            //! last mouse button
            //!
            KEY_MOUSEBTN_LAST = KEY_MOUSEBTN_7,

            //!
            //! first xb360 button
            //!
            KEY_XB360_FIRST = KEY_XB360_UP,

            //!
            //! last xb360 button
            //!
            KEY_XB360_LAST = KEY_XB360_Y,
        };
        #undef GNINPUT_DEFINE_KEYCODE
        //@}

        //!
        //! axis type
        //!
        enum Axis
        {
            AXIS_MOUSE_X,       //!< mouse X
            AXIS_MOUSE_Y,       //!< mouse Y
            AXIS_MOUSE_WHEEL_0, //!< mouse wheel 0
            AXIS_MOUSE_WHEEL_1, //!< mouse wheel 1

            AXIS_XB360_LEFT_TRIGGER,  //!< xb360 left trigger
            AXIS_XB360_RIGHT_TRIGGER, //!< xb360 right trigger
            AXIS_XB360_THUMB_LX,      //!< xb360 left thumb X
            AXIS_XB360_THUMB_LY,      //!< xb360 left thumb Y
            AXIS_XB360_THUMB_RX,      //!< xb360 right thumb X
            AXIS_XB360_THUMB_RY,      //!< xb360 right thumb Y

            NUM_AXISES, //!< number of axises.
        };

        // TODO: joystick support

        //!
        //! 定义按键的状态, used by struct KeyEvent
        //!
        enum KeyState
        {
            KS_DOWN   = 1<<0,
            KS_LCTRL  = 1<<1,
            KS_RCTRL  = 1<<2,
            KS_LALT   = 1<<3,
            KS_RALT   = 1<<4,
            KS_LSHIFT = 1<<5,
            KS_RSHIFT = 1<<6,
        };

        //@{
        // These are provided for easy construction of usual key event constants
        #define GNINPUT_KEYDOWN( keycode ) ( keycode | (::GN::input::KS_DOWN<<8) )
        #define GNINPUT_KEYUP( keycode )   ( keycode )
        //@}

        //!
        //! Key status structure
        //!
        union KeyStatus
        {
            uint8_t u8;  //!< Key status as unsigned integer
            int8_t  i8;  //!< Key status as signed integer

            struct
            {
                bool down   : 1; //!< key down
                bool lctrl  : 1; //!< left ctrl down
                bool rctrl  : 1; //!< right ctrl down
                bool lalt   : 1; //!< left alt down
                bool ralt   : 1; //!< right alt down
                bool lshift : 1; //!< left shift down
                bool rshift : 1; //!< right shift down
            };

            //!
            //! Equality
            //!
            bool operator==( const KeyStatus & rhs ) const { return u8 == rhs.u8; }

            //!
            //! Equality
            //!
            bool operator!=( const KeyStatus & rhs ) const { return u8 != rhs.u8; }

            //! \name Misc functions
            //@{
            bool ctrlDown() const { return lctrl && rctrl; }
            bool altDown() const { return lalt && ralt; }
            bool shiftDown() const { return lshift && rshift; }
            //@}
        };

        //!
        //! key event structure
        //!
        //! 一个keyevent_s实际上就是一个uint16_t类型的整数，
        //! 其低8位是key code，高8位是key state
        //!
        union KeyEvent
        {
            //! key event as unsigned 16bit integer
            uint16_t u16;

            //! key event as signed 16bit integer
            int16_t  i16;

            //! structured key states
            struct
            {
                uint8_t   code;   //!< Key code
                KeyStatus status; //!< Key status
            };

            //! \name constructor(s)
            //@{
            KeyEvent() {}
            KeyEvent( const KeyEvent & k ) : u16(k.u16) {}
            KeyEvent( KeyCode kc, KeyStatus ks )
                : code( static_cast<uint8_t>(kc) ), status(ks)
            { GN_ASSERT( kc < NUM_KEYS ); }
            //@}

            //!
            //! Equality
            //!
            bool operator == ( const KeyEvent & rhs ) const
            {
                return u16 == rhs.u16;
            }

            //!
            //! Equality
            //!
            bool operator != ( const KeyEvent & rhs ) const
            {
                return u16 != rhs.u16;
            }

            //@}

            //!
            //! set key data
            //!
            const KeyEvent & set( KeyCode kc, KeyStatus ks )
            {
                GN_ASSERT( kc < NUM_KEYS );
                code = static_cast<uint8_t>(kc);
                status = ks;
                return *this;
            }
        };

        //!
        //! main interface of input module (singleton)
        //!
        struct Input : public Singleton<Input>, public NoCopy
        {
            // ************************************************************************
            //      main interface functions
            // ************************************************************************

            //!
            //! Must call this function before you can retrieve input events from this interface.
            //!
            //! \Note parameters displayHandle is only used on X11 platform.
            //!
            virtual bool attachToWindow( HandleType displayHandle, HandleType windowHandle ) = 0;

            //!
            //! 获取并处理最新的输入事件
            //!
            virtual void processInputEvents() = 0;

            //!
            //! Return keyboard status indexed by KeyCode.
            //!
            virtual const KeyStatus * getKeyboardStatus() const = 0;

            //!
            //! Return absolute value of all axises.
            //!
            //! \note
            //! - Mouse positions are based left-up corner of the attached window.
            //!
            virtual const int * getAxisStatus() const = 0;

            //!
            //! 得到当前鼠标的位置（相对窗口的左上角）
            //!
            virtual void getMousePosition( int & x, int & y ) const = 0;

            // ************************************************************************
            //! \name   signals
            //! \note   These signals are used internally by garnet system.
            //          Client user should connect to their counterparts in inputdev_i
            // ************************************************************************

            //@{

            //!
            //! triggered when keyboard, mouse button, joystick button was pressed/released.
            //!
            Signal1<void,KeyEvent> sigKeyPress;

            //!
            //! triggered when a valid unicode character was generated by user input
            //!
            Signal1<void,wchar_t> sigCharPress;

            //!
            //! triggered when mouse/wheel/joystick moving/scrolling
            //!
            //! \note  参数2中返回的是相对位移
            //!
            Signal2<void,Axis,int> sigAxisMove;

            //@}
        };

        //!
        //! Input API type
        //!
        enum InputApi
        {
            API_NATIVE,    //!< OS native API (Win32, X11, XInput)
            API_DINPUT,    //!< DirectInput
            API_FAKE,      //!< Fake
            NUM_INPUT_API, //!< input API count.
        };

        //!
        //! Create instance of input system.
        //!
        Input * createInputSystem( InputApi = API_NATIVE );

        //!
        //! convert string to keycode
        //!
        //! \return 失败时返回 KEY_NONE
        //!
        KeyCode str2kc( const char * );

        //!
        //! convert keycode to string
        //!
        //! \return  失败时返回空字串（注意，是空字串""，不是NULL，
        //!          你可以用 GN::strEmpty() 来检测返回值
        //!
        const char * kc2str( int );

    }
}

#include "input/inputUtils.inl"

// *****************************************************************************
//                           End of GNinput.h
// *****************************************************************************
#endif // __GN_INPUT_GNINPUT_H__
