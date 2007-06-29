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
    /// parameter set for D3D9 hlsl kernel
    ///
    class D3D9HlslKernelParameterSet : public BaseKernelParameterSet
    {
        struct ConstUpdate
        {
            UInt32 firstRegister;
            UInt32 registerCount;
            ConstUpdate( UInt32 f, UInt32 c ) : firstRegister(f), registerCount(c) {}
        };

        IDirect3DDevice9                 * mDev;
        AutoComPtr<IDirect3DVertexShader9> mVs;
        AutoComPtr<IDirect3DPixelShader9>  mPs;
        AutoComPtr<ID3DXConstantTable>     mVsConstBuffer, mPsConstBuffer;
        KernelParameterHandle              mVsHandle, mPsHandle, mVscfHandle, mPscfHandle;
        DynaArray<ConstUpdate>             mVscfUpdate, mPscfUpdate; ///< use pooled memory to avoid runtime heap allocation.

    public:

        ///
        /// ctor
        ///
        D3D9HlslKernelParameterSet( IDirect3DDevice9 * dev, Kernel & e );

        ///
        /// dtor
        ///
        ~D3D9HlslKernelParameterSet() {}

        ///
        /// initialize
        ///
        bool init();

        //@{
        IDirect3DVertexShader9 * vs() const { return mVs; }
        IDirect3DPixelShader9  * ps() const { return mPs; }
        //@}

    private:

        void onVsSet( size_t offset, size_t count );
        void onVsUnset();

        void onPsSet( size_t offset, size_t count );
        void onPsUnset();

        void onVscfSet( size_t offset, size_t count );

        void onPscfSet( size_t offset, size_t count );
    };

    ///
    /// general kernel that accepts shader code as parameter
    ///
    class D3D9HlslKernel : public D3D9Kernel
    {
        KernelParameterHandle mVs, mPs;
        KernelParameterHandle mVsFloatConstants, mPsFloatConstants;
        KernelParameterHandle mPrimType, mPrimCount, mBaseIndex, mBaseVertex, mVertexCount;

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

        static Kernel * sCreator( GraphicsSystem & gs ) { return new D3D9HlslKernel(GN_SAFE_CAST<D3D9GraphicsSystem&>(gs)); }

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
        virtual KernelParameterSet * createParameterSet() { return new D3D9HlslKernelParameterSet( d3d9gs(), *this, getParameterCount() ); }
        virtual void                 render( const KernelParameterSet & param, KernelBinding binding );
        //@}

    private:

        inline void applyShader( const D3D9HlslKernelParameterSet & param );
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX2_D3D9HLSLKERNEL_H__
