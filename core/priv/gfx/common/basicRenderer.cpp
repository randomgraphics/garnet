#include "pch.h"
#include "BasicRenderer.h"

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

    // initialize sub-modules one by one
    if( !dispInit()  ) { quit(); return selfOK(); }
    if( !drawInit()  ) { quit(); return selfOK(); }

    // success
    return selfOK();

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
    dispQuit();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}
