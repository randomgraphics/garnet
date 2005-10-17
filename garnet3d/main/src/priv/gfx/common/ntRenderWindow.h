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
        NTRenderWindow() : mWindow(0), mHook(0) { ++msInstanceID; }
        ~NTRenderWindow() { quit(); }
        //@}

        // ********************************
        // public interface
        // ********************************

    public:

        //!
        //! initialize or reinitialize the render window based on current device setting.
        //!
        bool init( const DeviceSettings & );

        //!
        //! Delete render window
        //!
        void quit();

        //!
        //! Get window handle
        //!
        void * getWindow() const { return mWindow; }

        //!
        //! Get client size
        //!
        bool getClientSize( uint32_t & width, uint32_t & height ) const;

        //!
        //! This is hook functor.
        //!
        typedef Functor4<void,HWND,UINT,WPARAM,LPARAM> MsgHook;

        //!
        //! This signal will be triggered, whenever the windows receive a message.
        //!
        Signal4<void,HWND,UINT,WPARAM,LPARAM> sigMessage;

/*
        //!
        //! Add message hook.
        //!
        //! Note that this function has no XWin conterpart, neither removeMsgHook().
        //!
        void addMsgHook( UINT msg, const MsgHook & hook );

        //!
        //! Remove message hook.
        //!
        void removeMsgHook( const MsgHook & hook );
*/
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

        static unsigned int msInstanceID;
        static std::map<void*,NTRenderWindow*> msInstanceMap;

        // ********************************
        // private functions
        // ********************************
    private:

        bool createWindow( HWND parent, uint32_t width, uint32_t height, bool fullscreen );
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
