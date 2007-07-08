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
    class D3D9HlslKernel : public D3D9Kernel
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

        static Kernel * sCreator( GraphicsSystem & gs )
        {
            PIXPERF_FUNCTION_EVENT();
            return new D3D9HlslKernel(GN_SAFE_CAST<D3D9GraphicsSystem&>(gs));
        }

        ///
        /// ctor
        ///
        D3D9HlslKernel( D3D9GraphicsSystem & gs );
 
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

        // from base class
        //@{
        virtual KernelParameterSet * createParameterSet();
        virtual void                 render( const KernelParameterSet & param, KernelBinding binding );
        //@}
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX2_D3D9HLSLKERNEL_H__
