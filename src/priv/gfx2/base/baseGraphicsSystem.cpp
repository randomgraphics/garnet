#include "pch.h"
#include "garnet/gfx2/baseGraphicsSystem.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx2.base.BaseGraphicsSystem");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx2::BaseGraphicsSystem::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx2::BaseGraphicsSystem, () );

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::BaseGraphicsSystem::quit()
{
    GN_GUARD;

    deleteAllEffects();

    mGlobalEffectParameters.clear();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx2::EffectParameterHandle
GN::gfx2::BaseGraphicsSystem::getGlobalEffectParameterHandle( const StrA & name )
{
    GN_GUARD;

    EffectParameterHandle h = mGlobalEffectParameters.name2handle( name );
    
    if( 0 == h )
    {
        h = mGlobalEffectParameters.add( name );
    }

    return h;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::BaseGraphicsSystem::setGlobalEffectParameter(
    EffectParameterHandle handle, const EffectParameter & value )
{
    GN_GUARD_SLOW;

    if( !mGlobalEffectParameters.validHandle( handle ) )
    {
        GN_ERROR(sLogger)( "invalid global effect parameter handle : %d", handle );
        return;
    }

    mGlobalEffectParameters[handle].set( value );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::BaseGraphicsSystem::unsetGlobalEffectParameter( EffectParameterHandle handle )
{
    GN_GUARD_SLOW;

    if( !mGlobalEffectParameters.validHandle( handle ) )
    {
        GN_ERROR(sLogger)( "invalid global effect parameter handle : %d", handle );
        return;
    }

    mGlobalEffectParameters[handle].unset();

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
const GN::gfx2::EffectParameter *
GN::gfx2::BaseGraphicsSystem::getGlobalEffectParameter( EffectParameterHandle handle )
{
    GN_GUARD_SLOW;

    if( !mGlobalEffectParameters.validHandle( handle ) )
    {
        GN_ERROR(sLogger)( "invalid global effect parameter handle : %d", handle );
        return 0;
    }

    EffectParameterWrapper & p = mGlobalEffectParameters[handle];

    return p.empty() ? 0 : &p.get();

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::BaseGraphicsSystem::registerEffect(
    const StrA & name, const EffectFactory & factory )
{
    if( !factory.creator )
    {
        GN_ERROR(sLogger)( "null effect creator: %s.", name.cptr() );
        return;
    }

    UInt32 h = mEffects.name2handle( name );

    if( h )
    {
        GN_ERROR(sLogger)( "effect named '%s' does already exist.", name.cptr() );
        return;
    }

    EffectItem ei;
    ei.factory  = factory;
    ei.instance = 0;

    mEffects.add( name, ei );
}

//
//
// -----------------------------------------------------------------------------
GN::gfx2::Effect * GN::gfx2::BaseGraphicsSystem::getEffect( const StrA & name )
{
    UInt32 h = mEffects.name2handle( name );

    if( 0 == h )
    {
        GN_ERROR(sLogger)( "effect named '%s' not found.", name.cptr() );
        return 0;
    }

    EffectItem & ei = mEffects[h];

    if( 0 == ei.instance )
    {
        ei.instance = ei.factory.creator( *this );
        if( 0 == ei.instance )
        {
            GN_ERROR(sLogger)( "fail to create effect named '%s'.", name.cptr() );
        }
    }

    return ei.instance;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::BaseGraphicsSystem::deleteEffect( const StrA & name )
{
    UInt32 h = mEffects.name2handle( name );

    if( 0 == h )
    {
        GN_ERROR(sLogger)( "effect named '%s' not found.", name.cptr() );
        return;
    }

    EffectItem & ei = mEffects[h];

    safeDelete( ei.instance );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::BaseGraphicsSystem::deleteAllEffects()
{
    for ( UInt32 h = mEffects.first(); 0 != h; h = mEffects.next( h ) )
    {
        safeDelete( mEffects[h].instance );
    }
}