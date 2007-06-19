#include "pch.h"

static GN::Logger * sLogger = GN::getLogger( "GN.gfx2.D3D9Effect" );

// *****************************************************************************
// D3D9EffectDesc
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
const GN::gfx2::EffectPortDesc *
GN::gfx2::D3D9EffectDesc::getPortDesc( const StrA & name ) const
{
    PortDescContainer::const_iterator i = ports.find( name);

    if( ports.end() == i )
    {
        GN_ERROR(sLogger)( "invalid port name: %s", name.cptr() );
        return 0;
    }

    return &i->second;
}

//
//
// -----------------------------------------------------------------------------
const GN::gfx2::EffectParameterDesc *
GN::gfx2::D3D9EffectDesc::getParameterDesc( const StrA & name ) const
{
    ParameterValueDescContainer::const_iterator i = parameters.find( name);

    if( parameters.end() == i )
    {
        GN_ERROR(sLogger)( "invalid parameter name: %s", name.cptr() );
        return 0;
    }

    return &i->second;
}

// *****************************************************************************
// D3D9EffectBinding
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx2::D3D9EffectBinding::D3D9EffectBinding()
{
}

//
//
// -----------------------------------------------------------------------------
GN::gfx2::D3D9EffectBinding::~D3D9EffectBinding()
{
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx2::D3D9EffectBinding::setup( const EffectBindingDesc & )
{
    GN_UNIMPL();
    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::D3D9EffectBinding::apply()
{
}

// *****************************************************************************
// D3D9Effect
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx2::D3D9Effect::compatible( const Surface * surf, const StrA & port )
{
    /*if( 0 == surf )
    {
        GN_ERROR(sLogger)( "NULL surface pointr", port.cptr() );
        return false;
    }

    const EffectPortDesc * epd = mDesc.getPortDesc( port );
    if( 0 == epd )
    {
        GN_ERROR(sLogger)( "invalid port name: %s", port.cptr() );
        return false;
    }

    const D3D9Surface * d3dsurf = safeCast<const D3D9Surface*>(surf);

    const SurfaceDesc & sd = d3dsurf->getDesc();

    */

    GN_UNUSED_PARAM( surf );
    GN_UNUSED_PARAM( port );
    GN_UNIMPL_WARNING();

    return true;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx2::EffectBinding GN::gfx2::D3D9Effect::createBinding( const EffectBindingDesc & ebd )
{
    GN_GUARD;

    AutoObjPtr<D3D9EffectBinding> b( new D3D9EffectBinding );

    if( !b || !b->setup( ebd ) ) return 0;

    return mBindings.add( b.detach() );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::D3D9Effect::deleteBinding( EffectBinding b )
{
    GN_GUARD;

    if( !mBindings.validHandle( b ) )
    {
        GN_ERROR(sLogger)( "invalid effect binding handle: %d", b );
        return;
    }

    delete mBindings[b];
    mBindings.remove( b );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::D3D9Effect::bind( EffectBinding b )
{
    GN_GUARD_SLOW;

    GN_ASSERT( mBindings.validHandle( b ) );
    GN_ASSERT( mBindings[b] );

    mBindings[b]->apply();

    GN_UNGUARD_SLOW;
}
