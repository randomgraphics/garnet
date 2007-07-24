#include "pch.h"
#include "d3d9VtxBuf.h"
#include "d3d9IdxBuf.h"
#include "d3d9Texture.h"
#include "d3d9DepthBuffer.h"
#include "d3d9BuildInKernels.h"
#include "garnet/GNwin.h"

#if GN_MSVC
#pragma comment( lib, "dxerr9.lib" )
#pragma comment( lib, "d3d9.lib")
#if GN_DEBUG_BUILD
#pragma comment( lib, "d3dx9d.lib")
#else
#pragma comment( lib, "d3dx9.lib")
#endif
#endif // GN_MSVC

static GN::Logger * sLogger = GN::getLogger("GN.gfx2.D3D9GraphicsSystem");

bool gD3D9EnablePixPerf = true;

// *****************************************************************************
// device management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static LRESULT CALLBACK sStaticWindowProc( HWND wnd, UINT msg, WPARAM wp, LPARAM lp )
{
    GN_GUARD;

    switch( msg )
    {
        case WM_CLOSE :
            // ignore this message
            return 0;

        case WM_ERASEBKGND:
            // ignore this message
            return 0;
    }

    return DefWindowProc( wnd, msg, wp, lp );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static HWND sCreateWindow( const GN::gfx::GraphicsSystemCreationParameter & gscp )
{
    GN_GUARD;

    using namespace GN;

    HINSTANCE module = (HINSTANCE)GetModuleHandleW( 0 );
    GN_ASSERT( 0 != module );

    WNDCLASSEXW wcex;

    // generate an unique window class name
    StrW className;
    do
    {
        className.format( L"GNgfx2D3D9RenderWindow_%d", rand() );
    } while( ::GetClassInfoExW( module, className.cptr(), &wcex ) );

    // register window class
    GN_TRACE(sLogger)( "Register window class: %ls (module handle: 0x%X)", className.cptr(), module );
    wcex.cbSize         = sizeof(wcex);
    wcex.style          = 0;
    wcex.lpfnWndProc    = (WNDPROC)&sStaticWindowProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = module;
    wcex.hIcon          = LoadIcon (0, IDI_APPLICATION);
    wcex.hCursor        = LoadCursor (0,IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = 0;
    wcex.lpszClassName  = className.cptr();
    wcex.hIconSm        = LoadIcon(0, IDI_APPLICATION);
    if( 0 == ::RegisterClassExW(&wcex) )
    {
        GN_ERROR(sLogger)( "fail to register window class, %s!", getOSErrorInfo() );
        return 0;
    }

    // setup window style
    DWORD style;
    DWORD width;
    DWORD height;
    if( gscp.fullscr )
    {
        style  = WS_POPUP;
        width  = gscp.fullscrWidth;
        height = gscp.fullscrHeight;
    }
    else
    {
        style  = WS_OVERLAPPEDWINDOW;
        width  = gscp.windowedWidth;
        height = gscp.windowedHeight;
    }

    // calculate window size
    RECT rc = { 0, 0, width, height };
    ::AdjustWindowRectEx( &rc, style, 0, 0 );

    // create window
    HWND window = ::CreateWindowExW(
        0,
        className.cptr(),
        L"Garnet Render Window",
        style,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rc.right - rc.left, rc.bottom - rc.top,
        0, // no parent
        0, // no menu
        module,
        0 );
    if( 0 == window )
    {
        GN_ERROR(sLogger)( "fail to create window, %s!", getOSErrorInfo() );
        return false;
    }

    // show the window
    ::ShowWindow( window, SW_NORMAL );
    ::UpdateWindow( window );

    // success
    return window;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static void sDeleteWindow( HWND window )
{
    GN_GUARD;

    if( !::IsWindow( window ) ) return;

    // get window class name
    wchar_t className[256];
    ::GetClassNameW( window, className, 255 );

    // destroy window
    ::DestroyWindow( window );

    // unregister window class
    HINSTANCE module = (HINSTANCE)GetModuleHandleW( 0 );
    GN_TRACE(sLogger)( "Unregister window class: %S (module handle: 0x%X)", className, module );
    GN_MSW_CHECK( ::UnregisterClassW( className, module ) );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static const char * sD3DMsaaType2Str( D3DMULTISAMPLE_TYPE type )
{
    static const char * sTable[] =
    {
        "D3DMULTISAMPLE_NONE", // = 0,
        "D3DMULTISAMPLE_NONMASKABLE ", // = 1,
        "D3DMULTISAMPLE_2_SAMPLES", // = 2,
        "D3DMULTISAMPLE_3_SAMPLES", // = 3,
        "D3DMULTISAMPLE_4_SAMPLES", // = 4,
        "D3DMULTISAMPLE_5_SAMPLES", // = 5,
        "D3DMULTISAMPLE_6_SAMPLES", // = 6,
        "D3DMULTISAMPLE_7_SAMPLES", // = 7,
        "D3DMULTISAMPLE_8_SAMPLES", // = 8,
        "D3DMULTISAMPLE_9__SAMPLES", // = 9,
        "D3DMULTISAMPLE_10_SAMPLES", // = 10,
        "D3DMULTISAMPLE_11_SAMPLES", // = 11,
        "D3DMULTISAMPLE_12_SAMPLES", // = 12,
        "D3DMULTISAMPLE_13_SAMPLES", // = 13,
        "D3DMULTISAMPLE_14_SAMPLES", // = 14,
        "D3DMULTISAMPLE_15_SAMPLES", // = 15,
        "D3DMULTISAMPLE_16_SAMPLES", // = 16,
    };

    if( type < GN_ARRAY_COUNT(sTable) ) return sTable[type];
    else return "UNKNOWN_D3D_MSAA_TYPE";
}

//
//
// -----------------------------------------------------------------------------
static void sPrintDeviceInfo( GN::gfx::D3D9GraphicsSystemDesc & desc )
{
    using namespace GN;

    StrA devtype;
    switch( desc.caps.DeviceType )
    {
        case D3DDEVTYPE_HAL : devtype = "HAL";     break;
        case D3DDEVTYPE_REF : devtype = "REF";     break;
        case D3DDEVTYPE_SW  : devtype = "SW";      break;
        default             : devtype = "UNKNOWN"; break;
    }
    if( D3DCREATE_HARDWARE_VERTEXPROCESSING & desc.behavior &&
         D3DCREATE_PUREDEVICE & desc.behavior )
    {
        devtype += "( Pure-HW )";
    }
    else if( D3DCREATE_HARDWARE_VERTEXPROCESSING & desc.behavior )
    {
        devtype += "( Hardware )";
    }
    else if( D3DCREATE_MIXED_VERTEXPROCESSING & desc.behavior )
    {
        devtype += "( Mixed )";
    }
    else if( D3DCREATE_SOFTWARE_VERTEXPROCESSING & desc.behavior )
    {
        devtype += "( Software )";
    }

    UInt32 vsVerMajor, vsVerMinor, psVerMajor, psVerMinor;
    vsVerMajor = (desc.caps.VertexShaderVersion & 0xFF00) >> 8;
    vsVerMinor = desc.caps.VertexShaderVersion & 0xFF;
    psVerMajor = (desc.caps.PixelShaderVersion & 0xFF00) >> 8;
    psVerMinor = desc.caps.PixelShaderVersion & 0xFF;
    StrA vsver = strFormat( "%d.%d", vsVerMajor, vsVerMinor );
    StrA psver = strFormat( "%d.%d", psVerMajor, psVerMinor );
    StrA hwtnl;
    if( D3DDEVCAPS_HWTRANSFORMANDLIGHT & desc.caps.DevCaps )
        hwtnl = "Yes";
    else
        hwtnl = "No";

    // get adapter and driver information
    D3DADAPTER_IDENTIFIER9 aid;
    memset( &aid, 0, sizeof(aid) );
    GN_DX9_CHECK( desc.d3d->GetAdapterIdentifier( desc.adapter, 0, &aid ) );

    // output device information
    GN_INFO(sLogger)(
        "\n\n"
        "===================================================\n"
        "        DirectX Implementation Capabilities\n"
        "---------------------------------------------------\n"
        "    Device Type                    : %s\n"
        "    Driver                         : %s(%d.%d.%d.%d)\n"
        "    Adapter                        : %s\n"
        "    GDI Device Name                : %s\n"
        "    Backbuffer Size                : %d,%d\n"
        "    Display Mode                   : %s\n"
        "    Vertex Shader Version          : %s\n"
        "    Pixel Shader Version           : %s\n"
        "    Max Vertex Shader Constants    : %d\n"
        "    Hardware TnL (FFP)             : %s\n"
        "    Texture Blend Stages (FFP)     : %d\n"
        "    Max Simulaneous Textures (FFP) : %d\n"
        "    Nax Simulaneous Render Targets : %d\n"
        "    MSAA Type                      : %s\n"
        "    MSAA Quality                   : %d\n"
        "===================================================\n"
        "\n\n",
        devtype.cptr(),
        aid.Driver,
        HIWORD(aid.DriverVersion.HighPart),
        LOWORD(aid.DriverVersion.HighPart),
        HIWORD(aid.DriverVersion.LowPart),
        LOWORD(aid.DriverVersion.LowPart),
        aid.Description,
        aid.DeviceName,
        desc.pp.BackBufferWidth,
        desc.pp.BackBufferHeight,
        desc.pp.Windowed ? "Windowed" : "Fullscreen",
        vsver.cptr(),
        psver.cptr(),
        desc.caps.MaxVertexShaderConst,
        hwtnl.cptr(),
        desc.caps.MaxTextureBlendStages,
        desc.caps.MaxSimultaneousTextures,
        desc.caps.NumSimultaneousRTs,
        sD3DMsaaType2Str( desc.pp.MultiSampleType ),
        desc.pp.MultiSampleQuality );
}

//
//
// -----------------------------------------------------------------------------
static bool sCreateDevice(
    GN::gfx::D3D9GraphicsSystemDesc & desc,
    const GN::gfx::GraphicsSystemCreationParameter & gscp )
{
    GN_GUARD;

    PIXPERF_FUNCTION_EVENT();

    GN_ASSERT( ::IsWindow( (HWND)desc.window ) );

    // create D3D
    desc.d3d = Direct3DCreate9( D3D_SDK_VERSION );
    if( 0 == desc.d3d ) { GN_ERROR(sLogger)( "fail to create D3D object!" ); return false; }

    // determine devtype
    desc.devtype = D3DDEVTYPE_HAL;

    // determine adapter
    desc.adapter = gscp.monitor;

	// Look up nvidia adapter
    UINT nAdapter = desc.d3d->GetAdapterCount();
    GN_ASSERT( nAdapter );
    for( UInt32 i = 0; i < nAdapter; ++i )
    {
        D3DADAPTER_IDENTIFIER9 Identifier;
        GN_DX9_CHECK( desc.d3d->GetAdapterIdentifier( i, 0, &Identifier ) );
        GN_TRACE(sLogger)( "Enumerating D3D adapters: %s", Identifier.Description );
        if( strstr(Identifier.Description,"NVPerfHUD") )
        {
            GN_TRACE(sLogger)( "Found NVPerfHUD adapter. We will create D3D device using NVPerfHUD adapter." );
            desc.adapter = i;
            desc.devtype = D3DDEVTYPE_REF;
            break;
        }
    }

    // get device caps
    GN_DX9_CHECK_RV( desc.d3d->GetDeviceCaps( desc.adapter, desc.devtype, &desc.caps ), false );

    // determine behavior
    desc.behavior = 0;
    UInt32 vsver = (desc.caps.VertexShaderVersion & 0xFFFF);
    bool   hwtnl = !!(desc.caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT);
    if( vsver > 0 && hwtnl )
    {
        desc.behavior |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
    }
    else if( 0 == vsver && !hwtnl )
    {
        desc.behavior |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    }
    else
    {
        desc.behavior |= D3DCREATE_MIXED_VERTEXPROCESSING;
    }
    if( desc.caps.DevCaps & D3DDEVCAPS_PUREDEVICE )
    {
        desc.behavior |= D3DCREATE_PUREDEVICE;
    }

    // setup present parameters
    memset( &desc.pp, 0, sizeof(desc.pp) );
    desc.pp.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
    desc.pp.EnableAutoDepthStencil = FALSE;
    desc.pp.BackBufferCount        = 0;
    desc.pp.BackBufferFormat       = D3DFMT_X8R8G8B8;
    desc.pp.Windowed               = !gscp.fullscr;
    desc.pp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
    desc.pp.PresentationInterval   = gscp.vsync ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;
    desc.pp.hDeviceWindow          = (HWND)desc.window;
    desc.pp.MultiSampleType        = D3DMULTISAMPLE_NONE;
    desc.pp.MultiSampleQuality     = 0;
    if( gscp.fullscr )
    {
        desc.width                         = gscp.fullscrWidth;
        desc.height                        = gscp.fullscrHeight;
        desc.depth                         = gscp.fullscrDepth;
        desc.refrate                       = gscp.fullscrRefrate;
        desc.pp.BackBufferWidth            = gscp.fullscrWidth;
        desc.pp.BackBufferHeight           = gscp.fullscrHeight;
        desc.pp.FullScreen_RefreshRateInHz = gscp.fullscrRefrate;
    }
    else
    {
        desc.width                         = gscp.windowedWidth;
        desc.height                        = gscp.windowedHeight;
        desc.depth                         = 32; // TODO: get current screen depth;
        desc.refrate                       = 0;
        desc.pp.BackBufferWidth  = gscp.windowedWidth;
        desc.pp.BackBufferHeight = gscp.windowedHeight;
    }

    // create device
    GN_DX9_CHECK_RV(
        desc.d3d->CreateDevice(
            desc.adapter,
            desc.devtype,
            (HWND)desc.window,
            desc.behavior,
            &desc.pp,
            &desc.device ),
        false );

    // get device caps
    GN_DX9_CHECK_RV( desc.device->GetDeviceCaps( &desc.caps ), false );

    // print device information
    sPrintDeviceInfo( desc );

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static void sDeleteDevice( GN::gfx::D3D9GraphicsSystemDesc & desc )
{
    GN_GUARD;

#ifdef D3D_DEBUG_INFO
    if( desc.device )
    {
		desc.device->SetVertexShader(0);
		desc.device->SetPixelShader(0);

		GN_TRACE(sLogger)(
			"\n"
			"====== Dump unreleased D3D resources ========\n"
			"	SwapChains: %d\n"
			"	Textures: %d\n"
			"	VertexBuffers: %d\n"
			"	IndexBuffers: %d\n"
			"	VertexShaders: %d\n"
			"	PixelShaders: %d\n"
			"=============================================\n"
			"\n",
			desc.device->SwapChains,
			desc.device->Textures,
			desc.device->VertexBuffers,
			desc.device->IndexBuffers,
			desc.device->VertexShaders,
			desc.device->PixelShaders );
	}
#endif

    using namespace GN;

    safeRelease( desc.device );
    safeRelease( desc.d3d );

    GN_UNGUARD;
}

// *****************************************************************************
// surface management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static bool sMergeSurfaceType(
    GN::gfx::D3D9SurfaceType & result,
    GN::gfx::D3D9SurfaceType t1,
    GN::gfx::D3D9SurfaceType t2 )
{
    using namespace GN::gfx;

    if( t1 == t2 )
    {
        result = t1;
        return true;
    }
    else if( D3D9_SURFACE_TYPE_ANY == t1 )
    {
        result = t2;
        return true;
    }
    else if( D3D9_SURFACE_TYPE_ANY == t2 )
    {
        result = t1;
        return true;
    }

    switch( t1 )
    {
        case D3D9_SURFACE_TYPE_VB        :
        case D3D9_SURFACE_TYPE_IB        :
        case D3D9_SURFACE_TYPE_TEX_3D    :
            break;

        case D3D9_SURFACE_TYPE_TEX       :
            switch( t2 )
            {
                case D3D9_SURFACE_TYPE_TEX_2D    :
                case D3D9_SURFACE_TYPE_TEX_3D    :
                case D3D9_SURFACE_TYPE_TEX_CUBE  :
                case D3D9_SURFACE_TYPE_RTT_2D    :
                    result = t2;
                    return true;

                case D3D9_SURFACE_TYPE_RTS_COLOR :
                case D3D9_SURFACE_TYPE_RTS_DEPTH :
                    result = D3D9_SURFACE_TYPE_RTT_2D;
                    return true;

                default:
                    break;
            }
            break;

        case D3D9_SURFACE_TYPE_TEX_2D    :
            switch( t2 )
            {
                case D3D9_SURFACE_TYPE_TEX       :
                case D3D9_SURFACE_TYPE_RTT_2D    :
                case D3D9_SURFACE_TYPE_RTS_COLOR :
                case D3D9_SURFACE_TYPE_RTS_DEPTH :
                    result = D3D9_SURFACE_TYPE_RTT_2D;
                    return true;

                default:
                    break;
            }
            break;

        case D3D9_SURFACE_TYPE_TEX_CUBE  :
            switch( t2 )
            {
                case D3D9_SURFACE_TYPE_TEX       :
                case D3D9_SURFACE_TYPE_RTT_CUBE  :
                    result = D3D9_SURFACE_TYPE_RTT_CUBE;
                    return true;

                default:
                    break;
            }
            break;

        case D3D9_SURFACE_TYPE_RTT_2D    :
            switch( t2 )
            {
                case D3D9_SURFACE_TYPE_TEX       :
                case D3D9_SURFACE_TYPE_TEX_2D    :
                case D3D9_SURFACE_TYPE_RTS_COLOR :
                case D3D9_SURFACE_TYPE_RTS_DEPTH :
                    result = D3D9_SURFACE_TYPE_RTT_2D;
                    return true;

                default:
                    break;
            }
            break;

        case D3D9_SURFACE_TYPE_RTT_CUBE  :
            switch( t2 )
            {
                case D3D9_SURFACE_TYPE_TEX       :
                case D3D9_SURFACE_TYPE_RTT_2D    :
                    result = D3D9_SURFACE_TYPE_RTT_CUBE;
                    return true;

                default:
                    break;
            }
            break;

        case D3D9_SURFACE_TYPE_RTS_COLOR :
            switch( t2 )
            {
                case D3D9_SURFACE_TYPE_TEX       :
                case D3D9_SURFACE_TYPE_TEX_2D    :
                case D3D9_SURFACE_TYPE_RTT_2D    :
                    result = D3D9_SURFACE_TYPE_RTT_2D;
                    return true;

                default:
                    break;
            }
            break;

        case D3D9_SURFACE_TYPE_RTS_DEPTH :
            switch( t2 )
            {
                case D3D9_SURFACE_TYPE_TEX       :
                case D3D9_SURFACE_TYPE_TEX_2D    :
                case D3D9_SURFACE_TYPE_RTT_2D    :
                    result = D3D9_SURFACE_TYPE_RTT_2D;
                    return true;

                default:
                    break;
            }
            break;

        default:
            GN_UNEXPECTED();
    }

    // failed
    GN_ERROR(sLogger)( " '%s' and '%s' is incompatible.", d3d9SurfaceType2Str(t1), d3d9SurfaceType2Str(t2) );
    return false;
}

// *****************************************************************************
// D3D9UnstableResource
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D9UnstableResource::D3D9UnstableResource( D3D9GraphicsSystem & gs ) : mGraphicsSystem(gs)
{
    gs.sigDeviceRestore.connect( this, &D3D9UnstableResource::onRestore );
    gs.sigDeviceDispose.connect( this, &D3D9UnstableResource::onDispose );
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D9UnstableResource::~D3D9UnstableResource()
{
    mGraphicsSystem.sigDeviceRestore.disconnect( this );
    mGraphicsSystem.sigDeviceDispose.disconnect( this );
}

// *****************************************************************************
// ctor / dtor
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D9GraphicsSystem::D3D9GraphicsSystem()
{
    clear();
}

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9GraphicsSystem::init( const GraphicsSystemCreationParameter & gscp )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3D9GraphicsSystem, () );

    mSceneBegun = false;

    mDesc.display = (HandleType)0xdeadbeef;
    mDesc.window  = sCreateWindow( gscp );
    if( 0 == mDesc.window ) return failure();

    PIXPERF_FUNCTION_EVENT();

    if( !sCreateDevice( mDesc, gscp ) ) return failure();

    if( !restoreDevice() ) return failure();

    if( !beginScene() ) return failure();

    // register build-in kernels
    registerKernelFactory( D3D9ClearScreenKernel::KERNEL_NAME(), &D3D9ClearScreenKernel::sFactory, 100 );
    registerKernelFactory( D3D9HlslKernel::KERNEL_NAME(), &D3D9HlslKernel::sFactory, 100 );
    registerKernelFactory( D3D9QuadKernel::KERNEL_NAME(), &D3D9QuadKernel::sFactory, 100 );

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9GraphicsSystem::quit()
{
    GN_GUARD;

    PIXPERF_FUNCTION_EVENT();

    endScene();

    if( mDesc.device )
    {
        sigDeviceDispose();
    }

    sDeleteDevice( mDesc );

    sDeleteWindow( (HWND)mDesc.window );

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9GraphicsSystem::clear()
{
    memset( &mDesc, 0, sizeof(mDesc) );
    memset( mCurrentTextures, 0, sizeof(mCurrentTextures) );
    mCurrentRsb = 0;
}

// *****************************************************************************
// public method
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9GraphicsSystem::present()
{
    endScene();

    mDesc.device->Present( 0, 0, 0, 0 );

    // process render window messages
    GN::win::processWindowMessages( mDesc.window, true );

    // TODO: per-frame statistics

    if( !beginScene() )
    {
        GN_FATAL(sLogger)( "beginScene() failed!" );
        GN_UNEXPECTED();
    }
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Surface * GN::gfx::D3D9GraphicsSystem::createSurface(
    const SurfaceCreationParameter & scp )
{
    GN_GUARD;

    PIXPERF_FUNCTION_EVENT();

    // get surface layout template
    D3D9SurfaceType surftype = D3D9_SURFACE_TYPE_ANY;
    for( size_t i = 0; i < scp.bindings.size(); ++i )
    {
        const SurfaceCreationParameter::SurfaceBindingParameter & sbp = scp.bindings[i];

        const D3D9Kernel * kernel = safeCastPtr<const D3D9Kernel>( getKernel( sbp.kernel ) );
        if( 0 == kernel ) return 0;

        const D3D9KernelPort * port = kernel->getPortT<D3D9KernelPort>( sbp.port );
        if( 0 == port ) return 0;

        // check layout compability
        if( !port->getDesc().layout.compatible( scp.layout ) )
        {
            GN_ERROR(sLogger)( "Requested surface layout is incompatible with port '%s' of kernel '%s'", sbp.port.cptr(), sbp.kernel.cptr() );
            return false;
        }

        // merget surface type
        if( !sMergeSurfaceType( surftype, surftype, port->getDesc().surfaceType ) ) return false;
    }

    // create surface using the layout and type
    switch( surftype )
    {
        case D3D9_SURFACE_TYPE_VB        :
            return D3D9VtxBuf::sNewInstance( *this, scp.layout, scp.forcedAccessFlags, scp.hints );

        case D3D9_SURFACE_TYPE_IB        :
            return D3D9IdxBuf::sNewInstance( *this, scp.layout, scp.forcedAccessFlags, scp.hints );

        case D3D9_SURFACE_TYPE_TEX       :
        case D3D9_SURFACE_TYPE_TEX_2D    :
        case D3D9_SURFACE_TYPE_TEX_3D    :
        case D3D9_SURFACE_TYPE_TEX_CUBE  :
            return D3D9Texture::sNewInstance( *this, surftype, scp.layout, scp.forcedAccessFlags, scp.hints );

        case D3D9_SURFACE_TYPE_RTT_2D    :
        case D3D9_SURFACE_TYPE_RTT_CUBE  :
        case D3D9_SURFACE_TYPE_RTS_COLOR :
            GN_UNIMPL();
            return 0;

        case D3D9_SURFACE_TYPE_RTS_DEPTH :
            return D3D9DepthBuffer::sNewInstance( scp.layout, scp.forcedAccessFlags, scp.hints );

        case D3D9_SURFACE_TYPE_ANY :
            GN_ERROR(sLogger)( "fail to determine surface type." );
            return 0;

        default :
            GN_UNEXPECTED();
            return 0;
    }

    GN_UNGUARD;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9GraphicsSystem::restoreDevice()
{
    D3D9RenderStateBlock::sSetupDefaultDeviceStates( *this );

    return sigDeviceRestore();
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9GraphicsSystem::handleDeviceLost()
{
    GN_GUARD;

    PIXPERF_FUNCTION_EVENT();

    GN_ASSERT( mDesc.device );

    HRESULT hr = mDesc.device->TestCooperativeLevel();
    if( D3DERR_DEVICENOTRESET == hr )
    {
        GN_INFO(sLogger)( "\n============ Restore lost device ===============" );

        // send dispose signal
        sigDeviceDispose();

        // reset d3ddevice
        GN_DX9_CHECK_RV( mDesc.device->Reset( &mDesc.pp ), false );

        // send restore signal
        if( !restoreDevice() ) return false;

        GN_INFO(sLogger)( "=================================================\n" );
    }
    else if( D3DERR_DEVICELOST == hr )
    {
        GN_INFO(sLogger)( "\nDevice has lost and could NOT be restored by now.\nWait for 2 seconds to try again...\n" );
        ::Sleep( 2000 );
        return false;
    }
    else if (D3D_OK != hr)
    {
        // fatal error
        GN_ERROR(sLogger)( "TestCooperativeLevel() failed: %s!", ::DXGetErrorString9A(hr) );
        return false;
    }

    // success
    return true;

    GN_UNGUARD;
}

// *****************************************************************************
// global function
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
extern "C" GN_GFX2_D3D9_PUBLIC GN::gfx::D3D9GraphicsSystem *
GN::gfx::createD3D9GraphicsSystem( const GraphicsSystemCreationParameter & gscp )
{
    GN_GUARD;

    GN_ASSERT( GN_MAKE_FOURCC('D','X','9',0) == gscp.api.u32 );

    AutoObjPtr<D3D9GraphicsSystem> gs( new D3D9GraphicsSystem );

    if( !gs->init( gscp ) ) return 0;

    return gs.detach();

    GN_UNGUARD;
}
