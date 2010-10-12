#ifndef __GN_GFX_XENONTEXTURE_H__
#define __GN_GFX_XENONTEXTURE_H__
// *****************************************************************************
/// \file
/// \brief   Xenon texture classes
/// \author  chenlee (2005.11.5)
// *****************************************************************************

#include "../common/basicSurface.h"
#include "xenonResource.h"

namespace GN { namespace gfx
{
    enum XenonTextureDimension
    {
        XENON_TEXDIM_1D,
        XENON_TEXDIM_2D,
        XENON_TEXDIM_2D_ARRAY,
        XENON_TEXDIM_3D,
        XENON_TEXDIM_CUBE,
    };

    ///
    /// D3D texture
    ///
    class XenonTexture : public BasicTexture, public StdClass, public XenonResource
    {
         GN_DECLARE_STDCLASS( XenonTexture, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        XenonTexture( XenonGpu & r ) : XenonResource(r) { clear(); }
        virtual ~XenonTexture() { quit(); }
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
            mD3DTexture = 0;
            mStage = (UINT)NOT_BIND;
        }
        //@}

        // ********************************
        // From Texture
        // ********************************
    public:

        virtual void   updateMipmap(
                            size_t              face,
                            size_t              level,
                            const Box<uint32> * area,
                            size_t              rowPitch,
                            size_t              slicePitch,
                            const void        * data,
                            SurfaceUpdateFlag   flag );
        virtual void   readMipmap( size_t face, size_t level, MipmapData & data ) { GN_UNUSED_PARAM(face); GN_UNUSED_PARAM(level); GN_UNUSED_PARAM(data); GN_UNIMPL(); }
        virtual void   blobWrite( const void * data, size_t length ) { GN_UNUSED_PARAM(data); GN_UNUSED_PARAM(length); GN_UNIMPL(); }
        virtual size_t blobRead( void * data ) { GN_UNUSED_PARAM(data); GN_UNIMPL(); return 0; }
        virtual void   generateMipmapPyramid() { GN_UNIMPL(); }
        virtual void * getAPIDependentData() const { return mD3DTexture; }

        // ********************************
        // public functions
        // ********************************
    public:

        ///
        /// bind to D3D device
        ///
        static void sBindToDevice( IDirect3DDevice9 & dev, UINT stage, const XenonTexture * tex );

        // ********************************
        // private variables
        // ********************************
    private:

        enum
        {
            NOT_BIND = 0xFFFFFFFF
        };

        ///
        /// D3D texture instance pointer
        ///
        IDirect3DBaseTexture9 * mD3DTexture;

        ///
        /// D3D texture parameters
        ///
        //@{
        D3DFORMAT               mD3DFormat;
        XenonTextureDimension   mD3DDimension;
        DWORD                   mD3DUsage;
        //@}

        /// binding information
        mutable UINT            mStage;

        ///
        /// D3D filters( min, mag, mip )
        ///
        //mutable D3DTEXTUREFILTERTYPE mD3DFilters[3];

        ///
        /// texture wrapping modes
        ///
        //mutable D3DTEXTUREADDRESS mD3DWraps[3];

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_XENONTEXTURE_H__
