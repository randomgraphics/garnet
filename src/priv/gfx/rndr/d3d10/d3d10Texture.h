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
    class D3D10Renderer;

    ///
    /// D3D10 texture dimension enumeration
    ///
    enum D3D10TextureDimension
    {
        TEXDIM_1D, ///< 1D texture and 1D texture array
        TEXDIM_2D, ///< 2D texture and 2D texture array (including CUBE texture)
        TEXDIM_3D, ///< 3D texture
    };

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
        D3D10Texture( D3D10Renderer & r ) : D3D10Resource(r) { clear(); }
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
            const TextureDesc & desc = getDesc();

            return getSRView(
                mDefaultSRVFormat,
                0, desc.faces,
                0, desc.levels,
                0, desc.depth );
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

        // mapping subresource index to render target view
        typedef std::map<UInt32,AutoComPtr<ID3D10RenderTargetView> > RTViewMap;
        typedef std::map<D3D10_SHADER_RESOURCE_VIEW_DESC, AutoComPtr<ID3D10ShaderResourceView> > SRViewMap;

        D3D10TextureDimension      mTextureDimension;    ///< texture dimension
        DXGI_FORMAT                mTextureFormat;       ///< D3D10 texture format
        DXGI_FORMAT                mDefaultSRVFormat;    ///< default SRV format;
        D3D10_SRV_DIMENSION        mDefaultSrvDimension; ///< default SRV dimension
        ID3D10Resource           * mTexture;             ///< resource instance
        SRViewMap                  mSRViews;             ///< shader resource views
        RTViewMap                  mRTViews;             ///< render target views

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
