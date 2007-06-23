#include "pch.h"

static GN::Logger * sLogger = GN::getLogger( "GN.gfx2.D3D9Effect" );

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
bool GN::gfx2::D3D9EffectBinding::setup( D3D9Effect & effect, const EffectBindingDesc & ebd )
{
    GN_GUARD;

    BindItem b;
    D3D9EffectPort * port;

    for( size_t i = 0; i < ebd.bindings.size(); ++i )
    {
        const EffectPortBinding & epb = ebd.bindings[i];

        port = effect.getPort( epb.port );

        if( !port->compatible( epb.surf ) ) return false;

        b.port            = port;
        b.surf.surf       = epb.surf;
        b.surf.firstLevel = epb.firstLevel;
        b.surf.numLevels  = epb.numLevels;
        b.surf.firstFace  = epb.firstFace;
        b.surf.numFaces   = epb.numFaces;
        mBindItems.append( b );
    }

    // TODO: 

    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::D3D9EffectBinding::apply() const
{
    for( size_t i = 0; i < mBindItems.size(); ++i )
    {
        const BindItem & b = mBindItems[i];

        GN_ASSERT( b.port );

        b.port->bind( b.surf );
    }
}

// *****************************************************************************
// D3D9Effect
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
const GN::gfx2::D3D9EffectPort * GN::gfx2::D3D9Effect::getPort( const StrA & name ) const
{
    UInt32 h = mPorts.name2handle( name );
    if( 0 == h )
    {
        GN_ERROR(sLogger)( "invalid port name: %s", name.cptr() );
        return 0;
    }

    GN_ASSERT( mPorts[h] );

    return mPorts[h];
}

//
//
// -----------------------------------------------------------------------------
GN::gfx2::D3D9EffectPort * GN::gfx2::D3D9Effect::getPort( const StrA & name )
{
    UInt32 h = mPorts.name2handle( name );
    if( 0 == h )
    {
        GN_ERROR(sLogger)( "invalid port name: %s", name.cptr() );
        return 0;
    }

    GN_ASSERT( mPorts[h] );

    return mPorts[h];
}

//
//
// -----------------------------------------------------------------------------
const GN::gfx2::EffectPortDesc * GN::gfx2::D3D9Effect::getPortDesc( const StrA & name ) const
{
    const D3D9EffectPort * port = getPort( name );
    return port ? &port->getDesc() : 0;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx2::D3D9Effect::compatible( const Surface * surf, const StrA & portName )
{
    const D3D9EffectPort * port = getPort( portName );
    if( 0 == port ) return false;
    return port->compatible( surf );
}

//
//
// -----------------------------------------------------------------------------
GN::gfx2::EffectBinding GN::gfx2::D3D9Effect::createBinding( const EffectBindingDesc & ebd )
{
    GN_GUARD;

    AutoObjPtr<D3D9EffectBinding> b( new D3D9EffectBinding );

    if( !b || !b->setup( *this, ebd ) ) return 0;

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
void GN::gfx2::D3D9Effect::addPortRef( const StrA & name, D3D9EffectPort * port )
{
    if( mPorts.name2handle( name ) )
    {
        GN_ERROR(sLogger)( "addPortRef() failed: port named '%s' does exist already.", name.cptr() );
        GN_UNEXPECTED();
        return;
    }

    if( 0 == port )
    {
        GN_ERROR(sLogger)( "addPortRef() failed: NULL port pointer." );
        GN_UNEXPECTED();
        return;
    }

    mPorts.add( name, port );
}

//
//
// -----------------------------------------------------------------------------
GN::gfx2::EffectBinding GN::gfx2::D3D9Effect::createDefaultBinding()
{
    GN_GUARD;

    EffectBindingDesc bindNothing;

    EffectBinding b = createBinding( bindNothing );

    if( 0 == b )
    {
        GN_ERROR(sLogger)( "fail to create default binding!" );
    }

    return b;

    GN_UNGUARD;
}
