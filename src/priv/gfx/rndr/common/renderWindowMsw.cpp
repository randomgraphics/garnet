#include "pch.h"
#include "renderWindowMsw.h"

#if GN_MSWIN && !GN_XENON

std::map<void*,GN::gfx::RenderWindowMsw*> GN::gfx::RenderWindowMsw::msInstanceMap;

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::RenderWindowMsw::initExternalRenderWindow( HandleType, HandleType externalWindow )
{
    GN_GUARD;

    quit();

    if( !::IsWindow( (HWND)externalWindow ) )
    {
        GN_ERROR( "External render window handle must be valid." );
        return false;
    }

    if( msInstanceMap.end() != msInstanceMap.find( (HWND)externalWindow ) )
    {
        GN_ERROR( "You can't create multiple render window instance for single window handle." );
        return false;
    }

    // register a message hook to render window.
    mHook = ::SetWindowsHookEx( WH_CALLWNDPROC, &staticHookProc, 0, GetCurrentThreadId() );
    if( 0 == mHook )
    {
        GN_ERROR( "Fail to setup message hook : %s", getOSErrorInfo() );
        return false;
    }

    mWindow = (HWND)externalWindow;
    mUseExternalWindow = true;

    // success
    return postInit();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::RenderWindowMsw::initInternalRenderWindow(
    HandleType, HandleType parentWindow, HandleType monitor, uint32_t width, uint32_t height )
{
    GN_GUARD;

    GN_ASSERT( 0 != monitor && width > 0 && height > 0 );

    if( !mUseExternalWindow && mWindow && parentWindow == ::GetParent(mWindow) )
    {
        // calculate boundary size
        RECT rc = { 0, 0, width, height };
        GN_MSW_CHECK_RV(
            ::AdjustWindowRectEx(
                &rc,
                ::GetWindowLongA( mWindow, GWL_STYLE ),
                0,
                ::GetWindowLongA( mWindow, GWL_EXSTYLE ) ),
            false );

        // resize the window
        GN_MSW_CHECK_RV(
            ::SetWindowPos(
                mWindow, HWND_TOP,
                0, 0, // position, ignored.
                rc.right-rc.left, rc.bottom-rc.top, // size
                SWP_NOMOVE ),
            false );
    }
    else
    {
        quit();
        if( !createWindow( (HWND)parentWindow, (HMONITOR)monitor, width, height ) ) return false;
    }

    mUseExternalWindow = false;

    // success
    return postInit();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::RenderWindowMsw::quit()
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
        GN_INFO( "Unregister window class: %s (module handle: 0x%X)", mClassName.cptr(), mModuleInstance );
        GN_ASSERT( mModuleInstance );
        GN_MSW_CHECK( ::UnregisterClassA( mClassName.cptr(), mModuleInstance ) );
        mClassName.clear();
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::RenderWindowMsw::getClientSize( uint32_t & width, uint32_t & height ) const
{
    if( !::IsWindow(mWindow) )
    {
        GN_ERROR( "RenderWindowMsw class is yet to initialized!" );
        return false;
    }

    RECT rc;
    GN_MSW_CHECK_RV( ::GetClientRect( mWindow, &rc ), false );

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
bool GN::gfx::RenderWindowMsw::postInit()
{
    GN_GUARD;

    GN_ASSERT( ::IsWindow(mWindow) );

    // update monitor handle
    mMonitor = ::MonitorFromWindow( mWindow, MONITOR_DEFAULTTONEAREST );
    if( 0 == mMonitor )
    {
        GN_ERROR( "Fail to get monitor handle from window!" );
        return false;
    }

    //// Output monitor information
    //MONITORINFOEXA mi;
    //mi.cbSize = sizeof(mi);
    //GN_MSW_CHECK( ::GetMonitorInfoA( mMonitor, &mi ) );
    //GN_INFO( "窗口所在的设备名：%s", mi.szDevice );

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
bool
GN::gfx::RenderWindowMsw::createWindow( HWND parent, HMONITOR monitor, uint32_t width, uint32_t height )
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
    } while( ::GetClassInfoExA( mModuleInstance, mClassName.cptr(), &wcex ) );

    // register window class
    GN_INFO( "Register window class: %s (module handle: 0x%X)", mClassName.cptr(), mModuleInstance );
    wcex.cbSize         = sizeof(WNDCLASSEX);
    wcex.style          = 0;
    wcex.lpfnWndProc    = (WNDPROC)&staticWindowProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = mModuleInstance;
    wcex.hIcon          = LoadIcon (0, IDI_APPLICATION);
    wcex.hCursor        = LoadCursor (0,IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = 0;
    wcex.lpszClassName  = mClassName.cptr();
    wcex.hIconSm        = LoadIcon(0, IDI_APPLICATION);
    if( 0 == ::RegisterClassExA(&wcex) )
    {
        GN_ERROR( "fail to register window class, %s!", getOSErrorInfo() );
        return false;
    }

    // setup window style
    DWORD style = WS_OVERLAPPEDWINDOW;
    DWORD exStyle = parent ? WS_EX_TOOLWINDOW : 0;

    // get monitor's working area rectangle
    MONITORINFO mi;
    mi.cbSize = sizeof(mi);
    GN_MSW_CHECK_RV( GetMonitorInfoA( monitor, &mi ), false );

    // calculate window size
    RECT rc = { 0, 0, width, height };
    ::AdjustWindowRectEx( &rc, style, 0, exStyle );

    // create window
    mWindow = ::CreateWindowExA(
        exStyle,
        mClassName.cptr(),
        "", // no title
        style,
        mi.rcWork.left, mi.rcWork.top,
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
GN::gfx::RenderWindowMsw::handleMessage( HWND wnd, UINT msg, WPARAM wp, LPARAM lp )
{
    GN_GUARD;

    switch(msg)
    {
        case WM_CLOSE:
            // do not close the window. just trigger the signal
            Renderer::sSigWindowClosing();
            break;

        case WM_ENTERSIZEMOVE :
            mInsideSizeMove = true;
            break;

        case WM_EXITSIZEMOVE :
            mInsideSizeMove = false;
            mSizeChanged = true;
            break;

        case WM_SIZE :
            {
                //GN_INFO( "window resize to %dx%d", LOWORD(lp), HIWORD(lp) ) );
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
            GN_ERROR( "Fail to get monitor handle from window handle!" );
        }

        //// Output monitor information
        //MONITORINFOEXA mi;
        //mi.cbSize = sizeof(mi);
        //GN_MSW_CHECK( ::GetMonitorInfoA( mMonitor, &mi ) );
        //GN_INFO( "窗口所在的设备名：%s", mi.szDevice );
    }

    // trigger the message signal
    sigMessage( wnd, msg, wp, lp );

    GN_UNGUARD;
}


//
//
// -----------------------------------------------------------------------------
LRESULT
GN::gfx::RenderWindowMsw::windowProc( HWND wnd, UINT msg, WPARAM wp, LPARAM lp )
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
GN::gfx::RenderWindowMsw::staticWindowProc( HWND wnd, UINT msg, WPARAM wp, LPARAM lp )
{
    GN_GUARD;

    //GN_INFO( "GN::gfx::RenderWindowMsw procedure: wnd=0x%X, msg=%s", wnd, win::msg2str(msg) );

    std::map<void*,RenderWindowMsw*>::const_iterator iter = msInstanceMap.find(wnd);

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
GN::gfx::RenderWindowMsw::staticHookProc( int code, WPARAM wp, LPARAM lp )
{
    GN_GUARD;

    //GN_INFO( "wnd=0x%X, msg=%s", wnd, win::msg2str(msg) );

    std::map<void*,RenderWindowMsw*>::const_iterator iter =
        msInstanceMap.find( ((CWPSTRUCT*)lp)->hwnd );

    if( msInstanceMap.end() != iter )
    {
        // trigger render window message signal.
        CWPSTRUCT * cwp = (CWPSTRUCT*)lp;
        RenderWindowMsw * wnd = iter->second;
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
    GN_MSW_CHECK( ::GetWindowRect( hwnd, &mBoundsRect ) );
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
        GN_MSW_CHECK( ::SetMenu( mWindow, mMenu ) );
    }
    ::SetWindowLong( mWindow, GWL_STYLE, mStyle );
    ::SetWindowLong( mWindow, GWL_EXSTYLE, mExStyle );
    GN_MSW_CHECK( ::SetParent( mWindow, mParent ) );
    GN_MSW_CHECK( ::SetWindowPos(
        mWindow,
        WS_EX_TOPMOST & mExStyle ? HWND_TOPMOST : HWND_NOTOPMOST,
        mBoundsRect.left, mBoundsRect.top,
        mBoundsRect.right-mBoundsRect.left,
        mBoundsRect.bottom-mBoundsRect.top,
        SWP_FRAMECHANGED | SWP_NOOWNERZORDER | SWP_SHOWWINDOW ) );
    GN_MSW_CHECK( ::UpdateWindow( mWindow ) );

    mWindow = 0;

    GN_UNGUARD;
}


#endif // GN_MSWIN
