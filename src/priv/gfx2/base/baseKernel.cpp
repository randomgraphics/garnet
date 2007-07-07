#include "pch.h"
#include "garnet/gfx2/baseGraphicsSystem.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx2.base.BaseKernel");

// *****************************************************************************
// BaseKernel
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
const GN::gfx::StreamSource * GN::gfx::BaseKernel::getStream( size_t index ) const
{
    if( index >= mStreams.size() )
    {
        GN_ERROR(sLogger)( "stream index is out of range." );
        return 0;
    }

    GN_ASSERT( mStreams.at(index) );

    return mStreams.at(index);
}

//
//
// -----------------------------------------------------------------------------
const GN::gfx::StreamSource * GN::gfx::BaseKernel::getStream( const StrA & name ) const
{
    size_t index = mStreams.getIndex( name );
    if( (size_t)-1 == index ) return 0;

    GN_ASSERT( mStreams.at(index) );

    return mStreams.at(index);
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::KernelParameterSet * GN::gfx::BaseKernel::createParameterSet()
{
    GN_GUARD;

    return new BaseKernelParameterSet( *this );

    GN_UNGUARD;
}

// *****************************************************************************
// protected methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
size_t GN::gfx::BaseKernel::addStreamRef( const StrA & name, StreamSource * stream )
{
    if( 0 == stream )
    {
        GN_ERROR(sLogger)( "addStreamRef() failed: NULL stream pointer." );
        GN_UNEXPECTED();
        return (size_t)-1;
    }

    return mStreams.add( name, stream );
}

//
//
// -----------------------------------------------------------------------------
size_t
GN::gfx::BaseKernel::addParameter(
    const StrA        & name,
    KernelParameterType type,
    size_t              count )
{
    GN_GUARD;

    KernelParameterDesc desc;
    desc.type  = type;
    desc.count = count;

    return mParameters.add( name, desc );

    GN_UNGUARD;
}
