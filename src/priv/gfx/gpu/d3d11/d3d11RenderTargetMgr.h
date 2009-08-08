#ifndef __GN_GFX_D3D11RENDERTARGETMGR_H__
#define __GN_GFX_D3D11RENDERTARGETMGR_H__
// *****************************************************************************
/// \file
/// \brief   D3D11 render target manager
/// \author  chen@@CHENLI-HOMEPC (2007.1.11)
// *****************************************************************************

namespace GN { namespace gfx
{
    class D3D11Gpu;

    ///
    /// D3D11 render target manager
    ///
    class D3D11RTMgr : public StdClass
    {
        GN_DECLARE_STDCLASS( D3D11RTMgr, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3D11RTMgr( D3D11Gpu & r ) : mGpu(r) { clear(); }
        virtual ~D3D11RTMgr() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init();
        void quit();
    private:
        void clear()
        {
            mAutoColor0 = 0;
            mAutoDepthTexture = 0;
            mAutoDepth = 0;
            memset(mColors, 0, sizeof(mColors) );
            mDepth = 0;
            mNumColors = 0;
        }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        ///
        /// bind render target to device
        ///
        bool bind(
            const RenderTargetDesc & oldrt,
            const RenderTargetDesc & newrt,
            bool                     skipDirtyCheck,
            bool                   & renderTargetSizeChanged );

        ///
        /// get render target count
        ///
        UInt32 getRenderTargetCount() const { return mNumColors; }

        ///
        /// get render target view
        ///
        ID3D11RenderTargetView * getRenderTargetView( size_t index ) const
        {
            GN_ASSERT( index < mNumColors );
            return mColors[index];
        }

        ///
        /// get depth stencil view
        ///
        ID3D11DepthStencilView * getDepthStencilView() const { return mDepth; }

        ///
        /// get render target size in pixels
        ///
        const Vector2<UInt32> & getRenderTargetSize() const { return mRenderTargetSize; }

        // ********************************
        // private variables
        // ********************************
    private:

        D3D11Gpu & mGpu;

        ID3D11RenderTargetView * mAutoColor0;
        ID3D11Texture2D *        mAutoDepthTexture;
        ID3D11DepthStencilView * mAutoDepth;

        ID3D11RenderTargetView * mColors[GpuContext::MAX_COLOR_RENDER_TARGETS];
        ID3D11DepthStencilView * mDepth;
        size_t                   mNumColors;

        Vector2<UInt32>          mRenderTargetSize;

        // ********************************
        // private functions
        // ********************************
    private:

    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_D3D11RENDERTARGETMGR_H__
