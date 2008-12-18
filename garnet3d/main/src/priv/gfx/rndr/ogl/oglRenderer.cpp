#include "pch.h"
#include "oglRenderer.h"

GN::Logger * GN::gfx::OGLRenderer::sLogger = GN::getLogger("GN.gfx.rndr.OGL");

// *****************************************************************************
// Global functions
// *****************************************************************************

#if GN_BUILD_STATIC
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
bool GN::gfx::OGLRenderer::reset( const RendererOptions & ro )
{
    GN_GUARD;

    // prepare for function re-entrance.
    if( mOnGoingReset )
    {
        GN_WARN(sLogger)( "This call to reset() is ignored to avoid function re-entance!" );
        return true;
    }
    ScopeBool __dummy__(mOnGoingReset);

    // store old display settings
    const DispDesc oldDesc = getDispDesc();

    // setup new display settings
    if( !setupOptionAndDispDesc( ro ) ) return false;

    const DispDesc & newDesc = getDispDesc();

    if( 0 == getOGLRC() || oldDesc != newDesc )
    {
        // full device recreation
        deviceDestroy();
        return deviceCreate();
    }
    else
    {
        // do nothing, since new setting is equal to current setting
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

    if( !dispDeviceCreate() ) return false;
    if( !capsDeviceCreate() ) return false;
    if( !resourceDeviceCreate() ) return false;
    if( !contextDeviceCreate() ) return false;
    if( !drawDeviceCreate() ) return false;

    // trigger signal
    GN_TRACE(sLogger)( "GFX SIGNAL: OGL device create." );
    if( !gSigRendererCreate( *this ) ) return false;

    // success
    return true;

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
        GN_TRACE(sLogger)( "GFX SIGNAL: OGL device destroy." );
        gSigRendererDestroy( *this );
    }

    drawDeviceDestroy();
    contextDeviceDestroy();
    resourceDeviceDestroy();
    capsDeviceDestroy();
    dispDeviceDestroy();

    GN_UNGUARD;
}
