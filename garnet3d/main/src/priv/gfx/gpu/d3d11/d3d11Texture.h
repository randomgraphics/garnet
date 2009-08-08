#ifndef __GN_GFXD3D11_D3D11TEXTURE_H__
#define __GN_GFXD3D11_D3D11TEXTURE_H__
// *****************************************************************************
/// \file
/// \brief   D3D 10 texture class
/// \author  chenlee (2006.09.20)
// *****************************************************************************

#include "../common/basicSurface.h"
#include "d3d11Resource.h"

namespace GN { namespace gfx
{
    class D3D11Renderer;

    ///
    /// D3D texture
    ///
    class D3D11Texture : public BasicTexture, public D3D11Resource, public StdClass
    {
        GN_DECLARE_STDCLASS( D3D11Texture, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3D11Texture( D3D11Renderer & r ) : D3D11Resource(r) { clear(); }
        virtual ~D3D11Texture() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( const TextureDesc & desc );
        void quit();
    private:
        void clear()
        {
            mTexture = 0;
        }
        //@}

        // ********************************
        // From Texture
        // ********************************
    public:

        virtual void   updateMipmap( size_t face, size_t level, const Box<UInt32>* area, size_t rowPitch, size_t slicePitch, const void * data, SurfaceUpdateFlag flag );
        virtual void   readMipmap( size_t face, size_t level, MipmapData & data );
        virtual void   blobWrite( const void *, size_t ) { GN_UNIMPL(); }
        virtual size_t blobRead( void * ) { GN_UNIMPL(); return 0; }
        virtual void   generateMipmapPyramid() { GN_UNIMPL(); }
        virtual void * getAPIDependentData() const { return mTexture; }

        // ********************************
        // public functions
        // ********************************
    public:

        ///
        /// get shader resource view
        ///
        ID3D11ShaderResourceView * getSRView()
        {
            const TextureDesc & desc = getDesc();

            return getSRView(
                mReadingFormat,
                0, desc.faces,
                0, desc.levels,
                0, desc.depth );
        }

        ///
        /// get shader resource view
        ///
        ID3D11ShaderResourceView * getSRView(
            DXGI_FORMAT format,
            UInt32      firstFace,
            UInt32      numFaces,
            UInt32      firstMipLevel,
            UInt32      numLevels,
            UInt32      firstSlice,
            UInt32      numSlices );

        ///
        /// get render target view of specific subresource
        ///
        ID3D11RenderTargetView * getRTView( UInt32 face, UInt32 level, UInt32 slice );

        ///
        /// get render target view of specific subresource
        ///
        ID3D11DepthStencilView * getDSView( UInt32 face, UInt32 level, UInt32 slice );

        // ********************************
        // private variables
        // ********************************
    private:

        // mapping subresource index to render target view
        typedef std::map<D3D11_SHADER_RESOURCE_VIEW_DESC, AutoComPtr<ID3D11ShaderResourceView> > SRViewMap;
        typedef std::map<D3D11_RENDER_TARGET_VIEW_DESC,AutoComPtr<ID3D11RenderTargetView> > RTViewMap;
        typedef std::map<D3D11_DEPTH_STENCIL_VIEW_DESC,AutoComPtr<ID3D11DepthStencilView> > DSViewMap;

        D3D11_SRV_DIMENSION        mDimension;      ///< texture dimension
        DXGI_FORMAT                mTextureFormat;  ///< D3D11 texture format
        DXGI_FORMAT                mReadingFormat;  ///< format for reading from the texture
        DXGI_FORMAT                mWritingFormat;  ///< format for writing to the texture
        ID3D11Resource           * mTexture;        ///< resource instance
        SRViewMap                  mSRViews;        ///< shader resource views
        RTViewMap                  mRTViews;        ///< render target views
        DSViewMap                  mDSViews;        ///< depth stencil views

        // ********************************
        // private functions
        // ********************************
    private:
        bool createTexture();
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFXD3D11_D3D11TEXTURE_H__
