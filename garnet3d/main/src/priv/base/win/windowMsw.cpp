#include "pch.h"
#include "windowMsw.h"

std::map<void*,GN::win::WindowMsw*> GN::win::WindowMsw::msInstanceMap;

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::win::WindowMsw::init( const WindowCreationParams & wcp )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::win::WindowMsw, () );

    if( !createWindow( wcp ) ) { quit(); return selfOK(); }

    // success
    return selfOK();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::win::WindowMsw::quit()
{
    GN_GUARD;

    // destroy window
    if( ::IsWindow( mWindow ) )
    {
        GN_INFO( "Destroy window (handle: 0x%X)", mWindow );
        ::DestroyWindow( mWindow );

        // remove itself from instance map
        GN_ASSERT( msInstanceMap.end() != msInstanceMap.find(mWindow) );
        msInstanceMap.erase(mWindow);
    }

    // unregister window class
    if( !mClassName.empty() )
    {
        GN_INFO( "Unregister window class: %s (module handle: 0x%X)", mClassName.cstr(), mModuleInstance );
        GN_ASSERT( mModuleInstance );
        GN_MSW_CHECK( ::UnregisterClassA( mClassName.cstr(), mModuleInstance ) );
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
GN::win::DisplayHandle GN::win::WindowMsw::getDisplayHandle() const
{
    GN_GUARD;
    GN_ASSERT( ::IsWindow( mWindow ) );
    HMONITOR m = ::MonitorFromWindow( mWindow, MONITOR_DEFAULTTONEAREST );
    if( 0 == m ) GN_ERROR( "Fail to get monitor handle from window!" );
    return (DisplayHandle)m;
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
    GN_MSW_CHECK_RV( ::GetClientRect( mWindow, &rc ), sz );
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
void GN::win::WindowMsw::resize( size_t w, size_t h )
{
    GN_GUARD;
    GN_ASSERT( ::IsWindow( mWindow ) );
    RECT rc = { 0, 0, (int)w, (int)h };
    GN_MSW_CHECK_R( ::AdjustWindowRectEx(
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

    WNDCLASSEXA wcex;

    // generate an unique window class name
    do
    {
        mClassName.format( "GNwindowMsw_%d", rand() );
    } while( ::GetClassInfoExA( mModuleInstance, mClassName.cstr(), &wcex ) );

    // register window class
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
        GN_ERROR( "fail to register window class, %s!", getOSErrorInfo() );
        return false;
    }
    GN_INFO( "Register window class: %s (module handle: 0x%X)", mClassName.cstr(), mModuleInstance );

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
    mWindow = ::CreateWindowExA(
        exStyle,
        mClassName.cstr(),
        wcp.caption.cstr(),
        style,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rc.right - rc.left, rc.bottom - rc.top,
        parent,
        0, // no menu
        mModuleInstance,
        0 );
    if( 0 == mWindow )
    {
        GN_ERROR( "fail to create window, %s!", getOSErrorInfo() );
        return false;
    }
    GN_INFO( "Create window (handle: 0x%X)", mWindow );

    // add window handle to instance map
    GN_ASSERT(
        msInstanceMap.end() == msInstanceMap.find(mWindow) ||
        this == msInstanceMap.find(mWindow)->second );
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

    //GN_INFO( "GN::win::WindowMsw procedure: wnd=0x%X, msg=%s", wnd, win::msg2str(msg) );

    std::map<void*,WindowMsw*>::const_iterator iter = msInstanceMap.find(wnd);

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
