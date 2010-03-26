#ifndef __GN_GFX_RENDERWINDOWMSW_H__
#define __GN_GFX_RENDERWINDOWMSW_H__
// *****************************************************************************
/// \file
/// \brief   Render window class on NT platform
/// \author  chenlee (2005.10.4)
// *****************************************************************************

#if GN_MSWIN && !GN_XENON

namespace GN { namespace gfx
{
    ///
    /// Render window class on Windows (PC) platform
    ///
    class RenderWindowMsw : public StdClass
    {
        GN_DECLARE_STDCLASS( RenderWindowMsw, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        RenderWindowMsw() : mGpu(0), mWindow(0), mHook(0), mMonitor(0) {}
        ~RenderWindowMsw() { Quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        /// initialize render window to use external window
        bool initExternalWindow( Gpu * gpu, HandleType externalWindow );
        /// initialize render window to use internal widow.
        bool initInternalWindow( Gpu * gpu, HandleType parentWindow, HandleType monitor, UInt32 width, UInt32 height );
        void Quit();
    private:
        void Clear() { mGpu = 0; mWindow = 0; mHook = 0; mMonitor = 0; mOldWidth = 0; mOldHeight = 0; mOldMonitor = 0; }
        //@}

        // ********************************
        // public interface
        // ********************************

    public:

        ///
        /// Get window handle
        ///
        HWND GetWindowHandle() const { return mWindow; }

        ///
        /// Get monitor handle
        ///
        HMONITOR GetMonitorHandle() const { return mMonitor; }

        ///
        /// Get client size
        ///
        void GetClientSize( UInt32 & width, UInt32 & height ) const;

        ///
        /// handle render window size move, trigger renderer signal as apropriate.
        ///
        void handleSizeMove();

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

        Gpu * mGpu;
        HWND       mWindow;
        StrW       mClassName;
        HINSTANCE  mModuleInstance;
        HMONITOR   mMonitor;
        HHOOK      mHook;
        UInt32     mOldWidth, mOldHeight;
        HMONITOR   mOldMonitor;
        bool       mUseExternalWindow;
        bool       mInsideSizeMove;

        typedef GN::HashMap<void*,RenderWindowMsw*> WindowMap;

        static WindowMap msInstanceMap;

        // ********************************
        // private functions
        // ********************************
    private:

        bool    postInit();
        bool    NewWindow( HWND parent, HMONITOR monitor, UInt32 width, UInt32 height );
        void    handleMessage( HWND wnd, UINT msg, WPARAM wp, LPARAM lp );
        LRESULT windowProc( HWND wnd, UINT msg, WPARAM wp, LPARAM lp );
        static  LRESULT CALLBACK staticWindowProc( HWND wnd, UINT msg, WPARAM wp, LPARAM lp );
        static  LRESULT CALLBACK staticHookProc( int code, WPARAM wp, LPARAM lp );
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
}}

#endif

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_RENDERWINDOWMSW_H__
