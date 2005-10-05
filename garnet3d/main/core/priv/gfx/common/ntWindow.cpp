#include "pch.h"
#include "ntWindow.h"

#if GN_WINNT

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::Window::init( void * parent, uint32_t width, uint32_t height )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::Window, () );

    // create window
    mHandle = createWindow( (HWND)parent, width, height );
    if( !::IsWindow(mHandle) ) { quit(); return selfOK(); }

    // success
    return selfOK();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::Window::quit()
{
    GN_GUARD;

    // delete window
    if( ::IsWindow( mHandle ) ) ::DestroyWindow( mHandle ), mHandle = 0;

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
HWND
GN::gfx::Window::createWindow(
    HWND parent, uint32_t width, uint32_t height )
{
    GN_GUARD;

    // check parent
    if( 0 != parent && !::IsWindow(parent) )
    {
        GNGFX_ERROR( "Parameter 'parent' is not a valid window handle!" );
        return 0;
    }

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
        if( 0 == RegisterClassExA(&wcex) )
        {
            GNGFX_ERROR( "fail to register window class, %s!", getOSErrorInfo() );
            return 0;
        }
    }

    // calculate window size
    DWORD style = WS_POPUP | WS_BORDER | WS_CAPTION | WS_SIZEBOX; // sizable popup window.
    RECT rc = { 0, 0, width, height };
    AdjustWindowRect( &rc, style, 0 );

    // create window
    HWND wnd = CreateWindowA(
        sClassName,
        "", // no title
        style,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rc.right - rc.left, rc.bottom - rc.top,
        parent,
        0, // no menu
        moduleHandle,
        this );
    if( 0 == wnd )
    {
        GNGFX_ERROR( "fail to create window, %s!", getOSErrorInfo() );
        return 0;
    }

    // show the window
    ShowWindow( wnd, SW_NORMAL );
    UpdateWindow( wnd );

    mClosed = false;
    mMinimized = false;
    mInsideSizeMove = false;
    mSizeChanged = false;

    // success
    return wnd;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
LRESULT
GN::gfx::Window::windowProc(
    HWND wnd, UINT msg, WPARAM wp, LPARAM lp )
{
    GN_GUARD;

    return ::DefWindowProc( wnd, msg, wp, lp );

    GN_UNGUARD;
}

std::map<HWND, GN::gfx::Window*> GN::gfx::Window::sInstanceMap;

//
//
// -----------------------------------------------------------------------------
LRESULT
GN::gfx::Window::staticWindowProc(
    HWND wnd, UINT msg, WPARAM wp, LPARAM lp )
{
    GN_GUARD;

    // handle WM_CREATE
    if( WM_CREATE == msg && 0 != lp )
    {
        sInstanceMap[wnd] = (Window*)lp;
    }

    // find Renderer instance based on window handle
    std::map<HWND,Window*>::const_iterator iter = sInstanceMap.find(wnd);
    if( sInstanceMap.end() == iter ) return ::DefWindowProc( wnd, msg, wp, lp );

    // call Renderer specific window procedure
    return iter->second->windowProc( wnd, msg, wp, lp );

    GN_UNGUARD;
}

#endif // GN_WINNT
