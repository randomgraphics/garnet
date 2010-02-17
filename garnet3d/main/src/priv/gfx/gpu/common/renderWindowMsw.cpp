#include "pch.h"
#include "renderWindowMsw.h"

#if GN_MSWIN && !GN_XENON

std::map<void*,GN::gfx::RenderWindowMsw*> GN::gfx::RenderWindowMsw::msInstanceMap;

static GN::Logger * sLogger = GN::GetLogger("GN.gfx.gpu.common.renderWindow.MSW");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************


//
//
// -----------------------------------------------------------------------------
bool GN::gfx::RenderWindowMsw::initExternalWindow( Gpu * gpu, HandleType externalWindow )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::RenderWindowMsw, () );

    if( !gpu )
    {
        GN_ERROR(sLogger)( "Null renderer pointer." );
        return failure();
    }

    if( !::IsWindow( (HWND)externalWindow ) )
    {
        GN_ERROR(sLogger)( "External render window handle must be valid." );
        return failure();
    }

    if( msInstanceMap.end() != msInstanceMap.find( (HWND)externalWindow ) )
    {
        GN_ERROR(sLogger)( "You can't create multiple render window instance for single window handle." );
        return failure();
    }

    // register a message hook to render window.
    mHook = ::SetWindowsHookEx( WH_CALLWNDPROC, &staticHookProc, 0, GetCurrentThreadId() );
    if( 0 == mHook )
    {
        GN_ERROR(sLogger)( "Fail to setup message hook : %s", GetWin32LastErrorInfo() );
        return failure();
    }

    mGpu          = gpu;
    mWindow            = (HWND)externalWindow;
    mUseExternalWindow = true;

    // do post initialize tasks
    if( !postInit() ) return failure();

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::RenderWindowMsw::initInternalWindow(
    Gpu * gpu,
    HandleType parentWindow,
    HandleType monitor,
    UInt32     width,
    UInt32     height )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::RenderWindowMsw, () );

    GN_ASSERT( 0 != monitor && width > 0 && height > 0 );

    if( !createWindow( (HWND)parentWindow, (HMONITOR)monitor, width, height ) ) return failure();

    mGpu = gpu;
    mUseExternalWindow = false;

    // success
    if( !postInit() ) return failure();

    // success
    return success();

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

    // tru unregister window class
    if( !mClassName.Empty() )
    {
        GN_VERBOSE(sLogger)( "Unregister window class: %ls (module handle: 0x%X)", mClassName.GetRawPtr(), mModuleInstance );
        GN_ASSERT( mModuleInstance );
        GN_MSW_CHECK( ::UnregisterClassW( mClassName.GetRawPtr(), mModuleInstance ) );
        mClassName.Clear();
    }

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::RenderWindowMsw::getClientSize( UInt32 & width, UInt32 & height ) const
{
    GN_ASSERT( ::IsWindow(mWindow) );

    width = height = 0;

    RECT rc;
    GN_MSW_CHECK_RETURN_VOID( ::GetClientRect( mWindow, &rc ) );

    width = (UINT)(rc.right - rc.left);
    height = (UINT)(rc.bottom - rc.top);
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::RenderWindowMsw::handleSizeMove()
{
    GN_GUARD;

    const GpuOptions & ro = mGpu->getOptions();

    // do nothing if in full screen mode
    if( ro.fullscreen ) return;

    // get client window size
    UInt32 currentWidth, currentHeight;
    getClientSize( currentWidth, currentHeight );

    // compare with old window properties
    if( currentWidth  != mOldWidth ||
        currentHeight != mOldHeight ||
        mMonitor      != mOldMonitor )
    {
        mOldWidth   = currentWidth;
        mOldHeight  = currentHeight;
        mOldMonitor = mMonitor;

        // trigger renderer signal when window size is changed or window is moved to another monitor
        mGpu->getSignals().rendererWindowSizeMove( mMonitor, currentWidth, currentHeight );
    }

    GN_UNGUARD;
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
        GN_ERROR(sLogger)( "Fail to get monitor handle from window!" );
        return false;
    }

    //// Output monitor information
    //MONITORINFOEXA mi;
    //mi.cbSize = sizeof(mi);
    //GN_MSW_CHECK( ::GetMonitorInfoA( mMonitor, &mi ) );
    //GN_TRACE( "窗口所在的设备名：%s", mi.szDevice );

    // add window handle to instance map
    GN_ASSERT(
        msInstanceMap.end() == msInstanceMap.find(mWindow) ||
        this == msInstanceMap.find(mWindow)->second );
    msInstanceMap[mWindow] = this;

    // clear all state flags
    mInsideSizeMove = false;

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool
GN::gfx::RenderWindowMsw::createWindow( HWND parent, HMONITOR monitor, UInt32 width, UInt32 height )
{
    GN_GUARD;

    // check parent
    if( 0 != parent && !::IsWindow(parent) ) parent = 0;

    mModuleInstance = (HINSTANCE)GetModuleHandleW(0);
    GN_ASSERT( 0 != mModuleInstance );

    WNDCLASSEXW wcex;

    // generate an unique window class name
    do
    {
        mClassName.Format( L"GNgfxRenderWindow_%d", rand() );
    } while( ::GetClassInfoExW( mModuleInstance, mClassName.GetRawPtr(), &wcex ) );

    // register window class
    GN_VERBOSE(sLogger)( "Register window class: %ls (module handle: 0x%X)", mClassName.GetRawPtr(), mModuleInstance );
    wcex.cbSize         = sizeof(wcex);
    wcex.style          = 0;
    wcex.lpfnWndProc    = (WNDPROC)&staticWindowProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = mModuleInstance;
    wcex.hIcon          = LoadIcon (0, IDI_APPLICATION);
    wcex.hCursor        = LoadCursor (0,IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = 0;
    wcex.lpszClassName  = mClassName.GetRawPtr();
    wcex.hIconSm        = LoadIcon(0, IDI_APPLICATION);
    if( 0 == ::RegisterClassExW(&wcex) )
    {
        GN_ERROR(sLogger)( "fail to register window class, %s!", GetWin32LastErrorInfo() );
        return false;
    }

    // setup window style
    DWORD style = WS_OVERLAPPEDWINDOW;
    DWORD exStyle = parent ? WS_EX_TOOLWINDOW : 0;

    // get monitor's working area rectangle
    MONITORINFO mi;
    mi.cbSize = sizeof(mi);
    GN_MSW_CHECK_RETURN( GetMonitorInfoW( monitor, &mi ), false );

    // calculate window size
    RECT rc = { 0, 0, width, height };
    ::AdjustWindowRectEx( &rc, style, 0, exStyle );

    // create window
    mWindow = ::CreateWindowExW(
        exStyle,
        mClassName.GetRawPtr(),
        L"Garnet Render Window",
        style,
        mi.rcWork.left, mi.rcWork.top,
        rc.right - rc.left, rc.bottom - rc.top,
        parent,
        0, // no menu
        mModuleInstance,
        0 );
    if( 0 == mWindow )
    {
        GN_ERROR(sLogger)( "fail to create window, %s!", GetWin32LastErrorInfo() );
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

    bool resizedOrMoved = false;

    switch(msg)
    {
        case WM_CLOSE:
            // do not close the window. just trigger the signal
            GN_ASSERT( mGpu );
            mGpu->getSignals().rendererWindowClose();
            break;

        case WM_ENTERSIZEMOVE :
            mInsideSizeMove = true;
            break;

        case WM_EXITSIZEMOVE :
            mInsideSizeMove = false;
            break;

        case WM_SIZE :
            {
                //GN_TRACE( "window resize to %dx%d", LOWORD(lp), HIWORD(lp) ) );
                bool minimized = ( SIZE_MINIMIZED == wp );
                if( !minimized && !mInsideSizeMove ) resizedOrMoved = true;
            }
            break;

        case WM_MOVE :
            {
                if( !mInsideSizeMove ) resizedOrMoved = true;
            }
            break;

        default: ; // do nothing
    }

    //
    // Update monitor handle
    //
    if( resizedOrMoved )
    {
        // Update monitor handle
        mMonitor = ::MonitorFromWindow( wnd, MONITOR_DEFAULTTONEAREST );
        if( 0 == mMonitor )
        {
            GN_ERROR(sLogger)( "Fail to get monitor handle from window handle!" );
        }

        //// Output monitor information
        //MONITORINFOEXA mi;
        //mi.cbSize = sizeof(mi);
        //GN_MSW_CHECK( ::GetMonitorInfoA( mMonitor, &mi ) );
        //GN_TRACE( "窗口所在的设备名：%s", mi.szDevice );
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

    //GN_TRACE( "GN::gfx::RenderWindowMsw procedure: wnd=0x%X, msg=%s", wnd, win::msg2str(msg) );

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

    //GN_TRACE( "wnd=0x%X, msg=%s", wnd, win::msg2str(msg) );

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
        GN_ERROR(sLogger)( "Input window handle is invalid!" );
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

#endif // GN_MSWIN && !GN_XENON
