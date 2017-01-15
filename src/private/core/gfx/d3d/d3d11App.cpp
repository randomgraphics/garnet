#include "pch.h"
#include <garnet/GNinput.h>

using namespace GN;
using namespace GN::d3d11;

static Logger * sLogger = GN::getLogger("GN.d3d11.D3D11Application");

class D3D11Application::RenderWindow
{
public:

    struct Option
    {
        UINT width;
        UINT height;
        bool fullscreen;
    };

    static RenderWindow * Create(const Option & o)
    {
        std::unique_ptr<RenderWindow> instance(new RenderWindow);

        if(!instance->Init(o)) return nullptr;

        return instance.release();
    }

    ~RenderWindow()
    {
        cleanup();
    }

    HWND GetHWND() const
    {
        return _hwnd;
    }

    bool AdjustWindow(const Option & o)
    {
        return sAdjustWindow(_hwnd, o.width, o.height, o.fullscreen);
    }

private:

    HWND _hwnd;

    RenderWindow()
        : _hwnd(0)
    {
    }

    bool Init(const Option & o)
    {
        _hwnd = sCreateWindow(o.width, o.height, o.fullscreen);
        return 0 != _hwnd;
    }

    void cleanup()
    {
        if(0 != _hwnd)
        {
            sDestroyWindow(_hwnd);
            _hwnd = 0;
        }
    }

    // -----------------------------------------------------------------------------
    static HWND sCreateWindow(UINT width, UINT height, bool fullscreen)
    {
        // no parent window
        HWND parent = 0;

        HINSTANCE module = (HINSTANCE)GetModuleHandleW(0);
        GN_ASSERT( 0 != module );

        WNDCLASSEXW wcex;

        const wchar_t * classname = L"d3d11app_mainwindow";

        // register window class
        GN_VTRACE(sLogger)( "Register window class: %S (module handle: 0x%X)", classname, module );
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
            GN_ERROR(sLogger)("fail to register window class!");
            return 0;
        }

        // setup window style
        DWORD style = fullscreen ? WS_POPUP : WS_OVERLAPPEDWINDOW;
        DWORD exStyle = parent ? WS_EX_TOOLWINDOW : 0;

        // calculate window size
        RECT rc = { 0, 0, width, height };
        ::AdjustWindowRectEx( &rc, style, 0, exStyle );

        // create window
        HWND hwnd = ::CreateWindowExW(
            exStyle,
            classname,
            L"D3DApp",
            style,
            CW_USEDEFAULT, CW_USEDEFAULT,
            rc.right - rc.left, rc.bottom - rc.top,
            parent,
            0, // no menu
            module,
            0 );
        if( 0 == hwnd )
        {
            GN_ERROR(sLogger)("fail to create window!");
            return 0;
        }

        // show the window
        ::ShowWindow( hwnd, SW_NORMAL );
        ::UpdateWindow( hwnd );

        // success
        return hwnd;
    }

    // -----------------------------------------------------------------------------
    static bool sAdjustWindow( HWND window, UINT width, UINT height, bool fullscreen )
    {
        DWORD style = fullscreen ? WS_POPUP : WS_OVERLAPPEDWINDOW;
        style |= WS_VISIBLE;

        // modify render window style
        SetWindowLong( window, GWL_STYLE, style );

        // calculate boundary size
        RECT rc = { 0, 0, width, height };
        if(!::AdjustWindowRectEx(
                &rc,
                style,
                0,
                ::GetWindowLongA( window, GWL_EXSTYLE )))
        {
            GN_ERROR(sLogger)("Fail to adjust windows style.");
            return false;
        }

        // resize the window
        if(!::SetWindowPos(
                window, HWND_TOP,
                0, 0, // position, ignored.
                rc.right-rc.left, rc.bottom-rc.top, // size
                SWP_NOMOVE))
        {
            GN_ERROR(sLogger)("Fail to adjust windows size.");
            return false;
        }

        // success
        return true;
    }

    // -----------------------------------------------------------------------------
    static void sDestroyWindow( HWND hwnd )
    {
        if( IsWindow(hwnd) ) DestroyWindow( hwnd );
    }

    // -----------------------------------------------------------------------------
    static LRESULT CALLBACK
    sStaticWindowProc( HWND wnd, UINT msg, WPARAM wp, LPARAM lp )
    {
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

            default:
                // do nothing
                break;
        }

        return ::DefWindowProc( wnd, msg, wp, lp );
    }
};

// -----------------------------------------------------------------------------
static void sPrintD3D11DeviceInfo( ID3D11Device * dev, const DXGI_SWAP_CHAIN_DESC & scd )
{
    D3D_FEATURE_LEVEL fl = dev->GetFeatureLevel();

    GN_INFO(sLogger)(
        "\n\n"
        "===================================================\n"
        "        D3D11 Implementation Information\n"
        "---------------------------------------------------\n"
        "    Feature Level                  : %d.%d\n"
        "    Backbuffer Size                : %d,%d\n"
        "    Fullscreen                     : %s\n"
        "    MSAA Sample Count              : %d\n"
        "    MSAA Sample Quality            : %d\n"
        "===================================================\n"
        "\n\n",
        (fl >> 12) & 0xF, (fl>>8) & 0xF,
        scd.BufferDesc.Width,
        scd.BufferDesc.Height,
        scd.Windowed ? "False" : "True",
        scd.SampleDesc.Count,
        scd.SampleDesc.Quality
        );
}

// -----------------------------------------------------------------------------
static DXGI_SAMPLE_DESC
sConstructDXGISampleDesc(
    ID3D11Device       & device,
    int                  msaa, // 0: disabled, 1: low quality, 2: high quality.
    DXGI_FORMAT          format )
{
    DXGI_SAMPLE_DESC sd;

    const UINT high[] = { 16, 8, 4, 2, 0 };
    const UINT low[] = { 2, 4, 8, 16, 0 };

    if( msaa > 0 )
    {
        const UINT * count = (1==msaa) ? low : high;
        while(0 != *count)
        {
            UINT quality;
            if( S_OK == device.CheckMultisampleQualityLevels( format, *count, &quality ) && quality > 0 )
            {
                sd.Count = *count;
                sd.Quality = quality - 1;
                return sd;
            }
        }
        GN_WARN(sLogger)("Current device does not support requested MSAA mode" );
    }

    sd.Count = 1;
    sd.Quality = 0;
    return sd;
}

// -----------------------------------------------------------------------------
GN::d3d11::D3D11Application::D3D11Application()
    : mWindow(0)
    , mAdapter(0)
    , mDevice(0)
    , mContext(0)
    , mSwapChain(0)
#if GN_PLATFORM_HAS_D3D11_1
    , mDevice1(0)
    , mContext1(0)
    , mSwapChain1(0)
#endif
    , mBackBuf(0)
    , mBackRTV(0)
    , mDepthBuf(0)
    , mDepthDSV(0)
{
    //input::initializeInputSystem();
}

// -----------------------------------------------------------------------------
GN::d3d11::D3D11Application::~D3D11Application()
{
    input::shutdownInputSystem();
}

// -----------------------------------------------------------------------------
int GN::d3d11::D3D11Application::run( const D3D11AppOption * o )
{
    __try
    {
        if(!GN::input::initializeInputSystem())
        {
            cleanup();
            return -1;
        }

        if(nullptr != o)
        {
            mOption = *o;
        }
        else
        {
            D3D11AppOption defaultOptions;
            mOption = defaultOptions;
        }

        if( !init() ) { cleanup(); return -1; }

        if( !changeOption(mOption) ) { cleanup(); return -1; }

        // message loop
        MSG msg;
        while( true )
        {
            if( ::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
            {
                if( WM_QUIT == msg.message )
                {
                    cleanup();
                    return 0;
                }
                ::TranslateMessage( &msg );
                ::DispatchMessage(&msg);
            }
            else if( ::IsIconic( mWindow->GetHWND() ) )
            {
                GN_TRACE(sLogger)( "Minimized. Wait for more messages..." );
                ::WaitMessage();
            }
            else
            {
                // TODO: process input devies (keyboard, mouse, controller and etc.)

                // Idle time, do rendering and update
                onUpdate();
                onDraw();

                // present()
                mSwapChain->Present( 0, 0 );
            }
        }

        // donoe
        cleanup();
        return 0;
    }
    __except(EXCEPTION_EXECUTE_HANDLER)
    {
        cleanup();
        return -1;
    }
}

// -----------------------------------------------------------------------------
bool GN::d3d11::D3D11Application::changeOption( const D3D11AppOption & o )
{
    destroyDevice();
    mOption = o;
    return createDevice();
}

// -----------------------------------------------------------------------------
void GN::d3d11::D3D11Application::clearScreen( float r, float g, float b, float a, float d, BYTE s )
{
    float color[] = { r, g, b, a };
    mContext->ClearRenderTargetView( mBackRTV, color );
    mContext->ClearDepthStencilView( mDepthDSV, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, d, s );
}

// -----------------------------------------------------------------------------
bool GN::d3d11::D3D11Application::init()
{
    RenderWindow::Option o = { mOption.width, mOption.height, mOption.fullscreen };
    mWindow = RenderWindow::Create(o);
    if( nullptr == mWindow ) return false;
    return onInit( mOption );
}

// -----------------------------------------------------------------------------
void GN::d3d11::D3D11Application::cleanup()
{
    onCleanup();
    destroyDevice();
    safeDelete(mWindow);
    GN::input::shutdownInputSystem();
}

// -----------------------------------------------------------------------------
bool GN::d3d11::D3D11Application::createDevice()
{
    GN_ASSERT( nullptr != mWindow );
    GN_ASSERT( nullptr == mDevice );

    // adjust render window
    RenderWindow::Option o = {mOption.width, mOption.height, mOption.fullscreen};
    if (!mWindow->AdjustWindow(o)) return false;

    // use default adapter
    mAdapter = nullptr;

    // determine creation flags
    UINT flags = D3D11_CREATE_DEVICE_SINGLETHREADED | D3D11_CREATE_DEVICE_BGRA_SUPPORT;
    if( mOption.debug ) flags |= D3D11_CREATE_DEVICE_DEBUG;

    // Get the highest feature level
    D3D_FEATURE_LEVEL featureLevel;
    GN_RETURN_FALSE_ON_HR_FAILED(D3D11CreateDevice(
        NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags, NULL, 0,
        D3D11_SDK_VERSION, NULL, &featureLevel, NULL));

    // create device
    GN_RETURN_FALSE_ON_HR_FAILED(D3D11CreateDevice(
            mAdapter,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr, // software module
            flags,
            &featureLevel,
            1,
            D3D11_SDK_VERSION,
            &mDevice,
            nullptr,
            &mContext
            ));

    // setup debug and info-queue layer
    AutoComPtr<ID3D11Debug>      mDebug;
    AutoComPtr<ID3D11InfoQueue>  mInfoQueue;
    if( SUCCEEDED( mDevice->QueryInterface( IID_ID3D11Debug, (void**)&mDebug ) ) )
    {
        if( SUCCEEDED( mDebug->QueryInterface( IID_ID3D11InfoQueue, (void**)&mInfoQueue ) ) )
        {
            mInfoQueue->SetBreakOnSeverity( D3D11_MESSAGE_SEVERITY_CORRUPTION, true );
            //mInfoQueue->SetBreakOnSeverity( D3D11_MESSAGE_SEVERITY_ERROR, true );
            //mInfoQueue->SetBreakOnSeverity( D3D11_MESSAGE_SEVERITY_WARNING, true );
            //mInfoQueue->SetBreakOnSeverity( D3D11_MESSAGE_SEVERITY_INFO, true );

            // ignore some "expected" errors
            D3D11_MESSAGE_ID denied [] = {
                D3D11_MESSAGE_ID_DEVICE_DRAW_SAMPLER_NOT_SET,
                    D3D11_MESSAGE_ID_DEVICE_OMSETRENDERTARGETS_HAZARD,
                    D3D11_MESSAGE_ID_DEVICE_PSSETSHADERRESOURCES_HAZARD
                };
            D3D11_INFO_QUEUE_FILTER filter;
            memset( &filter, 0, sizeof(filter) );
            filter.DenyList.NumIDs = _countof(denied);
            filter.DenyList.pIDList = denied;
            mInfoQueue->AddStorageFilterEntries( &filter );
        }
    }

    AutoComPtr<IDXGIDevice> pDXGIDevice;
    GN_RETURN_FALSE_ON_HR_FAILED(mDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice));

    AutoComPtr<IDXGIAdapter> pDXGIAdapter;
    GN_RETURN_FALSE_ON_HR_FAILED(pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&pDXGIAdapter));

    AutoComPtr<IDXGIFactory> pIDXGIFactory;
    GN_RETURN_FALSE_ON_HR_FAILED(pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&pIDXGIFactory));

    // create swap chain
    DXGI_SWAP_CHAIN_DESC sd = {0};
    sd.BufferDesc.Width = mOption.width;
    sd.BufferDesc.Height = mOption.height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Denominator = 0;
    sd.BufferDesc.RefreshRate.Numerator = 0;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.SampleDesc = sConstructDXGISampleDesc(*mDevice, mOption.msaa, sd.BufferDesc.Format);
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 2; // use two buffers to enable flip effect
    sd.OutputWindow = mWindow->GetHWND();
    sd.Windowed = !mOption.fullscreen;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = 0;

    GN_RETURN_FALSE_ON_HR_FAILED(pIDXGIFactory->CreateSwapChain(mDevice, &sd, &mSwapChain));

#if GN_PLATFORM_HAS_D3D11_1
    // try query 11.1 interfaces.
    mDevice->QueryInterface<ID3D11Device1>(&mDevice1);
    mContext->QueryInterface<ID3D11DeviceContext1>(&mContext1);
    mSwapChain->QueryInterface<IDXGISwapChain1>(&mSwapChain1);
#endif

    // get default back buffer
    DXGI_SWAP_CHAIN_DESC scdesc;
    mSwapChain->GetDesc( &scdesc );
    GN_RETURN_FALSE_ON_HR_FAILED( mSwapChain->GetBuffer( 0, __uuidof(*mBackBuf), (void**)&mBackBuf ));
    D3D11_RENDER_TARGET_VIEW_DESC rtvd;
    rtvd.Format             = scdesc.BufferDesc.Format;
    rtvd.ViewDimension      = scdesc.SampleDesc.Count > 1 ? D3D11_RTV_DIMENSION_TEXTURE2DMS : D3D11_RTV_DIMENSION_TEXTURE2D;
    rtvd.Texture2D.MipSlice = 0;
    GN_RETURN_FALSE_ON_HR_FAILED( mDevice->CreateRenderTargetView( mBackBuf, NULL, &mBackRTV ));

    // create default depth texture
    D3D11_TEXTURE2D_DESC td;
    td.Width              = mOption.width;
    td.Height             = mOption.height;
    td.MipLevels          = 1;
    td.ArraySize          = 1;
    td.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
    td.SampleDesc         = scdesc.SampleDesc;
    td.Usage              = D3D11_USAGE_DEFAULT;
    td.BindFlags          = D3D11_BIND_DEPTH_STENCIL;
    td.CPUAccessFlags     = 0;
    td.MiscFlags          = 0;
    GN_RETURN_FALSE_ON_HR_FAILED( mDevice->CreateTexture2D( &td, NULL, &mDepthBuf ));

    // create depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvd = {DXGI_FORMAT_UNKNOWN};
    dsvd.Format             = td.Format;
    dsvd.ViewDimension      = scdesc.SampleDesc.Count > 1 ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvd.Texture2D.MipSlice = 0;
    GN_RETURN_FALSE_ON_HR_FAILED( mDevice->CreateDepthStencilView( mDepthBuf, &dsvd, &mDepthDSV ));

    // setup render targets
    mContext->OMSetRenderTargets( 1, &mBackRTV, mDepthDSV );

    // setup default viewport
    D3D11_VIEWPORT vp = { 0, 0, (float)scdesc.BufferDesc.Width, (float)scdesc.BufferDesc.Height, 0, 1.0f };
    mContext->RSSetViewports( 1, &vp );

    sPrintD3D11DeviceInfo( mDevice, scdesc );

    // success
    return onCreateDevice();
}

// -----------------------------------------------------------------------------
void GN::d3d11::D3D11Application::destroyDevice()
{
    if( mContext )
    {
        mContext->ClearState();
        onDestroyDevice();
    }

    safeRelease( mBackBuf );
    safeRelease( mBackRTV );
    safeRelease( mDepthBuf );
    safeRelease( mDepthDSV );
#if GN_PLATFORM_HAS_D3D11_1
    safeRelease( mSwapChain1 );
    safeRelease( mContext1 );
    safeRelease( mDevice1 );
#endif
    safeRelease( mSwapChain );
    safeRelease( mContext );
    safeRelease( mDevice );
    safeRelease( mAdapter );
}
