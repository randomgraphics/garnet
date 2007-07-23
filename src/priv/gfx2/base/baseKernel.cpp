#include "pch.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx2.base.BaseKernel");

// *****************************************************************************
// BaseKernel
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::StreamSource * GN::gfx::BaseKernel::getStream( size_t index ) const
{
    StreamSource * const * p = mStreams.get( index );
    if( 0 == p ) return 0;
    GN_ASSERT( *p );
    return *p;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::StreamSource * GN::gfx::BaseKernel::getStream( const StrA & name ) const
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

//
//
// -----------------------------------------------------------------------------
const GN::gfx::KernelPortDesc * GN::gfx::BaseKernel::getPortDesc( size_t index ) const
{
    if( index >= mPorts.size() )
    {
        GN_ERROR(sLogger)( "Port index is out of range." );
        return 0;
    }
    const BaseKernelPort * p = mPorts.at( index );

    GN_ASSERT( p );

    return &p->getDesc();
}

//
//
// -----------------------------------------------------------------------------
const GN::gfx::KernelPortDesc * GN::gfx::BaseKernel::getPortDesc( const StrA & name ) const
{
    size_t index = mPorts.getIndex( name );
    if( (size_t)-1 == index ) return 0;

    GN_ASSERT( mPorts.at(index) );

    return &mPorts.at(index)->getDesc();
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::BaseKernel::compatible( const Surface * surf, const StrA & portName ) const
{
    const BaseKernelPort * const * port = mPorts.get( portName );
    if( 0 == port ) return false;
    return (*port)->compatible( surf );
}

// *****************************************************************************
// protected methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
size_t GN::gfx::BaseKernel::addStreamRef( const StrA & name, StreamSource & stream )
{
    return mStreams.add( name, &stream );
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

//
//
// -----------------------------------------------------------------------------
size_t GN::gfx::BaseKernel::addPortRef( const StrA & name, BaseKernelPort & port )
{
    return mPorts.add( name, &port );
}
