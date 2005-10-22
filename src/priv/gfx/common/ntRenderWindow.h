#ifndef __GN_GFX_NTRENDERWINDOW_H__
#define __GN_GFX_NTRENDERWINDOW_H__
// *****************************************************************************
//! \file    ntRenderWindow.h
//! \brief   Render window class on NT platform
//! \author  chenlee (2005.10.4)
// *****************************************************************************

#if GN_WINNT

namespace GN { namespace gfx {
    //!
    //! Render window class on NT platform
    //!
    class NTRenderWindow
    {
        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        NTRenderWindow() : mWindow(0), mHook(0), mMonitor(0) { ++msInstanceID; }
        ~NTRenderWindow() { quit(); }
        //@}

        // ********************************
        // public interface
        // ********************************

    public:

        //!
        //! initialize or reinitialize the render window based on user options.
        //!
        bool init( const UserOptions & );

        //!
        //! Delete render window
        //!
        void quit();

        //!
        //! Get window handle
        //!
        HWND getWindow() const { return mWindow; }

        //!
        //! Get monitor handle
        //!
        HMONITOR getMonitor() const { return mMonitor; }

        //!
        //! Get client size
        //!
        bool getClientSize( uint32_t & width, uint32_t & height ) const;

        //!
        //! Get window size change flag.
        //!
        //! \param autoReset
        //!     If true, automatically clear the flag.
        //!
        bool getSizeChangeFlag( bool autoReset = true )
        {
            if( autoReset )
            {
                bool b = mSizeChanged;
                mSizeChanged = false;
                return b;
            }
            else return mSizeChanged;
        }

        //!
        //! Get monitor change flag.
        //!
        //! \param autoReset
        //!     If true, automatically clear the flag.
        //!
        bool getMonitorSwitchFlag( bool autoReset = true )
        {
            if( autoReset )
            {
                bool b = mMonitorSwitch;
                mMonitorSwitch = false;
                return b;
            }
            else return mMonitorSwitch;
        }

        //!
        //! This is hook functor.
        //!
        typedef Functor4<void,HWND,UINT,WPARAM,LPARAM> MsgHook;

        //!
        //! This signal will be triggered, whenever the windows receive a message.
        //!
        Signal4<void,HWND,UINT,WPARAM,LPARAM> sigMessage;

        // ********************************
        // private variables
        // ********************************
    private:

        HWND mWindow;
        StrA mClassName;
        HINSTANCE mModuleInstance;
        HHOOK mHook;

        bool mUseExternalWindow;

        bool mInsideSizeMove;
        bool mSizeChanged;

        HMONITOR mMonitor;
        bool mMonitorSwitch;

        static unsigned int msInstanceID;
        static std::map<void*,NTRenderWindow*> msInstanceMap;

        // ********************************
        // private functions
        // ********************************
    private:

        bool initExternalWindow( const UserOptions & uo );
        bool initInternalWindow( const UserOptions & uo );
        bool resizeInternalWindow( const UserOptions & uo );
        bool determineInternalWindowSize( const UserOptions & uo, uint32_t & width, uint32_t & height );

        bool createWindow( HWND parent, uint32_t width, uint32_t height, bool fullscreen );
        void handleMessage( HWND wnd, UINT msg, WPARAM wp, LPARAM lp );
        LRESULT windowProc( HWND wnd, UINT msg, WPARAM wp, LPARAM lp );
        static LRESULT CALLBACK staticWindowProc( HWND wnd, UINT msg, WPARAM wp, LPARAM lp );
        static LRESULT CALLBACK staticHookProc( int code, WPARAM wp, LPARAM lp );

    };

    //!
    //! Window properties containor.
    //!
    class WinProp
    {
        HWND  mParent;
        HMENU mMenu;
        RECT  mBoundsRect;
        DWORD mStyle;
        DWORD mExStyle;
        BOOL  mZoomed;

    public:

        //!
        //! Ctor
        //!
        //WinProp() {}

        //!
        //! save window properties
        //!
        bool save( HWND hwnd );

        //!
        //! Restore previously stored properites
        //!
        void restore( HWND hwnd );
    };
}}

#endif

// *****************************************************************************
//                           End of ntRenderWindow.h
// *****************************************************************************
#endif // __GN_GFX_NTRENDERWINDOW_H__
