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
        XenonTexture( XenonGpu & r ) : XenonResource(r) { Clear(); }
        virtual ~XenonTexture() { Quit(); }
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
            mD3DTexture = 0;
        }
        //@}

        // ********************************
        // From Texture
        // ********************************
    public:

        virtual void   UpdateMipmap(
                            size_t              face,
                            size_t              level,
                            const Box<UInt32> * area,
                            size_t              rowPitch,
                            size_t              slicePitch,
                            const void        * data,
                            SurfaceUpdateFlag   flag );
        virtual void   ReadMipmap( size_t face, size_t level, MipmapData & data ) { GN_UNUSED_PARAM(face); GN_UNUSED_PARAM(level); GN_UNUSED_PARAM(data); GN_UNIMPL(); }
        virtual void   BlobWrite( const void * data, size_t length ) { GN_UNUSED_PARAM(data); GN_UNUSED_PARAM(length); GN_UNIMPL(); }
        virtual size_t BlobRead( void * data ) { GN_UNUSED_PARAM(data); GN_UNIMPL(); return 0; }
        virtual void   GenerateMipmapPyramid() { GN_UNIMPL(); }
        virtual void * GetAPIDependentData() const { return mD3DTexture; }

        // ********************************
        // public functions
        // ********************************
    public:

        ///
        /// return D3D texture
        ///
        IDirect3DBaseTexture9 * getD3DTexture() const { return mD3DTexture; }

        // ********************************
        // private variables
        // ********************************
    private:

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
