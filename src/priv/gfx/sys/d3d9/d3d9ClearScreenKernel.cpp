#include "pch.h"
#include "d3d9ClearScreenKernel.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx2.D3D9ClearScreenKernel");

// *****************************************************************************
// local classes
// *****************************************************************************

namespace GN { namespace gfx
{
    struct D3D9ClearScreenParameterSet : public KernelParameterSet
    {
        BoolKernelParameter            cc;
        BoolKernelParameter            cd;
        BoolKernelParameter            cs;
        TypedKernelParameter<D3DCOLOR> color;
        FloatKernelParameter           depth;
        IntKernelParameter<int>        stencil;

        //@{

        D3D9ClearScreenParameterSet( Kernel & k )
            : KernelParameterSet( k )
            , cc( getKernelReflection(D3D9ClearScreenKernel::KERNEL_NAME()).parameters["CLEAR_COLOR"], true )
            , cd( getKernelReflection(D3D9ClearScreenKernel::KERNEL_NAME()).parameters["CLEAR_DEPTH"], true )
            , cs( getKernelReflection(D3D9ClearScreenKernel::KERNEL_NAME()).parameters["CLEAR_STENCIL"], true )
            , color( getKernelReflection(D3D9ClearScreenKernel::KERNEL_NAME()).parameters["COLOR"], D3DCOLOR_RGBA( 0, 0, 0, 255 ) )
            , depth( getKernelReflection(D3D9ClearScreenKernel::KERNEL_NAME()).parameters["DEPTH"], 1.0f )
            , stencil( getKernelReflection(D3D9ClearScreenKernel::KERNEL_NAME()).parameters["STENCIL"], 0 )
        {
        }

        virtual KernelParameter & get( size_t index )
        {
            switch( index )
            {
                case 0  : return cc;
                case 1  : return cd;
                case 2  : return cs;
                case 3  : return color;
                case 4  : return depth;
                case 5  : return stencil;
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
void GN::gfx::D3D9ClearScreenKernel::render( const KernelParameterSet & paramset, const KernelPortBinding * binding )
{
    const D3D9KernelPortBinding * b = binding ? safeCastPtr<const D3D9KernelPortBinding>( binding ) : getDefaultPortBinding();
    GN_ASSERT( b );
    b->apply();

    const D3D9ClearScreenParameterSet & p = safeCastRef<const D3D9ClearScreenParameterSet>( paramset );

    DWORD flags = 0;

    if( p.cc ) flags |= D3DCLEAR_TARGET;

    if( b->hasZBuf() )
    {
        if( p.cd ) flags |= D3DCLEAR_ZBUFFER;
        if( p.cs ) flags |= D3DCLEAR_STENCIL;
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
    , mTarget0( gs, baseref(), "TARGET0", 0 )
    , mDepth( gs, baseref(), "DEPTH" )
{
    GN_ASSERT( validate() );
}