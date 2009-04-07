#ifndef __GN_GFXD3D10_D3D10TEXTURE_H__
#define __GN_GFXD3D10_D3D10TEXTURE_H__
// *****************************************************************************
/// \file
/// \brief   D3D 10 texture class
/// \author  chenlee (2006.09.20)
// *****************************************************************************

#include "../common/basicSurface.h"

namespace GN { namespace gfx
{
    class D3D10Renderer;

    ///
    /// D3D texture
    ///
    class D3D10Texture : public BasicTexture, public StdClass
    {
         GN_DECLARE_STDCLASS( D3D10Texture, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        D3D10Texture( D3D10Renderer & r ) : mRenderer(r) { clear(); }
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
            mSRView = 0;
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
        /// get render target view of specific surface
        ///
        ID3D10RenderTargetView * getRTView( UInt32 face, UInt32 level, UInt32 slice );

        // ********************************
        // private variables
        // ********************************
    private:

        // mapping subresource index to render target view
        typedef std::map<UInt32,AutoComPtr<ID3D10RenderTargetView> > RTViewMap;

        D3D10Renderer            & mRenderer;
        ID3D10Resource           * mTexture; ///< resource instance
        ID3D10ShaderResourceView * mSRView; ///< view as shader resource
        RTViewMap                  mRTViews; ///< render target views

        // ********************************
        // private functions
        // ********************************
    private:
        bool createTexture();
        bool createDefaultViews();
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFXD3D10_D3D10TEXTURE_H__
