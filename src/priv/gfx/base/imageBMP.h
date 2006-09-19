#ifndef __GN_GFX_IMAGEBMP_H__
#define __GN_GFX_IMAGEBMP_H__
// *****************************************************************************
//! \file    imageBMP.h
//! \brief   BMP image reader
//! \author  chenlee (2005.6.2)
// *****************************************************************************


//!
//! BMP image reader.
//!
class BMPReader
{
    template<typename T>
    static GN_FORCE_INLINE T sSwap8In32( T in )
    {
        return ((in & 0xFF)<<24) |
            ((in & 0xFF00) << 8) |
            ((in & 0xFF0000) >> 8) |
            ((in) >> 24);
    }

    template<typename T>
    static GN_FORCE_INLINE T sSwap8In16( T in )
    {
        return ( in << 8 ) | ( in >> 8 ) ;
    }

    #pragma pack(push,1)
    struct BMPFileHeader
    {
        char     B;
        char     M;
        uint32_t size; 
        uint32_t reserved;
        uint32_t offBits; 

        void swapEndian()
        {
            size = sSwap8In32( size );
            offBits = sSwap8In32( offBits );
        }
    };
    struct BMPInfoHeader
    { 
        uint32_t size; 
        int32_t  width; 
        int32_t  height; 
        uint16_t planes; 
        uint16_t bitCount;
        uint32_t compression; 
        uint32_t sizeImage; 
        int32_t  xPelsPerMeter; 
        int32_t  yPelsPerMeter; 
        uint32_t clrUsed; 
        uint32_t clrImportant;

        void swapEndian()
        {
            size = sSwap8In32( size );
            width = sSwap8In32( width );
            height = sSwap8In32( height );

            planes = sSwap8In16( planes );
            bitCount = sSwap8In16( bitCount );

            compression = sSwap8In32( compression );
            sizeImage = sSwap8In32( sizeImage );
            xPelsPerMeter = sSwap8In32( xPelsPerMeter );
            yPelsPerMeter = sSwap8In32( yPelsPerMeter );
            clrUsed = sSwap8In32( clrUsed );
            clrImportant = sSwap8In32( clrImportant );
        }
    };
    struct BMPHeader
    {
        BMPFileHeader fileHeader;
        BMPInfoHeader infoHeader;

        void swapEndian() { fileHeader.swapEndian(); infoHeader.swapEndian(); }
    };
    #pragma pack(pop)

    BMPHeader       mHeader;
    const uint8_t * mImageSrc; // pointer to the first pixel
    uint32_t        mOutputBytesPerPixel;

public:

    //!
    //! constructor
    //!
    BMPReader() : mImageSrc(0) {}

    //!
    //! destructor
    //!
    ~BMPReader() {}

    //!
    //! Check file format. Return true if the file is BMP file
    //!
    bool checkFormat( GN::File & );

    //!
    //! read BMP header
    //!
    bool readHeader(
        GN::gfx::ImageDesc & o_desc, const uint8_t * i_buf, size_t i_size );

    //!
    //! read BMP image
    //!
    bool readImage( void * o_data ) const;
};

// *****************************************************************************
//                           End of imageBMP.h
// *****************************************************************************
#endif // __GN_GFX_IMAGEBMP_H__
