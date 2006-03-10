#include "pch.h"
#include "imageTGA.h"

// *****************************************************************************
// local functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static inline void sCopyPixel5551( const uint8_t * src, size_t srcStride, uint8_t * dst, size_t dstStride, size_t count )
{
    const uint16_t * s;
    uint16_t * d;
    for( size_t i = 0; i < count; ++i )
    {
        s = (const uint16_t *)src;
        d = (uint16_t *)dst;
        *d = *s;
        src += srcStride;
        dst += dstStride;
    }
}

//
//
// -----------------------------------------------------------------------------
static inline void sCopyPixel888( const uint8_t * src, size_t srcStride, uint8_t * dst, size_t dstStride, size_t count )
{
    for( size_t i = 0; i < count; ++i )
    {
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
        dst[3] = 0xFF;
        src += srcStride;
        dst += dstStride;
    }
}

//
//
// -----------------------------------------------------------------------------
static inline void sCopyPixel8888( const uint8_t * src, size_t srcStride, uint8_t * dst, size_t dstStride, size_t count )
{
    const uint32_t * s;
    uint32_t * d;
    for( size_t i = 0; i < count; ++i )
    {
		s = (const uint32_t*)src;
		d = (uint32_t*)dst;
		*d = *s;
        src += srcStride;
        dst += dstStride;
    }
}

//
//
// -----------------------------------------------------------------------------
static bool sReadRLERGBImage(
    const TGA_HEADER & header,
    const uint8_t * src,
    size_t srcSize,
    uint8_t * dst )
{
    GN_GUARD;

#define CHECK_SRC_SIZE( bytes ) \
    if( (src + (bytes) ) > end ) { GN_ERROR( "incomplete image data." ); return false; }

    const uint8_t * end = src + srcSize;

    size_t bppSrc = header.bits / 8;

    size_t numPixels = header.width * header.height;

    size_t i = 0;

    uint8_t chunk;

    while( src < end && i < numPixels )
    {
        // read chunk header
        CHECK_SRC_SIZE( 1 );
        chunk = *src;
        ++src;

        size_t runSize = ( chunk & 0x7f ) + 1;

        if( chunk & 0x80 )
        {
            // read RLE chunk
            CHECK_SRC_SIZE( bppSrc );
            switch( header.bits )
            {
                case 16: sCopyPixel5551( src, 0, dst, 2, runSize ); dst += runSize*2; break;
                case 24: sCopyPixel888( src, 0, dst, 4, runSize ); dst += runSize*4; break;
                case 32: sCopyPixel8888( src, 0, dst, 4, runSize ); dst += runSize*4; break;
                default:
                    GN_ERROR( "unsupport uncompressed RGB TGA image bits: %d", header.bits );
                    return false;
            }
            src += bppSrc;
        }
        else
        {
            // read RAW chunk
            CHECK_SRC_SIZE( bppSrc * runSize );
            switch( header.bits )
            {
                case 16: sCopyPixel5551( src, 2, dst, 2, runSize ); dst += runSize*2; break;
                case 24: sCopyPixel888( src, 3, dst, 4, runSize ); dst += runSize*4; break;
                case 32: sCopyPixel8888( src, 4, dst, 4, runSize ); dst += runSize*4; break;
                default:
                    GN_ERROR( "unsupport uncompressed RGB TGA image bits: %d", header.bits );
                    return false;
            }
            src += bppSrc * runSize;
        }
        i += runSize;
    }

    // check for incomplete image
    if( i < numPixels )
    {
        GN_ERROR( "incomplete image data." );
        return false;
    }

    // success
    return true;

    GN_UNGUARD;
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool TGAReader::checkFormat( GN::File & fp )
{
    GN_GUARD;

    TGA_HEADER mHeader;

    // read TGA mHeader
    if( !fp.seek( 0, GN::FSEEK_SET ) ) return false;
    if( sizeof(mHeader) != fp.read( &mHeader, sizeof(mHeader) ) ) return false;

    // check mHeader fields
    return (  0 == mHeader.colourmaptype
          ||  1 == mHeader.colourmaptype )
        && (  0 == mHeader.imagetype
          ||  1 == mHeader.imagetype
          ||  2 == mHeader.imagetype
          ||  3 == mHeader.imagetype
          ||  9 == mHeader.imagetype
          || 10 == mHeader.imagetype
          || 11 == mHeader.imagetype
          || 32 == mHeader.imagetype
          || 33 == mHeader.imagetype )
        && (  0 == mHeader.colourmapbits
          || 15 == mHeader.colourmapbits
          || 16 == mHeader.colourmapbits
          || 24 == mHeader.colourmapbits
          || 32 == mHeader.colourmapbits )
        && ( 16 == mHeader.bits
          || 24 == mHeader.bits
          || 32 == mHeader.bits );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool TGAReader::readHeader(
    GN::gfx::ImageDesc & o_desc, const uint8_t * i_buf, size_t i_size )
{
    GN_GUARD;

    GN_ASSERT( i_buf && i_size );

    // copy TGA mHeader
    if( i_size <= sizeof(mHeader) )
    {
        GN_ERROR( "File size is too small to hold TGA file mHeader." );
        return false;
    }
    ::memcpy( &mHeader, i_buf, sizeof(mHeader) );

    // What can we handle
    if( mHeader.imagetype != 2 && mHeader.imagetype != 3 && mHeader.imagetype != 10 )
    {
        GN_ERROR( "We can only handle TGA image type 2(rgb), 3(grey) and 10(rle-rgb).");
        return false;
    }
    if( mHeader.colourmaptype != 0 && mHeader.colourmaptype != 1 )
    {
        GN_ERROR( "We can only handle colour map types of 0 and 1." );
        return false;
    }

    // point mSrc to the start of image data
    size_t skipOver = sizeof(mHeader)
                    + mHeader.identsize
                    + mHeader.colourmaptype * mHeader.colourmaplength * mHeader.colourmapbits / 8;
    if( i_size < skipOver )
    {
        GN_ERROR( "File size is not large enough: minimum(%d) actual(%d).", skipOver, i_size );
        return false;
    }
    mSrc  = i_buf + skipOver;
    mSize = i_size - skipOver;

    // determine image format
    if( 2 == mHeader.imagetype || 10 == mHeader.imagetype )
    {   // RGB image
        switch( mHeader.bits )
        {
            case 16 : o_desc.format = GN::gfx::FMT_BGRA_5_5_5_1_UNORM; mOutputBytesPerPixel = 2; break;
            case 24 : o_desc.format = GN::gfx::FMT_BGRA_8_8_8_8_UNORM; mOutputBytesPerPixel = 4; break;
            case 32 : o_desc.format = GN::gfx::FMT_BGRA_8_8_8_8_UNORM; mOutputBytesPerPixel = 4; break;
            default :
                GN_ERROR( "unsupport/invalid RGB image bits: %d.", mHeader.bits );
                return false;
        }
    }
    else
    {   // grayscale image
        GN_ASSERT( 3 == mHeader.imagetype );
        switch( mHeader.bits )
        {
            case  8 : o_desc.format = GN::gfx::FMT_L_8_UNORM; mOutputBytesPerPixel = 1; break;
            case 16 : o_desc.format = GN::gfx::FMT_L_16_UNORM; mOutputBytesPerPixel = 2; break;
            case 32 : o_desc.format = GN::gfx::FMT_L_32_UNORM; mOutputBytesPerPixel = 4; break;
            default :
                GN_ERROR( "unsupport/invalid grey image bits: %d.", mHeader.bits );
                return false;
        }
    }

    // update o_desc
	o_desc.setFaceAndLevel( 1, 1 ); // 2D image
    GN::gfx::MipmapDesc & m = o_desc.getMipmap( 0, 0 );
    m.width      = mHeader.width;
    m.height     = mHeader.height;
    m.depth      = 1;
    m.rowPitch   = mHeader.width * mOutputBytesPerPixel;
    m.slicePitch = m.rowPitch * mHeader.height;
    m.levelPitch = m.slicePitch;
    GN_ASSERT( o_desc.valid() );

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool TGAReader::readImage( void * o_data )
{
    GN_GUARD;

    if( 0 == o_data )
    {
        GN_ERROR( "NULL output buffer!" );
        return false;
    }

    GN_ASSERT( mSrc );
    const uint8_t * src = mSrc;
    uint8_t * dst = (uint8_t*)o_data;
    size_t bppSrc = mHeader.bits / 8;
    size_t numPixels = mHeader.width * mHeader.height;

    // check image descriptor
    union ImageDescriptor
    {
        unsigned char u8;
        struct 
        {
            int attr        : 4; // bits of attribute channel.
            int _           : 1; // reserved
            int flip        : 1; // vertical flip
                                 //     0 = Origin in lower left-hand corner
                                 //     1 = Origin in upper left-hand corner
            int interleaved : 2; // interleave flag
                                 //     00 = non-interleaved
                                 //     01 = two-way (even/odd) interleaving
                                 //     10 = four way interleaving
                                 //     11 = reserved
        };
    };
    ImageDescriptor id;
    id.u8 = mHeader.descriptor;

    // read image to temporary buffer, if image is flipped and/or interleaved.
    std::vector<uint8_t> tempBuf;
    if( 0 != id.interleaved || 0 == id.flip )
    {
        tempBuf.resize( numPixels * bppSrc );
        dst = &tempBuf[0];
    }

    switch( mHeader.imagetype )
    {
        // uncompressed RGB image
        case 2 :
        {
            if( mSize < numPixels * bppSrc )
            {
                GN_ERROR( "incomplete image data." );
                return false;
            }
            switch( mHeader.bits )
            {
                case 16: sCopyPixel5551( src, 2, dst, 2, numPixels ); break;
                case 24: sCopyPixel888( src, 3, dst, 4, numPixels ); break;
                case 32: sCopyPixel8888( src, 4, dst, 4, numPixels ); break;
                default:
                    GN_ERROR( "unsupport uncompressed RGB TGA image bits: %d", mHeader.bits );
                    return false;
            }
            break;
        }

        // uncompressed grayscale image
        case 3:
        {
            if( mSize < numPixels * bppSrc )
            {
                GN_ERROR( "incomplete image data." );
                return false;
            }
            switch( mHeader.bits )
            {
                case  8: memcpy( dst, src, numPixels*1 ); break;
                case 16: memcpy( dst, src, numPixels*2 ); break;
                case 32: memcpy( dst, src, numPixels*4 ); break;
                default:
                    GN_ERROR( "unsupport uncompressed grayscale TGA image bits: %d", mHeader.bits );
                    return false;
            }
            break;
        }

        // RLE-compressed RGB image
        case 10 :
            if( !sReadRLERGBImage( mHeader, src, mSize, dst ) ) return false;
            break;

        default:
            GN_ERROR( "unsupport TGA image type : %d", mHeader.imagetype );
            return false;
    }

    // TODO: handle interleaved image
    if( id.interleaved ) GN_WARN( "TGA image is interleaved!" );

    // flip the image
    if( 0 == id.flip )
    {
        dst = (uint8_t*)o_data;
        size_t rowPitch = mHeader.width * mOutputBytesPerPixel;
        for( size_t y = 0; y < mHeader.height; ++y )
        {
            memcpy( &dst[rowPitch*(mHeader.height-y-1)], &tempBuf[rowPitch*y], rowPitch );
        }
    }

    // success
    return true;

    GN_UNGUARD;
}
