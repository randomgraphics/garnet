#include "pch.h"
#include "imageTGA.h"

//
// TGA image header
//
#pragma pack(push,1)
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

    size_t srcBpp = header.bits / 8;

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
            CHECK_SRC_SIZE( srcBpp );
            switch( header.bits )
            {
                case 16: sCopyPixel5551( src, 0, dst, 2, runSize ); dst += runSize*2; break;
                case 24: sCopyPixel888( src, 0, dst, 4, runSize ); dst += runSize*4; break;
                case 32: sCopyPixel8888( src, 0, dst, 4, runSize ); dst += runSize*4; break;
                default:
                    GN_ERROR( "unsupport uncompressed RGB TGA image bits: %d", header.bits );
                    return false;
            }
            src += srcBpp;
        }
        else
        {
            // read RAW chunk
            CHECK_SRC_SIZE( srcBpp * runSize );
            switch( header.bits )
            {
                case 16: sCopyPixel5551( src, 2, dst, 2, runSize ); dst += runSize*2; break;
                case 24: sCopyPixel888( src, 3, dst, 4, runSize ); dst += runSize*4; break;
                case 32: sCopyPixel8888( src, 4, dst, 4, runSize ); dst += runSize*4; break;
                default:
                    GN_ERROR( "unsupport uncompressed RGB TGA image bits: %d", header.bits );
                    return false;
            }
            src += srcBpp * runSize;
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

    TGA_HEADER header;

    // read TGA header
    if( !fp.seek( 0, GN::FSEEK_SET ) ) return false;
    size_t sz;
    if( !fp.read( &header, sizeof(TGA_HEADER), &sz ) || sizeof(TGA_HEADER) != sz ) return false;

    // check header fields
    return (  0 == header.colourmaptype
          ||  1 == header.colourmaptype )
        && (  0 == header.imagetype
          ||  1 == header.imagetype
          ||  2 == header.imagetype
          ||  3 == header.imagetype
          ||  9 == header.imagetype
          || 10 == header.imagetype
          || 11 == header.imagetype
          || 32 == header.imagetype
          || 33 == header.imagetype )
        && (  0 == header.colourmapbits
          || 15 == header.colourmapbits
          || 16 == header.colourmapbits
          || 24 == header.colourmapbits
          || 32 == header.colourmapbits )
        && ( 16 == header.bits
          || 24 == header.bits
          || 32 == header.bits );

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

    const TGA_HEADER & header = *(const TGA_HEADER*)i_buf;

    // copy TGA header
    if( i_size <= sizeof(TGA_HEADER) )
    {
        GN_ERROR( "File size is too small to hold TGA file header." );
        return false;
    }

    // What can we handle
    if( header.imagetype != 2 && header.imagetype != 3 && header.imagetype != 10 )
    {
        GN_ERROR( "We can only handle TGA image type 2(rgb), 3(grey) and 10(rle-rgb).");
        return false;
    }
    if( header.colourmaptype != 0 && header.colourmaptype != 1 )
    {
        GN_ERROR( "We can only handle colour map types of 0 and 1." );
        return false;
    }

    // point mSrc to the start of image data
    size_t minimalSize = sizeof(TGA_HEADER)
                       + header.identsize
                       + header.colourmaptype * header.colourmaplength * header.colourmapbits / 8;
    if( i_size < minimalSize )
    {
        GN_ERROR( "File size is not large enough: minimum(%d) actual(%d).", minimalSize, i_size );
        return false;
    }

    // determine image format
    if( 2 == header.imagetype || 10 == header.imagetype )
    {   // RGB image
        switch( header.bits )
        {
            case 16 : o_desc.format = GN::gfx::FMT_BGRA_5_5_5_1_UNORM; mOutputBytesPerPixel = 2; break;
            case 24 : o_desc.format = GN::gfx::FMT_BGRA_8_8_8_8_UNORM; mOutputBytesPerPixel = 4; break;
            case 32 : o_desc.format = GN::gfx::FMT_BGRA_8_8_8_8_UNORM; mOutputBytesPerPixel = 4; break;
            default :
                GN_ERROR( "unsupport/invalid RGB image bits: %d.", header.bits );
                return false;
        }
    }
    else
    {   // grayscale image
        GN_ASSERT( 3 == header.imagetype );
        switch( header.bits )
        {
            case  8 : o_desc.format = GN::gfx::FMT_L_8_UNORM; mOutputBytesPerPixel = 1; break;
            case 16 : o_desc.format = GN::gfx::FMT_L_16_UNORM; mOutputBytesPerPixel = 2; break;
            case 32 : o_desc.format = GN::gfx::FMT_L_32_UNORM; mOutputBytesPerPixel = 4; break;
            default :
                GN_ERROR( "unsupport/invalid grey image bits: %d.", header.bits );
                return false;
        }
    }

    // update o_desc
	o_desc.setFaceAndLevel( 1, 1 ); // 2D image
    GN::gfx::MipmapDesc & m = o_desc.getMipmap( 0, 0 );
    m.width      = header.width;
    m.height     = header.height;
    m.depth      = 1;
    m.rowPitch   = (uint32_t)( header.width * mOutputBytesPerPixel );
    m.slicePitch = m.rowPitch * header.height;
    m.levelPitch = m.slicePitch;
    GN_ASSERT( o_desc.valid() );

    // success
    mImageSrc  = i_buf;
    mImageSize = i_size;
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

    GN_ASSERT( mImageSrc );

    const TGA_HEADER & header = *(const TGA_HEADER*)mImageSrc;
    size_t skipOver = sizeof(TGA_HEADER)
                   + header.identsize
                   + header.colourmaptype * header.colourmaplength * header.colourmapbits / 8;
    const uint8_t * src = mImageSrc + skipOver;
    uint8_t * dst = (uint8_t*)o_data;
    size_t srcSize = mImageSize - skipOver;
    size_t srcBpp = header.bits / 8;
    size_t numPixels = header.width * header.height;

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
    id.u8 = header.descriptor;

    // read image to temporary buffer, if image is flipped and/or interleaved.
    std::vector<uint8_t> tempBuf;
    if( 0 != id.interleaved || 0 == id.flip )
    {
        tempBuf.resize( numPixels * srcBpp );
        dst = &tempBuf[0];
    }

    switch( header.imagetype )
    {
        // uncompressed RGB image
        case 2 :
        {
            if( srcSize < numPixels * srcBpp )
            {
                GN_ERROR( "incomplete image data." );
                return false;
            }
            switch( header.bits )
            {
                case 16: sCopyPixel5551( src, 2, dst, 2, numPixels ); break;
                case 24: sCopyPixel888( src, 3, dst, 4, numPixels ); break;
                case 32: sCopyPixel8888( src, 4, dst, 4, numPixels ); break;
                default:
                    GN_ERROR( "unsupport uncompressed RGB TGA image bits: %d", header.bits );
                    return false;
            }
            break;
        }

        // uncompressed grayscale image
        case 3:
        {
            if( srcSize < numPixels * srcBpp )
            {
                GN_ERROR( "incomplete image data." );
                return false;
            }
            switch( header.bits )
            {
                case  8: memcpy( dst, src, numPixels*1 ); break;
                case 16: memcpy( dst, src, numPixels*2 ); break;
                case 32: memcpy( dst, src, numPixels*4 ); break;
                default:
                    GN_ERROR( "unsupport uncompressed grayscale TGA image bits: %d", header.bits );
                    return false;
            }
            break;
        }

        // RLE-compressed RGB image
        case 10 :
            if( !sReadRLERGBImage( header, src, srcSize, dst ) ) return false;
            break;

        default:
            GN_ERROR( "unsupport TGA image type : %d", header.imagetype );
            return false;
    }

    // TODO: handle interleaved image
    if( id.interleaved ) GN_WARN( "TGA image is interleaved!" );

    // flip the image
    if( 0 == id.flip )
    {
        dst = (uint8_t*)o_data;
        size_t rowPitch = header.width * mOutputBytesPerPixel;
        for( size_t y = 0; y < header.height; ++y )
        {
            memcpy( &dst[rowPitch*(header.height-y-1)], &tempBuf[rowPitch*y], rowPitch );
        }
    }

    // success
    return true;

    GN_UNGUARD;
}
