#include "pch.h"

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx2::D3D9GraphicsSystem::init( const GraphicsSystemCreationParameter & gscp )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx2::D3D9GraphicsSystem, () );

    GN_UNUSED_PARAM( gscp );

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::D3D9GraphicsSystem::quit()
{
    GN_GUARD;

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}