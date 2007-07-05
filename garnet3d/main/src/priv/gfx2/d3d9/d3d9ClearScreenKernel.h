#ifndef __GN_GFX2_D3D9CLEARSCREENKERNEL_H__
#define __GN_GFX2_D3D9CLEARSCREENKERNEL_H__
// *****************************************************************************
/// \file
/// \brief   D3D9 clear screen kernel
/// \author  chenli@@FAREAST (2007.6.19)
// *****************************************************************************

namespace GN { namespace gfx
{
    ///
    /// clear screen kernel
    ///
    class D3D9ClearScreenKernel : public D3D9Kernel
    {
        D3D9RenderTargetPort mTarget0;
        D3D9DepthBufferPort  mDepth;

        size_t mColorValue, mDepthValue, mStencilValue;

        static Kernel * sCreator( GraphicsSystem & gs ) { return new D3D9ClearScreenKernel( GN_SAFE_CAST<D3D9GraphicsSystem&>(gs) ); }

        ///
        /// ctor
        ///
        D3D9ClearScreenKernel( D3D9GraphicsSystem & gs )
            : D3D9Kernel(gs)
            , mTarget0(gs)
            , mDepth(gs)
        {
            // setup ports
            addPortRef( "TARGET0", &mTarget0 );
            addPortRef( "DEPTH", &mDepth );

            // setup parameters
            mColorValue = addParameter( "COLOR_VALUE", KERNEL_PARAMETER_TYPE_FLOAT, 4 );
            //setParameter( mColorValue, KernelParameter(.0f, .0f, .0f, .0f) );

            mDepthValue = addParameter( "DEPTH_VALUE", KERNEL_PARAMETER_TYPE_FLOAT, 1 );
            //setParameter( mDepthValue, .0f );

            mStencilValue = addParameter( "STENCIL_VALUE", KERNEL_PARAMETER_TYPE_INT, 1 );
            //setParameter( mStencilValue, 0 );
        }


    public:

        ///
        /// get kernel factory
        ///
        static KernelFactory sGetFactory()
        {
            KernelFactory f;
            f.quality = 0;
            f.creator = &sCreator;
            return f;
        }

        virtual void render( const KernelParameterSet & param, KernelBinding binding )
        {
            GN_ASSERT( &param.getKernel() == (Kernel*)this );

            D3D9KernelBinding & b = getPortBinding( binding );
            D3D9GraphicsSystem & gs = d3d9gs();
            IDirect3DDevice9  * dev = gs.d3ddev();

            b.apply();

            const BaseKernelParameter * c = GN_SAFE_CAST<const BaseKernelParameter*>( param.getParameter( mColorValue ) );
            const BaseKernelParameter * z = GN_SAFE_CAST<const BaseKernelParameter*>( param.getParameter( mDepthValue ) );
            const BaseKernelParameter * s = GN_SAFE_CAST<const BaseKernelParameter*>( param.getParameter( mStencilValue ) );
            GN_ASSERT( c && z && s );

            DWORD flags   = 0;
            DWORD color   = 0;
            float depth   = .0f;
            DWORD stencil = 0;

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
