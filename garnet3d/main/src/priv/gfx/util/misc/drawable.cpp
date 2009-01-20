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
        rndr->drawIndexed( prim, numidx, startvtx, minvtxidx, numvtx, startidx );
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
    numvtx = rhs.numvtx;
    numidx = rhs.numidx;
    minvtxidx = rhs.minvtxidx;
    startidx = rhs.startidx;

    // copy parameters
    gpps.resize( rhs.gpps.size() );
    for( size_t i = 0; i < gpps.size(); ++i )
    {
        gpps[i] = rhs.gpps[i];
    }

    return *this;
}
