#include "pch.h"
#include "basicRendererXenon.h"

#if GN_XENON

static GN::Logger * GN::sLogger = GN::getLogger("GN.gfx.rndr.common");

// *****************************************************************************
//                         BasicRendererXenon init / quit functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::BasicRendererXenon::init( const RendererOptions & o )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( BasicRendererXenon, () );

    // initialize sub-components one by one
    if( !dispInit(o) ) return failure();

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BasicRendererXenon::quit()
{
    GN_GUARD;

    // shutdown sub-components in reverse sequence
    dispQuit();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// private function
// *****************************************************************************

//
//
// ----------------------------------------------------------------------------
bool GN::gfx::BasicRendererXenon::dispInit( const RendererOptions & ro )
{
    // get current display mode
    XVIDEO_MODE xvm;
    XGetVideoMode( &xvm );

    // fill DispDesc structure
    DispDesc desc;
    mDispDesc.width         = (0==ro.displayMode.width)  ? xvm.dwDisplayWidth  : ro.displayMode.width;
    mDispDesc.height        = (0==ro.displayMode.height) ? xvm.dwDisplayHeight : ro.displayMode.height;
    mDispDesc.depth         = (0==ro.displayMode.depth)  ? 32                  : ro.displayMode.depth;
    mDispDesc.refrate       = ro.displayMode.refrate;
    mDispDesc.displayHandle = 0;
    mDispDesc.monitorHandle = 0;
    mDispDesc.windowHandle  = 0;

    // store renderer options
    mOptions = ro;

    // success
    return true;
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::BasicRendererXenon::dispQuit()
{
}

#endif
