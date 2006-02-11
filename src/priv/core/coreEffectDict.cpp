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

    mDict = new gfx::effect::EffectDictionary;

    // connect to renderer signals
    GN::gfx::Renderer::sSigDeviceDestroy.connect( mDict, &gfx::effect::EffectDictionary::dispose );

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

    if( mDict )
    {
        GN::gfx::Renderer::sSigDeviceDestroy.disconnect( mDict );
        delete mDict;
    }

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}
