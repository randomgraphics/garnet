#include "pch.h"
#include "garnet/base/ntWindow.h"

#if GN_MSWIN && !GN_XENON

// *****************************************************************************
// Public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::NTWindow::create( const CreateParam & cp )
{
    GN_GUARD;

    destroy(); // destroy old widow

    // check parent
    HWND parent = ::IsWindow((HWND)cp.parent) ? (HWND)cp.parent : 0;

    WNDCLASSEXA wcex;

    // get instance handle
    mInstanceHandle = (HINSTANCE)::GetModuleHandle(0);

    // generate an unique window class name
    do
    {
        mClassName.format( "GNbaseWindowWrapper_%d", rand() );
    } while( ::GetClassInfoExA( mInstanceHandle, mClassName.cstr(), &wcex ) );

    // register window class
    GN_INFO( "Register window class: %s (module handle: 0x%X)",
        mClassName.cstr(), mInstanceHandle );
    wcex.cbSize         = sizeof(WNDCLASSEX);
    wcex.style          = 0;//CS_NOCLOSE;
    wcex.lpfnWndProc    = (WNDPROC)&sMsgRouter;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = mInstanceHandle;
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

    // setup window style
    DWORD style;
    DWORD exStyle;
    if( parent )
    {
        if( cp.style )
            style = cp.style;
        else
            style = WS_POPUP | WS_BORDER | WS_CAPTION | WS_SIZEBOX;
        exStyle = WS_EX_TOOLWINDOW;
    }
    else
    {
        if( cp.style )
            style = cp.style;
        else
            style = WS_OVERLAPPEDWINDOW;
        exStyle = 0;
    }

    // calculate window size
    RECT rc = { 0, 0, cp.clientWidth, cp.clientHeight };
    ::AdjustWindowRectEx( &rc, style, 0, exStyle );

    // create window
    mWindow = ::CreateWindowExA(
        exStyle,
        mClassName.cstr(),
        cp.title,
        style,
        cp.left, cp.top,
        rc.right - rc.left, rc.bottom - rc.top,
        parent,
        (HMENU)cp.menu,
        mInstanceHandle,
        this );
    if( 0 == mWindow )
    {
        GN_ERROR( "fail to create window, %s!", getOSErrorInfo() );
        return false;
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::NTWindow::destroy()
{
    GN_GUARD;

    if( ::IsWindow( mWindow ) )
    {
        GN_WIN_CHECK( ::DestroyWindow( mWindow ) );
    }
    mWindow = 0;

    if( !mClassName.empty() )
    {
        GN_INFO( "Unregister window class: %s (module handle: 0x%X)",
            mClassName.cstr(), mInstanceHandle );
        GN_WIN_CHECK( ::UnregisterClassA( mClassName.cstr(), mInstanceHandle ) );
        mClassName.clear();
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::NTWindow::showWindow( bool show ) const
{
    GN_GUARD;
    if( !::IsWindow( mWindow ) )
    {
        GN_ERROR( "Window class is yet to initialized!" );
        return;
    }
    ::ShowWindow( mWindow, show ? SW_NORMAL : SW_HIDE );
    ::UpdateWindow( mWindow );
    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
HMONITOR GN::NTWindow::getMonitor() const
{
    GN_GUARD;

    if( !::IsWindow(mWindow) )
    {
        GN_ERROR( "Window class is yet to initialized!" );
        return 0;
    }
    return ::MonitorFromWindow( mWindow, MONITOR_DEFAULTTONEAREST );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::NTWindow::getClientSize( uint32_t & width, uint32_t & height ) const
{
    GN_GUARD;

    if( !::IsWindow(mWindow) )
    {
        GN_ERROR( "Impl class is yet to initialized!" );
        return false;
    }

    RECT rc;
    GN_WIN_CHECK_RV( ::GetClientRect( mWindow, &rc ), false );

    width = (UINT)(rc.right - rc.left);
    height = (UINT)(rc.bottom - rc.top);

    return true;

    GN_UNGUARD;
}

// *****************************************************************************
// Private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
LRESULT GN::NTWindow::winProc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp )
{
    GN_GUARD;
    if( mWinProc )
        return mWinProc( hwnd, msg, wp, lp );
    else
        return ::DefWindowProc( hwnd, msg, wp, lp );
    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
LRESULT CALLBACK GN::NTWindow::sMsgRouter( HWND wnd, UINT msg, WPARAM wp, LPARAM lp )
{
    GN_GUARD;

    //GN_INFO( "GN::NTWindow procedure: wnd=0x%X, msg=%s", wnd, GN::winMsg2Str(msg) );

    NTWindow * ptr;

    // handle WM_NCCREATE
    if( WM_NCCREATE == msg )
    {
        GN_ASSERT( lp );
        ptr = (NTWindow*)((CREATESTRUCT*)lp)->lpCreateParams;
        ::SetWindowLongPtrA( wnd, GWLP_USERDATA, (LONG_PTR)ptr );
    }
    else
    {
        ptr = (NTWindow*)::GetWindowLongPtrA( wnd, GWLP_USERDATA );
    }

    // call Window specific procedure
    return ptr
        ? ptr->winProc( wnd, msg, wp, lp )
        : ::DefWindowProc( wnd, msg, wp, lp );

    GN_UNGUARD;
}

#endif // GN_MSWIN && !GN_XENON
