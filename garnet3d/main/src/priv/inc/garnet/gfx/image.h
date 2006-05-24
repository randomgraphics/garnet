#ifndef __GN_GFX_IMAGE_H__
#define __GN_GFX_IMAGE_H__
// *****************************************************************************
//! \file    image.h
//! \brief   Image descriptor and image reader
//! \author  chenlee (2005.11.13)
// *****************************************************************************

namespace GN { namespace gfx
{
    //!
    //! mipmap descriptor
    //!
    //! \note
    //! - for 2D texture, depth is always 1
    //! - for cube texture, depth is always 6
    //! - for 3D texture, depth is based on mipmap level
    //! - slicePitch 和 levelPitch 可以通过mipmap的其他参数计算出来。
    //!   这里列出这两个值仅仅是为了避免重复计算，方便使用。
    //!
    struct MipmapDesc
    {
        uint32_t width,  //!< mipmap width in pixel
                 height, //!< mipmap height in pixel
                 depth;  //!< mipmap depth in pixel

        //!
        //! bytes of one row of texel. For DXT compressed texture, this
        //! is 1/4 of bytes of one "block row"
        //!
        uint32_t rowPitch;

        //!
        //! bytes of one slice. Equals rowPitch * height
        //!
        uint32_t slicePitch;

        //!
        //! total bytes of this mip level. Equals slicePitch * depth.
        //!
        uint32_t levelPitch;
    };

    //!
    //! image descriptor
    //!
    struct ImageDesc
    {
        // ****************************
        //! \name member data
        // ****************************

        //@{

        ClrFmt       format;    //!< color format
        uint32_t     numFaces;  //!< number of image faces. 6 for cubemaps, 1 for others
        uint32_t     numLevels; //!< number of avaliable mipmaps
        MipmapDesc * mipmaps;   //!< mipmap array, face major. Mip data of face n, mips m is : mip[f*numFaces+m]

        //@}

        // ****************************
        //! \name ctor / dtor
        // ****************************

        //@{

        ImageDesc() : mipmaps(0) {}
        ~ImageDesc() { safeMemFree( mipmaps ); }

        //@}

        // ****************************
        //! \name copy operation
        // ****************************

        //@{

        ImageDesc( const ImageDesc & d )
            : format( d.format )
            , numFaces( d.numFaces )
            , numLevels( d.numLevels )
            , mipmaps( 0 )
        {
            if( d.mipmaps > 0 )
            {
                setFaceAndLevel( numFaces, numLevels );
                size_t mipCount = numFaces * numLevels;
                memcpy( mipmaps, d.mipmaps, sizeof(MipmapDesc)*mipCount );
            }
        }

        ImageDesc & operator=( const ImageDesc & rhs )
        {
            format = rhs.format;
            numFaces = rhs.numFaces;
            numLevels = rhs.numLevels;
            if( rhs.mipmaps > 0 )
            {
                setFaceAndLevel( numFaces, numLevels );
                size_t mipCount = numFaces * numLevels;
                memcpy( mipmaps, rhs.mipmaps, sizeof(MipmapDesc)*mipCount );
            }
            return *this;
        }

        //@}

        // ****************************
        //! \name member functions
        // ****************************

        //@{

        //!
        //! make sure an meaningfull image descriptor
        //!
        bool valid() const;

        //!
        //! set image face count and level count, allocate mipmap array as well.
        //!
        bool setFaceAndLevel( size_t faces, size_t levels );

        //!
        //! return descriptor of specific mipmap
        //!
        MipmapDesc & getMipmap( size_t face, size_t level );

        //!
        //! return descriptor of specific mipmap
        //!
        const MipmapDesc & getMipmap( size_t face, size_t level ) const;

        //!
        //! total bytes of the whole image
        //!
        GN_INLINE size_t getTotalBytes() const;

        //!
        //! bytes of one mip level
        //!
        GN_INLINE size_t getLevelBytes( size_t level ) const;

        //!
        //! bytes per face
        //!
        GN_INLINE size_t getFaceBytes() const;

        //!
        //! offset of specific pixel
        //!
        GN_INLINE size_t getPixelOffset( size_t face, size_t level, size_t x, size_t y, size_t z ) const;

        //!
        //! offset of specific scanline
        //!
        GN_INLINE size_t getScanlineOffset( size_t face, size_t level, size_t y, size_t z ) const;

        //!
        //! offset of specific slice
        //!
        GN_INLINE size_t getSliceOffset( size_t face, size_t level, size_t z ) const;

        //!
        //! offset of specific mip level
        //!
        GN_INLINE size_t getLevelOffset( size_t face, size_t level ) const;

        //!
        //! offset of specific face
        //!
        GN_INLINE size_t getFaceOffset( size_t face ) const;

        //@}
    };

    //!
    //! image reader
    //!
    class ImageReader
    {
    public:
        ImageReader();                  //!< constructor
        ~ImageReader();                 //!< destructor
        bool reset( File & );           //!< reset image reader
        bool readHeader( ImageDesc & ); //!< read image header
        bool readImage( void * data );  //!< read image content

    private:
        class Impl;
        Impl * mImpl;
    };
}}

#if GN_ENABLE_INLINE
#include "image.inl"
#endif

// *****************************************************************************
//                           End of image.h
// *****************************************************************************
#endif // __GN_GFX_IMAGE_H__
