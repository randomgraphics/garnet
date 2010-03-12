#include "pch.h"
#include "windowMsw.h"

#if GN_MSWIN && !GN_XENON

GN::HashMap<void*,GN::win::WindowMsw*> GN::win::WindowMsw::msInstanceMap;

static GN::Logger * sLogger = GN::GetLogger("GN.win.MSW");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::win::WindowMsw::Init( const WindowCreationParams & wcp )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::win::WindowMsw, () );

    if( !createWindow( wcp ) ) return Failure();

    // success
    return Success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::win::WindowMsw::Quit()
{
    GN_GUARD;

    // destroy window
    if( ::IsWindow( mWindow ) )
    {
        GN_TRACE(sLogger)( "Destroy window (handle: 0x%X)", mWindow );
        ::DestroyWindow( mWindow );

        // remove itself from instance map
        GN_ASSERT( NULL != msInstanceMap.Find(mWindow) );
        msInstanceMap.Remove(mWindow);
    }

    // unregister window class
    if( !mClassName.Empty() )
    {
        GN_TRACE(sLogger)( "Unregister window class: %S (module handle: 0x%X)", mClassName.ToRawPtr(), mModuleInstance );
        GN_ASSERT( mModuleInstance );
        GN_MSW_CHECK( ::UnregisterClassW( mClassName.ToRawPtr(), mModuleInstance ) );
        mClassName.Clear();
    }

    // standard Quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// Public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::win::MonitorHandle GN::win::WindowMsw::getMonitorHandle() const
{
    GN_GUARD;
    GN_ASSERT( ::IsWindow( mWindow ) );
    HMONITOR m = ::MonitorFromWindow( mWindow, MONITOR_DEFAULTTONEAREST );
    if( 0 == m ) GN_ERROR(sLogger)( "Fail to get monitor handle from window!" );
    return (MonitorHandle)m;
    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::Vector2<size_t> GN::win::WindowMsw::getClientSize() const
{
    GN_GUARD;
    GN_ASSERT( ::IsWindow( mWindow ) );
    Vector2<size_t> sz(0,0);
    RECT rc;
    GN_MSW_CHECK_RETURN( ::GetClientRect( mWindow, &rc ), sz );
    sz.x = (size_t)(rc.right - rc.left);
    sz.y = (size_t)(rc.bottom - rc.top);
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
void GN::win::WindowMsw::repaint()
{
    GN_GUARD;
    GN_MSW_CHECK( ::RedrawWindow(
        mWindow,
        NULL, NULL, // rc, rgn
        RDW_ERASE | RDW_FRAME | RDW_INTERNALPAINT | RDW_INVALIDATE | RDW_ALLCHILDREN ) );
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

// *****************************************************************************
// Private methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::win::WindowMsw::createWindow( const WindowCreationParams & wcp )
{
    GN_GUARD;

    // check parent
    HWND parent = (HWND)wcp.parent;
    if( 0 != parent && !::IsWindow(parent) ) parent = 0;

    // get module instance
    mModuleInstance = (HINSTANCE)GetModuleHandleA(0);
    GN_ASSERT( 0 != mModuleInstance );

    WNDCLASSEXW wcex;

    // generate an unique window class name
    do
    {
        mClassName.Format( L"GNwindowMsw_%d", rand() );
    } while( ::GetClassInfoExW( mModuleInstance, mClassName.ToRawPtr(), &wcex ) );

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
    wcex.lpszClassName  = mClassName.ToRawPtr();
    wcex.hIconSm        = LoadIcon(0, IDI_APPLICATION);
    if( 0 == ::RegisterClassExW(&wcex) )
    {
        GN_ERROR(sLogger)( "fail to register window class, %s!", GetWin32LastErrorInfo() );
        return false;
    }
    GN_TRACE(sLogger)( "Register window class: %S (module handle: 0x%X)", mClassName.ToRawPtr(), mModuleInstance );

    // setup window style
    DWORD exStyle = parent ? WS_EX_TOOLWINDOW : 0;
    DWORD style = 0;
    if( parent )
    {
        style |= WS_CHILD;
        exStyle |= WS_EX_TOOLWINDOW;
    }
    else
    {
        style |=  WS_OVERLAPPED;
    }
    if( wcp.hasBorder ) style |= WS_BORDER | WS_THICKFRAME;
    if( wcp.hasTitleBar ) style |= WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
    if( wcp.topMost ) exStyle |= WS_EX_TOPMOST;

    // calculate window size
    RECT rc = { 0, 0, (int)wcp.clientWidth, (int)wcp.clientHeight };
    ::AdjustWindowRectEx( &rc, style, 0, exStyle );

    // create window
    mWindow = ::CreateWindowExW(
        exStyle,
        mClassName.ToRawPtr(),
        Mbs2Wcs(wcp.caption).ToRawPtr(),
        style,
        CW_USEDEFAULT, CW_USEDEFAULT,
        wcp.clientWidth ? (rc.right - rc.left) : CW_USEDEFAULT,
        wcp.clientHeight ? (rc.bottom - rc.top) : CW_USEDEFAULT,
        parent,
        0, // no menu
        mModuleInstance,
        0 );
    if( 0 == mWindow )
    {
        GN_ERROR(sLogger)( "fail to create window, %s!", GetWin32LastErrorInfo() );
        return false;
    }
    GN_TRACE(sLogger)( "Create window (handle: 0x%X)", mWindow );

    // add window handle to instance map
    GN_ASSERT(
        NULL == msInstanceMap.Find(mWindow) ||
        this == *msInstanceMap.Find(mWindow) );
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
    WindowMsw * * ppwindow = msInstanceMap.Find(wnd);

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

#endif
