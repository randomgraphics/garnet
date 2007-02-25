#include "pch.h"
#include "d3d10Renderer.h"

#if GN_MSVC

#pragma comment(lib, "d3d10.lib")
#if GN_DEBUG_BUILD
#pragma comment(lib, "d3dx10d.lib")
#else
#pragma comment(lib, "d3dx10.lib")
#endif

#endif // GN_MSVC

GN::Logger * GN::gfx::D3D10Renderer::sLogger = GN::getLogger("GN.gfx.rndr.D3D10");

// *****************************************************************************
// Global functions
// *****************************************************************************

bool gD3D10EnablePixPerf = true; // default is enabled

#if GN_STATIC
GN::gfx::Renderer *
createD3D10Renderer()
#else
extern "C" GN_EXPORT GN::gfx::Renderer *
GNgfxCreateRenderer()
#endif
{
    GN_GUARD;

    GN::AutoObjPtr<GN::gfx::D3D10Renderer> p( new GN::gfx::D3D10Renderer );
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
bool GN::gfx::D3D10Renderer::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3D10Renderer, () );

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
void GN::gfx::D3D10Renderer::quit()
{
    GN_GUARD;

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
bool GN::gfx::D3D10Renderer::changeOptions( const RendererOptions & ro, bool forceRecreation )
{
    GN_GUARD;

    // prepare for function re-entrance.
    if( mDeviceChanging )
    {
        GN_WARN(sLogger)( "This call to changeOptions() is ignored to avoid function re-entance!" );
        return true;
    }
    ScopeBool __dummy__(mDeviceChanging);

    RendererOptions newro = ro;

#if GN_XENON
    if( !newro.fullscreen )
    {
        GN_WARN(sLogger)( "Windowed mode is not supported on Xenon platform. Force fullscreen mode." );
        newro.fullscreen = true;
        newro.displayMode.set(0,0,0,0);
    }
    if( newro.useExternalWindow )
    {
        GN_WARN(sLogger)( "External render windowe is not supported on Xenon platform. Force internal render window." );
        newro.useExternalWindow = false;
    }
#endif

    // store old settings
    const RendererOptions oldro = getOptions();
    const DispDesc oldDesc = getDispDesc();

    // setup new settings
    if( !processUserOptions( newro ) ) return false;

    const DispDesc & newDesc = getDispDesc();

    if( 0 == mDevice ||
        forceRecreation ||
        oldDesc.windowHandle != newDesc.windowHandle ||
        oldDesc.monitorHandle != newDesc.monitorHandle ||
        oldro.reference != newro.reference ||
        oldro.software != newro.software ||
        oldro.pure != newro.pure ||
        oldro.multithread != newro.multithread ||
        oldro.msaa != newro.msaa )
    {
        // we have to recreate the whole device.
        deviceDestroy();
        return deviceCreate();
    }
    else if(
        oldDesc != newDesc ||
        oldro.fullscreen != newro.fullscreen )
    {
        GN_UNIMPL_WARNING();
        return false;
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
bool GN::gfx::D3D10Renderer::deviceCreate()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    GN_ASSERT( mDeviceChanging );

    if( !dispDeviceCreate()     ) return false;
    if( !capsDeviceCreate()     ) return false;
    if( !resourceDeviceCreate() ) return false;
    if( !contextDeviceCreate()  ) return false;
    if( !drawDeviceCreate()     ) return false;

    // trigger signals
    GN_TRACE(sLogger)( "GFX SIGNAL: D3D10 device create." );
    if( !sSigCreate() ) return false;

    GN_TRACE(sLogger)( "GFX SIGNAL: D3D10 device restore." );
    if( !sSigRestore() ) return false;

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10Renderer::deviceDestroy()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    if( mDevice )
    {
        GN_TRACE(sLogger)( "GFX SIGNAL: D3D10 device dispose." );
        sSigDispose();

        GN_TRACE(sLogger)( "GFX SIGNAL: D3D10 device destroy." );
        sSigDestroy();
    }

    drawDeviceDestroy();
    contextDeviceDestroy();
    resourceDeviceDestroy();
    capsDeviceDestroy();
    dispDeviceDestroy();

    GN_UNGUARD;
}
