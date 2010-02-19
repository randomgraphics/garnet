#ifndef __GN_INPUT_BASICINPUTMSW_H__
#define __GN_INPUT_BASICINPUTMSW_H__
// *****************************************************************************
/// \file
/// \brief   Basic input system for NT platform
/// \author  chenlee (2005.10.25)
// *****************************************************************************

#include "basicInputXInput.h"

#if GN_MSWIN && !GN_XENON

namespace GN { namespace input
{
    ///
    /// Basic input system for MS windows. 实现windows系统下输入系统的基本功能.
    ///
    class BasicInputMsw : public BasicXInput, public StdClass
    {
         GN_DECLARE_STDCLASS( BasicInputMsw, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        BasicInputMsw()          { Clear(); }
        virtual ~BasicInputMsw() { Quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool Init();
        void Quit();
    private:
        void Clear()
        {
            mXInputLibrary = 0;
            mXInputGetState = 0;
            mMsgHook = mCwpHook = 0;
            mMouseCapture = false;
        }
        //@}

        // ********************************
        // public interface
        // ********************************
    public:

        void processInputEvents();
        bool attachToWindow( HandleType, HandleType );
        void getMousePosition( int & x, int & y ) const;

        // ********************************
        // protected functions
        // ********************************
    protected:

        ///
        /// window message handler (overridable)
        ///
        virtual void msgHandler( UINT, WPARAM, LPARAM );

        ///
        /// called in msgHandler to add input event to event queue
        //@{
        void pushKeyPress( KeyCode key, bool keydown );
        void pushCharPress( char ch );
        void pushAxisMove( Axis axis, int distance );
        void pushAxisAbs( Axis axis, int pos );
        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        struct InputEvent
        {
            int type; // 0 : key, 1: char, 2: axis_move, 3: axis_abs

            struct KeyEvent
            {
                KeyCode code;
                bool    down;
            } key;

            struct AxisEvent
            {
                Axis a;
                int  d;
            } axis;

            char ch;
        };

        HMODULE mXInputLibrary;
        HHOOK   mMsgHook, mCwpHook;
        bool    mMouseCapture;
        HWND    mWindow;

        std::queue<InputEvent> mInputEvents;
        Mutex                  mEventQueueMutex;

        // ********************************
        // private functions
        // ********************************
    private:

        bool setupXInputFunctionPointers();
        bool setupWindowHooks( HWND window ); // hook to specific window.
        void removeWindowHooks();

        void captureMouse();
        void releaseMouse();

        static LRESULT CALLBACK
        sMsgHookProc( int nCode, WPARAM wParam, LPARAM lParam );

        static LRESULT CALLBACK
        sCwpHookProc( int nCode, WPARAM wParam, LPARAM lParam );
    };
}}

#endif // GN_MSWIN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_INPUT_BASICINPUTMSW_H__
