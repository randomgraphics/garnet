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
        XenonTexture( XenonRenderer & r ) : XenonResource(r) { clear(); }
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
        }
        //@}

        // ********************************
        // From Texture
        // ********************************
    public:

        virtual void   updateMipmap(
                            size_t              face,
                            size_t              level,
                            const Box<UInt32> * area,
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
        /// Bind myself to specific stage
        ///
        void bind( UINT stage ) const
        {
            XenonRenderer & r = getRenderer();
            r.getDeviceInlined().SetTexture( stage, mD3DTexture );
        }

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
        D3DFORMAT mD3DFormat;
        DWORD     mD3DUsage;
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

        ///
        /// convert garnet cube face to D3D tag
        ///
        static D3DCUBEMAP_FACES sCubeFace2D3D( size_t face )
        {
            static D3DCUBEMAP_FACES sTable[ NUM_CUBEFACES ] =
            {
                D3DCUBEMAP_FACE_POSITIVE_X,
                D3DCUBEMAP_FACE_NEGATIVE_X,
                D3DCUBEMAP_FACE_POSITIVE_Y,
                D3DCUBEMAP_FACE_NEGATIVE_Y,
                D3DCUBEMAP_FACE_POSITIVE_Z,
                D3DCUBEMAP_FACE_NEGATIVE_Z,
            };
            GN_ASSERT( face < 6 );
            return sTable[face];
        }
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_XENONTEXTURE_H__
