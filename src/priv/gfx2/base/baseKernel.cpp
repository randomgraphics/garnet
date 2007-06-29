#include "pch.h"
#include "garnet/gfx2/baseGraphicsSystem.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx2.base.BaseKernel");

// *****************************************************************************
// BaseKernelParameterSet
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
const GN::gfx::KernelParameter *
GN::gfx::BaseKernelParameterSet::getParameter( KernelParameterHandle handle ) const
{
    GN_GUARD_SLOW;

    BaseKernel & e = (BaseKernel&)getKernel();

    size_t idx;
    if( !e.getParameterIndex( idx, handle ) ) return 0;

    GN_ASSERT( idx < mParameters.size() );

    const KernelParameterWrapper & p = mParameters[idx];

    if( p.empty() )
    {
        return e.getGlobalParameterByIndex( idx );
    }
    else
    {
        return &p.get();
    }

    GN_UNGUARD_SLOW;
}


//
//
// -----------------------------------------------------------------------------
void GN::gfx::BaseKernelParameterSet::setParameter(
    KernelParameterHandle handle, const KernelParameter & value )
{
    GN_GUARD_SLOW;

    BaseKernel & e = (BaseKernel&)getKernel();

    size_t idx;
    if( !e.getParameterIndex( idx, handle ) ) return;

    GN_ASSERT( idx < mParameters.size() );

    mParameters[idx].set( value );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BaseKernelParameterSet::setRawParameter(
    KernelParameterHandle handle,
    size_t                offset,
    size_t                bytes,
    const void          * data )
{
    GN_GUARD_SLOW;

    BaseKernel & e = (BaseKernel&)getKernel();

    size_t idx;
    if( !e.getParameterIndex( idx, handle ) ) return;

    GN_ASSERT( idx < mParameters.size() );

    mParameters[idx].setRaw( offset ,bytes, data );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BaseKernelParameterSet::unsetParameter( KernelParameterHandle handle )
{
    GN_GUARD_SLOW;

    BaseKernel & e = (BaseKernel&)getKernel();

    size_t idx;
    if( !e.getParameterIndex( idx, handle ) ) return;

    GN_ASSERT( idx < mParameters.size() );

    mParameters[idx].unset();

    GN_UNGUARD_SLOW;
}


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

    return new BaseKernelParameterSet( *this, mParameters.size() );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::BaseKernel::hasProperity( const StrA & name ) const
{
    return 0 != mProperties.name2handle( name );
}

//
//
// -----------------------------------------------------------------------------
const GN::gfx::KernelProperty *
GN::gfx::BaseKernel::getProperity( const StrA & name ) const
{
    UInt32 h = mProperties.name2handle( name );
    if( 0 == h )
    {
        GN_ERROR(sLogger)( "invalid property name: %s.", name.cptr() );
        return 0;
    }
    return &mProperties[h].get();
}

// *****************************************************************************
// protected methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::KernelParameterHandle
GN::gfx::BaseKernel::addParameter( const StrA & name, const KernelParameterDesc & desc )
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
    pi.desc   = desc;
    pi.global = mGraphicsSystem.getGlobalKernelParameterHandle( name );

    return h;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BaseKernel::setProperty(
    const StrA & name,
    const KernelProperty & property )
{
    UInt32 h = mProperties.name2handle( name );

    if( 0 == h )
    {
        h = mProperties.add( name );
    }

    mProperties[h].set( property );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::BaseKernel::unsetProperty( const StrA & name )
{
    UInt32 h = mProperties.name2handle( name );

    if( 0 == h )
    {
        GN_ERROR(sLogger)( "invalid property name: %s", name.cptr() );
    }
    else
    {
        mProperties.remove( h );
    }
}
