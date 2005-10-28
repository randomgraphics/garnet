#include "pch.h"
#include "ntRenderWindow.h"

#if GN_MSWIN

unsigned int GN::gfx::NTRenderWindow::msInstanceID = 0;
std::map<void*,GN::gfx::NTRenderWindow*> GN::gfx::NTRenderWindow::msInstanceMap;

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
bool GN::gfx::NTRenderWindow::init( const UserOptions & uo )
{
    GN_GUARD;

    // initialize render window
    if( uo.useExternalWindow )
    {
        quit();
        if( !initExternalWindow( uo ) ) return false;
    }
    else
    {
        if( !mUseExternalWindow && mWindow )
        {
            if( !resizeInternalWindow( uo ) ) return false;
        }
        else
        {
            quit();
            if( !initInternalWindow( uo ) ) return false;
        }
    }
    GN_ASSERT( mWindow );
    mUseExternalWindow = uo.useExternalWindow;

    // update monitor handle
    mMonitor = ::MonitorFromWindow( mWindow, MONITOR_DEFAULTTONEAREST );
    if( 0 == mMonitor )
    {
        GNGFX_ERROR( "Fail to get monitor handle from window!" );
        return false;
    }

    //// Output monitor information
    //MONITORINFOEXA mi;
    //mi.cbSize = sizeof(mi);
    //GN_WIN_CHECK( ::GetMonitorInfoA( mMonitor, &mi ) );
    //GNGFX_INFO( "窗口所在的设备名：%s", mi.szDevice );

    // add window handle to instance map
    GN_ASSERT(
        msInstanceMap.end() == msInstanceMap.find(mWindow) ||
        this == msInstanceMap.find(mWindow)->second );
    msInstanceMap[mWindow] = this;

    // clear all state flags
    mInsideSizeMove = false;
    mSizeChanged = false;

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

    // unregister window class
    if( !mClassName.empty() )
    {
        GN_INFO( "Unregister window class: %s (module handle: 0x%X)", mClassName.cstr(), mModuleInstance );
        GN_ASSERT( mModuleInstance );
        GN_WIN_CHECK( ::UnregisterClassA( mClassName.cstr(), mModuleInstance ) );
        mClassName.clear();
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
bool GN::gfx::NTRenderWindow::initExternalWindow( const UserOptions & uo )
{
    GN_GUARD;

    GN_ASSERT( uo.useExternalWindow );

    if( !::IsWindow((HWND)uo.renderWindow) )
    {
        GNGFX_ERROR( "External render window handle must be valid." );
        return false;
    }

    if( msInstanceMap.end() != msInstanceMap.find(uo.renderWindow) )
    {
        GNGFX_ERROR( "You can't create multiple render window instance for single window handle." );
        return false;
    }

    mWindow = (HWND)uo.renderWindow;

    // register a message hook to render window.
    mHook = ::SetWindowsHookEx( WH_CALLWNDPROC, &staticHookProc, 0, GetCurrentThreadId() );
    if( 0 == mHook )
    {
        GNGFX_ERROR( "Fail to setup message hook : %s", getOSErrorInfo() );
        return false;
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::NTRenderWindow::initInternalWindow( const UserOptions & uo )
{
    GN_GUARD;

    GN_ASSERT( !uo.useExternalWindow );

    uint32_t w, h;
    if( !determineInternalWindowSize( uo, w, h ) ) return false;
    GN_ASSERT( w > 0 && h > 0 );

    if( !createWindow( (HWND)uo.parentWindow, w, h ) ) return false;

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::NTRenderWindow::resizeInternalWindow( const UserOptions & uo )
{
    GN_GUARD;

    GN_ASSERT( !uo.useExternalWindow && !!::IsWindow(mWindow) );

    uint32_t w, h;
    if( !determineInternalWindowSize( uo, w, h ) ) return false;
    GN_ASSERT( w > 0 && h > 0 );

    // calculate boundary size
    RECT rc = { 0, 0, w, h };
    GN_WIN_CHECK_RV(
        ::AdjustWindowRectEx(
            &rc,
            ::GetWindowLongA( mWindow, GWL_STYLE ),
            0,
            ::GetWindowLongA( mWindow, GWL_EXSTYLE ) ),
        false );

    // resize the window
    GN_WIN_CHECK_RV(
        ::SetWindowPos(
            mWindow, HWND_TOP,
            0, 0, // position, ignored.
            rc.right-rc.left, rc.bottom-rc.top, // size
            SWP_NOMOVE ),
        false );

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::NTRenderWindow::determineInternalWindowSize(
    const UserOptions & uo, uint32_t & width, uint32_t & height )
{
    GN_GUARD;

    // Determine window size
    if( uo.fullscreen )
    {
        const DisplayMode & dm = uo.displayMode;

        if( 0 == dm.width || 0 == dm.height )
        {
            // In fullscreen mode, default window size is determined by display mode.

            // Get user specified monitor size
            HMONITOR monitor;
            if( 0 == uo.monitorHandle )
            {
                if( !::IsWindow((HWND)uo.parentWindow) )
                {
                    POINT pt = { LONG_MIN, LONG_MIN }; // Make sure primary monitor are returned.
                    monitor = ::MonitorFromPoint( pt, MONITOR_DEFAULTTOPRIMARY );
                    if( 0 == monitor )
                    {
                        GN_ERROR( "Fail to get primary monitor handle." );
                        return false;
                    }
                }
                else
                {
                    monitor = ::MonitorFromWindow( (HWND)uo.renderWindow, MONITOR_DEFAULTTONEAREST );
                }
            }
            else
            {
                monitor = (HMONITOR)uo.monitorHandle;
            }
            GN_ASSERT( monitor );

            if( !::sGetMonitorSize( monitor, width, height ) ) return false;
        }

        if( dm.width ) width = dm.width;
        if( dm.height ) height = dm.height;
    }
    else
    {
        // In windowed mode, default window size is 640x480
        width = uo.windowedWidth ? uo.windowedWidth : 640;
        height = uo.windowedHeight ? uo.windowedHeight : 480;
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool
GN::gfx::NTRenderWindow::createWindow( HWND parent, uint32_t width, uint32_t height )
{
    GN_GUARD;

    // check parent
    if( 0 != parent && !::IsWindow(parent) ) parent = 0;

    mModuleInstance = (HINSTANCE)GetModuleHandleA(0);
    GN_ASSERT( 0 != mModuleInstance );

    WNDCLASSEXA wcex;

    // generate an unique window class name
    do
    {
        mClassName.format( "GNgfxRenderWindow_%d", rand() );
    } while( ::GetClassInfoExA( mModuleInstance, mClassName.cstr(), &wcex ) );

    // register window class
    GN_INFO( "Register window class: %s (module handle: 0x%X)", mClassName.cstr(), mModuleInstance );
    wcex.cbSize         = sizeof(WNDCLASSEX);
    wcex.style          = CS_NOCLOSE;
    wcex.lpfnWndProc    = (WNDPROC)&staticWindowProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = mModuleInstance;
    wcex.hIcon          = LoadIcon (0, IDI_APPLICATION);
    wcex.hCursor        = LoadCursor (0,IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = 0;
    wcex.lpszClassName  = mClassName.cstr();
    wcex.hIconSm        = LoadIcon(0, IDI_APPLICATION);
    if( 0 == ::RegisterClassExA(&wcex) )
    {
        GNGFX_ERROR( "fail to register window class, %s!", getOSErrorInfo() );
        return false;
    }

    // setup window style
    DWORD style = WS_OVERLAPPEDWINDOW;
    DWORD exStyle = parent ? WS_EX_TOOLWINDOW : 0;

    // calculate window size
    RECT rc = { 0, 0, width, height };
    ::AdjustWindowRectEx( &rc, style, 0, exStyle );

    // create window
    mWindow = ::CreateWindowExA(
        exStyle,
        mClassName.cstr(),
        "", // no title
        style,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rc.right - rc.left, rc.bottom - rc.top,
        parent,
        0, // no menu
        mModuleInstance,
        0 );
    if( 0 == mWindow )
    {
        GNGFX_ERROR( "fail to create window, %s!", getOSErrorInfo() );
        return false;
    }

    // show the window
    ::ShowWindow( mWindow, SW_NORMAL );
    ::UpdateWindow( mWindow );

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void
GN::gfx::NTRenderWindow::handleMessage( HWND wnd, UINT msg, WPARAM wp, LPARAM lp )
{
    GN_GUARD;

    switch(msg)
    {
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
        default: ; // do nothing
    }

    //
    // Update monitor handle
    //
    if( mSizeChanged )
    {
        // Update monitor handle
        mMonitor = ::MonitorFromWindow( wnd, MONITOR_DEFAULTTONEAREST );
        if( 0 == mMonitor )
        {
            GNGFX_ERROR( "Fail to get monitor handle from window handle!" );
        }

        //// Output monitor information
        //MONITORINFOEXA mi;
        //mi.cbSize = sizeof(mi);
        //GN_WIN_CHECK( ::GetMonitorInfoA( mMonitor, &mi ) );
        //GNGFX_INFO( "窗口所在的设备名：%s", mi.szDevice );
    }

    // trigger the message signal
    sigMessage( wnd, msg, wp, lp );

    GN_UNGUARD;
}


//
//
// -----------------------------------------------------------------------------
LRESULT
GN::gfx::NTRenderWindow::windowProc( HWND wnd, UINT msg, WPARAM wp, LPARAM lp )
{
    GN_GUARD;

    handleMessage( wnd, msg, wp, lp );

    switch (msg)
    {
        case WM_CLOSE :
            ::PostQuitMessage(0);
            return 0;

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

    //GNGFX_INFO( "GN::gfx::NTRenderWindow procedure: wnd=0x%X, msg=%s", wnd, win::msg2str(msg) );

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

    //GNGFX_INFO( "wnd=0x%X, msg=%s", wnd, win::msg2str(msg) );

    std::map<void*,NTRenderWindow*>::const_iterator iter =
        msInstanceMap.find( ((CWPSTRUCT*)lp)->hwnd );

    if( msInstanceMap.end() != iter )
    {
        // trigger render window message signal.
        CWPSTRUCT * cwp = (CWPSTRUCT*)lp;
        NTRenderWindow * wnd = iter->second;
        GN_ASSERT( cwp && wnd );
        wnd->handleMessage( cwp->hwnd, cwp->message, cwp->wParam, cwp->lParam );
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

    if( !::IsWindow(hwnd) )
    {
        GN_ERROR( "Input window handle is invalid!" );
        return false;
    }

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
    mWindow = hwnd;
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::WinProp::restore()
{
    GN_GUARD;

    if( !::IsWindow(mWindow) ) return;

    if( !(WS_CHILD & mStyle) )
    {
        // NOTE: can't attach mMenu to child window
        GN_WIN_CHECK( ::SetMenu( mWindow, mMenu ) );
    }
    ::SetWindowLong( mWindow, GWL_STYLE, mStyle );
    ::SetWindowLong( mWindow, GWL_EXSTYLE, mExStyle );
    GN_WIN_CHECK( ::SetParent( mWindow, mParent ) );
    GN_WIN_CHECK( ::SetWindowPos(
        mWindow,
        WS_EX_TOPMOST & mExStyle ? HWND_TOPMOST : HWND_NOTOPMOST,
        mBoundsRect.left, mBoundsRect.top,
        mBoundsRect.right-mBoundsRect.left,
        mBoundsRect.bottom-mBoundsRect.top,
        SWP_FRAMECHANGED | SWP_NOOWNERZORDER | SWP_SHOWWINDOW ) );
    GN_WIN_CHECK( ::UpdateWindow( mWindow ) );

    mWindow = 0;

    GN_UNGUARD;
}


#endif // GN_MSWIN
