#include "pch.h"
#include "d3d9VtxDecl.h"

static GN::Logger * sLogger = GN::getLogger( "GN.gfx2.D3D9KernelBase" );

//
//
// -----------------------------------------------------------------------------
static const GN::gfx::KernelPort *
sFindPortByName( const GN::StrA & name, const GN::gfx::KernelPort * ports, size_t count )
{
    if( 0 == ports ) return 0;

    for( size_t i = 0; i < count; ++i )
    {
        if( ports[i].name == name )
            // found
            return &ports[i];
    }

    // not found
    return 0;
}

// *****************************************************************************
// D3D9KernelPortBinding
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D9KernelPortBinding::D3D9KernelPortBinding( D3D9KernelBase & e )
    : mKernel( e )
    , mVtxDecl( 0 )
    , mHasZBuf(0)
    , mHasIdxBuf(0)
{
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D9KernelPortBinding::~D3D9KernelPortBinding()
{
    safeRelease( mVtxDecl );
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9KernelPortBinding::setup( const KernelPort * ports, size_t count )
{
    GN_GUARD;

    GN_ASSERT( 0 == mVtxDecl );
    GN_ASSERT( 0 == mHasZBuf );
    GN_ASSERT( 0 == mHasIdxBuf );

    BindItem b;

    StackArray<const SurfaceElementFormat *,MAX_SURFACE_ELEMENT_ATTRIBUTES> vtxfmt;

    size_t numPorts = mKernel.getNumPorts();
    for( size_t i = 0; i < numPorts; ++i )
    {
        const D3D9KernelPort & port = mKernel.getPortByIndex( i );
        const D3D9KernelPortDesc & desc = port.getDesc();

        const KernelPort * p = sFindPortByName( desc.name, ports, count );

        if( 0 == p || 0 == p->view.surf )
        {
            if( D3D9_KERNEL_PORT_RENDER_TARGET == desc.portType ||
                D3D9_KERNEL_PORT_DEPTH_BUFFER == desc.portType ||
                D3D9_KERNEL_PORT_TEXTURE == desc.portType )
            {
                b.port        = i;
                b.target.surf = 0;
                mBindItems.append( b );
            }
        }
        else
        {
            if( !port.compatible( p->view.surf ) )
            {
                return false;
            }

            b.port   = i;
            b.target = p->view;

            if( p->view.surf )
            {
                switch( desc.portType )
                {
                    case D3D9_KERNEL_PORT_DEPTH_BUFFER:
                        mHasZBuf = true;
                        break;

                    case D3D9_KERNEL_PORT_VTXBUF:
                        vtxfmt.append( &p->view.surf->getDesc().layout.format );
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

                if( D3D9_KERNEL_PORT_VTXBUF != desc.portType &&
                    D3D9_KERNEL_PORT_IDXBUF != desc.portType )
                {
                    mBindItems.append( b );
                }
            }
        }
    }

    // create vertex declaration
    if( !vtxfmt.empty() )
    {
        mVtxDecl = createD3D9VtxDecl( mKernel.gfxsys().d3ddev(), vtxfmt.cptr(), vtxfmt.size() );
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9KernelPortBinding::apply() const
{
    // setup vertex decl
    if( mVtxDecl )
    {
        mKernel.gfxsys().d3ddev()->SetVertexDeclaration( mVtxDecl );
    }

    // bind each port
    for( size_t i = 0; i < mBindItems.size(); ++i )
    {
        const BindItem & b = mBindItems[i];

        mKernel.getPortByIndex(b.port).bind( b.target );
    }
}

// *****************************************************************************
// D3D9KernelBase
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D9KernelBase::D3D9KernelBase( D3D9GraphicsSystem & gs, const char * kernelName )
    : mGraphicsSystem(gs)
    , mPorts( strFormat("D3D9 kernel named '%s'",kernelName), "ports" )
    , mDefaultBinding(0)
{
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D9KernelBase::~D3D9KernelBase()
{
    for( KernelPortBinding b = mBindings.first(); b != 0; b = mBindings.next(b) )
    {
        delete mBindings[b];
    };
    mBindings.clear();
}

//
//
// -----------------------------------------------------------------------------
const GN::gfx::D3D9KernelPort *
GN::gfx::D3D9KernelBase::getPortByName( const StrA & name ) const
{
    const D3D9KernelPort * const * port = mPorts.get( name );
    if( 0 == port ) return 0;
    GN_ASSERT( *port );
    return *port;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9KernelBase::compatible( const Surface * surf, const StrA & portName ) const
{
    const D3D9KernelPort * const * port = mPorts.get( portName );
    if( 0 == port ) return false;
    return (*port)->compatible( surf );
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::KernelPortBinding GN::gfx::D3D9KernelBase::createPortBinding( const KernelPort * ports, size_t count )
{
    GN_GUARD;

    AutoObjPtr<D3D9KernelPortBinding> b( new D3D9KernelPortBinding(*this) );

    if( !b || !b->setup( ports, count ) ) return 0;

    return mBindings.add( b.detach() );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9KernelBase::deletePortBinding( KernelPortBinding b )
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
size_t GN::gfx::D3D9KernelBase::addPortRef( D3D9KernelPort & port )
{
    if( port.getDesc().name.empty() )
    {
        GN_ERROR(sLogger)( "addPortRef() failed: empty port name." );
        GN_UNEXPECTED();
        return (size_t)-1;
    }

    return mPorts.add( port.getDesc().name, &port );
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::KernelPortBinding GN::gfx::D3D9KernelBase::createDefaultBinding()
{
    GN_GUARD;

    KernelPortBinding b = createPortBinding( 0, 0 );

    if( 0 == b )
    {
        GN_ERROR(sLogger)( "fail to create default binding!" );
    }

    return b;

    GN_UNGUARD;
}
