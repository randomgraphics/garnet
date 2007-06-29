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
    /// 
    ///
    class D3D9HlslKernelParameterSet : public BaseKernelParameterSet
    {
        GN_DECLARE_STDCLASS( D3D9HlslKernelParameterSet, BaseKernelParameterSet );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3D9HlslKernelParameterSet( D3D9Kernel & e );
        virtual ~D3D9HlslKernelParameterSet() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init();
        void quit();
    private:
        void clear() { mVscfUpdate.clear(); mPscfUpdate.clear(); }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        ///
        /// apply shader and constants to device
        ///
        void apply() const;

        // ********************************
        // private variables
        // ********************************
    private:

        ///
        /// record offset and range of shader const update
        ///
        struct ConstUpdate
        {
            //@{
            UInt32 firstRegister;
            UInt32 registerCount;
            void clear() { firstRegister = 0; registerCount = 0; }
            void merge( UInt32 f, UInt32 c )
            {
                if( 0 == registerCount )
                {
                    firstRegister = f;
                    registerCount = c;
                }
                else
                {
                    firstRegister = min( firstRegister, f );
                    registerCount = max( registerCount, c ) - firstRegister;
                }
            }
            //@}
        };

        IDirect3DDevice9                 * mDev;
        AutoComPtr<IDirect3DVertexShader9> mVs;
        AutoComPtr<IDirect3DPixelShader9>  mPs;
        AutoComPtr<ID3DXConstantTable>     mVsConstBuffer, mPsConstBuffer;
        KernelParameterHandle              mVsHandle, mPsHandle, mVscfHandle, mPscfHandle;
        ConstUpdate                        mVscfUpdate, mPscfUpdate;

        // ********************************
        // private functions
        // ********************************
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
        virtual KernelParameterSet * createParameterSet();
        virtual void                 render( const KernelParameterSet & param, KernelBinding binding );
        //@}
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX2_D3D9HLSLKERNEL_H__
