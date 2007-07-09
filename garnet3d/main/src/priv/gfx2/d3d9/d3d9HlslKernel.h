#ifndef __GN_GFX2_D3D9HLSLKERNEL_H__
#define __GN_GFX2_D3D9HLSLKERNEL_H__
// *****************************************************************************
/// \file
/// \brief   d3d9 HLSL kernel, which accepts HLSL shader code as parameter
/// \author  chenli@@FAREAST (2007.6.22)
// *****************************************************************************

namespace GN { namespace gfx
{
    ///
    /// general kernel that accepts shader code as parameter
    ///
    class D3D9HlslKernel : public Hlsl9Kernel, public D3D9Kernel
    {
        size_t mPrimType, mPrimCount, mBaseIndex, mBaseVertex, mVertexCount;

        // ports
        D3D9RenderTargetPort mRenderTarget0;
        D3D9RenderTargetPort mRenderTarget1;
        D3D9RenderTargetPort mRenderTarget2;
        D3D9RenderTargetPort mRenderTarget3;
        D3D9DepthBufferPort  mDepthBuffer;
        D3D9TexturePort      mTexture0;
        D3D9TexturePort      mTexture1;
        D3D9TexturePort      mTexture2;
        D3D9TexturePort      mTexture3;
        D3D9TexturePort      mTexture4;
        D3D9TexturePort      mTexture5;
        D3D9TexturePort      mTexture6;
        D3D9TexturePort      mTexture7;
        D3D9VtxBufPort       mVtxBuf0;
        D3D9VtxBufPort       mVtxBuf1;
        D3D9VtxBufPort       mVtxBuf2;
        D3D9VtxBufPort       mVtxBuf3;
        D3D9VtxBufPort       mVtxBuf4;
        D3D9VtxBufPort       mVtxBuf5;
        D3D9VtxBufPort       mVtxBuf6;
        D3D9VtxBufPort       mVtxBuf7;
        D3D9IdxBufPort       mIdxBuf;

        D3D9RenderStateBlock mRsb;


        ///
        /// ctor
        ///
        D3D9HlslKernel( D3D9GraphicsSystem & gs );
 
    public:

        ///
        /// kernel creator
        ///
        static Kernel * sCreator( GraphicsSystem & gs )
        {
            PIXPERF_FUNCTION_EVENT();
            return new D3D9HlslKernel(GN_SAFE_CAST<D3D9GraphicsSystem&>(gs));
        }

        // from base class
        //@{
        virtual Hlsl9ParameterSet * createParameterSet();
        virtual KernelPortBinding   createPortBinding( const Hlsl9PortBinding & );
        virtual void                deletePortBinding( KernelPortBinding b ) { D3D9KernelBase::deletePortBinding( b ); }
        virtual bool                compatible( const Surface * surf, const StrA & port ) const { return D3D9KernelBase::compatible( surf, port ); }
        virtual void                render( const KernelParameterSet & param, KernelPortBinding binding );
        //@}
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX2_D3D9HLSLKERNEL_H__
