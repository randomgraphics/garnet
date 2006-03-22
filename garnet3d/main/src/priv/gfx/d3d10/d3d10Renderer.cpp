#include "pch.h"
#include "d3d10Renderer.h"

#if GN_MSVC

#pragma comment(lib, "d3d10.lib")
#if GN_DEBUG
#pragma comment(lib, "d3dx10d.lib")
#else
#pragma comment(lib, "d3dx10.lib")
#endif

#endif // GN_MSVC

// *****************************************************************************
// Global functions
// *****************************************************************************

#if GN_STATIC
GN::gfx::Renderer *
createD3D10Renderer( const GN::gfx::RendererOptions & ro )
#else
extern "C" GN_EXPORT GN::gfx::Renderer *
GNgfxCreateRenderer( const GN::gfx::RendererOptions & ro )
#endif
{
    GN_GUARD;

    GN::AutoObjPtr<GN::gfx::D3D10Renderer> p( new GN::gfx::D3D10Renderer );
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
bool GN::gfx::D3D10Renderer::init(const RendererOptions & ro )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3D10Renderer, () );

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
void GN::gfx::D3D10Renderer::quit()
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
bool GN::gfx::D3D10Renderer::changeOptions( const RendererOptions & ro, bool forceRecreation )
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
bool GN::gfx::D3D10Renderer::deviceCreate( bool triggerInitSignal )
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
    if( triggerInitSignal )
    {
        GN_INFO( "GFX SIGNAL: D3D10 renderer init." );
        if( !sSigInit() ) return false;
    }

    GN_INFO( "GFX SIGNAL: D3D10 device create." );
    if( !sSigDeviceCreate() ) return false;

    GN_INFO( "GFX SIGNAL: D3D10 device restore." );
    if( !sSigDeviceRestore() ) return false;

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10Renderer::deviceDestroy( bool triggerQuitSignal )
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    if( mDevice )
    {
        GN_INFO( "GFX SIGNAL: D3D10 device dispose." );
        sSigDeviceDispose();

        GN_INFO( "GFX SIGNAL: D3D10 device destroy." );
        sSigDeviceDestroy();

        if( triggerQuitSignal )
        {
            GN_INFO( "GFX SIGNAL: D3D10 renderer quit." );
            sSigQuit();
        }
    }

    drawDeviceDestroy();
    contextDeviceDestroy();
    resourceDeviceDestroy();
    capsDeviceDestroy();
    dispDeviceDestroy();

    GN_UNGUARD;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D10Renderer::doOptionChange( RendererOptions ro, OptionChangingType type )
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

    if( OCT_CREATE == type )
    {
        // reinitiate the whole device.
        deviceDestroy( true );
        return deviceCreate( true );
    }
    else if(
        OCT_INIT == type ||
        oldDesc.windowHandle != newDesc.windowHandle ||
        oldDesc.monitorHandle != newDesc.monitorHandle ||
        oldOptions.reference != ro.reference ||
        oldOptions.software != ro.software ||
        oldOptions.pure != ro.pure ||
        oldOptions.multithread != ro.multithread ||
        oldOptions.msaa != ro.msaa )
    {
        // we have to recreate the whole device.
        deviceDestroy( false );
        return deviceCreate( false );
    }
    else if(
        oldDesc != newDesc ||
        oldOptions.fullscreen != ro.fullscreen )
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
