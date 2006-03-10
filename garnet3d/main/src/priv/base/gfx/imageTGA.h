#ifndef __GN_GFX_IMAGETGA_H__
#define __GN_GFX_IMAGETGA_H__
// *****************************************************************************
//! \file    imageTGA.h
//! \brief   TGA image reader
//! \author  chenlee (2005.6.2)
// *****************************************************************************

#pragma pack(push,1)

//!
//! TGA image header
//!
struct TGA_HEADER
{
    uint8_t  identsize;          //!< size of ID field that follows 18 uint8_t header (0 usually)
    uint8_t  colourmaptype;      //!< type of colour map 0=none, 1=has palette
    uint8_t  imagetype;          //!< type of image 0=none,1=indexed,2=rgb,3=grey,(9,10,11,32,33)=rle packed

    uint16_t colourmapstart;     //!< first colour map entry in palette
    uint16_t colourmaplength;    //!< number of colours in palette
    uint8_t  colourmapbits;      //!< number of bits per palette entry 15,16,24,32

    uint16_t xstart;             //!< image x origin
    uint16_t ystart;             //!< image y origin
    uint16_t width;              //!< image width in pixels
    uint16_t height;             //!< image height in pixels
    uint8_t  bits;               //!< image bits per pixel 8,16,24,32
    uint8_t  descriptor;         //!< image descriptor bits (vh flip bits)
};

#pragma pack(pop)
GN_CASSERT( sizeof(TGA_HEADER) == 18 );

//!
//! tga image reader.
//!
class TGAReader
{
    TGA_HEADER      mHeader;
    const uint8_t * mSrc;
    size_t          mSize; // image data size in bytes;
    size_t          mOutputBytesPerPixel;

public:

    //!
    //! constructor
    //!
    TGAReader() : mSrc(0) {}

    //!
    //! destructor
    //!
    ~TGAReader() {}

    //!
    //! Check file format. Return true if the file is TGA file
    //!
    bool checkFormat( GN::File & );

    //!
    //! read TGA header
    //!
    bool readHeader(
        GN::gfx::ImageDesc & o_desc, const uint8_t * i_buf, size_t i_size );

    //!
    //! read TGA image
    //!
    bool readImage( void * o_data );
};

// *****************************************************************************
//                           End of imageTGA.h
// *****************************************************************************
#endif // __GN_GFX_IMAGETGA_H__
