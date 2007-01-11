#ifndef __GN_GFX_IMAGETGA_H__
#define __GN_GFX_IMAGETGA_H__
// *****************************************************************************
/// \file    imageTGA.h
/// \brief   TGA image reader
/// \author  chenlee (2005.6.2)
// *****************************************************************************

///
/// tga image reader.
///
class TGAReader
{
    const UInt8 * mImageSrc;
    size_t          mImageSize;
    size_t          mOutputBytesPerPixel;

public:

    ///
    /// constructor
    ///
    TGAReader() : mImageSrc(0) {}

    ///
    /// destructor
    ///
    ~TGAReader() {}

    ///
    /// Check file format. Return true if the file is TGA file
    ///
    bool checkFormat( GN::File & );

    ///
    /// read TGA header
    ///
    bool readHeader(
        GN::gfx::ImageDesc & o_desc, const UInt8 * i_buf, size_t i_size );

    ///
    /// read TGA image
    ///
    bool readImage( void * o_data );
};

// *****************************************************************************
//                           End of imageTGA.h
// *****************************************************************************
#endif // __GN_GFX_IMAGETGA_H__
