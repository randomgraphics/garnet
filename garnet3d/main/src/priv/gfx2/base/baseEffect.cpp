#include "pch.h"
#include "garnet/gfx2/baseGraphicsSystem.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx2.base.BaseEffect");

// *****************************************************************************
// public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::BaseEffect::setParameter(
    const StrA & name, const EffectParameter & value )
{
    GN_GUARD;

    UInt32 h = mParameters.name2handle( name );

    if( 0 == h )
    {
        const EffectDesc & desc = getDesc();
        if( 0 == desc.getParameterDesc( name ) )
        {
            GN_ERROR(sLogger)( "parameter named '%s' does not exist.", name.cptr() );
            return;
        }

        mParameters.add( name, value );
    }
    else
    {
        mParameters[h] = value;
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::BaseEffect::unsetParameter( const StrA & name )
{
    GN_GUARD;

    UInt32 h = mParameters.name2handle( name );
    if( h )
    {
        mParameters.remove( h );
    }
    else
    {
        GN_ERROR(sLogger)( "parameter named '%s' does not exist.", name.cptr() );
    }

    GN_UNGUARD;
}
