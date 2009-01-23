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

    // apply parameters
    if( rc.gpuProgram )
    {
        for( size_t i = 0; i < gpps.size(); ++i )
        {
            GpuProgramParam * gpp = gpps[i].get();

            if( gpp )
            {
                rc.gpuProgram->setParameter( i, gpp->get(), gpp->size() );
            }
        }
    }

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

    // copy parameters
    gpps.resize( rhs.gpps.size() );
    for( size_t i = 0; i < gpps.size(); ++i )
    {
        gpps[i] = rhs.gpps[i];
    }

    return *this;
}
