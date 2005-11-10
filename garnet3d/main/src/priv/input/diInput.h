#ifndef __GN_INPUT_DIINPUT_H__
#define __GN_INPUT_DIINPUT_H__
// *****************************************************************************
//! \file    diInput.h
//! \brief   使用DirectInput的输入模块
//! \author  chenlee (2005.10.29)
// *****************************************************************************

#include "basicMswInput.h"

#if GN_MSWIN

// ****************************************************************************
//! \name                IDX单元中的一些常用的宏定义
// ****************************************************************************

//@{

#define DI_CHECK_DO( func, something )                       \
    {                                                        \
        HRESULT rr = func;                                   \
        if( FAILED(rr) )                                     \
        {                                                    \
            GNINPUT_ERROR( "%s", DXGetErrorString9A(rr) );   \
            something                                        \
        }                                                    \
    }

#if GN_DEBUG
#define DI_CHECK( func )           DI_CHECK_DO( func, )
#else
#define DI_CHECK( func )           func;
#endif
#define DI_CHECK_R( func )         DI_CHECK_DO( func, return; )
#define DI_CHECK_RV( func, rval )  DI_CHECK_DO( func, return rval; )

//@}

namespace GN { namespace input
{
    //!
    //! 使用DirectInput的输入系统
    //!    
    class DIInput : public BasicMswInput
    {
         GN_DECLARE_STDCLASS( DIInput, BasicMswInput );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        DIInput()          { clear(); buildKeyMap(); }
        virtual ~DIInput() { quit(); }
        //@}

        // ********************************
        // standard init/quit
        // ********************************

        //@{
    public:
        bool init();
        void quit();
        bool ok() const
        {
            return MyParent::ok()
                && 0 != mLibrary
                && 0 != mDInput
                && 0 != mKeyboard
                && 0 != mMouse;
        }
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
        // from BasicMswInput
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

#endif // GN_MSWIN

// *****************************************************************************
//                           End of diInput.h
// *****************************************************************************
#endif // __GN_INPUT_DIINPUT_H__
