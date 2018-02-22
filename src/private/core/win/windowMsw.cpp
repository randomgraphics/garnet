#include "pch.h"
#include "windowMsw.h"

#if GN_WINPC

GN::Dictionary<void*,GN::win::WindowMsw*> GN::win::WindowMsw::msInstanceMap;

static GN::Logger * sLogger = GN::getLogger("GN.win.MSW");

//
//
// -----------------------------------------------------------------------------
static HMONITOR sGetWindowMonitor(HWND window) {
    HMONITOR monitor;
    if ( ::IsWindow( window ) ) {
        monitor = ::MonitorFromWindow( window, MONITOR_DEFAULTTONEAREST );
    } else {
        // return primary monitor
        POINT pt = { LONG_MIN, LONG_MIN };
        monitor = ::MonitorFromPoint( pt, MONITOR_DEFAULTTOPRIMARY );
        if ( 0 == monitor ) {
            GN_ERROR(sLogger)( "Fail to get primary monitor handle." );
            return 0;
        }
    }
    GN_ASSERT( monitor );
    return monitor;
}

#if 0
struct MonitorEnumInfo
{
    HMONITOR handle;
    size_t   targetIndex;
    size_t   currentIndex;
};
static BOOL CALLBACK sMonitorEnumProc( HMONITOR hMonitor, HDC, LPRECT, LPARAM dwData )
{
    MonitorEnumInfo * mei = (MonitorEnumInfo*)dwData;
    if( mei->currentIndex == mei->targetIndex )
    {
        mei->handle = hMonitor;
        return FALSE;
    }
    else
    {
        ++mei->currentIndex;
        return TRUE;
    }
}
static HMONITOR getMonitorByIndex( size_t i )
{
    MonitorEnumInfo mei = { 0, i, 0 };
    ::EnumDisplayMonitors( 0, 0, &sMonitorEnumProc, (LPARAM)&mei );
    return mei.handle;
}
#endif

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::win::WindowMsw::init( const WindowCreationParameters & wcp )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::win::WindowMsw, () );

    if( !createWindow( wcp ) ) return failure();

    mIsExternal = false;

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::win::WindowMsw::init( const WindowAttachingParameters & wap )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::win::WindowMsw, () );

    if( !::IsWindow( (HWND)wap.window ) )
    {
        GN_ERROR(sLogger)( "External render window handle must be valid." );
        return failure();
    }

    if( NULL != msInstanceMap.find( (HWND)wap.window ) )
    {
        GN_ERROR(sLogger)( "You can't create multiple render window instance for single window handle." );
        return failure();
    }

    // register a message hook to render window.
    mHook = ::SetWindowsHookEx( WH_CALLWNDPROC, &staticHookProc, 0, GetCurrentThreadId() );
    if( 0 == mHook )
    {
        GN_ERROR(sLogger)( "Fail to setup message hook : %s", getWin32LastErrorInfo() );
        return failure();
    }

    mWindow     = (HWND)wap.window;
    mIsExternal = true;

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::win::WindowMsw::quit()
{
    GN_GUARD;

    // delete hook
    if( mHook ) ::UnhookWindowsHookEx( mHook ), mHook = 0;

     ::DestroyWindow( mWindow );

    // destroy window
    if( ::IsWindow( mWindow ) )
    {
        if( !mIsExternal )
        {
            GN_TRACE(sLogger)( "Destroy window (handle: 0x%X)", mWindow );
            ::DestroyWindow( mWindow );
        }

        // remove itself from instance map
        GN_ASSERT( NULL != msInstanceMap.find(mWindow) );
        msInstanceMap.remove(mWindow);
    }

    // unregister window class
    if( !mClassName.empty() )
    {
        GN_TRACE(sLogger)( "Unregister window class: %S (module handle: 0x%X)", mClassName.rawptr(), mModuleInstance );
        GN_ASSERT( mModuleInstance );
        GN_MSW_CHECK( ::UnregisterClassW( mClassName.rawptr(), mModuleInstance ) );
        mClassName.clear();
    }

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// Public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
intptr_t GN::win::WindowMsw::getMonitorHandle() const
{
    GN_GUARD;
    GN_ASSERT( ::IsWindow( mWindow ) );
    return (intptr_t)sGetWindowMonitor(mWindow);
    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::Vector2<uint32_t> GN::win::WindowMsw::getClientSize() const
{
    GN_GUARD;
    GN_ASSERT( ::IsWindow( mWindow ) );
    Vector2<uint32_t> sz(0,0);
    RECT rc;
    GN_MSW_CHECK_RETURN( ::GetClientRect( mWindow, &rc ), sz );
    sz.x = (uint32_t)(rc.right - rc.left);
    sz.y = (uint32_t)(rc.bottom - rc.top);
    return sz;
    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::win::WindowMsw::show()
{
    GN_GUARD;
    GN_ASSERT( ::IsWindow( mWindow ) );
    ::ShowWindow( mWindow, SW_NORMAL );
    ::UpdateWindow( mWindow );
    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::win::WindowMsw::hide()
{
    GN_GUARD;
    GN_ASSERT( ::IsWindow( mWindow ) );
    ::ShowWindow( mWindow, SW_HIDE );
    ::UpdateWindow( mWindow );
    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::win::WindowMsw::moveTo( int x, int y )
{
    GN_GUARD;
    GN_MSW_CHECK( ::SetWindowPos(
        mWindow,
        HWND_TOP,
        x, y,
        0, 0, // size
        SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER ) );
    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::win::WindowMsw::setClientSize( size_t w, size_t h )
{
    GN_GUARD;
    GN_ASSERT( ::IsWindow( mWindow ) );
    RECT rc = { 0, 0, (int)w, (int)h };
    GN_MSW_CHECK_RETURN_VOID( ::AdjustWindowRectEx(
        &rc,
        (DWORD)::GetWindowLong( mWindow, GWL_STYLE ),
        (BOOL)::GetMenu( mWindow ),
        (DWORD)::GetWindowLong( mWindow, GWL_EXSTYLE ) ) );
    GN_MSW_CHECK( ::SetWindowPos(
        mWindow,
        HWND_TOP,
        0, 0, // x, y
        rc.right - rc.left, rc.bottom - rc.top,
        SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER ) );
    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::win::WindowMsw::run()
{
    GN_GUARD_ALWAYS;

    GN_ASSERT( ::IsWindow( mWindow ) );

    MSG msg;
    while( ::GetMessage(&msg, NULL, 0, 0 ) )
    {
        ::TranslateMessage( &msg );
        ::DispatchMessage(&msg);
    }

    GN_UNGUARD_ALWAYS;
}

//
//
// -------------------------------------------------------------------------
bool GN::win::WindowMsw::runUntilNoNewEvents( bool blockWhileMinized )
{
    GN_GUARD_SLOW;

    GN_ASSERT( ::IsWindow( (HWND)mWindow ) );

    MSG msg;
    while( true )
    {
        if( ::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
        {
            if( WM_QUIT == msg.message )
            {
                return false;
            }
            ::TranslateMessage( &msg );
            ::DispatchMessage(&msg);
        }
        else if( ::IsIconic( (HWND)mWindow ) && blockWhileMinized )
        {
            GN_TRACE(sLogger)( "Wait for window messages..." );
            ::WaitMessage();
        }
        else return true; // Idle time
    }

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// Private methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::win::WindowMsw::createWindow( const WindowCreationParameters & wcp ) {
    GN_GUARD;

    // check parent
    HWND parent = (HWND)wcp.parent;
    if( 0 != parent && !::IsWindow(parent) ) parent = 0;

    // get module instance
    mModuleInstance = (HINSTANCE)GetModuleHandleA(0);
    GN_ASSERT( 0 != mModuleInstance );

    // determine monitor handle
    auto monitor = (HMONITOR)wcp.monitor;
    if (0 == monitor) {
        monitor = sGetWindowMonitor(parent);
        if( 0 == monitor ) return false;
    }

    // determine client size
    auto width = wcp.clientWidth;
    auto height = wcp.clientHeight;
    if (0 == width || 0 == height) {
        MONITORINFOEXA mi;
        DEVMODEA windm;
        mi.cbSize = sizeof(mi);
        windm.dmSize = sizeof(windm);
        windm.dmDriverExtra = 0;
        GN_MSW_CHECK_RETURN( ::GetMonitorInfoA( monitor, &mi ), false );
        GN_MSW_CHECK_RETURN( ::EnumDisplaySettingsA( mi.szDevice, ENUM_CURRENT_SETTINGS, &windm ), false );
        if (0 == width) width = windm.dmPelsWidth;
        if (0 == height) height = windm.dmPelsHeight;
    }

    // generate an unique window class name
    WNDCLASSEXW wcex = {};
    do {
        mClassName.format( L"GNwindowMsw_%d", rand() );
    } while( ::GetClassInfoExW( mModuleInstance, mClassName.rawptr(), &wcex ) );

    // register window class
    wcex.cbSize         = sizeof(WNDCLASSEXW);
    wcex.style          = wcp.closebox ? 0 : CS_NOCLOSE;
    wcex.lpfnWndProc    = (WNDPROC)&staticWindowProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = mModuleInstance;
    wcex.hIcon          = LoadIcon (0, IDI_APPLICATION);
    wcex.hCursor        = LoadCursor (0,IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = 0;
    wcex.lpszClassName  = mClassName.rawptr();
    wcex.hIconSm        = LoadIcon(0, IDI_APPLICATION);
    if( 0 == ::RegisterClassExW(&wcex) ) {
        GN_ERROR(sLogger)( "fail to register window class, %s!", getWin32LastErrorInfo() );
        return false;
    }
    GN_TRACE(sLogger)( "Register window class: %S (module handle: 0x%X)", mClassName.rawptr(), mModuleInstance );

    // setup window style
    DWORD exStyle = 0;
    DWORD style = 0;
    if( parent ) {
        style |= WS_CHILD;
        exStyle |= WS_EX_TOOLWINDOW;
    } else {
        if( wcp.hasBorder ) style |= WS_BORDER | WS_THICKFRAME;
        if( wcp.hasTitleBar )
            style |= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
        else
            style |= WS_POPUP;
        if( wcp.topMost ) exStyle |= WS_EX_TOPMOST;
    }

    // calculate window size
    GN_ASSERT(width > 0 && height > 0);
    RECT rc = { 0, 0, (int)width, (int)height };
    GN_MSW_CHECK_RETURN(::AdjustWindowRectEx( &rc, style, 0, exStyle ), false);

    // create window
    mWindow = ::CreateWindowExW(
        exStyle,
        mClassName.rawptr(),
        mbs2wcs(wcp.caption).rawptr(),
        style,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rc.right - rc.left,
        rc.bottom - rc.top,
        parent,
        0, // no menu
        mModuleInstance,
        0 );
    if( 0 == mWindow ) {
        GN_ERROR(sLogger)( "fail to create window, %s!", getWin32LastErrorInfo() );
        return false;
    }
    GN_TRACE(sLogger)( "Create window (handle: 0x%X)", mWindow );

    // add window handle to instance map
    GN_ASSERT(
        NULL == msInstanceMap.find(mWindow) ||
        this == *msInstanceMap.find(mWindow) );
    msInstanceMap[mWindow] = this;

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
LRESULT
GN::win::WindowMsw::windowProc( HWND wnd, UINT msg, WPARAM wp, LPARAM lp )
{
    GN_GUARD;

    switch (msg)
    {
        case WM_CLOSE :
            ::PostQuitMessage(0);
            return 0;

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
GN::win::WindowMsw::staticWindowProc( HWND wnd, UINT msg, WPARAM wp, LPARAM lp )
{
    GN_GUARD;

    //GN_TRACE(sLogger)( "GN::win::WindowMsw procedure: wnd=0x%X, msg=%s", wnd, win::msg2str(msg) );
    WindowMsw * * ppwindow = msInstanceMap.find(wnd);

    // call class specific window procedure
    if( NULL == ppwindow )
    {
        return ::DefWindowProc( wnd, msg, wp, lp );
    }
    else
    {
        GN_ASSERT( *ppwindow );
        return (*ppwindow)->windowProc( wnd, msg, wp, lp );
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
LRESULT CALLBACK
GN::win::WindowMsw::staticHookProc( int code, WPARAM wp, LPARAM lp )
{
    GN_GUARD;

    //GN_TRACE( "wnd=0x%X, msg=%s", wnd, win::msg2str(msg) );

    WindowMsw ** ppwnd = msInstanceMap.find( ((CWPSTRUCT*)lp)->hwnd );

    if( NULL != ppwnd )
    {
        // TODO: process window messages here.
        // CWPSTRUCT * cwp = (CWPSTRUCT*)lp;
        // WindowMsw * wnd = *ppwnd;
        // GN_ASSERT( cwp && wnd );
        // ProcessMessage( cwp->hwnd, cwp->message, cwp->wParam, cwp->lParam );
    }

    return ::CallNextHookEx( 0, code, wp, lp );

    GN_UNGUARD;
}

#endif
