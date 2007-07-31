#include "pch.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx2.base.BaseKernel");

static const GN::gfx::KernelReflection & sGetNonDummyReflection( const char * name )
{
    bool dummy;
    const GN::gfx::KernelReflection & refl = GN::gfx::getKernelReflection( name, &dummy );
    if( dummy )
    {
        GN_UNEXPECTED();
    }
    return refl;
}

// *****************************************************************************
// BaseKernel
// *****************************************************************************

GN::gfx::BaseKernel::BaseKernel( const char * name )
    : mRefl( sGetNonDummyReflection( name ) )
    , mStreams( mRefl.streams.size() )
    , mPorts( mRefl.ports.size() )
{
    memset( mStreams.cptr(), 0, mStreams.size() * sizeof(void*) );
    memset( mPorts.cptr(), 0, mPorts.size() * sizeof(void*) );
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::StreamSource * GN::gfx::BaseKernel::getStream( size_t index ) const
{
    if( index > mStreams.size() )
    {
        GN_ERROR(sLogger)( "stream index is out of range!" );
        return 0;
    }

    GN_ASSERT( mStreams[index] );

    return mStreams[index];
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::BaseKernel::compatible( const Surface * surf, const StrA & portName ) const
{
    const BaseKernelPort * port = getPort( portName );
    if( 0 == port ) return false;
    return port->compatible( surf );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BaseKernel::setStreamRef( size_t index, StreamSource & stream )
{
    GN_ASSERT( index <= mStreams.size() );
    mStreams[index] = &stream;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BaseKernel::setPortRef( size_t index, BaseKernelPort & port )
{
    GN_ASSERT( index <= mPorts.size() );
    mPorts[index] = &port;
}

// *****************************************************************************
// protected methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::BaseKernel::validate() const
{
    const KernelReflection & refl = getRefl();

    if( &refl != &getKernelReflection( getName() ) )
    {
        GN_ERROR(sLogger)( "%s validate() failed : invalid kernel reflection reference.", getName().cptr() );
        return false;
    }

    // check streams
    for( size_t i = 0; i < refl.streams.size(); ++i )
    {
        if( !getStream( i ) )
        {
            GN_ERROR(sLogger)( "%s validate() failed : stream(%s) : null pointer.", getName().cptr(), refl.streams[i].name.cptr() );
            return false;
        }
    }

    // check ports
    for( size_t i = 0; i < refl.ports.size(); ++i )
    {
        if( !getPort( refl.ports[i].name ) )
        {
            GN_ERROR(sLogger)( "%s validate() failed : port(%s) : null pointer.", getName().cptr(), refl.ports[i].name.cptr() );
            return false;
        }
    }

    // success
    return true;
}
