#include "pch.h"
#include "coreResourceDict.h"

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::core::CoreEffectDict::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::core::CoreEffectDict, () );

    // connect to renderer signals
    GN::gfx::Renderer::sSigDeviceDestroy.connect( mDict, &gfx::effect::EffectDictionary::disposeAll );

    // success
    return selfOK();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::core::CoreEffectDict::quit()
{
    GN_GUARD;

    GN::gfx::Renderer::sSigDeviceDestroy.disconnect( mDict );
    mDict.clear();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}
