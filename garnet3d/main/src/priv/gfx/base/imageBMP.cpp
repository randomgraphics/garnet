#include "pch.h"
#include "imageBMP.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.base.image");

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool BMPReader::checkFormat( GN::File & fp )
{
    GN_GUARD;

    BMPHeader header;

    // read BMP header
    if( !fp.seek( 0, GN::FSEEK_SET ) ) return false;
    size_t sz;
    if( !fp.read( &header, sizeof(BMPHeader), &sz ) || sizeof(BMPHeader) != sz ) return false;

#if GN_PPC
    header.swapEndian();
#endif

    // check header fields
    return 'B' == header.fileHeader.B
        && 'M' == header.fileHeader.M
        && 1 == header.infoHeader.planes;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool BMPReader::readHeader(
    GN::gfx::ImageDesc & o_desc, const uint8_t * i_buf, size_t i_size )
{
    GN_GUARD;

    GN_ASSERT( i_buf && i_size );

    // copy BMP header
    if( i_size <= sizeof(BMPHeader) )
    {
        GN_ERROR(sLogger)( "File size is too small to hold BMP file header." );
        return false;
    }
    memcpy( &mHeader, i_buf, sizeof(BMPHeader) );
#if GN_PPC
    mHeader.swapEndian();
#endif

    // What can we handle
    if( 0 != mHeader.infoHeader.compression ||
        16 != mHeader.infoHeader.bitCount &&
        24 != mHeader.infoHeader.bitCount &&
        32 != mHeader.infoHeader.bitCount )
    {
        GN_ERROR(sLogger)( "We can only handle uncompressed high-color and/or true-color BMP image.");
        return false;
    }

    // point mSrc to the start of image data
    size_t headerSize = mHeader.fileHeader.offBits;
    if( i_size < headerSize )
    {
        GN_ERROR(sLogger)( "File size is not large enough: minimum(%d) actual(%d).", headerSize, i_size );
        return false;
    }

    // determine image format
    switch( mHeader.infoHeader.bitCount )
    {
        case 16 : o_desc.format = GN::gfx::FMT_BGRX_5_5_5_1_UNORM; mOutputBytesPerPixel = 2; break;
        case 24 : o_desc.format = GN::gfx::FMT_BGRX_8_8_8_8_UNORM; mOutputBytesPerPixel = 4; break;
        case 32 : o_desc.format = GN::gfx::FMT_BGRX_8_8_8_8_UNORM; mOutputBytesPerPixel = 4; break;
        default :
            GN_ERROR(sLogger)( "unsupport/invalid RGB image bits: %d.", mHeader.infoHeader.bitCount );
            return false;
    }

    // update o_desc
	o_desc.setFaceAndLevel( 1, 1 ); // 2D image
    GN::gfx::MipmapDesc & m = o_desc.getMipmap( 0, 0 );
    m.width      = mHeader.infoHeader.width;
    m.height     = mHeader.infoHeader.height;
    m.depth      = 1;
    m.rowPitch   = m.width * mOutputBytesPerPixel;
    m.slicePitch = m.rowPitch * m.height;
    m.levelPitch = m.slicePitch;
    GN_ASSERT( o_desc.valid() );

    // success
    mImageSrc = i_buf + mHeader.fileHeader.offBits;
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool BMPReader::readImage( void * o_data ) const
{
    GN_GUARD;

    if( 0 == o_data )
    {
        GN_ERROR(sLogger)( "NULL output buffer!" );
        return false;
    }

    GN_ASSERT( mImageSrc );

    const uint8_t * src = mImageSrc;
    size_t width = (size_t)mHeader.infoHeader.width;
    size_t height = (size_t)mHeader.infoHeader.height;
    size_t srcPitch = ( width * mHeader.infoHeader.bitCount / 8 + 3 ) & ~3; // align to 4 bytes.
    size_t dstPitch = width * mOutputBytesPerPixel;
    uint8_t * dst = (uint8_t*)o_data + dstPitch * ( height - 1 );

    switch( mHeader.infoHeader.bitCount )
    {
        case 16:
            for( size_t y = 0; y < height; ++y )
            {
#if GN_PPC
                GN::swapEndian8In16( dst, src, width );
#else
                memcpy( dst, src, width * 2 );
#endif
                src += srcPitch;
                dst -= dstPitch;
            }
            break;

        case 24:
            for( size_t y = 0; y < height; ++y )
            {
                const uint8_t * s = src;
                uint8_t * d = dst;
                for( size_t x = 0; x < width; ++x, s+=3, d+=4 )
                {
#if GN_PPC
                    d[1] = s[2];
                    d[2] = s[1];
                    d[3] = s[0];
#else
                    d[0] = s[0];
                    d[1] = s[1];
                    d[2] = s[2];
#endif
                }
                src += srcPitch;
                dst -= dstPitch; 
            }
            break;

        case 32:
            for( size_t y = 0; y < height; ++y )
            {
#if GN_PPC
                GN::swapEndian8In32( dst, src, width );
#else
                memcpy( dst, src, width * 4 );
#endif
                src += srcPitch;
                dst -= dstPitch; 
            }
            break;

        default:
            GN_ERROR(sLogger)( "unsupport uncompressed RGB BMP image bits: %d", mHeader.infoHeader.bitCount );
            return false;
    }

    // success
    return true;

    GN_UNGUARD;
}
