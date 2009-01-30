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
    GN_ASSERT( rndr );

    // bind context
    rndr->bindContext( rc );

    // do rendering
    if( rc.idxbuf )
    {
        rndr->drawIndexed( prim, numidx, basevtx, startvtx, numvtx, startidx );
    }
    else
    {
        rndr->draw( prim, numvtx, startvtx );
    }
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::Drawable &
GN::gfx::Drawable::operator=( const Drawable & rhs )
{
    rndr = rhs.rndr;
    rc = rhs.rc;
    prim = rhs.prim;
    startvtx = rhs.startvtx;
    numvtx = rhs.numvtx;
    startidx = rhs.startidx;
    numidx = rhs.numidx;
    basevtx = rhs.basevtx;

    return *this;
}
