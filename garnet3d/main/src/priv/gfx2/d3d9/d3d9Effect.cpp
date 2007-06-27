#include "pch.h"
#include "d3d9VtxDecl.h"

static GN::Logger * sLogger = GN::getLogger( "GN.gfx2.D3D9Effect" );

// *****************************************************************************
// D3D9EffectBinding
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx2::D3D9EffectBinding::D3D9EffectBinding( D3D9Effect & e )
    : mEffect( e )
    , mVtxDecl( 0 )
    , mHasZBuf(0)
    , mHasIdxBuf(0)
{
}

//
//
// -----------------------------------------------------------------------------
GN::gfx2::D3D9EffectBinding::~D3D9EffectBinding()
{
    safeRelease( mVtxDecl );
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx2::D3D9EffectBinding::setup( const EffectBindingDesc & ebd )
{
    GN_GUARD;

    GN_ASSERT( 0 == mVtxDecl );
    GN_ASSERT( 0 == mHasZBuf );
    GN_ASSERT( 0 == mHasIdxBuf );

    BindItem b;

    std::map<StrA,EffectBindingTarget>::const_iterator iter;

    StackArray<const SurfaceElementFormat *,MAX_SURFACE_ELEMENT_ATTRIBUTES> vtxfmt;

    for(
        UInt32 portHandle = mEffect.getFirstPortHandle();
        0 != portHandle;
        portHandle = mEffect.getNextPortHandle( portHandle ) )
    {
        const D3D9EffectPort & port = mEffect.getPort( portHandle );

        iter = ebd.bindings.find( mEffect.getPortName( portHandle ) );

        D3D9EffectPortType portType = port.getDesc().portType;

        if( ebd.bindings.end() == iter )
        {
            if( D3D9_EFFECT_PORT_RENDER_TARGET == portType ||
                D3D9_EFFECT_PORT_DEPTH_BUFFER == portType ||
                D3D9_EFFECT_PORT_TEXTURE == portType )
            {
                b.port        = portHandle;
                b.target.surf = 0;
                mBindItems.append( b );
            }
        }
        else
        {
            if( !port.compatible( iter->second.surf ) )
            {
                return false;
            }

            b.port   = portHandle;
            b.target = iter->second;

            if( iter->second.surf )
            {
                switch( portType )
                {
                    case D3D9_EFFECT_PORT_DEPTH_BUFFER:
                        mHasZBuf = true;
                        break;

                    case D3D9_EFFECT_PORT_VTXBUF:
                        vtxfmt.append( &iter->second.surf->getDesc().layout.format );
                        break;

                    case D3D9_EFFECT_PORT_IDXBUF :
                        mHasIdxBuf = true;
                        break;

                    default:
                        // do nothing
                        break;
                };

                mBindItems.append( b );
            }
            else
            {
                // target surface is NULL.

                if( D3D9_EFFECT_PORT_VTXBUF != portType &&
                    D3D9_EFFECT_PORT_IDXBUF != portType )
                {
                    mBindItems.append( b );
                }
            }
        }
    }

    // create vertex declaration
    if( !vtxfmt.empty() )
    {
        mVtxDecl = createD3D9VtxDecl( mEffect.d3d9gs().d3ddev(), vtxfmt.cptr(), vtxfmt.size() );
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx2::D3D9EffectBinding::apply() const
{
    // setup vertex decl
    if( mVtxDecl )
    {
        mEffect.d3d9gs().d3ddev()->SetVertexDeclaration( mVtxDecl );
    }

    // bind each port
    for( size_t i = 0; i < mBindItems.size(); ++i )
    {
        const BindItem & b = mBindItems[i];

        mEffect.getPort(b.port).bind( b.target );
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

    AutoObjPtr<D3D9EffectBinding> b( new D3D9EffectBinding(*this) );

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
