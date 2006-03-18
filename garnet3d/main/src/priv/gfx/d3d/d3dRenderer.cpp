#include "pch.h"
#include "d3dRenderer.h"
#include "d3dResource.h"

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

#pragma comment(lib, "d3d9.lib") // D3D for PC has no d3d9d.lib

#endif // GN_XENON

#endif // GN_MSVC

// *****************************************************************************
// Global functions
// *****************************************************************************

#if GN_STATIC
GN::gfx::Renderer *
createD3DRenderer( const GN::gfx::RendererOptions & ro )
#else
extern "C" GN_EXPORT GN::gfx::Renderer *
GNgfxCreateRenderer( const GN::gfx::RendererOptions & ro )
#endif
{
    GN_GUARD;

    GN::AutoObjPtr<GN::gfx::D3DRenderer> p( new GN::gfx::D3DRenderer );
    if( !p->init(ro) ) return 0;
    return p.detach();

    GN_UNGUARD;
}

// *****************************************************************************
// init/quit functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3DRenderer::init(const RendererOptions & ro )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3DRenderer, () );

    // init sub-components
    if( !dispInit()         ) { quit(); return selfOK(); }
    if( !capsInit()         ) { quit(); return selfOK(); }
    if( !resourceInit()     ) { quit(); return selfOK(); }
    if( !contextInit()      ) { quit(); return selfOK(); }
    if( !drawInit()         ) { quit(); return selfOK(); }

    // create & reset device data
    if( !doOptionChange( ro, OCT_INIT ) ) { quit(); return selfOK(); }

    // successful
    return selfOK();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::quit()
{
    GN_GUARD;

    deviceDestroy( true );

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
bool GN::gfx::D3DRenderer::changeOptions( const RendererOptions & ro, bool forceRecreation )
{
    GN_GUARD;
    return doOptionChange( ro, forceRecreation ? OCT_CREATE : OCT_AUTO );
    GN_UNGUARD;
}

// *****************************************************************************
// device management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3DRenderer::deviceCreate( bool triggerInitSignal )
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
    if( triggerInitSignal )
    {
        GN_INFO( "GFX SIGNAL: D3D renderer init." );
        if( !sSigInit() ) return false;
    }

    GN_INFO( "GFX SIGNAL: D3D device create." );
    if( !sSigDeviceCreate() ) return false;

    GN_INFO( "GFX SIGNAL: D3D device restore." );
    if( !sSigDeviceRestore() ) return false;

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3DRenderer::deviceRestore()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    if( !dispDeviceRestore() ) return false;
    if( !capsDeviceRestore() ) return false;
    if( !resourceDeviceRestore() ) return false;
    if( !contextDeviceRestore() ) return false;
    if( !drawDeviceRestore() ) return false;

    // trigger reset event
    GN_INFO( "GFX SIGNAL: D3D device restore." );
    if( !sSigDeviceRestore() ) return false;

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::deviceDispose()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    GN_INFO( "GFX SIGNAL: D3D device dispose." );
    sSigDeviceDispose();

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
void GN::gfx::D3DRenderer::deviceDestroy( bool triggerQuitSignal )
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    if( mDevice )
    {
        GN_INFO( "GFX SIGNAL: D3D device dispose." );
        sSigDeviceDispose();

        GN_INFO( "GFX SIGNAL: D3D device destroy." );
        sSigDeviceDestroy();

        if( triggerQuitSignal )
        {
            GN_INFO( "GFX SIGNAL: D3D renderer quit." );
            sSigQuit();
        }
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

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3DRenderer::doOptionChange( RendererOptions ro, OptionChangingType type )
{
    GN_GUARD;

    // prepare for function re-entrance.
    if( mDeviceChanging )
    {
        GN_WARN( "This call to changeOptions() is ignored to avoid function re-entance!" );
        return true;
    }
    ScopeBool __dummy__(mDeviceChanging);

#if GN_XENON
    if( !ro.fullscreen )
    {
        GN_WARN( "Windowed mode is not supported on Xenon platform. Force fullscreen mode." );
        ro.fullscreen = true;
        ro.displayMode.set(0,0,0,0);
    }
    if( ro.useExternalWindow )
    {
        GN_WARN( "External render windowe is not supported on Xenon platform. Force internal render window." );
        ro.useExternalWindow = false;
    }
#endif

    // store old settings
    const RendererOptions oldOptions = getOptions();
    const DispDesc oldDesc = getDispDesc();

    // setup new settings
    if( !processUserOptions( ro ) ) return false;

    const DispDesc & newDesc = getDispDesc();

    if( OCT_CREATE == type ||
        OCT_INIT == type ||
        oldDesc.windowHandle != newDesc.windowHandle ||
        oldDesc.monitorHandle != newDesc.monitorHandle ||
        oldOptions.reference != ro.reference ||
        oldOptions.software != ro.software ||
        oldOptions.pure != ro.pure ||
        oldOptions.multithread != ro.multithread )
    {
        // we have to recreate the whole device.
        deviceDestroy( OCT_INIT == type );
        return deviceCreate( OCT_INIT == type );
    }
    else if(
        oldDesc != newDesc ||
        oldOptions.msaa != ro.msaa ||
        oldOptions.fullscreen != ro.fullscreen ||
        oldOptions.vsync != ro.vsync )
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
