#include "pch.h"
#include "garnet/gfx2/baseGraphicsSystem.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx2.base.BaseEffect");

// *****************************************************************************
// BaseEffectParameterSet
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
const GN::gfx2::EffectParameter *
GN::gfx2::BaseEffectParameterSet::getParameter( EffectParameterHandle handle ) const
{
    GN_GUARD_SLOW;

    BaseEffect & e = (BaseEffect&)getEffect();

    size_t idx;
    if( !e.getParameterIndex( idx, handle ) ) return 0;

    GN_ASSERT( idx < mParameters.size() );

    const EffectParameterWrapper & p = mParameters[idx];

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
void GN::gfx2::BaseEffectParameterSet::setParameter(
    EffectParameterHandle handle, const EffectParameter & value )
{
    GN_GUARD_SLOW;

    BaseEffect & e = (BaseEffect&)getEffect();

    size_t idx;
    if( !e.getParameterIndex( idx, handle ) ) return;

    GN_ASSERT( idx < mParameters.size() );

    mParameters[idx].set( value );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::BaseEffectParameterSet::unsetParameter( EffectParameterHandle handle )
{
    GN_GUARD_SLOW;

    BaseEffect & e = (BaseEffect&)getEffect();

    size_t idx;
    if( !e.getParameterIndex( idx, handle ) ) return;

    GN_ASSERT( idx < mParameters.size() );

    mParameters[idx].unset();

    GN_UNGUARD_SLOW;
}


// *****************************************************************************
// BaseEffect
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
const GN::gfx2::EffectParameterDesc *
GN::gfx2::BaseEffect::getParameterDesc( const StrA & name ) const
{
    GN_GUARD;

    EffectParameterHandle h = mParameterHandles.name2handle( name );

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
GN::gfx2::EffectParameterHandle
GN::gfx2::BaseEffect::getParameterHandle( const StrA & name ) const
{
    GN_GUARD;

    EffectParameterHandle h = mParameterHandles.name2handle( name );

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
GN::gfx2::EffectParameterSet * GN::gfx2::BaseEffect::createParameterSet()
{
    GN_GUARD;

    return new BaseEffectParameterSet( *this, mParameters.size() );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx2::BaseEffect::hasProperity( const StrA & name ) const
{
    return 0 != mProperties.name2handle( name );
}

//
//
// -----------------------------------------------------------------------------
const GN::gfx2::EffectProperty *
GN::gfx2::BaseEffect::getProperity( const StrA & name ) const
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
GN::gfx2::EffectParameterHandle
GN::gfx2::BaseEffect::addParameter( const StrA & name, const EffectParameterDesc & desc )
{
    GN_GUARD;

    EffectParameterHandle h = mParameterHandles.name2handle( name );

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
    pi.global = mGraphicsSystem.getGlobalEffectParameterHandle( name );

    return h;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::BaseEffect::setProperty(
    const StrA & name,
    const EffectProperty & property )
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
void GN::gfx2::BaseEffect::unsetProperty( const StrA & name )
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
