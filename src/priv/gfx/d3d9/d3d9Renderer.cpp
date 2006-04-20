#include "pch.h"
#include "d3d9Renderer.h"
#include "d3d9Resource.h"

#if GN_MSVC

#pragma comment( lib, "dxerr9.lib" )
#if GN_DEBUG
#pragma comment(lib, "d3dx9d.lib")
#else
#pragma comment(lib, "d3dx9.lib")
#endif

#if GN_XENON

#pragma comment(lib, "xboxkrnl.lib")
#pragma comment(lib, "xbdm.lib")

#if GN_DEBUG
#pragma comment(lib, "d3d9d.lib")
#pragma comment(lib, "xapilibd.lib")
#pragma comment(lib, "xgraphicsd.lib")
#else
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "xapilib.lib")
#pragma comment(lib, "xgraphics.lib")
#endif // GN_DEBUG
// more libraries: xnetd.lib xaudiod.lib xactd.lib vcompd.lib

#else // !GN_XENON

#pragma comment(lib, "d3d9.lib") // D3D9 for PC has no d3d9d.lib

#endif // GN_XENON

#endif // GN_MSVC

// *****************************************************************************
// Global functions
// *****************************************************************************

#if GN_STATIC
GN::gfx::Renderer *
createD3D9Renderer()
#else
extern "C" GN_EXPORT GN::gfx::Renderer *
GNgfxCreateRenderer()
#endif
{
    GN_GUARD;

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

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3D9Renderer, () );

    // init sub-components
    if( !dispInit()         ) { quit(); return selfOK(); }
    if( !capsInit()         ) { quit(); return selfOK(); }
    if( !resourceInit()     ) { quit(); return selfOK(); }
    if( !contextInit()      ) { quit(); return selfOK(); }
    if( !drawInit()         ) { quit(); return selfOK(); }

    // successful
    return selfOK();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9Renderer::quit()
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
bool GN::gfx::D3D9Renderer::changeOptions( const RendererOptions & options, bool forceRecreation )
{
    GN_GUARD;

    // prepare for function re-entrance.
    if( mDeviceChanging )
    {
        GN_WARN( "This call to changeOptions() is ignored to avoid function re-entance!" );
        return true;
    }
    ScopeBool __dummy__(mDeviceChanging);

    RendererOptions newro = options;

#if GN_XENON
    if( !newro.fullscreen )
    {
        GN_WARN( "Windowed mode is not supported on Xenon platform. Force fullscreen mode." );
        newro.fullscreen = true;
        newro.displayMode.set(0,0,0,0);
    }
    if( newro.useExternalWindow )
    {
        GN_WARN( "External render windowe is not supported on Xenon platform. Force internal render window." );
        newro.useExternalWindow = false;
    }
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
    GN_INFO( "GFX SIGNAL: D3D9 device create." );
    if( !sSigCreate() ) return false;

    GN_INFO( "GFX SIGNAL: D3D9 device restore." );
    if( !sSigRestore() ) return false;

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

    _GNGFX_DEVICE_TRACE();

    if( !dispDeviceRestore() ) return false;
    if( !capsDeviceRestore() ) return false;
    if( !resourceDeviceRestore() ) return false;
    if( !contextDeviceRestore() ) return false;
    if( !drawDeviceRestore() ) return false;

    // trigger reset event
    GN_INFO( "GFX SIGNAL: D3D9 device restore." );
    if( !sSigRestore() ) return false;

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

    _GNGFX_DEVICE_TRACE();

    GN_INFO( "GFX SIGNAL: D3D9 device dispose." );
    sSigDispose();

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

    _GNGFX_DEVICE_TRACE();

    if( mDevice )
    {
        GN_INFO( "GFX SIGNAL: D3D9 device dispose." );
        sSigDispose();

        GN_INFO( "GFX SIGNAL: D3D9 device destroy." );
        sSigDestroy();
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
