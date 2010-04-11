#ifndef __GN_INPUT_DIINPUT_H__
#define __GN_INPUT_DIINPUT_H__
// *****************************************************************************
/// \file
/// \brief   使用DirectInput的输入模块
/// \author  chenlee (2005.10.29)
// *****************************************************************************

#include "basicInputMsw.h"

#ifdef HAS_DINPUT

#include <dinput.h>

namespace GN { namespace input
{
    ///
    /// 使用DirectInput的输入系统
    ///
    class InputDInput : public BasicInputMsw
    {
         GN_DECLARE_STDCLASS( InputDInput, BasicInputMsw );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        InputDInput()          { clear(); buildKeyMap(); }
        virtual ~InputDInput() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init();
        void quit();
    private:
        void clear()
        {
            mLibrary = 0;
            mDInput = 0;
            mKeyboard = 0;
            mMouse = 0;
            mAttached = false;
            mAcquired = false;
            mLost = false;
        }
        //@}

        // ********************************
        // from Input
        // ********************************
    public:

        bool attachToWindow( HandleType, HandleType );

        void processInputEvents();

        // ********************************
        // from BasicInputMsw
        // ********************************
    protected:

        void msgHandler( UINT, WPARAM, LPARAM );

        // ********************************
        // private variables
        // ********************************
    private:

        // DI keycode -> custom keycode
        KeyCode               mKeyMap[0x200];

        HMODULE               mLibrary;
        LPDIRECTINPUT8        mDInput;
        LPDIRECTINPUTDEVICE8  mKeyboard;
        LPDIRECTINPUTDEVICE8  mMouse;

        bool                  mAttached;
        bool                  mAcquired;
        bool                  mLost;

        // ********************************
        // private functions
        // ********************************
    private:

        bool acquire();
        bool unacquire();

        void pollKeyboard();
        void pollMouse();

        void buildKeyMap();

        bool diInit();
        void diQuit();
        bool kbInit();
        bool mouseInit();
    };
}}

#endif // HAS_DINPUT

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_INPUT_DIINPUT_H__
