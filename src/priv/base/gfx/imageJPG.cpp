#include "pch.h"
#include "imageJPG.h"

#if GN_MSVC
#pragma warning(disable:4611) // interaction between 'function' and C++ object destruction is non-portable
#endif

uint8_t JpegDataSource::sFakeEOI[2] = { 0xFF, JPEG_EOI };

//
//
// -----------------------------------------------------------------------------
bool JPGReader::checkFormat( GN::File & fp )
{
    GN_GUARD;

    char buf[5];

    if( !fp.seek( 6, GN::FSEEK_SET ) ) return false;

    size_t sz;
    if( !fp.read( buf, 4, &sz ) || 4 != sz ) return false;

    buf[4] = 0;

    return 0 == GN::strCmp( buf, "JFIF" );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool JPGReader::readHeader(
    GN::gfx::ImageDesc & o_desc, const uint8_t * i_buf, size_t i_size )
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
        // force RGB output, if not gray-scale image
        o_desc.format = GN::gfx::FMT_BGRX_8_8_8_8_UNORM;
        bpp = 4;
    }
    else
    {
        o_desc.format = GN::gfx::FMT_L_8_UNORM;
        bpp = 1;
    }

    // fill image descriptor
    o_desc.setFaceAndLevel( 1, 1 ); // 2D image
    GN::gfx::MipmapDesc & m = o_desc.getMipmap( 0, 0 );
    m.width         = (uint16_t)mCInfo.image_width;
    m.height        = (uint16_t)mCInfo.image_height;
    m.depth         = 1;
    m.rowPitch      = (uint32_t)(bpp * mCInfo.image_width);
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
    GN::AutoObjPtr<uint8_t> rgbBuf;
    uint8_t * decompressedBuf;
    if( !grayscale )
    {
        // create temporary RGB_8_8_8 buffer
        rgbBuf.attach( new uint8_t[rowPitch*height] );
        decompressedBuf = rgbBuf;
    }
    else decompressedBuf = (uint8_t*)o_data;

    // read scanlines
    std::vector<uint8_t*> scanlines;
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

    // convert RGB_8_8_8 to BGRX_8_8_8_8
    if( !grayscale )
    {
        GN_ASSERT( rgbBuf );
        uint8_t * src = (uint8_t*)rgbBuf.get();
        uint8_t * dst = (uint8_t*)o_data;
        for( size_t y = 0; y < height; ++y )
        {
            for( size_t x = 0; x < width; ++x )
            {
                dst[0] = src[2];
                dst[1] = src[1];
                dst[2] = src[0];
                dst[3] = 0xFF;
                src += 3;
                dst += 4;
            }
        }
    }

    // success
    return true;

    GN_UNGUARD;
}
