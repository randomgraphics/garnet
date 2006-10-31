#include "pch.h"
#include "basicRenderer.h"

GN::Logger * GN::gfx::BasicRenderer::sLogger = GN::getLogger("GN.gfx.rndr.common");

// *****************************************************************************
//                         BasicRenderer init / quit functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::BasicRenderer::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( BasicRenderer, () );

    // initialize sub-components one by one
    if( !dispInit()    ) return failure();
    if( !resInit()     ) return failure();
    if( !contextInit() ) return failure();
    if( !drawInit()    ) return failure();

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BasicRenderer::quit()
{
    GN_GUARD;

    // shutdown sub-components in reverse sequence
    drawQuit();
    contextQuit();
    resQuit();
    dispQuit();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}
