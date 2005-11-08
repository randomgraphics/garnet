#ifndef __GN_GFX_NTRENDERWINDOW_H__
#define __GN_GFX_NTRENDERWINDOW_H__
// *****************************************************************************
//! \file    ntRenderWindow.h
//! \brief   Render window class on NT platform
//! \author  chenlee (2005.10.4)
// *****************************************************************************

#if GN_MSWIN

namespace GN { namespace gfx
{
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
        //! (re)initialize render window to use external window
        //!
        bool initExternalRenderWindow( void * unused, void * externalWindow );

        //!
        //! (re)initialize render window to use internal widow.
        //!
        bool initInternalRenderWindow( void * unused, void * parentWindow, uint32_t width, uint32_t height );

        //!
        //! Delete render window
        //!
        void quit();

        //!
        //! Get display handle. For compability to X Window class, no use.
        //!
        void * getDisplay() const { return 0; }

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
        static std::map<void*,NTRenderWindow*> msInstanceMap;

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
}}

#endif

// *****************************************************************************
//                           End of ntRenderWindow.h
// *****************************************************************************
#endif // __GN_GFX_NTRENDERWINDOW_H__
