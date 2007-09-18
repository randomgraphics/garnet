#include "pch.h"
#include "garnet/GNinput.h"

using namespace GN;

static GN::Logger * sLogger = GN::getLogger("GN.d3d10.d3d10app");

#if GN_MSVC
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3d10.lib" )
#pragma comment( lib, "dxgi.lib" )
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

void sPrintDeviceInfo( const DXGI_SWAP_CHAIN_DESC & scd )
{
    GN_INFO(sLogger)(
        "\n\n"
        "===================================================\n"
        "        D3D10 Implementation Information\n"
        "---------------------------------------------------\n"
        "    Backbuffer Size                : %d,%d\n"
        "    Fullscreen                     : %s\n"
        "    MSAA Sample Count              : %d\n"
        "    MSAA Sample Quality            : %d\n"
        "===================================================\n"
        "\n\n",
        scd.BufferDesc.Width,
        scd.BufferDesc.Height,
        scd.Windowed ? "False" : "True",
        scd.SampleDesc.Count,
        scd.SampleDesc.Quality
        );
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

//
//
// -----------------------------------------------------------------------------
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
    , mBackRTV(0)
    , mDepthDSV(0)
    , mDebug(0)
    , mInfoQueue(0)
{
    input::createInputSystem();
}

//
//
// -----------------------------------------------------------------------------
GN::d3d10::D3D10Application::~D3D10Application()
{
    if( gInputPtr ) delete gInputPtr;
}

#include <conio.h>

//
//
// -----------------------------------------------------------------------------
int GN::d3d10::D3D10Application::run( const D3D10AppOption & o )
{
    GN_GUARD_ALWAYS;

    mOption = o;

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
            // process input message
            if( gInputPtr )
            {
                gInput.processInputEvents();
            }

            // Idle time, do rendering and update
            onUpdate();
            onDraw();

            // present()
            mSwapChain->Present( 0, 0 );
        }
    }

    // done
    quit();
    return 0;

    GN_UNGUARD_ALWAYS_NO_THROW;
    printf( "Press ENTER key to continue..." );
    _getch();
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

//
//
// -----------------------------------------------------------------------------
void GN::d3d10::D3D10Application::clearScreen( float r, float g, float b, float a, float d, UInt8 s )
{
    float color[] = { r, g, b, a };
    mDevice->ClearRenderTargetView( mBackRTV, color );
    mDevice->ClearDepthStencilView( mDepthDSV, D3D10_CLEAR_DEPTH|D3D10_CLEAR_STENCIL, d, s );
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

    if( gInputPtr )
    {
        gInput.attachToWindow( 0, mWindow );
    }

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

    // create factory
    AutoComPtr<IDXGIFactory> factory;
    GN_DX10_CHECK_RV( CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&factory) ), false );

	// select adapter (use NVIDIA PerfHUD device, if avaliable)
	mAdapter = 0;
	bool perfhud = false;
	UINT nadapter = 0;
	while( factory->EnumAdapters( nadapter, &mAdapter ) != DXGI_ERROR_NOT_FOUND )
	{
        if( 0 == mAdapter ) continue;

		DXGI_ADAPTER_DESC adaptDesc;

        GN_DX10_CHECK_DO( mAdapter->GetDesc( &adaptDesc ), continue; );

        GN_INFO(sLogger)( "Enumerating D3D adapters: %S", adaptDesc.Description );

		if( 0 == strCmp( adaptDesc.Description, L"NVIDIA PerfHUD" ) )
		{
			GN_INFO(sLogger)( "USE NVPerfHUD device." );
			perfhud = true;
			break;
		}

		safeRelease( mAdapter );

		++nadapter;
	}
	if( !perfhud ) safeRelease( mAdapter );

    // determine driver type
    D3D10_DRIVER_TYPE driverType;
    if( mOption.ref || perfhud )
        driverType = D3D10_DRIVER_TYPE_REFERENCE;
    else
        driverType = D3D10_DRIVER_TYPE_HARDWARE;

    // create device
    UINT flags = 0;
#if GN_DEBUG_BUILD
    flags |= D3D10_CREATE_DEVICE_DEBUG;
#endif
    flags |= D3D10_CREATE_DEVICE_SINGLETHREADED;
    GN_DX10_CHECK_RV(
        D3D10CreateDevice( mAdapter, driverType, 0, flags, D3D10_SDK_VERSION, &mDevice ),
        false );

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
    sd.SampleDesc = constructSampleDesc( mDevice, mOption.msaa, DXGI_FORMAT_R8G8B8A8_UNORM );
    sd.Windowed = !mOption.fullscreen;

    // create swap chain
    GN_DX10_CHECK_RV( factory->CreateSwapChain( mDevice, &sd, &mSwapChain ), false );

    // get default back buffer
	DXGI_SWAP_CHAIN_DESC scdesc;
    mSwapChain->GetDesc( &scdesc );
	AutoComPtr<ID3D10Texture2D> backbuf;
    GN_DX10_CHECK_RV( mSwapChain->GetBuffer( 0, __uuidof(*backbuf), (void**)&backbuf ), false );
    D3D10_RENDER_TARGET_VIEW_DESC rtvd;
    rtvd.Format             = scdesc.BufferDesc.Format;
    rtvd.ViewDimension      = scdesc.SampleDesc.Count > 1 ? D3D10_RTV_DIMENSION_TEXTURE2DMS : D3D10_RTV_DIMENSION_TEXTURE2D;
    rtvd.Texture2D.MipSlice = 0;
    GN_DX10_CHECK_RV( mDevice->CreateRenderTargetView( backbuf, NULL, &mBackRTV ), false );

    // create default depth texture
    AutoComPtr<ID3D10Texture2D> depthbuf;
    D3D10_TEXTURE2D_DESC td;
    td.Width              = mOption.width;
    td.Height             = mOption.height;
    td.MipLevels          = 1;
    td.ArraySize          = 1;
    td.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
    td.SampleDesc         = scdesc.SampleDesc;
    td.Usage              = D3D10_USAGE_DEFAULT;
    td.BindFlags          = D3D10_BIND_DEPTH_STENCIL;
    td.CPUAccessFlags     = 0;
    td.MiscFlags          = 0;
    GN_DX10_CHECK_RV( mDevice->CreateTexture2D( &td, NULL, &depthbuf ), false );

    // create depth stencil view
    D3D10_DEPTH_STENCIL_VIEW_DESC dsvd;
    dsvd.Format             = td.Format;
    dsvd.ViewDimension      = scdesc.SampleDesc.Count > 1 ? D3D10_DSV_DIMENSION_TEXTURE2DMS : D3D10_DSV_DIMENSION_TEXTURE2D;
    dsvd.Texture2D.MipSlice = 0;
    GN_DX10_CHECK_RV( mDevice->CreateDepthStencilView( depthbuf, &dsvd, &mDepthDSV ), false );

    // setup render targets
    mDevice->OMSetRenderTargets( 1, &mBackRTV, mDepthDSV );

	// setup debug and info-queue layer
	if( SUCCEEDED( mDevice->QueryInterface( IID_ID3D10Debug, (void**)&mDebug ) ) )
	{
		if( SUCCEEDED( mDebug->QueryInterface( IID_ID3D10InfoQueue, (void**)&mInfoQueue ) ) )
		{
			//mInfoQueue->SetBreakOnSeverity( D3D10_MESSAGE_SEVERITY_CORRUPTION, true );
			//mInfoQueue->SetBreakOnSeverity( D3D10_MESSAGE_SEVERITY_ERROR, true );
			//mInfoQueue->SetBreakOnSeverity( D3D10_MESSAGE_SEVERITY_WARNING, true );
			//mInfoQueue->SetBreakOnSeverity( D3D10_MESSAGE_SEVERITY_INFO, true );

			// ignore some "expected" errors
  			D3D10_MESSAGE_ID denied [] = {
  			    D3D10_MESSAGE_ID_DEVICE_DRAW_SAMPLER_NOT_SET,
                D3D10_MESSAGE_ID_DEVICE_OMSETRENDERTARGETS_HAZARD,
                D3D10_MESSAGE_ID_DEVICE_PSSETSHADERRESOURCES_HAZARD
            };
 			D3D10_INFO_QUEUE_FILTER filter;
			memset( &filter, 0, sizeof(filter) );
			filter.DenyList.NumIDs = GN_ARRAY_COUNT(denied);
			filter.DenyList.pIDList = denied;
			mInfoQueue->AddStorageFilterEntries( &filter );
		}
	}

    // setup default viewport
    D3D10_VIEWPORT vp = { 0, 0, scdesc.BufferDesc.Width, scdesc.BufferDesc.Height, 0, 1.0f };
    mDevice->RSSetViewports( 1, &vp );

    sPrintDeviceInfo( scdesc );

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
    safeRelease( mBackRTV );
    safeRelease( mDepthDSV );
	safeRelease( mSwapChain );
	safeRelease( mDevice );
	safeRelease( mAdapter );
}
