#include "pch.h"
#include "d3d9ClearScreenKernel.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx2.D3D9ClearScreenKernel");

// *****************************************************************************
// local classes
// *****************************************************************************

namespace GN { namespace gfx
{
    struct D3D9ColorParameter : public SimpleKernelParameter<D3DCOLOR>
    {
        D3D9ColorParameter( const KernelParameterDesc & desc, D3DCOLOR initial )
            : SimpleKernelParameter<D3DCOLOR>( desc, initial ) {}

        void setf( size_t offset, size_t count, const float * values )
        {
            if( 0 != offset || 4 != count || NULL == values )
            {
                GN_ERROR(sLogger)( "invalid parameter value." );
                return;
            }

            value = D3DCOLOR_COLORVALUE( values[0], values[1], values[2], values[3] );
        }
    };

    struct D3D9ClearScreenParameterSet : public KernelParameterSet
    {
        IntKernelParameter<DWORD> flags;
        D3D9ColorParameter        color;
        FloatKernelParameter      depth;
        IntKernelParameter<int>   stencil;

        //@{

        D3D9ClearScreenParameterSet( Kernel & k )
            : KernelParameterSet( k )
            , flags( *k.getParameterDesc(0), D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL )
            , color( *k.getParameterDesc(1), D3DCOLOR_RGBA( 0, 0, 0, 255 ) )
            , depth( *k.getParameterDesc(2), 1.0f )
            , stencil( *k.getParameterDesc(3), 0 )
        {
        }

        virtual KernelParameter & get( size_t index )
        {
            switch( index )
            {
                case 0  : return flags;
                case 1  : return color;
                case 2  : return depth;
                case 3  : return stencil;
                default :
                    GN_ERROR(sLogger)( "parameter index is out of range!" );
                    return DummyKernelParameter::sGetInstance();
            }
        }

        //@}
    };
}}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::KernelParameterSet *
GN::gfx::D3D9ClearScreenKernel::createParameterSet()
{
    return new D3D9ClearScreenParameterSet(*this);
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9ClearScreenKernel::render( const KernelParameterSet & param, KernelPortBinding binding )
{
    D3D9KernelPortBinding & b = getPortBinding( binding );
    b.apply();

    const D3D9ClearScreenParameterSet & p = safeCastRef<const D3D9ClearScreenParameterSet>( param );

    DWORD flags;
    if( b.hasZBuf() )
    {
        flags = p.flags;
    }
    else
    {
        flags = p.flags & ~(D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL);
    }

    if( flags ) d3d9gs().d3ddev()->Clear( 0, 0, flags, p.color, p.depth, p.stencil );
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D9ClearScreenKernel::D3D9ClearScreenKernel( D3D9GraphicsSystem & gs )
    : D3D9Kernel( KERNEL_NAME(), gs )
    , mTarget0(gs,"TARGET0",0)
    , mDepth(gs,"DEPTH")
{
    addPortRef( mTarget0 );
    addPortRef( mDepth );

    addParameter( "FLAGS", KERNEL_PARAMETER_TYPE_INT, 1 );
    addParameter( "COLOR", KERNEL_PARAMETER_TYPE_FLOAT, 4 );
    addParameter( "DEPTH", KERNEL_PARAMETER_TYPE_FLOAT, 1 );
    addParameter( "STENCIL", KERNEL_PARAMETER_TYPE_INT, 1 );
}
