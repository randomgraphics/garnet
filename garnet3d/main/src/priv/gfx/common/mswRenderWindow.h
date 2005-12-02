#ifndef __GN_GFX_MSWRENDERWINDOW_H__
#define __GN_GFX_MSWRENDERWINDOW_H__
// *****************************************************************************
//! \file    mswRenderWindow.h
//! \brief   Render window class on NT platform
//! \author  chenlee (2005.10.4)
// *****************************************************************************

namespace GN { namespace gfx
{
#if GN_XENON

    //!
    //! Render window class on Xenon platform
    //!
    class MSWRenderWindow
    {
        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        MSWRenderWindow() : mWidth(0), mHeight(0) {}
        ~MSWRenderWindow() { quit(); }
        //@}

        // ********************************
        // public interface
        // ********************************

    public:

        //!
        //! (re)initialize render window to use external window
        //!
        bool initExternalRenderWindow( HandleType, HandleType ) { return true; }

        //!
        //! (re)initialize render window to use internal widow.
        //!
        bool initInternalRenderWindow( HandleType, HandleType, uint32_t width, uint32_t height )
        {
            mWidth = width;
            mHeight = height;
            return true;
        }

        //!
        //! Delete render window
        //!
        void quit() { mWidth = 0; mHeight = 0; }

        //!
        //! Get display handle. For compability to X Window class, no use.
        //!
        HandleType getDisplay() const { return 0; }

        //!
        //! Get window handle
        //!
        HWND getWindow() const { return 0; }

        //!
        //! Get monitor handle
        //!
        HMONITOR getMonitor() const { return 0; }

        //!
        //! Get client size
        //!
        bool getClientSize( uint32_t & width, uint32_t & height ) const
        { width = mWidth; height = mHeight; return true; }

        //!
        //! Get window size change flag.
        //!
        bool getSizeChangeFlag( bool = true ) { return false; }

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
        uint32_t mWidth, mHeight;
    };

    //!
    //! Window properties containor (for compability to PC, no use at all)
    //!
    struct WinProp
    {
        //!
        //! save window properties
        //!
        bool save( HWND ) { return true; }

        //!
        //! Restore previously stored properites
        //!
        void restore() {}
    };

#elif GN_MSWIN

    //!
    //! Render window class on Windows (PC) platform
    //!
    class MSWRenderWindow
    {
        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        MSWRenderWindow() : mWindow(0), mHook(0), mMonitor(0) { ++msInstanceID; }
        ~MSWRenderWindow() { quit(); }
        //@}

        // ********************************
        // public interface
        // ********************************

    public:

        //!
        //! (re)initialize render window to use external window
        //!
        bool initExternalRenderWindow( HandleType unused, HandleType externalWindow );

        //!
        //! (re)initialize render window to use internal widow.
        //!
        bool initInternalRenderWindow( HandleType unused, HandleType parentWindow, uint32_t width, uint32_t height );

        //!
        //! Delete render window
        //!
        void quit();

        //!
        //! Get display handle. For compability to X Window class, no use.
        //!
        HandleType getDisplay() const { return 0; }

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

        static unsigned int msInstanceID;
        static std::map<void*,MSWRenderWindow*> msInstanceMap;

        // ********************************
        // private functions
        // ********************************
    private:

        bool postInit();
        bool createWindow( HWND parent, uint32_t width, uint32_t height );
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
        HWND  mWindow;
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
        WinProp() : mWindow(0) {}

        //!
        //! Dtor
        //!
        ~WinProp() { restore(); }

        //!
        //! save window properties
        //!
        bool save( HWND hwnd );

        //!
        //! Restore previously stored properites
        //!
        void restore();
    };
#endif
}}

// *****************************************************************************
//                           End of mswRenderWindow.h
// *****************************************************************************
#endif // __GN_GFX_MSWRENDERWINDOW_H__
