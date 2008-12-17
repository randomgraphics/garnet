#ifndef __GN_GFX_RENDERWINDOWMSW_H__
#define __GN_GFX_RENDERWINDOWMSW_H__
// *****************************************************************************
/// \file
/// \brief   Render window class on NT platform
/// \author  chenlee (2005.10.4)
// *****************************************************************************

namespace GN { namespace gfx
{
#if GN_XENON

    class Renderer;

    ///
    /// Render window class on Xenon platform
    ///
    class RenderWindowMsw
    {
        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        RenderWindowMsw() : mWidth(0), mHeight(0) {}
        ~RenderWindowMsw() { quit(); }
        //@}

        // ********************************
        // public interface
        // ********************************

    public:

        ///
        /// set pointer to renderer that is associated with this window
        ///
        void setRenderer( Renderer * r ) { mRenderer = r; }

        ///
        /// (re)initialize render window to use external window
        ///
        bool initExternalRenderWindow( HandleType disp, HandleType window ) { return true; }

        ///
        /// (re)initialize render window to use internal widow.
        ///
        bool initInternalRenderWindow( HandleType disp, HandleType parent, HandleType monitor, UInt32 width, UInt32 height )
        {
            mWidth = width;
            mHeight = height;
            return true;
        }

        ///
        /// Delete render window
        ///
        void quit() { mWidth = 0; mHeight = 0; }

        ///
        /// Get display handle. For compability to X Window class, no use.
        ///
        HandleType getDisplay() const { return 0; }

        ///
        /// Get window handle
        ///
        HWND getWindow() const { return (HWND)1; }

        ///
        /// Get monitor handle
        ///
        HMONITOR getMonitor() const { return (HMONITOR)1; }

        ///
        /// Get client size
        ///
        bool getClientSize( UInt32 & width, UInt32 & height ) const
        { width = mWidth; height = mHeight; return true; }

        ///
        /// Get window size change flag.
        ///
        bool getSizeChangeFlag( bool = true ) { return false; }

        ///
        /// This is hook functor.
        ///
        typedef Delegate4<void,HWND,UINT,WPARAM,LPARAM> MsgHook;

        ///
        /// This signal will be triggered, whenever the windows receive a message.
        ///
        Signal4<void,HWND,UINT,WPARAM,LPARAM> sigMessage;

        // ********************************
        // private variables
        // ********************************
    private:
        UInt32 mWidth, mHeight;
        Renderer * mRenderer;
    };

    ///
    /// Window properties containor (for compability to PC, no use at all)
    ///
    struct WinProp
    {
        ///
        /// save window properties
        ///
        bool save( HWND ) { return true; }

        ///
        /// Restore previously stored properites
        ///
        void restore() {}
    };

#elif GN_MSWIN

    ///
    /// Render window class on Windows (PC) platform
    ///
    class RenderWindowMsw
    {
        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        RenderWindowMsw() : mWindow(0), mHook(0), mMonitor(0) {}
        ~RenderWindowMsw() { quit(); }
        //@}

        // ********************************
        // public interface
        // ********************************

    public:

        ///
        /// set pointer to renderer that is associated with this window
        ///
        void setRenderer( Renderer * r ) { mRenderer = r; }

        ///
        /// (re)initialize render window to use external window
        ///
        bool initExternalRenderWindow( HandleType dispUnused, HandleType externalWindow );

        ///
        /// (re)initialize render window to use internal widow.
        ///
        bool initInternalRenderWindow( HandleType dispUnused, HandleType parentWindow, HandleType monitor, UInt32 width, UInt32 height );

        ///
        /// Delete render window
        ///
        void quit();

        ///
        /// Get display handle. For compability to X Window class, no use.
        ///
        HandleType getDisplay() const { return 0; }

        ///
        /// Get window handle
        ///
        HWND getWindow() const { return mWindow; }

        ///
        /// Get monitor handle
        ///
        HMONITOR getMonitor() const { return mMonitor; }

        ///
        /// Get client size
        ///
        bool getClientSize( UInt32 & width, UInt32 & height ) const;

        ///
        /// Get window size change flag.
        ///
        /// \param autoReset
        ///     If true, automatically clear the flag.
        ///
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

        ///
        /// This is hook functor.
        ///
        typedef Delegate4<void,HWND,UINT,WPARAM,LPARAM> MsgHook;

        ///
        /// This signal will be triggered, whenever the windows receive a message.
        ///
        Signal4<void,HWND,UINT,WPARAM,LPARAM> sigMessage;

        // ********************************
        // private variables
        // ********************************
    private:

        Renderer * mRenderer;

        HWND mWindow;
        StrW mClassName;
        HINSTANCE mModuleInstance;
        HHOOK mHook;

        bool mUseExternalWindow;

        bool mInsideSizeMove;
        bool mSizeChanged;

        HMONITOR mMonitor;

        static std::map<void*,RenderWindowMsw*> msInstanceMap;

        // ********************************
        // private functions
        // ********************************
    private:

        bool postInit();
        bool createWindow( HWND parent, HMONITOR monitor, UInt32 width, UInt32 height );
        void handleMessage( HWND wnd, UINT msg, WPARAM wp, LPARAM lp );
        LRESULT windowProc( HWND wnd, UINT msg, WPARAM wp, LPARAM lp );
        static LRESULT CALLBACK staticWindowProc( HWND wnd, UINT msg, WPARAM wp, LPARAM lp );
        static LRESULT CALLBACK staticHookProc( int code, WPARAM wp, LPARAM lp );

    };

    ///
    /// Window properties containor.
    ///
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

        ///
        /// Ctor
        ///
        WinProp() : mWindow(0) {}

        ///
        /// Dtor
        ///
        ~WinProp() { restore(); }

        ///
        /// save window properties
        ///
        bool save( HWND hwnd );

        ///
        /// Restore previously stored properites
        ///
        void restore();
    };
#endif
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_RENDERWINDOWMSW_H__
