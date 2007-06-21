#include "pch.h"
#include "garnet/gfx2/baseGraphicsSystem.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx2.base.BaseEffect");

// *****************************************************************************
// public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
const GN::gfx2::EffectParameterDesc *
GN::gfx2::BaseEffect::getParameterDesc( const StrA & name ) const
{
    GN_GUARD;

    EffectParameterHandle h = mParameters.name2handle( name );

    if( 0 == h )
    {
        GN_ERROR(sLogger)( "invalid parameter name: %s", name.cptr() );
        return 0;
    }

    return &mParameters[h].desc;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx2::EffectParameterHandle
GN::gfx2::BaseEffect::getParameterHandle( const StrA & name ) const
{
    GN_GUARD;

    EffectParameterHandle h = mParameters.name2handle( name );

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
void GN::gfx2::BaseEffect::setParameter(
    EffectParameterHandle handle, const EffectParameter & value )
{
    GN_GUARD_SLOW;

    if( !mParameters.validHandle( handle ) )
    {
        GN_ERROR(sLogger)( "invalid arameter handle: %d", handle );
        return;
    }

    mParameters[handle].param.set( value );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::BaseEffect::unsetParameter( EffectParameterHandle handle )
{
    GN_GUARD_SLOW;

    if( !mParameters.validHandle( handle ) )
    {
        GN_ERROR(sLogger)( "invalid arameter handle: %d", handle );
        return;
    }

    mParameters[handle].param.unset();

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// protected methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
const GN::gfx2::EffectParameter * GN::gfx2::BaseEffect::getParameter( EffectParameterHandle handle ) const
{
    GN_GUARD_SLOW;

    if( !mParameters.validHandle( handle ) )
    {
        GN_ERROR(sLogger)( "invalid arameter handle: %d", handle );
        return 0;
    }

    const ParameterItem & pi = mParameters[handle];

    if( pi.param.empty() )
    {
        return mGraphicsSystem.getGlobalEffectParameter( pi.global );
    }
    else
    {
        return &pi.param.get();
    }

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx2::EffectParameterHandle
GN::gfx2::BaseEffect::addParameter( const StrA & name, const EffectParameterDesc & desc )
{
    GN_GUARD;

    EffectParameterHandle h = mParameters.name2handle( name );

    if( h )
    {
        GN_ERROR(sLogger)( "parameter named '%s' does exist already.", name.cptr() );
        GN_UNEXPECTED();
        return 0;
    }

    h = mParameters.add( name );

    ParameterItem & pi = mParameters[h];

    GN_ASSERT( pi.param.empty() );

    pi.desc   = desc;
    pi.global = mGraphicsSystem.getGlobalEffectParameterHandle( name );

    return h;

    GN_UNGUARD;
}
