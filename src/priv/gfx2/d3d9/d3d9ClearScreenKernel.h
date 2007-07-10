#ifndef __GN_GFX2_D3D9CLEARSCREENKERNEL_H__
#define __GN_GFX2_D3D9CLEARSCREENKERNEL_H__
// *****************************************************************************
/// \file
/// \brief   D3D9 clear screen kernel
/// \author  chenli@@FAREAST (2007.6.19)
// *****************************************************************************

namespace GN { namespace gfx
{
    struct D3D9ClearScreenParameterSet : public ClearScreenKernelParameterSet
    {
        DWORD    flags;
        D3DCOLOR color;
        float    depth;
        int      stencil;

        //@{

        D3D9ClearScreenParameterSet()
            : flags( D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL )
            , color( D3DCOLOR_RGBA( 0, 0, 0, 255 ) )
            , depth( 1.0f )
            , stencil( 0 )
        {
        }

        virtual void setClearColor( bool enabled, float r, float g, float b, float a )
        {
            if( enabled )
            {
                flags |= D3DCLEAR_TARGET;
            }
            else
            {
                flags &= ~D3DCLEAR_TARGET;
            }
            color = D3DCOLOR_COLORVALUE( r, g, b, a );
        }

        virtual void setClearDepth( bool enabled, float z )
        {
            if( enabled )
            {
                flags |= D3DCLEAR_ZBUFFER;
            }
            else
            {
                flags &= ~D3DCLEAR_ZBUFFER;
            }
            depth = z;
        }

        virtual void setClearStencil( bool enabled, int s )
        {
            if( enabled )
            {
                flags |= D3DCLEAR_STENCIL;
            }
            else
            {
                flags &= ~D3DCLEAR_STENCIL;
            }
            stencil = s;
        }

        //@}
    };

    ///
    /// clear screen kernel
    ///
    class D3D9ClearScreenKernel : public ClearScreenKernel, public D3D9KernelBase
    {
        D3D9RenderTargetPort mTarget0;
        D3D9DepthBufferPort  mDepth;

        ///
        /// ctor
        ///
        D3D9ClearScreenKernel( D3D9GraphicsSystem & gs )
            : D3D9KernelBase(gs,KERNEL_NAME())
            , mTarget0(gs,"TARGET0",0)
            , mDepth(gs,"DEPTH")
        {
            addPortRef( mTarget0 );
            addPortRef( mDepth );
        }

    public:

        ///
        /// kernel factory
        ///
        static Kernel * sFactory( GraphicsSystem & gs )
        {
            PIXPERF_FUNCTION_EVENT();
            return new D3D9ClearScreenKernel( safeCastRef<D3D9GraphicsSystem>(gs) );
        }

        virtual bool                      compatible( const Surface * surf, const StrA & port ) const { return D3D9KernelBase::compatible( surf, port ); }

        virtual ClearScreenKernelParameterSet * createParameterSet() { return new D3D9ClearScreenParameterSet; }

        virtual KernelPortBinding         createPortBinding( const ClearScreenKernelPortBinding & b )
        {
            return D3D9KernelBase::createPortBinding( &b.target );
        }

        virtual void                      deletePortBinding( KernelPortBinding b ) { D3D9KernelBase::deletePortBinding( b ); }

        virtual void                      render( const KernelParameterSet & param, KernelPortBinding binding )
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

            if( flags ) gfxsys().d3ddev()->Clear( 0, 0, flags, p.color, p.depth, p.stencil );
        }
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX2_D3D9CLEARSCREENKERNEL_H__
