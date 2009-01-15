#include "pch.h"

using namespace GN;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.Mesh");

// *****************************************************************************
// Mesh class
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::Mesh::init( const MeshDesc & desc )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::Mesh, () );

    // success
    mDesc = desc;
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::Mesh::quit()
{
    GN_GUARD;

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::Mesh::applyToDrawable( Drawable & );

//
//
// -----------------------------------------------------------------------------
void GN::gfx::Mesh::applySubsetToDrawable( Drawable & drawable, size_t fistIdx, size_t numIndices )
{
}
