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

    // initialize monitor handle
    if( 0 == ds.monitorHandle )
    {
        if( !windowValid )
        {
            // make sure pt is outside of any possible display monitor.
            POINT pt = { LONG_MIN, LONG_MIN };
            mMonitor = ::MonitorFromPoint( pt, MONITOR_DEFAULTTOPRIMARY );
            if( 0 == mMonitor )
            {
                GN_ERROR( "Fail to get primary monitor handle." );
                return false;
            }
        }
        else
        {
            mMonitor = ::MonitorFromWindow( (HWND)ds.renderWindow, MONITOR_DEFAULTTONEAREST );
        }
    }
    else
    {
        mMonitor = (HMONITOR)ds.monitorHandle;
    }
    GN_ASSERT( mMonitor );

    // initialize render window
    if( ds.useExternalWindow )
    {
        if( !windowValid )
        {
            GNGFX_ERROR( "External render window handle must be valid." );
            return false;
        }
        mWindow = (HWND)ds.renderWindow;
    }
    else
    {
        uint32_t w, h;
        if( 0 == ds.width || 0 == ds.height )
        {
            if( ds.fullscreen )
            {
                if( !::sGetMonitorSize( mMonitor, w, h ) ) return false;
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

    // delete window
    if( ::IsWindow( mWindow ) && !mUseExternalWindow)
    {
        ::DestroyWindow( mWindow );
    }
    mWindow = 0;

    mMonitor = 0;

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
        this );
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

/*    static int level = 0;

    struct Local
    {
        int & l;
        Local( int & l_ ) : l(l_) { ++l; }
        ~Local() { --l; }
    };
    Local haha(level);*/

    //GN_INFO( "Proc level = %d", level );

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
                //GNGFX_INFO( "window resize to %dx%d", LOWORD(lParam), HIWORD(lParam) ) );
                bool minimized = ( SIZE_MINIMIZED == wp );
                if( !minimized && !mInsideSizeMove ) mSizeChanged = true;
            }
            break;

        // 防止不必要的清除背景的操作
        case WM_ERASEBKGND :
            return 1;

        default: ; // do nothing
    }

    // update monitor handle
    if( mSizeChanged )
    {
        mMonitor = ::MonitorFromWindow( mWindow, MONITOR_DEFAULTTONEAREST );
    }

    return ::DefWindowProc( wnd, msg, wp, lp );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
LRESULT
GN::gfx::NTRenderWindow::staticWindowProc( HWND wnd, UINT msg, WPARAM wp, LPARAM lp )
{
    GN_GUARD;

    //GNGFX_INFO( "wnd=0x%X, msg=%s", wnd, GN::winMsg2Str(msg) );

    NTRenderWindow * ptr;

    // handle WM_NCCREATE
    if( WM_NCCREATE == msg )
    {
        GN_ASSERT( lp );
        ptr = (NTRenderWindow*)((CREATESTRUCT*)lp)->lpCreateParams;
        ::SetWindowLongA( wnd, GWL_USERDATA, (LONG)ptr );
    }
    else
    {
        ptr = (NTRenderWindow*)::GetWindowLong( wnd, GWL_USERDATA );
    }

    // call Renderer specific window procedure
    return ptr
        ? ptr->windowProc( wnd, msg, wp, lp )
        : ::DefWindowProc( wnd, msg, wp, lp );

    GN_UNGUARD;
}

#endif // GN_WINNT
