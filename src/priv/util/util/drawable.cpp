#include "pch.h"
#include "garnet/GNutil.h"

using namespace GN;
using namespace GN::util;

// *****************************************************************************
// local var/types/functions
// *****************************************************************************

class PrivateGpp : public GpuProgramParam
{
public:

    /// constructor
    PrivateGpp( size_t size ) : GpuProgramParam( false, size ) {}
};

class SharedGpp;
static std::map<StrA,SharedGpp*> sSharedGppMap;

class SharedGpp : public GpuProgramParam
{
    std::map<StrA,SharedGpp*>::const_iterator mIter;

public:

    /// constructor
    SharedGpp( const StrA & name, size_t size )
        : GpuProgramParam( false, size )
    {
        mIter = sSharedGppMap.insert( std::make_pair(name, this) ).first;
    }

    /// dtor
    ~SharedGpp()
    {
        sSharedGppMap.erase( mIter );
    }
};

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GpuProgramParam * GN::util::createPrivateGpuProgramParam( size_t size )
{
    return new PrivateGpp( size );
}

//
//
// -----------------------------------------------------------------------------
GpuProgramParam * GN::util::createSharedGpuProgramParam( const StrA & name, size_t size )
{
    std::map<StrA,SharedGpp*>::const_iterator iter = sSharedGppMap.find( name );

    if( iter != sSharedGppMap.end() )
        // found!
        return iter->second;
    else
        // not found!
        return new SharedGpp( name, size );
}

// *****************************************************************************
// drawable class
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::util::Drawable::draw() const
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
GN::util::Drawable &
GN::util::Drawable::operator=( const Drawable & rhs )
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
        AutoRef<GpuProgramParam> & d = gpps[i];
        const AutoRef<GpuProgramParam> & s = rhs.gpps[i];

        if( NULL != s )
        {
            if( s->shared() )
            {
                d = s;
            }
            else
            {
                // make a clone for private parameter
                d.attach( createPrivateGpuProgramParam( s->size() ) );
                d->set( s->get(), s->size() );
            }
        }
    }

    return *this;
}

