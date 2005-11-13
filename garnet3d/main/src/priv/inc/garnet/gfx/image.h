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
    //! image descriptor
    //!
    struct ImageDesc
    {
        // ****************************
        //! \name consts and types
        // ****************************

        //@{

        enum
        {
            MAX_MIPLEVELS = 16,                 //!< maximum mipmap levels
            MAX_IMGSIZE   = (1<<MAX_MIPLEVELS)  //!< maximum image size (64K)
        };

        //!
        //! image type
        //!
        enum ImageType
        {
            IMG_1D,   //!< 1D image
            IMG_2D,   //!< 2D image
            IMG_3D,   //!< volume mage
            IMG_CUBE, //!< cubemap image

            NUM_IMAGE_TYPES, //!< number of available image types
            IMG_INVALID      //!< indicate invalid image type
        };

        //!
        //! mipmap descriptor
        //!
        //! \note
        //! - for 2D texture, depth is always 1
        //! - for cube texture, depth is always 6
        //! - for 3D texture, depth is based on mipmap level
        //! - 两个pitch值可以通过mipmap的其他参数计算出来。
        //!   这里列出这两个值仅仅是为了避免重复计算，方便使用。
        //!
        struct MipDesc
        {
            uint16_t width,  //!< mipmap width in pixel
                     height, //!< mipmap height in pixel
                     depth;  //!< mipmap depth in pixel

            //!
            //! bytes of one row. For DXT compressed texture, this
            //  is bytes of one "block row" (4 scanlines, that is)
            //!
            uint32_t rowPitch;

            //!
            //! bytes of one slice.
            //!
            uint32_t slicePitch;

            //!
            //! total bytes of this mip level.
            //!
            uint32_t levelPitch;
        };

        //@}

        // ****************************
        //! \name member data
        // ****************************

        //@{

        ImageType   type;                //!< image type
        ClrFmt      format;              //!< color format
        uint8_t     numMips;             //!< number of avaliable mipmaps
        MipDesc     mips[MAX_MIPLEVELS]; //!< mipmaps

        //@}

        // ****************************
        //! \name member functions
        // ****************************

        //@{

        //!
        //! make sure an meaningfull image descriptor
        //!
        bool validate() const;

        //!
        //! total bytes of the whole image
        //!
        GN_INLINE size_t getTotalBytes() const;

        //!
        //! bytes of one mip level
        //!
        GN_INLINE size_t getLevelBytes( uint8_t level ) const;

        //!
        //! offset of specific pixel
        //!
        GN_INLINE size_t getPixelOffset( uint8_t level, uint16_t x, uint16_t y, uint16_t z ) const;

        //!
        //! offset of specific scanline
        //!
        GN_INLINE size_t getScanlineOffset( uint8_t level, uint16_t y, uint16_t z ) const;

        //!
        //! offset of specific slice
        //!
        GN_INLINE size_t getSliceOffset( uint8_t level, uint16_t z ) const;

        //!
        //! offset of specific mip level
        //!
        GN_INLINE size_t getLevelOffset( uint8_t level ) const { return getSliceOffset(level,0); }

        //@}

        // ****************************
        //! \name member operators
        // ****************************

        //@{
        MipDesc & operator [] ( size_t );
        const MipDesc & operator [] ( size_t ) const;
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
