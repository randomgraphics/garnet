#include "pch.h"

using namespace GN;

static GN::Logger * sLogger = GN::getLogger("GN.d3d9.d3d9app");

#if GN_MSVC
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )
#pragma comment( lib, "dxerr9.lib" )
#endif

// *****************************************************************************
// local functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static LRESULT CALLBACK
sStaticWindowProc( HWND wnd, UINT msg, WPARAM wp, LPARAM lp )
{
    GN_GUARD;

    switch (msg)
    {
        case WM_CLOSE :
            ::PostQuitMessage(0);
            return 0;

        case WM_ERASEBKGND:
            return 0;

        default: ; // do nothing
    }

    return ::DefWindowProc( wnd, msg, wp, lp );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static HWND sCreateWindow( HWND parent, HMONITOR monitor, UInt32 width, UInt32 height, bool fullscreen )
{
    // check parent
    if( !::IsWindow(parent) ) parent = 0;

    HINSTANCE module = (HINSTANCE)GetModuleHandleW(0);
    GN_ASSERT( 0 != module );

    WNDCLASSEXW wcex;

    const wchar_t * classname = L"d3dapp_mainwindow";

    // register window class
    GN_TRACE(sLogger)( "Register window class: %S (module handle: 0x%X)", classname, module );
    wcex.cbSize         = sizeof(wcex);
    wcex.style          = 0;
    wcex.lpfnWndProc    = (WNDPROC)&sStaticWindowProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = module;
    wcex.hIcon          = LoadIcon( 0, IDI_APPLICATION );
    wcex.hCursor        = LoadCursor( 0, IDC_ARROW );
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = 0;
    wcex.lpszClassName  = classname;
    wcex.hIconSm        = LoadIcon( 0, IDI_APPLICATION );
    if( 0 == ::RegisterClassExW(&wcex) )
    {
        GN_ERROR(sLogger)( "fail to register window class, %s!", getOSErrorInfo() );
        return 0;
    }

    // setup window style
    DWORD style = fullscreen ? WS_POPUP : WS_OVERLAPPEDWINDOW;
    DWORD exStyle = parent ? WS_EX_TOOLWINDOW : 0;

    // get monitor's working area rectangle
    MONITORINFO mi;
    mi.cbSize = sizeof(mi);
    GN_MSW_CHECK_RV( GetMonitorInfoW( monitor, &mi ), false );

    // calculate window size
    RECT rc = { 0, 0, width, height };
    ::AdjustWindowRectEx( &rc, style, 0, exStyle );

    // create window
    HWND hwnd = ::CreateWindowExW(
        exStyle,
        classname,
        L"D3DApp",
        style,
        mi.rcWork.left, mi.rcWork.top,
        rc.right - rc.left, rc.bottom - rc.top,
        parent,
        0, // no menu
        module,
        0 );
    if( 0 == hwnd )
    {
        GN_ERROR(sLogger)( "fail to create window, %s!", getOSErrorInfo() );
        return false;
    }

    // show the window
    ::ShowWindow( hwnd, SW_NORMAL );
    ::UpdateWindow( hwnd );

    // success
    return hwnd;
}

//
//
// -----------------------------------------------------------------------------
static bool sAdjustWindow( HWND window, UInt32 width, UInt32 height, bool fullscreen )
{
    DWORD style = fullscreen ? WS_POPUP : WS_OVERLAPPEDWINDOW;
    style |= WS_VISIBLE;

    // modify render window style
    SetWindowLong( window, GWL_STYLE, style );

    // calculate boundary size
    RECT rc = { 0, 0, width, height };
    GN_MSW_CHECK_RV(
        ::AdjustWindowRectEx(
            &rc,
            style,
            0,
            ::GetWindowLongA( window, GWL_EXSTYLE ) ),
        false );

    // resize the window
    GN_MSW_CHECK_RV(
        ::SetWindowPos(
            window, HWND_TOP,
            0, 0, // position, ignored.
            rc.right-rc.left, rc.bottom-rc.top, // size
            SWP_NOMOVE ),
        false );

    // success
    return true;
}


static void sDestroyWindow( HWND hwnd )
{
    if( IsWindow(hwnd) ) DestroyWindow( hwnd );
}

//
//
// ------------------------------------------------------------------------
static bool
sSetupD3dpp( D3DPRESENT_PARAMETERS & d3dpp,
             HWND window,
             IDirect3D9 & d3d,
             UINT adapter,
             D3DDEVTYPE devtype,
             const GN::d3d9::D3D9AppOption & o )
{
    GN_UNUSED_PARAM( d3d );
    GN_UNUSED_PARAM( adapter );
    GN_UNUSED_PARAM( devtype );

    // clear all field, first
    ZeroMemory( &d3dpp, sizeof(d3dpp) );

    // setup depth parameters
    d3dpp.EnableAutoDepthStencil = true;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
    d3dpp.Flags                 |= D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;

    // set display mode parameters
    d3dpp.Windowed = !o.fullscreen;
    if( o.fullscreen )
    {
        d3dpp.BackBufferCount  = 0;
        d3dpp.BackBufferWidth  = o.fsWidth;
        d3dpp.BackBufferHeight = o.fsHeight;
        d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
    }
    else
    {
        d3dpp.BackBufferCount  = 0;
        d3dpp.BackBufferWidth  = o.windowedWidth;
        d3dpp.BackBufferHeight = o.windowedHeight;
        d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    }

    // set other parameters
    d3dpp.SwapEffect           = D3DSWAPEFFECT_DISCARD;
    d3dpp.PresentationInterval = o.vsync ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;
    d3dpp.hDeviceWindow        = window;

    // success
    return true;
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::d3d9::D3D9Application::D3D9Application()
    : mWindow(0)
    , mD3D(0)
    , mDevice(0)
    , mRunning(false)
{
}

//
//
// -----------------------------------------------------------------------------
GN::d3d9::D3D9Application::~D3D9Application()
{
}

//
//
// -----------------------------------------------------------------------------
int GN::d3d9::D3D9Application::run( const D3D9AppOption * )
{
    GN_GUARD_ALWAYS;

    if( !init() ) { quit(); return -1; }

    if( !changeOption(mOption) ) { quit(); return -1; }

    // message loop
    MSG msg;
    while( true )
    {
        if( ::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
        {
            if( WM_QUIT == msg.message )
            {
                quit();
                return 0;
            }
            ::TranslateMessage( &msg );
            ::DispatchMessage(&msg);
        }
        else if( ::IsIconic( mWindow ) )
        {
            GN_TRACE(sLogger)( "Wait for window messages..." );
            ::WaitMessage();
        }
        else
        {
            // Idle time, do rendering and update
            onDraw();
        }
    }

    // done
    quit();
    return 0;

    GN_UNGUARD_ALWAYS_NO_THROW;

    quit();
    return -1;
}

//
//
// -----------------------------------------------------------------------------
bool GN::d3d9::D3D9Application::changeOption( const D3D9AppOption & o )
{
    disposeDevice();
    destroyDevice();
    mOption = o;
    return createDevice() && restoreDevice();
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::d3d9::D3D9Application::init()
{
    // get primary monitor
    POINT pt = { LONG_MIN, LONG_MIN };
    mOption.monitor = ::MonitorFromPoint( pt, MONITOR_DEFAULTTOPRIMARY );

    mWindow = sCreateWindow(
            mOption.parent,
            mOption.monitor,
            mOption.windowedWidth,
            mOption.windowedHeight,
            mOption.fullscreen );
    if( 0 == mWindow ) return false;

    mD3D = Direct3DCreate9( D3D_SDK_VERSION );
    if( 0 == mD3D )
    {
        GN_ERROR(sLogger)( "fail to create D3D instance!" );
        return false;
    }

    // success
    mRunning = true;
    return onInit( mOption );
}

//
//
// -----------------------------------------------------------------------------
void GN::d3d9::D3D9Application::quit()
{
    mRunning = false;

    disposeDevice();
    destroyDevice();

    onQuit();

    safeRelease( mD3D );

    sDestroyWindow( mWindow ); mWindow = 0;
}

//
//
// -----------------------------------------------------------------------------
bool GN::d3d9::D3D9Application::createDevice()
{
    PixPerfScopeEvent pixevent( 0, "Create" );

    GN_ASSERT( IsWindow(mWindow) );
    GN_ASSERT( 0 == mDevice );

    // Initiate adapter ID
    mAdapter = 0;

    mDeviceType = D3DDEVTYPE_HAL;

	// Look for nvidia adapter
    UINT nAdapter = mD3D->GetAdapterCount();
    GN_ASSERT( nAdapter );
    for( UInt32 i = 0; i < nAdapter; ++i )
    {
        D3DADAPTER_IDENTIFIER9 Identifier;
        GN_DX9_CHECK( mD3D->GetAdapterIdentifier( i, 0, &Identifier ) );
        GN_TRACE(sLogger)( "Enumerating D3D adapters: %s", Identifier.Description );
        if( strstr(Identifier.Description,"PerfHUD") )
        {
            GN_TRACE(sLogger)( "Found NVPerfHUD adapter. We will create D3D device using NVPerfHUD adapter." );
            mAdapter = i;
            mDeviceType = D3DDEVTYPE_REF;
            break;
        }
    }

    // Look for an adapter ordinal that is tied to a HMONITOR
    if( 0 == mAdapter && 0 != mOption.monitor )
    {
        for( UINT i = 0; i < nAdapter; ++i )
        {
            if( mD3D->GetAdapterMonitor( i ) == mOption.monitor )
            {
                mAdapter = i;
                break;
            }
        }
    }

    // init d3d present parameters
    if( !sSetupD3dpp( mPresentParameters, mWindow, *mD3D, mAdapter, mDeviceType, mOption ) ) return false;

    // define device behavior
    mBehavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;

    // device found, create it!
    GN_DX9_CHECK_RV(
        mD3D->CreateDevice(
            mAdapter,
            mDeviceType,
            mWindow,
            mBehavior,
            &mPresentParameters,
            &mDevice ),
        false );

    // success
    return onCreate();
}

//
//
// -----------------------------------------------------------------------------
bool GN::d3d9::D3D9Application::restoreDevice()
{
    PixPerfScopeEvent pixevent( 0, "Restore" );

    GN_ASSERT( mWindow );
    GN_ASSERT( mDevice );

    UInt32 w = mOption.fullscreen ? mOption.fsWidth : mOption.windowedWidth;
    UInt32 h = mOption.fullscreen ? mOption.fsHeight : mOption.windowedHeight;
    sAdjustWindow( mWindow, w, h, mOption.fullscreen );

    if( !sSetupD3dpp( mPresentParameters, mWindow, *mD3D, mAdapter, mDeviceType, mOption ) ) return false;
    GN_DX9_CHECK_RV( mDevice->Reset( &mPresentParameters ), false );

    // success
    return onRestore();
}

//
//
// -----------------------------------------------------------------------------
void GN::d3d9::D3D9Application::disposeDevice()
{
    if( mDevice )
    {
        onDispose();
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::d3d9::D3D9Application::destroyDevice()
{
    if( mDevice )
    {
        onDestroy();
        mDevice->Release();
        mDevice = 0;
    }
}
