#include "pch.h"
#include "basicGpuXenon.h"

#if GN_XENON

static GN::Logger * sLogger = GN::GetLogger("GN.gfx.gpu.common");

// *****************************************************************************
//                         BasicGpuXenon init / quit functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::BasicGpuXenon::init( const GpuOptions & o )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( BasicGpuXenon, (o) );

    // initialize sub-components one by one
    if( !dispInit(o) ) return failure();

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BasicGpuXenon::quit()
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
bool GN::gfx::BasicGpuXenon::dispInit( const GpuOptions & ro )
{
    // get current display mode
    XVIDEO_MODE xvm;
    XGetVideoMode( &xvm );

    // fill DispDesc structure
    mDispDesc.width         = (0==ro.displayMode.width)  ? xvm.dwDisplayWidth  : ro.displayMode.width;
    mDispDesc.height        = (0==ro.displayMode.height) ? xvm.dwDisplayHeight : ro.displayMode.height;
    mDispDesc.depth         = (0==ro.displayMode.depth)  ? 32                  : ro.displayMode.depth;
    mDispDesc.refrate       = ro.displayMode.refrate;
    mDispDesc.displayHandle = 0;
    mDispDesc.monitorHandle = 0;
    mDispDesc.windowHandle  = 0;

    // store GPU options
    mOptions = ro;

    // success
    return true;
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::BasicGpuXenon::dispQuit()
{
}

#endif
