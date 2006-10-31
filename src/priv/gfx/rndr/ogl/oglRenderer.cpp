#include "pch.h"
#include "oglRenderer.h"

GN::Logger * GN::gfx::OGLRenderer::sLogger = GN::getLogger("GN.gfx.rndr.OGL");

// *****************************************************************************
// Global functions
// *****************************************************************************

#if GN_STATIC
GN::gfx::Renderer * createOGLRenderer()
#else
extern "C" GN_EXPORT GN::gfx::Renderer * GNgfxCreateRenderer()
#endif
{
    GN_GUARD;

    GN::AutoObjPtr<GN::gfx::OGLRenderer> p( new GN::gfx::OGLRenderer );
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
bool GN::gfx::OGLRenderer::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::OGLRenderer, () );

    // init sub-components
    if( !dispInit()     ) return failure();
    if( !capsInit()     ) return failure();
    if( !resourceInit() ) return failure();
    if( !contextInit()  ) return failure();
    if( !drawInit()     ) return failure();

    // successful
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::quit()
{
    GN_GUARD;

    deviceDestroy();

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

    // prepare for function re-entrance.
    if( mDeviceChanging )
    {
        GN_WARN(sLogger)( "This call to changeOptions() is ignored to avoid function re-entance!" );
        return true;
    }
    ScopeBool __dummy__(mDeviceChanging);

    // store old display settings
    const RendererOptions & oldOptions = getOptions();
    const DispDesc oldDesc = getDispDesc();

    // setup new display settings
    if( !processUserOptions( ro ) ) return false;

    const DispDesc & newDesc = getDispDesc();

    if( forceRecreation ||
        0 == getOGLRC() ||
        oldDesc.windowHandle != newDesc.windowHandle )
    {
        // we have to perform a full device recreation
        deviceDestroy();
        return deviceCreate();
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

// *****************************************************************************
// init/quit functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLRenderer::deviceCreate()
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
    GN_TRACE(sLogger)( "GFX SIGNAL: OGL device create." );
    if( !sSigCreate() ) return false;

    GN_TRACE(sLogger)( "GFX SIGNAL: OGL device restore." );
    if( !sSigRestore() ) return false;

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
    GN_TRACE(sLogger)( "GFX SIGNAL: OGL device restore." );
    if( !sSigRestore() ) return false;

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

    GN_TRACE(sLogger)( "GFX SIGNAL: OGL device dispose." );
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
void GN::gfx::OGLRenderer::deviceDestroy()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    if( getOGLRC() )
    {
        GN_TRACE(sLogger)( "GFX SIGNAL: OGL device dispose." );
        sSigDispose();

        GN_TRACE(sLogger)( "GFX SIGNAL: OGL device destroy." );
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
