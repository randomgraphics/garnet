#include "pch.h"
#include "d3d9DepthBuffer.h"
#include "d3d9BackBuffer.h"
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
static HWND sCreateWindow( const GN::gfx2::GraphicsSystemCreationParameter & gscp )
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
        width, height,
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
static bool sCreateDevice(
    GN::gfx2::D3D9GraphicsSystemDesc & desc,
    const GN::gfx2::GraphicsSystemCreationParameter & gscp )
{
    GN_GUARD;

    GN_ASSERT( ::IsWindow( desc.window ) );

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

    // determine behavior
    UInt32 behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;

    // setup present parameters
    memset( &desc.pp, 0, sizeof(desc.pp) );
    desc.pp.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
    desc.pp.EnableAutoDepthStencil = FALSE;
    desc.pp.BackBufferCount        = 0;
    desc.pp.BackBufferFormat       = D3DFMT_X8R8G8B8;
    desc.pp.Windowed               = !gscp.fullscr;
    desc.pp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
    desc.pp.PresentationInterval   = gscp.vsync ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;
    desc.pp.hDeviceWindow          = desc.window;
    desc.pp.MultiSampleType        = D3DMULTISAMPLE_NONE;
    desc.pp.MultiSampleQuality     = 0;
    if( gscp.fullscr )
    {
        desc.pp.BackBufferWidth            = gscp.fullscrWidth;
        desc.pp.BackBufferHeight           = gscp.fullscrHeight;
        desc.pp.FullScreen_RefreshRateInHz = gscp.fullscrRefrate;
    }
    else
    {
        desc.pp.BackBufferWidth  = gscp.windowedWidth;
        desc.pp.BackBufferHeight = gscp.windowedHeight;
    }

    // create device
    GN_DX9_CHECK_RV(
        desc.d3d->CreateDevice(
            desc.adapter,
            desc.devtype,
            desc.window,
            behavior,
            &desc.pp,
            &desc.device ),
        false );

    // get device caps
    GN_DX9_CHECK_RV( desc.device->GetDeviceCaps( &desc.caps ), false );

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static void sDeleteDevice( GN::gfx2::D3D9GraphicsSystemDesc & desc )
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

///
/// convert surface type to string
///
static const char * d3d9SurfaceType2Str( int type )
{
    using namespace GN::gfx2;

    static const char * table[] =
    {
        "VB",
        "IB",
        "TEX_2D",
        "TEX_3D",
        "TEX_CUBE",
        "RTT_2D",
        "RTT_CUBE",
        "RTS_COLOR",
        "RTS_DEPTH",
        "RTS_BACKBUF",
    };
    if( 0 <= type && type <= SURFACE_TYPE_BACKBUF ) return table[type];
    if( -1 == type ) return "ANY";
    return "INVALID";
}

//
//
// -----------------------------------------------------------------------------
static bool sMergeSurfaceType(
    GN::gfx2::D3D9SurfaceType & result,
    GN::gfx2::D3D9SurfaceType t1,
    GN::gfx2::D3D9SurfaceType t2 )
{
    using namespace GN::gfx2;

    if( t1 == t2 )
    {
        result = t1;
        return true;
    }
    else if( SURFACE_TYPE_ANY == t1 )
    {
        result = t2;
        return true;
    }
    else if( SURFACE_TYPE_ANY == t2 )
    {
        result = t1;
        return true;
    }

    switch( t1 )
    {
        case SURFACE_TYPE_VB        :
        case SURFACE_TYPE_IB        :
        case SURFACE_TYPE_TEX_3D    :
            break;

        case SURFACE_TYPE_TEX_2D    :
            switch( t2 )
            {
                case SURFACE_TYPE_RTT_2D    :
                case SURFACE_TYPE_RTS_COLOR :
                case SURFACE_TYPE_RTS_DEPTH :
                case SURFACE_TYPE_BACKBUF   :
                    result = SURFACE_TYPE_RTT_2D;
                    return true;

                default:
                    break;
            }
            break;

        case SURFACE_TYPE_TEX_CUBE  :
            switch( t2 )
            {
                case SURFACE_TYPE_RTT_CUBE  :
                    result = SURFACE_TYPE_RTT_CUBE;
                    return true;

                default:
                    break;
            }
            break;

        case SURFACE_TYPE_RTT_2D    :
            switch( t2 )
            {
                case SURFACE_TYPE_TEX_2D    :
                case SURFACE_TYPE_RTS_COLOR :
                case SURFACE_TYPE_RTS_DEPTH :
                case SURFACE_TYPE_BACKBUF   :
                    result = SURFACE_TYPE_RTT_2D;
                    return true;

                default:
                    break;
            }
            break;

        case SURFACE_TYPE_RTT_CUBE  :
            switch( t2 )
            {
                case SURFACE_TYPE_RTT_2D  :
                    result = SURFACE_TYPE_RTT_CUBE;
                    return true;

                default:
                    break;
            }
            break;

        case SURFACE_TYPE_RTS_COLOR :
            switch( t2 )
            {
                case SURFACE_TYPE_TEX_2D    :
                case SURFACE_TYPE_RTT_2D    :
                    result = SURFACE_TYPE_RTT_2D;
                    return true;

                case SURFACE_TYPE_BACKBUF   :
                    result = SURFACE_TYPE_RTS_COLOR;
                    return true;

                default:
                    break;
            }
            break;

        case SURFACE_TYPE_RTS_DEPTH :
            switch( t2 )
            {
                case SURFACE_TYPE_TEX_2D    :
                case SURFACE_TYPE_RTT_2D    :
                    result = SURFACE_TYPE_RTT_2D;
                    return true;

                default:
                    break;
            }
            break;

        case SURFACE_TYPE_BACKBUF :
            switch( t2 )
            {
                case SURFACE_TYPE_TEX_2D    :
                case SURFACE_TYPE_RTT_2D    :
                    result = SURFACE_TYPE_RTT_2D;
                    return true;

                case SURFACE_TYPE_RTS_COLOR :
                    result = SURFACE_TYPE_RTS_COLOR;
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
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx2::D3D9GraphicsSystem::init( const GraphicsSystemCreationParameter & gscp )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx2::D3D9GraphicsSystem, () );

    mSceneBegun = false;

    mDesc.window = sCreateWindow( gscp );
    if( 0 == mDesc.window ) return failure();

    if( !sCreateDevice( mDesc, gscp ) ) return failure();

    if( !sigDeviceRestore() ) return failure();

    if( !beginScene() ) return failure();

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::D3D9GraphicsSystem::quit()
{
    GN_GUARD;

    endScene();

    if( mDesc.device )
    {
        sigDeviceDispose();
    }

    sDeleteDevice( mDesc );

    sDeleteWindow( mDesc.window );

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::D3D9GraphicsSystem::clear()
{
    memset( &mDesc, 0, sizeof(mDesc) );
}

// *****************************************************************************
// public method
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::D3D9GraphicsSystem::onFrame()
{
    // process render window messages
    GN::win::processWindowMessages( mDesc.window, true );

    // per-frame statistics
}

//
//
// -----------------------------------------------------------------------------
GN::gfx2::Surface * GN::gfx2::D3D9GraphicsSystem::createSurface(
    const SurfaceCreationParameter & scp )
{
    GN_GUARD;

    // check input parameter
    if( !scp.layout.check() ) return 0;

    // get surface layout template
    D3D9SurfaceType surftype = SURFACE_TYPE_ANY;
    SurfaceLayoutTemplate templ = scp.layout;
    for( size_t i = 0; i < scp.bindings.size(); ++i )
    {
        const SurfaceCreationParameter::EffectBinding & eb = scp.bindings[i];

        const D3D9EffectDesc * desc = getEffectDesc( eb.effect );
        if( 0 == desc ) return 0;

        const D3D9EffectPortDesc * port = (const D3D9EffectPortDesc *)desc->getPortDesc( eb.port );
        if( 0 == port ) return 0;

        // merge layout template
        if( !templ.mergeWith( port->layout ) ) return false;

        // merget surface type
        if( !sMergeSurfaceType( surftype, surftype, port->surfaceType ) ) return false;
    }

    // create surface using the layout and type
    switch( surftype )
    {
        case SURFACE_TYPE_VB        :
        case SURFACE_TYPE_IB        :
        case SURFACE_TYPE_TEX_2D    :
        case SURFACE_TYPE_TEX_3D    :
        case SURFACE_TYPE_TEX_CUBE  :
        case SURFACE_TYPE_RTT_2D    :
        case SURFACE_TYPE_RTT_CUBE  :
        case SURFACE_TYPE_RTS_COLOR :
            GN_UNIMPL();
            return 0;

        case SURFACE_TYPE_RTS_DEPTH :
            return D3D9DepthBuffer::sNewInstance( templ, scp.forcedAccessFlags, scp.hints );

        case SURFACE_TYPE_BACKBUF :
            return D3D9BackBuffer::sNewInstance( templ, scp.forcedAccessFlags, scp.hints );

        case SURFACE_TYPE_ANY :
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
bool GN::gfx2::D3D9GraphicsSystem::handleDeviceLost()
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
        if( !sigDeviceRestore() ) return false;

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
extern "C" GN_GFX2_D3D9_PUBLIC GN::gfx2::D3D9GraphicsSystem *
GN::gfx2::createD3D9GraphicsSystem( const GraphicsSystemCreationParameter & gscp )
{
    GN_GUARD;

    GN_ASSERT( GN_MAKE_FOURCC('D','X','9',0) == gscp.api.u32 );

    AutoObjPtr<D3D9GraphicsSystem> gs( new D3D9GraphicsSystem );

    if( !gs->init( gscp ) ) return 0;

    return gs.detach();

    GN_UNGUARD;
}
