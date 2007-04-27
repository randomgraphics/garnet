#include "pch.h"
#include "drawThread.h"

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::engine::RenderEngine::DrawThread::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( DrawThread, () );

    // Do custom init here

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::DrawThread::quit()
{
    GN_GUARD;

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}