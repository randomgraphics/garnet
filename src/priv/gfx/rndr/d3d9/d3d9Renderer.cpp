#include "pch.h"
#include "d3d9Renderer.h"
#include "d3d9Resource.h"

#if GN_MSVC
#pragma comment( lib, "dxerr9.lib" )
#if GN_XENON
#if GN_DEBUG_BUILD
#pragma comment(lib, "d3d9d.lib")
#pragma comment(lib, "d3dx9d.lib")
#else // GN_DEBUG_BUILD
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#endif // GN_DEBUG_BUILD
#else // GN_XENON
#pragma comment(lib, "d3d9.lib")
#if GN_DEBUG_BUILD
#pragma comment(lib, "d3dx9d.lib")
#else
#pragma comment(lib, "d3dx9.lib")
#endif
#endif // GN_XENON
#endif // GN_MSVC

GN::Logger * GN::gfx::D3D9Renderer::sLogger = GN::getLogger("GN.gfx.rndr.D3D9");

// *****************************************************************************
// Global functions
// *****************************************************************************

bool gD3D9EnablePixPerf = true; // default is enabled

#if GN_STATIC
GN::gfx::Renderer *
createD3D9Renderer()
#else
extern "C" GN_EXPORT GN::gfx::Renderer *
GNgfxCreateRenderer()
#endif
{
    GN_GUARD;

    struct LOCAL
    {
        static bool sLookForNvPerfHUD()
        {
#if GN_XENON
            return false;
#else
            GN::AutoComPtr<IDirect3D9> d3d;
            d3d.attach( Direct3DCreate9(D3D_SDK_VERSION) );
            if( !d3d ) return false;
            UINT nAdapter = d3d->GetAdapterCount();
            GN_ASSERT( nAdapter );
            for( UInt32 i = 0; i < nAdapter; ++i )
            {
                D3DADAPTER_IDENTIFIER9 Identifier;
                GN_DX9_CHECK( d3d->GetAdapterIdentifier( i, 0, &Identifier ) );
                if( strstr(Identifier.Description,"PerfHUD") ) return true;
            }
            return false;
#endif
        }
    };

    // check for NVPerfHUD
    gD3D9EnablePixPerf = !LOCAL::sLookForNvPerfHUD();

    // create renderer
    GN::AutoObjPtr<GN::gfx::D3D9Renderer> p( new GN::gfx::D3D9Renderer );
    if( !p->init() ) return 0;
    return p.detach();

    GN_UNGUARD;
}

// *****************************************************************************
// init/quit functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9Renderer::init()
{
    GN_GUARD;

    GN_ASSERT( getCurrentThreadId() == mThreadId );

    PIXPERF_FUNCTION_EVENT();

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3D9Renderer, () );

    // init sub-components
    if( !dispInit()         ) return failure();
    if( !capsInit()         ) return failure();
    if( !resourceInit()     ) return failure();
    if( !contextInit()      ) return failure();
    if( !drawInit()         ) return failure();

    // successful
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9Renderer::quit()
{
    GN_GUARD;

    GN_ASSERT( getCurrentThreadId() == mThreadId );

    PIXPERF_FUNCTION_EVENT();

    deviceDestroy();

    drawQuit();
    contextQuit();
    resourceQuit();
    capsQuit();
    dispQuit();

    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// from Renderer
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9Renderer::changeOptions( const RendererOptions & options, bool forceRecreation )
{
    GN_GUARD;

    GN_ASSERT( getCurrentThreadId() == mThreadId );

    PIXPERF_FUNCTION_EVENT();

    // prepare for function re-entrance.
    if( mDeviceChanging )
    {
        GN_WARN(sLogger)( "This call to changeOptions() is ignored to avoid function re-entance!" );
        return true;
    }
    ScopeBool __dummy__(mDeviceChanging);

    RendererOptions newro = options;

#if GN_XENON
    // Xenon does not support windowed mode and external render window.
    newro.fullscreen = true;
    //newro.displayMode.set(0,0,0,0);
    newro.useExternalWindow = false;
#endif

    // store old settings
    const RendererOptions oldro = getOptions();
    const DispDesc oldDesc = getDispDesc();

    // setup new settings
    if( !processUserOptions( newro ) ) return false;

    const DispDesc & newDesc = getDispDesc();

    if( forceRecreation ||
        0 == mDevice ||
        oldDesc.windowHandle != newDesc.windowHandle ||
        oldDesc.monitorHandle != newDesc.monitorHandle ||
        oldro.reference != newro.reference ||
        oldro.software != newro.software ||
        oldro.pure != newro.pure ||
        oldro.multithread != newro.multithread )
    {
        // we have to recreate the whole device.
        deviceDestroy();
        return deviceCreate();
    }
    else if(
        oldDesc != newDesc ||
        oldro.msaa != newro.msaa ||
        oldro.fullscreen != newro.fullscreen ||
        oldro.vsync != newro.vsync )
    {
        // a device reset should be enough
        deviceDispose();
        return deviceRestore();
    }
    else
    {
        // do nothing, if new setting is equal to current setting
        return true;
    }

    GN_UNGUARD;
}

// *****************************************************************************
// device management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9Renderer::deviceCreate()
{
    GN_GUARD;

    GN_ASSERT( getCurrentThreadId() == mThreadId );

    PIXPERF_FUNCTION_EVENT();

    _GNGFX_DEVICE_TRACE();

    GN_ASSERT( mDeviceChanging );

    #define COMPONENT_RECREATE(X) if( !X##DeviceCreate() || !X##DeviceRestore() ) return false;

    COMPONENT_RECREATE( disp );
    COMPONENT_RECREATE( caps );
    COMPONENT_RECREATE( resource );
    COMPONENT_RECREATE( context );
    COMPONENT_RECREATE( draw );

    #undef COMPONENT_RECREATE

    // trigger signals
    GN_TRACE(sLogger)( "GFX SIGNAL: D3D9 device create." );
    if( !gSigRendererCreate() ) return false;

    GN_TRACE(sLogger)( "GFX SIGNAL: D3D9 device restore." );
    if( !gSigRendererRestore() ) return false;

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9Renderer::deviceRestore()
{
    GN_GUARD;

    GN_ASSERT( getCurrentThreadId() == mThreadId );

    PIXPERF_FUNCTION_EVENT();

    _GNGFX_DEVICE_TRACE();

    if( !dispDeviceRestore() ) return false;
    if( !capsDeviceRestore() ) return false;
    if( !resourceDeviceRestore() ) return false;
    if( !contextDeviceRestore() ) return false;
    if( !drawDeviceRestore() ) return false;

    // trigger reset event
    GN_TRACE(sLogger)( "GFX SIGNAL: D3D9 device restore." );
    if( !gSigRendererRestore() ) return false;

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9Renderer::deviceDispose()
{
    GN_GUARD;

    GN_ASSERT( getCurrentThreadId() == mThreadId );

    PIXPERF_FUNCTION_EVENT();

    _GNGFX_DEVICE_TRACE();

    GN_TRACE(sLogger)( "GFX SIGNAL: D3D9 device dispose." );
    gSigRendererDispose();

    drawDeviceDispose();
    contextDeviceDispose();
    resourceDeviceDispose();
    capsDeviceDispose();
    dispDeviceDispose();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9Renderer::deviceDestroy()
{
    GN_GUARD;

    GN_ASSERT( getCurrentThreadId() == mThreadId );

    PIXPERF_FUNCTION_EVENT();

    _GNGFX_DEVICE_TRACE();

    if( mDevice )
    {
        GN_TRACE(sLogger)( "GFX SIGNAL: D3D9 device dispose." );
        gSigRendererDispose();

        GN_TRACE(sLogger)( "GFX SIGNAL: D3D9 device destroy." );
        gSigRendererDestroy();
    }

    #define COMPONENT_DESTROY(X) X##DeviceDispose(); X##DeviceDestroy();

    COMPONENT_DESTROY( draw );
    COMPONENT_DESTROY( context );
    COMPONENT_DESTROY( resource );
    COMPONENT_DESTROY( caps );
    COMPONENT_DESTROY( disp );

    #undef COMPONENT_DESTROY

    GN_UNGUARD;
}
