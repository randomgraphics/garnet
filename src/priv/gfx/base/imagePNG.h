#ifndef __GN_GFX_IMAGEPNG_H__
#define __GN_GFX_IMAGEPNG_H__
// *****************************************************************************
/// \file    imagePNG.h
/// \brief   PNG image reader
/// \author  chenlee (2005.6.2)
// *****************************************************************************

extern "C"
{
#include <png.h>
}

///
/// png image reader.
///
class PNGReader
{
    png_struct    * mPng;
    png_info      * mInfo;
    UInt32        mRowPitch;
    const UInt8 * mStart;
    size_t          mSize;

    static void PNGAPI error( png_structp png, png_const_charp msg );
    static void PNGAPI warning( png_structp, png_const_charp msg );

    ///
    /// destroy PNG structures
    ///
    void destroyPng();

    ///
    /// read memory buffer
    ///
    static void
    readImageData( png_struct * i_png, png_byte * o_data, png_size_t i_length );

public:

    ///
    /// constructor
    ///
    PNGReader() : mPng(0), mInfo(0) {}

    ///
    /// destructor
    ///
    ~PNGReader() { destroyPng(); }

    ///
    /// Check file format. Return true if the file is PNG file
    ///
    bool checkFormat( GN::File & );

    ///
    /// read PNG header
    ///
    bool readHeader(
        GN::gfx::ImageDesc & o_desc, const UInt8 * i_buf, size_t i_size );

    ///
    /// read PNG image
    ///
    bool readImage( void * o_data );
};

// *****************************************************************************
//                           End of imagePNG.h
// *****************************************************************************
#endif // __GN_GFX_IMAGEPNG_H__
