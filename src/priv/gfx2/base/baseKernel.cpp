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
size_t GN::gfx::BaseKernel::addStreamRef( StreamSource & stream )
{
    return mStreams.add( stream.getDesc().name, &stream );
}

//
//
// -----------------------------------------------------------------------------
size_t GN::gfx::BaseKernel::addParameter(
    const StrA        & name,
    KernelParameterType type,
    size_t              count )
{
    GN_GUARD;

    KernelParameterDesc desc;
    desc.name  = name;
    desc.type  = type;
    desc.count = count;

    return mParameters.add( name, desc );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
size_t GN::gfx::BaseKernel::addPortRef( BaseKernelPort & port )
{
    return mPorts.add( port.getDesc().name, &port );
}
