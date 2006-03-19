#include "pch.h"
#include "oglRenderer.h"

#if GN_MSVC
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib" )
#endif

// *****************************************************************************
// Global functions
// *****************************************************************************

#if GN_STATIC
GN::gfx::Renderer *
createOGLRenderer( const GN::gfx::RendererOptions & ro )
#else
extern "C" GN_EXPORT GN::gfx::Renderer *
GNgfxCreateRenderer( const GN::gfx::RendererOptions & ro )
#endif
{
    GN_GUARD;

    GN::AutoObjPtr<GN::gfx::OGLRenderer> p( new GN::gfx::OGLRenderer );
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
bool GN::gfx::OGLRenderer::init(const RendererOptions & ro )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::OGLRenderer, () );

    // init sub-components
    if( !dispInit()     ) { quit(); return selfOK(); }
    if( !capsInit()     ) { quit(); return selfOK(); }
    if( !resourceInit() ) { quit(); return selfOK(); }
    if( !contextInit()  ) { quit(); return selfOK(); }
    if( !drawInit()     ) { quit(); return selfOK(); }

    // create & reset device data
    if( !doOptionChange( ro, OCT_INIT ) ) { quit(); return selfOK(); }

    // successful
    return selfOK();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::quit()
{
    GN_GUARD;

    deviceDestroy( true );

    drawQuit();
    contextQuit();
    resourceQuit();
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
bool GN::gfx::OGLRenderer::changeOptions( const RendererOptions & ro, bool forceRecreation )
{
    GN_GUARD;
    return doOptionChange( ro, forceRecreation ? OCT_CREATE : OCT_AUTO );
    GN_UNGUARD;
}

// *****************************************************************************
// init/quit functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLRenderer::deviceCreate( bool triggerInitSignal )
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

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
        GN_INFO( "GFX SIGNAL: OGL renderer init." );
        if( !sSigInit() ) return false;
    }

    GN_INFO( "GFX SIGNAL: OGL device create." );
    if( !sSigDeviceCreate() ) return false;

    GN_INFO( "GFX SIGNAL: OGL device restore." );
    if( !sSigDeviceRestore() ) return false;

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLRenderer::deviceRestore()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    if( !dispDeviceRestore() ) return false;
    if( !capsDeviceRestore() ) return false;
    if( !resourceDeviceRestore() ) return false;
    if( !contextDeviceRestore() ) return false;
    if( !drawDeviceRestore() ) return false;

    // trigger reset event
    GN_INFO( "GFX SIGNAL: OGL device restore." );
    if( !sSigDeviceRestore() ) return false;

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::deviceDispose()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    GN_INFO( "GFX SIGNAL: OGL device dispose." );
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
void GN::gfx::OGLRenderer::deviceDestroy( bool triggerQuitSignal )
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    if( getOGLRC() )
    {
        GN_INFO( "GFX SIGNAL: OGL device dispose." );
        sSigDeviceDispose();

        GN_INFO( "GFX SIGNAL: OGL device destroy." );
        sSigDeviceDestroy();

        if( triggerQuitSignal )
        {
            GN_INFO( "GFX SIGNAL: OGL renderer quit." );
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
bool GN::gfx::OGLRenderer::doOptionChange( RendererOptions ro, OptionChangingType type )
{
    GN_GUARD;

    // prepare for function re-entrance.
    if( mDeviceChanging )
    {
        GN_WARN( "This call to changeOptions() is ignored to avoid function re-entance!" );
        return true;
    }
    ScopeBool __dummy__(mDeviceChanging);

    // store old display settings
    const RendererOptions oldOptions = getOptions();
    const DispDesc oldDesc = getDispDesc();

    // setup new display settings
    if( !processUserOptions( ro ) ) return false;

    const DispDesc & newDesc = getDispDesc();

    if( OCT_INIT == type )
    {
        deviceDestroy( true );
        return deviceCreate( true );
    }
    if( OCT_CREATE == type ||
        oldDesc.windowHandle != newDesc.windowHandle )
    {
        // we have to perform a full device recreation
        deviceDestroy( false );
        return deviceCreate( false );
    }
    else if( oldDesc != newDesc ||
        oldOptions.fullscreen != ro.fullscreen ||
        oldOptions.vsync != ro.vsync ||
        oldOptions.autoRestore != ro.autoRestore )
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
