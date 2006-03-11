#ifndef __GN_INPUT_BASICINPUTMSW_H__
#define __GN_INPUT_BASICINPUTMSW_H__
// *****************************************************************************
//! \file    basicInputMsw.h
//! \brief   Basic input system for NT platform
//! \author  chenlee (2005.10.25)
// *****************************************************************************

#include "basicInput.h"

#if GN_MSWIN && !GN_XENON

namespace GN { namespace input
{
    //!
    //! Basic input system for MS windows. 实现windows系统下输入系统的基本功能.
    //!
    class BasicInputMsw : public BasicInput, public StdClass
    {
         GN_DECLARE_STDCLASS( BasicInputMsw, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        BasicInputMsw()          { clear(); }
        virtual ~BasicInputMsw() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init();
        void quit();
        bool ok() const { return MyParent::ok(); }
    private:
        void clear()
        {
            mMsgHook = mCwpHook = 0;
            mMouseCapture = false;
            mXInputPacketNumber = 0;
        }
        //@}

        // ********************************
        // public interface
        // ********************************
    public:

        bool attachToWindow( HandleType, HandleType );
        void processInputEvents();
        void getMousePosition( int & x, int & y ) const;

        // ********************************
        // protected functions
        // ********************************
    protected:

        //!
        //! Get window handle
        //!
        HWND getAttachedWindow() const { return mWindow; }

        //!
        //! process mouse movement
        //!
        void processMouseMove();

        //!
        //! window message handler (overridable)
        //!
        virtual void msgHandler( UINT, WPARAM, LPARAM );

        // ********************************
        // private variables
        // ********************************
    private:

        HWND mWindow;
        HHOOK mMsgHook, mCwpHook;
        bool  mMouseCapture;

        DWORD mXInputPacketNumber;

        // ********************************
        // private functions
        // ********************************
    private:

        bool setupWindowHooks();
        void removeWindowHooks();

        void captureMouse();
        void releaesMouse();

        static LRESULT CALLBACK
        sMsgHookProc( int nCode, WPARAM wParam, LPARAM lParam );

        static LRESULT CALLBACK
        sCwpHookProc( int nCode, WPARAM wParam, LPARAM lParam );
    };
}}

#endif // GN_MSWIN

// *****************************************************************************
//                           End of basicWinInput.h
// *****************************************************************************
#endif // __GN_INPUT_BASICINPUTMSW_H__
