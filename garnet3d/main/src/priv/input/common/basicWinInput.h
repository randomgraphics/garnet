#ifndef __GN_INPUT_BASICWININPUT_H__
#define __GN_INPUT_BASICWININPUT_H__
// *****************************************************************************
//! \file    basicWinInput.h
//! \brief   Basic input system for NT platform
//! \author  chenlee (2005.10.25)
// *****************************************************************************

#include "basicInput.h"

#if GN_WINNT

namespace GN { namespace input {
    //!
    //! Baisc input system for WIN32. 实现windows系统下输入系统的基本功能.
    //!    
    class BasicWinInput : public BasicInput, public StdClass
    {
         GN_DECLARE_STDCLASS( BasicWinInput, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        BasicWinInput()          { clear(); }
        virtual ~BasicWinInput() { quit(); }
        //@}

        // ********************************
        // standard init/quit
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
        }
        //@}

        // ********************************
        // from Input
        // ********************************
    public:

        virtual bool changeUserOptions( const UserOptions & uo );

        virtual void getMousePos( int & x, int & y ) const;

        // ********************************
        // protected functions
        // ********************************
    protected:

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

        UserOptions mUserOptions;

        HHOOK mMsgHook, mCwpHook;
        POINT mMousePosition; // store current mouse position
        bool  mMouseCapture;

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

#endif // GN_WINNT

// *****************************************************************************
//                           End of basicWinInput.h
// *****************************************************************************
#endif // __GN_INPUT_BASICWININPUT_H__
