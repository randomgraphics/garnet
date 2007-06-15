#include "pch.h"
#include "garnet/GNgfx2D3D9.h"

//
//
// -----------------------------------------------------------------------------
GN::gfx2::GraphicsSystem *
GN::gfx2::createGraphicsSystem( const GraphicsSystemCreationParameter & gscp )
{
    GN_GUARD;

    AutoObjPtr<D3D9GraphicsSystem> gs( new D3D9GraphicsSystem );

    if( !gs->init( gscp ) ) return false;

    return gs.detach();

    GN_UNGUARD;
}
