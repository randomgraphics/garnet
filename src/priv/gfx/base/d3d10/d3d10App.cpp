#include "pch.h"

using namespace GN;

static GN::Logger * sLogger = GN::getLogger("GN.d3d10.d3d10app");

#if GN_MSVC
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3d10.lib" )
#pragma comment( lib, "dxerr.lib" )
#pragma comment( lib, "dxguid.lib" )
#if GN_DEBUG_BUILD
#pragma comment( lib, "d3dx10d.lib" )
#else
#pragma comment( lib, "d3dx10.lib" )
#endif
#endif

// *****************************************************************************
// local functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
LRESULT CALLBACK
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

        case WM_KEYUP:
            if( VK_ESCAPE == wp )
            {
                ::PostQuitMessage(0);
            }
            break;

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

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::d3d10::D3D10Application::D3D10Application()
    : mWindow(0)
    , mAdapter(0)
    , mDevice(0)
    , mSwapChain(0)
    , mDebug(0)
    , mInfoQueue(0)
{
}

//
//
// -----------------------------------------------------------------------------
GN::d3d10::D3D10Application::~D3D10Application()
{
}

//
//
// -----------------------------------------------------------------------------
int GN::d3d10::D3D10Application::run( const D3D10AppOption * )
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
bool GN::d3d10::D3D10Application::changeOption( const D3D10AppOption & o )
{
    destroyDevice();
    mOption = o;
    return createDevice();
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::d3d10::D3D10Application::init()
{
    // get primary monitor
    POINT pt = { LONG_MIN, LONG_MIN };
    mOption.monitor = ::MonitorFromPoint( pt, MONITOR_DEFAULTTOPRIMARY );

    mWindow = sCreateWindow(
            mOption.parent,
            mOption.monitor,
            mOption.width,
            mOption.height,
            mOption.fullscreen );
    if( 0 == mWindow ) return false;

    // success
    return onInit( mOption );
}

//
//
// -----------------------------------------------------------------------------
void GN::d3d10::D3D10Application::quit()
{
    onQuit();

    destroyDevice();

    sDestroyWindow( mWindow ); mWindow = 0;
}

//
//
// -----------------------------------------------------------------------------
bool GN::d3d10::D3D10Application::createDevice()
{
    PixPerfScopeEvent pixevent( 0, L"Create" );

    GN_ASSERT( IsWindow(mWindow) );
    GN_ASSERT( 0 == mDevice );

    // adjust render window
    if( !sAdjustWindow( mWindow, mOption.width, mOption.height, mOption.fullscreen ) ) return false;

    // setup creation flags
    UINT flags = 0;
#if GN_DEBUG_BUILD
    flags |= D3D10_CREATE_DEVICE_DEBUG;
#endif
    flags |= D3D10_CREATE_DEVICE_SINGLETHREADED;

    // setup swap chain descriptor
    GN_CASSERT( D3D10_SDK_VERSION >= 28 );
    DXGI_SWAP_CHAIN_DESC sd;
    ::memset( &sd, 0, sizeof(sd) );
    sd.BufferCount = 1;
    sd.BufferDesc.Width = mOption.width;
    sd.BufferDesc.Height = mOption.height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = mWindow;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = !mOption.fullscreen;

    // create device
    GN_DX10_CHECK_RV(
        D3D10CreateDeviceAndSwapChain(
            mAdapter,
            D3D10_DRIVER_TYPE_HARDWARE,
            NULL, // software module handle
            flags,
            D3D10_SDK_VERSION,
            &sd,
            &mSwapChain,
            &mDevice ),
        false );

	// setup debug and info-queue layer
	if( SUCCEEDED( mDevice->QueryInterface( IID_ID3D10Debug, (void**)&mDebug ) ) )
	{
		if( SUCCEEDED( mDebug->QueryInterface( IID_ID3D10InfoQueue, (void**)&mInfoQueue ) ) )
		{
			mInfoQueue->SetBreakOnSeverity( D3D10_MESSAGE_SEVERITY_CORRUPTION, true );
			mInfoQueue->SetBreakOnSeverity( D3D10_MESSAGE_SEVERITY_ERROR, true );
			mInfoQueue->SetBreakOnSeverity( D3D10_MESSAGE_SEVERITY_WARNING, true );
			//mInfoQueue->SetBreakOnSeverity( D3D10_MESSAGE_SEVERITY_INFO, true );
		}
	}

    // success
    return onCreate();
}

//
//
// -----------------------------------------------------------------------------
void GN::d3d10::D3D10Application::destroyDevice()
{
    if( mDevice )
    {
        mDevice->ClearState();
        onDestroy();
    }

	safeRelease( mInfoQueue );
	safeRelease( mDebug );
	safeRelease( mSwapChain );
	safeRelease( mDevice );
	safeRelease( mAdapter );
}
