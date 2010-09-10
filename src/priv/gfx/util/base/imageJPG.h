#ifndef __GN_GFX_IMAGEJPG_H__
#define __GN_GFX_IMAGEJPG_H__
// *****************************************************************************
/// \file
/// \brief   Jpeg image reader
/// \author  chenlee (2005.6.2)
// *****************************************************************************

extern "C"
{
#include <jpeglib.h>
#include <jerror.h>
}
#include <setjmp.h>

///
/// custom jpeg error handler
///
class JpegErrorHandler : public jpeg_error_mgr
{
    jmp_buf * mJumpBuf;

    static void sErrorExit( j_common_ptr cinfo );

public:
    ///
    /// Construct JPEG error handler
    ///
    JpegErrorHandler( jmp_buf * buf ) : mJumpBuf(buf)
    {
        GN_ASSERT( buf );
        jpeg_std_error( this );
        error_exit = &sErrorExit;
    }
};

///
/// custom jpeg source manager
///
class JpegDataSource : public jpeg_source_mgr
{
    const uint8 * mStart;
    size_t          mSize;
    static uint8  sFakeEOI[2];

    static void doNothing(j_decompress_ptr) {}
    //
    static JPEG_BOOL fillInputBuffer(j_decompress_ptr cinfo)
    {
        // this function should never be called for an valid jpeg stream
        WARNMS( cinfo, JWRN_JPEG_EOF );
        GN_ASSERT( cinfo->src );
        JpegDataSource * src = (JpegDataSource*)cinfo->src;
        src->next_input_byte = sFakeEOI;
        src->bytes_in_buffer = 2;
        return true;
    }
    //
    static void skipInputData(j_decompress_ptr cinfo, long getNumBytes)
    {
        if( 0 == getNumBytes ) return;
        GN_ASSERT( cinfo->src && getNumBytes > 0 );
        JpegDataSource * src = (JpegDataSource*)cinfo->src;
        if( size_t(getNumBytes) <= src->bytes_in_buffer )
        {
            src->next_input_byte += getNumBytes;
            src->bytes_in_buffer -= getNumBytes;
        }
        else
        {
            // program should not reach here for an valid jpeg stream
            WARNMS( cinfo, JWRN_JPEG_EOF );
            src->next_input_byte = sFakeEOI;
            src->bytes_in_buffer = 2;
        }
    }

public:

    ///
    /// Initialize JPEG data source
    ///
    void init( const JOCTET * i_buf, size_t i_size )
    {
        GN_ASSERT( i_buf && i_size );

        // initialize data pointer
        next_input_byte = i_buf;
        bytes_in_buffer = i_size;
        mStart = i_buf;
        mSize = i_size;

        // initialize function pointers
        init_source = &doNothing;
        fill_input_buffer = &fillInputBuffer;
        skip_input_data = &skipInputData;
        resync_to_restart = &jpeg_resync_to_restart;
        term_source = &doNothing;
    }
};

///
/// jpeg image reader
///
class JPGReader
{
    jmp_buf                 mJumpBuf;
    jpeg_decompress_struct  mCInfo;
    JpegErrorHandler        mErr;
    JpegDataSource          mSrc;

public:

    ///
    /// constructor
    ///
    JPGReader() : mErr(&mJumpBuf)
    {
        memset( &mCInfo, 0, sizeof(mCInfo) );
    }

    ///
    /// destructor
    ///
    ~JPGReader()
    {
        jpeg_destroy_decompress(&mCInfo);
    }

    ///
    /// Check file format. Return true if the file is JPEG file
    ///
    bool checkFormat( GN::File & );

    ///
    /// read JPEG header
    ///
    bool readHeader(
        GN::gfx::ImageDesc & o_desc, const uint8 * i_buf, size_t i_size );

    ///
    /// read JPEG image
    ///
    bool readImage( void * o_data );
};
// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_IMAGEJPG_H__
