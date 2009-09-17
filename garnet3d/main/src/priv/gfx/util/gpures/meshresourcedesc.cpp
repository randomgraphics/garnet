#include "pch.h"

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpures");

// *****************************************************************************
// Local stuff
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------

// *****************************************************************************
// public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::MeshResourceDesc::loadFromFile( File & )
{
    clear();
    GN_UNIMPL();
    return false;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::MeshResourceDesc::loadFromFile( const char * )
{
    clear();
    GN_UNIMPL();
    return false;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::MeshResourceDesc::saveToFile( File & ) const
{
    GN_UNIMPL();
    return false;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::MeshResourceDesc::saveToFile( const char * ) const
{
    GN_UNIMPL();
    return false;
}
