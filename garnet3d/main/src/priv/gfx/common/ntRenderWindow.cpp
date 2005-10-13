#include "pch.h"
#include "ntRenderWindow.h"

#if GN_WINNT

//!
//! get current display width and height
// ----------------------------------------------------------------------------
static bool
sGetMonitorSize( void * monitor, uint32_t & width, uint32_t & height )
{
    GN_GUARD;

    // get monitor information
    MONITORINFOEXA mi;
    mi.cbSize = sizeof(mi);
    GN_WIN_CHECK_RV( GetMonitorInfoA( (HMONITOR)monitor, &mi ), false );

    // get display mode
    width = (uint32_t) ( mi.rcMonitor.right - mi.rcMonitor.left );
    height = (uint32_t) (mi.rcMonitor.bottom - mi.rcMonitor.top );

    // success
    return true;

    GN_UNGUARD;
}

std::map<void*,GN::gfx::NTRenderWindow*> GN::gfx::NTRenderWindow::msInstanceMap;

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::NTRenderWindow::init( const DeviceSettings & ds )
{
    GN_GUARD;

    quit(); // release old window

    // Note: ds.renderWindow and ds.parentWindow are actually same variable,
    // so here we can use one check for both of them.
    bool windowValid = !!::IsWindow((HWND)ds.renderWindow);

    // initialize render window
    if( ds.useExternalWindow )
    {
        if( !windowValid )
        {
            GNGFX_ERROR( "External render window handle must be valid." );
            return false;
        }
        if( msInstanceMap.end() != msInstanceMap.find(ds.renderWindow) )
        {
            GNGFX_ERROR( "You can't create multiple render window instance for single window handle." );
            return false;
        }

        mWindow = (HWND)ds.renderWindow;

        // register a message hook to rende window.
        mHook = ::SetWindowsHookEx( WH_CALLWNDPROC, &staticHookProc, 0, GetCurrentThreadId() );
        if( 0 == mHook )
        {
            GNGFX_ERROR( "Fail to setup message hook : %s", getOSErrorInfo() );
            return false;
        }
    }
    else
    {
        uint32_t w, h;
        if( 0 == ds.width || 0 == ds.height )
        {
            if( ds.fullscreen )
            {
                // Get user specified monitor size
                HMONITOR monitor;
                if( 0 == ds.monitorHandle )
                {
                    if( !windowValid )
                    {
                        POINT pt = { LONG_MIN, LONG_MIN }; // Make sure primaray monitore are returned.
                        monitor = ::MonitorFromPoint( pt, MONITOR_DEFAULTTOPRIMARY );
                        if( 0 == monitor )
                        {
                            GN_ERROR( "Fail to get primary monitor handle." );
                            return false;
                        }
                    }
                    else
                    {
                        monitor = ::MonitorFromWindow( (HWND)ds.renderWindow, MONITOR_DEFAULTTONEAREST );
                    }
                }
                else
                {
                    monitor = (HMONITOR)ds.monitorHandle;
                }
                GN_ASSERT( monitor );

                if( !::sGetMonitorSize( monitor, w, h ) ) return false;
            }
            else
            {
                // Both fullsreen and windowValid are false. So we have to use
                // default width and height
                w = 640;
                h = 480;
            }
            w = ds.width ? ds.width : w;
            h = ds.height ? ds.height : h;
        }
        else
        {
            w = ds.width;
            h = ds.height;
        }
        GN_ASSERT( w > 0 && h > 0 );
        if( !createWindow( (HWND)ds.parentWindow, w, h, ds.fullscreen ) ) return false;
    }
    GN_ASSERT( mWindow );
    mUseExternalWindow = ds.useExternalWindow;

    // add window handle to instance map
    GN_ASSERT( msInstanceMap.end() == msInstanceMap.find(mWindow) );
    msInstanceMap[mWindow] = this;

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::NTRenderWindow::quit()
{
    GN_GUARD;

    // delete hook
    if( mHook ) ::UnhookWindowsHookEx( mHook ), mHook = 0;

    // delete window
    if( ::IsWindow( mWindow ) )
    {
        if( !mUseExternalWindow) ::DestroyWindow( mWindow );

        GN_ASSERT( msInstanceMap.end() != msInstanceMap.find(mWindow) );
        msInstanceMap.erase(mWindow);

        mWindow = 0;
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::NTRenderWindow::getClientSize( uint32_t & width, uint32_t & height ) const
{
    if( !::IsWindow(mWindow) )
    {
        GNGFX_ERROR( "NTRenderWindow class is yet to initialized!" );
        return false;
    }

    RECT rc;
    GN_WIN_CHECK_RV( ::GetClientRect( mWindow, &rc ), false );

    width = (UINT)(rc.right - rc.left);
    height = (UINT)(rc.bottom - rc.top);

    return true;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool
GN::gfx::NTRenderWindow::createWindow( HWND parent, uint32_t width, uint32_t height, bool fullscreen )
{
    GN_GUARD;

    // check parent
    if( 0 != parent && !::IsWindow(parent) ) parent = 0;

    static const char * sClassName = "GNgfxWindowClass";

    HINSTANCE moduleHandle = (HINSTANCE)::GetModuleHandle(0);

    WNDCLASSEXA wcex;

    // find the window class
    if( !::GetClassInfoEx( moduleHandle, sClassName, &wcex ) )
    {
        // register window class
        wcex.cbSize         = sizeof(WNDCLASSEX);
        wcex.style          = 0;//CS_NOCLOSE;
        wcex.lpfnWndProc    = (WNDPROC)&staticWindowProc;
        wcex.cbClsExtra     = 0;
        wcex.cbWndExtra     = 0;
        wcex.hInstance      = moduleHandle;
        wcex.hIcon          = LoadIcon (0, IDI_APPLICATION);
        wcex.hCursor        = LoadCursor (0,IDC_ARROW);
        wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
        wcex.lpszMenuName   = 0;
        wcex.lpszClassName  = sClassName;
        wcex.hIconSm        = LoadIcon(0, IDI_APPLICATION);
        if( 0 == ::RegisterClassExA(&wcex) )
        {
            GNGFX_ERROR( "fail to register window class, %s!", getOSErrorInfo() );
            return false;
        }
    }

    // setup window style
    DWORD style = WS_POPUP;
    if( !fullscreen ) style |= WS_CAPTION | WS_BORDER | WS_SIZEBOX;
    DWORD exStyle = parent ? WS_EX_TOOLWINDOW : 0;

    // calculate window size
    RECT rc = { 0, 0, width, height };
    ::AdjustWindowRectEx( &rc, style, 0, exStyle );

    // create window
    mWindow = ::CreateWindowExA(
        exStyle,
        sClassName,
        "", // no title
        style,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rc.right - rc.left, rc.bottom - rc.top,
        parent,
        0, // no menu
        moduleHandle,
        0 );
    if( 0 == mWindow )
    {
        GNGFX_ERROR( "fail to create window, %s!", getOSErrorInfo() );
        return false;
    }

    // show the window
    ::ShowWindow( mWindow, SW_NORMAL );
    ::UpdateWindow( mWindow );

    mInsideSizeMove = false;
    mSizeChanged = false;

    // success
    return true;

    GN_UNGUARD;
}


//
//
// -----------------------------------------------------------------------------
LRESULT
GN::gfx::NTRenderWindow::windowProc( HWND wnd, UINT msg, WPARAM wp, LPARAM lp )
{
    GN_GUARD;

    // trigger the message signal
    sigMessage( wnd, msg, wp, lp );

    switch (msg)
    {
        case WM_CLOSE :
            ::PostQuitMessage(0);
            return 0;

        case WM_ENTERSIZEMOVE :
            mInsideSizeMove = true;
            break;

        case WM_EXITSIZEMOVE :
            mInsideSizeMove = false;
            mSizeChanged = true;
            break;

        case WM_SIZE :
            {
                //GNGFX_INFO( "window resize to %dx%d", LOWORD(lp), HIWORD(lp) ) );
                bool minimized = ( SIZE_MINIMIZED == wp );
                if( !minimized && !mInsideSizeMove ) mSizeChanged = true;
            }
            break;

        // 防止不必要的清除背景的操作
        case WM_ERASEBKGND :
            return 1;

        default: ; // do nothing
    }

    // call default procedure
    return ::DefWindowProc( wnd, msg, wp, lp );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
LRESULT CALLBACK
GN::gfx::NTRenderWindow::staticWindowProc( HWND wnd, UINT msg, WPARAM wp, LPARAM lp )
{
    GN_GUARD;

    //GNGFX_INFO( "wnd=0x%X, msg=%s", wnd, GN::winMsg2Str(msg) );

    std::map<void*,NTRenderWindow*>::const_iterator iter = msInstanceMap.find(wnd);

    // call class specific window procedure
    if( msInstanceMap.end() == iter )
    {
        return ::DefWindowProc( wnd, msg, wp, lp );
    }
    else
    {
        GN_ASSERT( iter->second );
        return iter->second->windowProc( wnd, msg, wp, lp );
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
LRESULT CALLBACK
GN::gfx::NTRenderWindow::staticHookProc( int code, WPARAM wp, LPARAM lp )
{
    GN_GUARD;

    //GNGFX_INFO( "wnd=0x%X, msg=%s", wnd, GN::winMsg2Str(msg) );

    std::map<void*,NTRenderWindow*>::const_iterator iter =
        msInstanceMap.find( ((CWPSTRUCT*)lp)->hwnd );

    if( msInstanceMap.end() != iter )
    {
        // trigger render window message signal.
        CWPSTRUCT * cwp = (CWPSTRUCT*)lp;
        NTRenderWindow * wnd = iter->second;
        GN_ASSERT( cwp && wnd );
        wnd->sigMessage( cwp->hwnd, cwp->message, cwp->wParam, cwp->lParam );
    }

    return ::CallNextHookEx( 0, code, wp, lp );

    GN_UNGUARD;
}

// *****************************************************************************
// class WinProp
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::WinProp::save( HWND hwnd )
{
    GN_GUARD;

    mParent = ::GetParent( hwnd );
    mMenu = ::GetMenu( hwnd );
    GN_WIN_CHECK( ::GetWindowRect( hwnd, &mBoundsRect ) );
    if( mParent )
    {
        // transform mBoundsRect to mParent's space
        POINT offset = {0,0};
        ::ScreenToClient( mParent, &offset );
        mBoundsRect.left += offset.x;
        mBoundsRect.right += offset.x;
        mBoundsRect.top += offset.y;
        mBoundsRect.bottom += offset.y;
    }
    mStyle   = ::GetWindowLong( hwnd, GWL_STYLE );
    mExStyle = ::GetWindowLong( hwnd, GWL_EXSTYLE );
    mZoomed  = ::IsZoomed( hwnd );

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::WinProp::restore( HWND hwnd )
{
    GN_GUARD;

    if( !(WS_CHILD & mStyle) )
    {
        // NOTE: can't attach mMenu to child window
        GN_WIN_CHECK( ::SetMenu( hwnd, mMenu ) );
    }
    ::SetWindowLong( hwnd, GWL_STYLE, mStyle );
    ::SetWindowLong( hwnd, GWL_EXSTYLE, mExStyle );
    GN_WIN_CHECK( ::SetParent( hwnd, mParent ) );
    GN_WIN_CHECK( ::SetWindowPos(
        hwnd,
        WS_EX_TOPMOST & mExStyle ? HWND_TOPMOST : HWND_NOTOPMOST,
        mBoundsRect.left, mBoundsRect.top,
        mBoundsRect.right-mBoundsRect.left,
        mBoundsRect.bottom-mBoundsRect.top,
        SWP_FRAMECHANGED | SWP_NOOWNERZORDER | SWP_SHOWWINDOW ) );
    GN_WIN_CHECK( ::UpdateWindow( hwnd ) );

    GN_UNGUARD;
}


#endif // GN_WINNT
