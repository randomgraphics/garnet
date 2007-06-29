#include "pch.h"
#include "d3d9VtxDecl.h"

static GN::Logger * sLogger = GN::getLogger( "GN.gfx2.D3D9Kernel" );

// *****************************************************************************
// D3D9KernelBinding
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D9KernelBinding::D3D9KernelBinding( D3D9Kernel & e )
    : mKernel( e )
    , mVtxDecl( 0 )
    , mHasZBuf(0)
    , mHasIdxBuf(0)
{
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D9KernelBinding::~D3D9KernelBinding()
{
    safeRelease( mVtxDecl );
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9KernelBinding::setup( const KernelBindingDesc & ebd )
{
    GN_GUARD;

    GN_ASSERT( 0 == mVtxDecl );
    GN_ASSERT( 0 == mHasZBuf );
    GN_ASSERT( 0 == mHasIdxBuf );

    BindItem b;

    std::map<StrA,KernelBindingTarget>::const_iterator iter;

    StackArray<const SurfaceElementFormat *,MAX_SURFACE_ELEMENT_ATTRIBUTES> vtxfmt;

    for(
        UInt32 portHandle = mKernel.getFirstPortHandle();
        0 != portHandle;
        portHandle = mKernel.getNextPortHandle( portHandle ) )
    {
        const D3D9KernelPort & port = mKernel.getPort( portHandle );

        iter = ebd.bindings.find( mKernel.getPortName( portHandle ) );

        D3D9KernelPortType portType = port.getDesc().portType;

        if( ebd.bindings.end() == iter )
        {
            if( D3D9_KERNEL_PORT_RENDER_TARGET == portType ||
                D3D9_KERNEL_PORT_DEPTH_BUFFER == portType ||
                D3D9_KERNEL_PORT_TEXTURE == portType )
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
                    case D3D9_KERNEL_PORT_DEPTH_BUFFER:
                        mHasZBuf = true;
                        break;

                    case D3D9_KERNEL_PORT_VTXBUF:
                        vtxfmt.append( &iter->second.surf->getDesc().layout.format );
                        break;

                    case D3D9_KERNEL_PORT_IDXBUF :
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

                if( D3D9_KERNEL_PORT_VTXBUF != portType &&
                    D3D9_KERNEL_PORT_IDXBUF != portType )
                {
                    mBindItems.append( b );
                }
            }
        }
    }

    // create vertex declaration
    if( !vtxfmt.empty() )
    {
        mVtxDecl = createD3D9VtxDecl( mKernel.d3d9gs().d3ddev(), vtxfmt.cptr(), vtxfmt.size() );
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9KernelBinding::apply() const
{
    // setup vertex decl
    if( mVtxDecl )
    {
        mKernel.d3d9gs().d3ddev()->SetVertexDeclaration( mVtxDecl );
    }

    // bind each port
    for( size_t i = 0; i < mBindItems.size(); ++i )
    {
        const BindItem & b = mBindItems[i];

        mKernel.getPort(b.port).bind( b.target );
    }
}

// *****************************************************************************
// D3D9Kernel
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
const GN::gfx::D3D9KernelPort * GN::gfx::D3D9Kernel::getPort( const StrA & name ) const
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
GN::gfx::D3D9KernelPort * GN::gfx::D3D9Kernel::getPort( const StrA & name )
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
const GN::gfx::KernelPortDesc * GN::gfx::D3D9Kernel::getPortDesc( const StrA & name ) const
{
    const D3D9KernelPort * port = getPort( name );
    return port ? &port->getDesc() : 0;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9Kernel::compatible( const Surface * surf, const StrA & portName )
{
    const D3D9KernelPort * port = getPort( portName );
    if( 0 == port ) return false;
    return port->compatible( surf );
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::KernelBinding GN::gfx::D3D9Kernel::createBinding( const KernelBindingDesc & ebd )
{
    GN_GUARD;

    AutoObjPtr<D3D9KernelBinding> b( new D3D9KernelBinding(*this) );

    if( !b || !b->setup( ebd ) ) return 0;

    return mBindings.add( b.detach() );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9Kernel::deleteBinding( KernelBinding b )
{
    GN_GUARD;

    if( !mBindings.validHandle( b ) )
    {
        GN_ERROR(sLogger)( "invalid kernel binding handle: %d", b );
        return;
    }

    delete mBindings[b];
    mBindings.remove( b );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9Kernel::addPortRef( const StrA & name, D3D9KernelPort * port )
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
GN::gfx::KernelBinding GN::gfx::D3D9Kernel::createDefaultBinding()
{
    GN_GUARD;

    KernelBindingDesc bindNothing;

    KernelBinding b = createBinding( bindNothing );

    if( 0 == b )
    {
        GN_ERROR(sLogger)( "fail to create default binding!" );
    }

    return b;

    GN_UNGUARD;
}