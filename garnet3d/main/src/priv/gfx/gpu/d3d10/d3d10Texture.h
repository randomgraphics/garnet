#ifndef __GN_GFXD3D10_D3D10TEXTURE_H__
#define __GN_GFXD3D10_D3D10TEXTURE_H__
// *****************************************************************************
/// \file
/// \brief   D3D 10 texture class
/// \author  chenlee (2006.09.20)
// *****************************************************************************

#include "../common/basicSurface.h"
#include "d3d10Resource.h"

namespace GN { namespace gfx
{
    class D3D10Gpu;

    ///
    /// D3D texture
    ///
    class D3D10Texture : public BasicTexture, public D3D10Resource, public StdClass
    {
        GN_DECLARE_STDCLASS( D3D10Texture, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3D10Texture( D3D10Gpu & r ) : D3D10Resource(r) { clear(); }
        virtual ~D3D10Texture() { quit(); }
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
        ID3D10ShaderResourceView * getSRView()
        {
            const TextureDesc & td = getDesc();

            return getSRView(
                mReadingFormat,
                0, td.faces,
                0, td.levels,
                0, td.depth );
        }

        ///
        /// get shader resource view
        ///
        ID3D10ShaderResourceView * getSRView(
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
        ID3D10RenderTargetView * getRTView( UInt32 face, UInt32 level, UInt32 slice );

        ///
        /// get render target view of specific subresource
        ///
        ID3D10DepthStencilView * getDSView( UInt32 face, UInt32 level, UInt32 slice );

        // ********************************
        // private variables
        // ********************************
    private:

        typedef HashMap<
            D3D10_SHADER_RESOURCE_VIEW_DESC,
            AutoComPtr<ID3D10ShaderResourceView>,
            HashMapUtils::MemoryHash<D3D10_SHADER_RESOURCE_VIEW_DESC>,
            HashMapUtils::MemoryCompare<D3D10_SHADER_RESOURCE_VIEW_DESC>
            > SRViewMap;

        typedef HashMap<
            D3D10_RENDER_TARGET_VIEW_DESC,
            AutoComPtr<ID3D10RenderTargetView>,
            HashMapUtils::MemoryHash<D3D10_RENDER_TARGET_VIEW_DESC>,
            HashMapUtils::MemoryCompare<D3D10_RENDER_TARGET_VIEW_DESC>
            > RTViewMap;

        typedef HashMap<
            D3D10_DEPTH_STENCIL_VIEW_DESC,
            AutoComPtr<ID3D10DepthStencilView>,
            HashMapUtils::MemoryHash<D3D10_DEPTH_STENCIL_VIEW_DESC>,
            HashMapUtils::MemoryCompare<D3D10_DEPTH_STENCIL_VIEW_DESC>
            > DSViewMap;

        D3D10_SRV_DIMENSION        mDimension;      ///< texture dimension
        DXGI_FORMAT                mTextureFormat;  ///< D3D10 texture format
        DXGI_FORMAT                mReadingFormat;  ///< format for reading from the texture
        DXGI_FORMAT                mWritingFormat;  ///< format for writing to the texture
        ID3D10Resource           * mTexture;        ///< resource instance
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
#endif // __GN_GFXD3D10_D3D10TEXTURE_H__
