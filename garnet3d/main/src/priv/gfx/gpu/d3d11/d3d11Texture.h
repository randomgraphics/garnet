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
    class D3D11Gpu;

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
        D3D11Texture( D3D11Gpu & r ) : D3D11Resource(r) { Clear(); }
        virtual ~D3D11Texture() { Quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool Init( const TextureDesc & desc );
        void Quit();
    private:
        void Clear()
        {
            mTexture = 0;
        }
        //@}

        // ********************************
        // From Texture
        // ********************************
    public:

        virtual void   UpdateMipmap( size_t face, size_t level, const Box<UInt32>* area, size_t rowPitch, size_t slicePitch, const void * data, SurfaceUpdateFlag flag );
        virtual void   ReadMipmap( size_t face, size_t level, MipmapData & data );
        virtual void   BlobWrite( const void *, size_t ) { GN_UNIMPL(); }
        virtual size_t BlobRead( void * ) { GN_UNIMPL(); return 0; }
        virtual void   GenerateMipmapPyramid() { GN_UNIMPL(); }
        virtual void * GetAPIDependentData() const { return mTexture; }

        // ********************************
        // public functions
        // ********************************
    public:

        ///
        /// get shader resource view
        ///
        ID3D11ShaderResourceView * getSRView()
        {
            const TextureDesc & desc = GetDesc();

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
        typedef HashMap<
            D3D11_SHADER_RESOURCE_VIEW_DESC,
            AutoComPtr<ID3D11ShaderResourceView>,
            HashMapUtils::MemoryHash<D3D11_SHADER_RESOURCE_VIEW_DESC>,
            HashMapUtils::MemoryCompare<D3D11_SHADER_RESOURCE_VIEW_DESC>
            > SRViewMap;

        typedef HashMap<
            D3D11_RENDER_TARGET_VIEW_DESC,
            AutoComPtr<ID3D11RenderTargetView>,
            HashMapUtils::MemoryHash<D3D11_RENDER_TARGET_VIEW_DESC>,
            HashMapUtils::MemoryCompare<D3D11_RENDER_TARGET_VIEW_DESC>
            > RTViewMap;

        typedef HashMap<
            D3D11_DEPTH_STENCIL_VIEW_DESC,
            AutoComPtr<ID3D11DepthStencilView>,
            HashMapUtils::MemoryHash<D3D11_DEPTH_STENCIL_VIEW_DESC>,
            HashMapUtils::MemoryCompare<D3D11_DEPTH_STENCIL_VIEW_DESC>
            > DSViewMap;

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
