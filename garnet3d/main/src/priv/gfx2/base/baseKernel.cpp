#include "pch.h"
#include "garnet/gfx2/baseGraphicsSystem.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx2.base.BaseKernel");

// *****************************************************************************
// BaseKernel
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
const GN::gfx::KernelParameterDesc *
GN::gfx::BaseKernel::getParameterDesc( const StrA & name ) const
{
    GN_GUARD;

    KernelParameterHandle h = mParameterHandles.name2handle( name );

    if( 0 == h )
    {
        GN_ERROR(sLogger)( "invalid parameter name: %s", name.cptr() );
        return 0;
    }

    return &mParameters[mParameterHandles[h]].desc;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::KernelParameterHandle
GN::gfx::BaseKernel::getParameterHandle( const StrA & name ) const
{
    GN_GUARD;

    KernelParameterHandle h = mParameterHandles.name2handle( name );

    if( 0 == h )
    {
        GN_ERROR(sLogger)( "parameter named '%s' does not exist.", name.cptr() );
    }

    return h;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::KernelParameterSet * GN::gfx::BaseKernel::createParameterSet()
{
    GN_GUARD;

    AutoObjPtr<BaseKernelParameterSet> p( new BaseKernelParameterSet( *this ) );

    if( !p->init() ) return 0;

    return p.detach();

    GN_UNGUARD;
}

// *****************************************************************************
// protected methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::KernelParameterHandle
GN::gfx::BaseKernel::addParameter(
    const StrA        & name,
    KernelParameterType type,
    size_t              count )
{
    GN_GUARD;

    KernelParameterHandle h = mParameterHandles.name2handle( name );

    if( h )
    {
        GN_ERROR(sLogger)( "parameter named '%s' does exist already.", name.cptr() );
        GN_UNEXPECTED();
        return 0;
    }

    size_t index = mParameters.size();

    mParameters.resize( mParameters.size() + 1 );

    h = mParameterHandles.add( name, index );
    if( 0 == h )
    {
        GN_ERROR(sLogger)( "fail to create handle form parameter '%s'.", name.cptr() );
        mParameters.popBack();
        return 0;
    }

    ParameterItem & pi = mParameters.back();
    pi.desc.type = type;
    pi.desc.count = count;

    return h;

    GN_UNGUARD;
}
