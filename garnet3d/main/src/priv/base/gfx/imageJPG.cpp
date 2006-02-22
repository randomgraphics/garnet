#include "pch.h"
#include "imageJPG.h"

#if GN_MSVC
#pragma warning(disable:4611) // interaction between 'function' and C++ object destruction is non-portable
#endif

uint8_t JpegDataSource::sFakeEOI[2] = { 0xFF, JPEG_EOI };

//
//
// -----------------------------------------------------------------------------
bool JpegReader::readHeader(
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
    if( mCInfo.image_width > GN::gfx::ImageDesc::MAX_IMGSIZE ||
        mCInfo.image_height > GN::gfx::ImageDesc::MAX_IMGSIZE )
    {
        GN_ERROR( "image size is too large!" );
        return false;
    }

    // check image format
    size_t bpp;
    if( JCS_GRAYSCALE != mCInfo.jpeg_color_space )
    {
        // force RGB output, if not gray-scale image
        o_desc.format = GN::gfx::FMT_RGB_8_8_8_UNORM;
        bpp = 3;
    }
    else
    {
        o_desc.format = GN::gfx::FMT_L_8_UNORM;
        bpp = 1;
    }

    // fill image descriptor
    GN::gfx::ImageDesc::MipDesc & m = o_desc.mips[0];
    o_desc.type     = GN::gfx::ImageDesc::IMG_2D;
    o_desc.numMips  = 1;
    m.width         = (uint16_t)mCInfo.image_width;
    m.height        = (uint16_t)mCInfo.image_height;
    m.depth         = 1;
    m.rowPitch      = (uint32_t)(sizeof(JOCTET) * bpp * mCInfo.image_width);
    m.slicePitch    = m.rowPitch * m.height;
    m.levelPitch    = m.slicePitch;

    // success
    GN_ASSERT( o_desc.validate() );
    return true;

    // failed
    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool JpegReader::readImage( void * o_data )
{
    GN_GUARD;

    GN_ASSERT( o_data );

    if( JCS_GRAYSCALE != mCInfo.jpeg_color_space )
    {
        // force RGB output, if not gray-scale image
        mCInfo.out_color_space = JCS_RGB;
    }

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

    // read scanlines
    uint32_t rowPitch = mCInfo.output_width * mCInfo.output_components;
    std::vector<uint8_t*> scanlines;
    scanlines.resize( size_t(mCInfo.rec_outbuf_height) );
    for( size_t i = 0; i < scanlines.size(); ++i )
    {
        scanlines[i] = ((uint8_t*)o_data) + rowPitch * i;
    }
    JDIMENSION left_scanlines = mCInfo.output_height;
    JDIMENSION readen_scanlines;
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

    // success
    jpeg_finish_decompress( &mCInfo );
    return true;

    GN_UNGUARD;
}
