#include "pch.h"

using namespace GN;
using namespace GN::gfx;

// *****************************************************************************
// drawable class
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::Drawable::draw() const
{
    GN_ASSERT( gpu );

    // bind context
    gpu->bindContext( rc );

    // do rendering
    if( rc.idxbuf )
    {
        gpu->drawIndexed( prim, numidx, basevtx, startvtx, numvtx, startidx );
    }
    else
    {
        gpu->draw( prim, numvtx, startvtx );
    }
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Drawable &
GN::gfx::Drawable::operator=( const Drawable & rhs )
{
    gpu = rhs.gpu;
    rc = rhs.rc;
    prim = rhs.prim;
    startvtx = rhs.startvtx;
    numvtx = rhs.numvtx;
    startidx = rhs.startidx;
    numidx = rhs.numidx;
    basevtx = rhs.basevtx;

    return *this;
}
