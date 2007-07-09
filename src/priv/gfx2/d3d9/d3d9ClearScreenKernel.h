#ifndef __GN_GFX2_D3D9CLEARSCREENKERNEL_H__
#define __GN_GFX2_D3D9CLEARSCREENKERNEL_H__
// *****************************************************************************
/// \file
/// \brief   D3D9 clear screen kernel
/// \author  chenli@@FAREAST (2007.6.19)
// *****************************************************************************

namespace GN { namespace gfx
{
    struct D3D9ClearScreenParameterSet : public ClearScreenParameterSet
    {
        DWORD    flags;
        D3DCOLOR color;
        float    depth;
        int      stencil;

        //@{
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
    class D3D9ClearScreenKernel : public D3D9Kernel
    {
        D3D9RenderTargetPort mTarget0;
        D3D9DepthBufferPort  mDepth;

        ///
        /// ctor
        ///
        D3D9ClearScreenKernel( D3D9GraphicsSystem & gs )
            : D3D9Kernel(gs)
            , mTarget0(gs)
            , mDepth(gs)
        {
            addPortRef( "TARGET0", &mTarget0 );
            addPortRef( "DEPTH", &mDepth );
        }

    public:

        ///
        /// kernel creator
        ///
        static Kernel * sCreator( GraphicsSystem & gs )
        {
            PIXPERF_FUNCTION_EVENT();
            return new D3D9ClearScreenKernel( GN_SAFE_CAST<D3D9GraphicsSystem&>(gs) );
        }

        virtual void render( const ClearScreenParameterSet & param, KernelPortBinding binding )
        {
            GN_ASSERT( &param.getKernel() == (Kernel*)this );

            D3D9GraphicsSystem & gs = gs();
            IDirect3DDevice9  * dev = gs.d3ddev();

            D3D9KernelPortBinding & b = getPortBinding( binding );
            b.apply();

            DWORD flags = 
            if( !c->empty() )
            {
                flags |= D3DCLEAR_TARGET;
                const float * p = c->toFloat();
                color = D3DCOLOR_COLORVALUE( p[0], p[1], p[2], p[3] );
            }

            if( b.hasZBuf() )
            {
                if( !z->empty() )
                {
                    flags |= D3DCLEAR_ZBUFFER;
                    depth = *z->toFloat();
                }

                if( s )
                {
                    flags |= D3DCLEAR_STENCIL;
                    stencil = *z->toInt();
                }
            }

            if( flags ) dev->Clear( 0, 0, flags, color, depth, stencil );
        }
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX2_D3D9CLEARSCREENKERNEL_H__
