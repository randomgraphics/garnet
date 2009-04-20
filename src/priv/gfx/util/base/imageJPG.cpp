#include "pch.h"
#include "imageJPG.h"

#if GN_MSVC
#pragma warning(disable:4611) // interaction between 'function' and C++ object destruction is non-portable
#endif

UInt8 JpegDataSource::sFakeEOI[2] = { 0xFF, JPEG_EOI };

static GN::Logger * sLogger = GN::getLogger("GN.gfx.base.image.JPG");

//
//
// -----------------------------------------------------------------------------
void JpegErrorHandler::sErrorExit( j_common_ptr cinfo )
{
    char buffer[JMSG_LENGTH_MAX];
    (*cinfo->err->format_message) (cinfo, buffer);
    GN_ERROR(sLogger)( buffer );
    JpegErrorHandler * err = (JpegErrorHandler*)cinfo->err;
    longjmp( *err->mJumpBuf, -1 );
}

//
//
// -----------------------------------------------------------------------------
bool JPGReader::checkFormat( GN::File & fp )
{
    GN_GUARD;

    char buf[11];

    if( !fp.seek( 0, GN::FILE_SEEK_SET ) ) return false;

    size_t sz;
    if( !fp.read( buf, 11, &sz ) || 11 != sz ) return false;

    buf[10] = 0;

    return
        0xFF == (unsigned char)buf[0] &&
        0xD8 == (unsigned char)buf[1] &&
        ( 0 == GN::strCmp( buf+6, "JFIF" ) || 0 == GN::strCmp( buf+6, "Exif" ) );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool JPGReader::readHeader(
    GN::gfx::ImageDesc & o_desc, const UInt8 * i_buf, size_t i_size )
{
    GN_GUARD;

    // catch jpg error
    if ( 0 != setjmp( mJumpBuf ) )
    {
        // jpeg read error
        return false;
    }

    // destroy old decompresser
    jpeg_destroy_decompress( &mCInfo );

    // initialize decompresser
    mCInfo.err = &mErr;
    jpeg_create_decompress( &mCInfo );

    // specify o_data source
    mSrc.init( i_buf, i_size );
    mCInfo.src = &mSrc;

    // read jpeg header
    jpeg_read_header( &mCInfo, JPEG_TRUE );

    // check image format
    size_t bpp;
    if( JCS_GRAYSCALE != mCInfo.jpeg_color_space )
    {
        // force RGBA output, if not gray-scale image
        o_desc.format = GN::gfx::ColorFormat::RGBA_8_8_8_8_UNORM;
        bpp = 4;
    }
    else
    {
        o_desc.format = GN::gfx::ColorFormat::L_8_UNORM;
        bpp = 1;
    }

    // fill image descriptor
    o_desc.setFaceAndLevel( 1, 1 ); // 2D image
    GN::gfx::MipmapDesc & m = o_desc.getMipmap( 0, 0 );
    m.width         = (UInt16)mCInfo.image_width;
    m.height        = (UInt16)mCInfo.image_height;
    m.depth         = 1;
    m.rowPitch      = (UInt32)(bpp * mCInfo.image_width);
    m.slicePitch    = m.rowPitch * m.height;
    m.levelPitch    = m.slicePitch;

    // success
    GN_ASSERT( o_desc.valid() );
    return true;

    // failed
    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool JPGReader::readImage( void * o_data )
{
    GN_GUARD;

    GN_ASSERT( o_data );

    bool grayscale = JCS_GRAYSCALE == mCInfo.jpeg_color_space;

    // force RGB output, if not gray-scale image
    if( grayscale ) mCInfo.out_color_space = JCS_RGB;

    // catch jpg error
    if ( 0 != setjmp( mJumpBuf ) )
    {
        // jpeg read error
        return false;
    }

    // start decompressing
    if( !jpeg_start_decompress( &mCInfo ) ) return false;

    // make sure no scaling, no quantizing.
    GN_ASSERT(
        mCInfo.output_width == mCInfo.image_width &&
        mCInfo.output_height == mCInfo.image_height &&
        mCInfo.out_color_components == mCInfo.output_components );

	size_t width = (size_t)mCInfo.output_width;
	size_t height = (size_t)mCInfo.output_height;
    size_t rowPitch = width * mCInfo.output_components;
    GN::AutoObjPtr<UInt8> rgbBuf;
    UInt8 * decompressedBuf;
    if( !grayscale )
    {
        // create temporary RGB_8_8_8 buffer
        rgbBuf.attach( new UInt8[rowPitch*height] );
        decompressedBuf = rgbBuf;
    }
    else decompressedBuf = (UInt8*)o_data;

    // read scanlines
    std::vector<UInt8*> scanlines;
    scanlines.resize( height );
    for( size_t i = 0; i < scanlines.size(); ++i )
    {
        scanlines[i] = decompressedBuf + rowPitch * i;
    }
    size_t left_scanlines = height;
    size_t readen_scanlines;
    size_t     readen_bytes;
    while( left_scanlines > 0 )
    {
        readen_scanlines = jpeg_read_scanlines(
            &mCInfo, &scanlines[0], (JDIMENSION)scanlines.size() );
        GN_ASSERT( readen_scanlines <= left_scanlines );
        left_scanlines -= readen_scanlines;

        readen_bytes = readen_scanlines * rowPitch;
        for( size_t i = 0; i < scanlines.size(); ++i )
        {
            scanlines[i] += readen_bytes;
        }
    }

    jpeg_finish_decompress( &mCInfo );

    // convert RGB_8_8_8 to RGBA_8_8_8_8
    if( !grayscale )
    {
        GN_ASSERT( rgbBuf );
        UInt8 * src = (UInt8*)rgbBuf.get();
        UInt8 * dst = (UInt8*)o_data;
        for( size_t y = 0; y < height; ++y )
        {
            for( size_t x = 0; x < width; ++x )
            {
#if GN_XENON
                dst[3] = src[0];
                dst[2] = src[1];
                dst[1] = src[2];
                dst[0] = 0xFF;
#else
                dst[0] = src[0];
                dst[1] = src[1];
                dst[2] = src[2];
                dst[3] = 0xFF;
#endif
                src += 3;
                dst += 4;
            }
        }
    }

    // success
    return true;

    GN_UNGUARD;
}
