#include "pch.h"
#if !GN_ENABLE_INLINE
#include "garnet/base/image.inl"
#endif
#include "image_png.h"
#include "image_jpg.h"
#include "image_dds.h"

// *****************************************************************************
// ImageDesc
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::ImageDesc::validate() const
{
    // check type
    if( type < 0 || type >= NUM_IMAGE_TYPES )
    {
        GN_ERROR( "invalid image type!" );
        return false;
    }

    // check format
    if( format < 0 || format >= NUM_CLRFMTS )
    {
        GN_ERROR( "invalid image format!" );
        return false;
    }

    // check numMips
    if( numMips > MAX_MIPLEVELS )
    {
        GN_ERROR( "numMips is out of range!" );
        return false;
    }

    const GN::ClrFmtDesc & fd = GN::getClrFmtDesc( format );

    // check mipmaps
    for ( uint8_t i = 0; i < numMips; ++ i )
    {
        const MipDesc & m = mips[i];

        // check image size
        if( 0 == m.width || 0 == m.height || 0 == m.depth )
        {
            GN_ERROR( "mipmaps[%d] size is zero!", i );
            return false;
        }
        if( IMG_1D == type && ( 1 != m.height || 1 != m.depth ) )
        {
            GN_ERROR( "height and depth must be 1 for 1D image!" );
            return false;
        }
        if( IMG_2D == type && 1 != m.depth )
        {
            GN_ERROR( "depth must be 1 for 2D image!" );
            return false;
        }
        if( IMG_CUBE == type && 6 != m.depth )
        {
            GN_ERROR( "depth must be 6 for cubemap!" );
            return false;
        }
        if( IMG_CUBE == type && (m.width != m.height) )
        {
            GN_ERROR( "width and height must be equal for cubemap!" );
            return false;
        }

        // check pitches
        uint16_t w = m.width / fd.blockWidth;
        uint16_t h = m.height / fd.blockHeight;
        if( 0 == w ) w = 1;
        if( 0 == h ) h = 1;
        if( m.rowPitch != (uint32_t)w * fd.blockWidth * fd.blockHeight * fd.bits / 8 )
        {
            GN_ERROR( "rowPitch of mipmaps[%d] is incorrect!", i );
            return false;
        }
        if( m.slicePitch != m.rowPitch * h )
        {
            GN_ERROR( "slicePitch of mipmaps[%d] is incorrect!", i );
            return false;
        }
    }

    // success
    return true;
}

// *****************************************************************************
// imgreade_c::Impl
// *****************************************************************************

//!
//! implementation class of image reader
//!
class GN::ImageReader::Impl
{
    //!
    //! file format tag
    //!
    enum FileFormat
    {
        UNKNOWN = 0,
        JPEG,
        BMP,
        PNG,
        DDS,
    };

    //!
    //! image reader state
    //!
    enum ReaderState
    {
        INVALID = 0,
        INITIALIZED,
        HEADER_READEN,
        DATA_READEN,
    };

    std::vector<uint8_t> mSrc;

    JpegReader  mJpg;
    PngReader   mPng;
    DDSReader   mDds;

    FileFormat  mFormat;
    ReaderState mState;

public:

    //!
    //! default ctor
    //!
    Impl() : mFormat(UNKNOWN), mState(INVALID) {}

    //!
    //! reset image reader
    //!
    bool reset( File & i_file )
    {
        GN_GUARD;

        static const size_t HEADER_BYTES = 10;

        // reset internal states
        mFormat = UNKNOWN;
        mState  = INVALID;

        // get file size
        size_t sz = i_file.size();
        if( sz <= HEADER_BYTES )
        {
            GN_ERROR( "image file size is too small!" );
            return false;
        }
        mSrc.resize( sz );

        // read file header
        sz = i_file.read( &mSrc[0], HEADER_BYTES );
        if( size_t(-1) == sz || sz < HEADER_BYTES )
        {
            GN_ERROR( "fail to read image header!" );
            return false;
        }

        // detect file format
        if( 'J' == mSrc[6] && 'F' == mSrc[7] &&
            'I' == mSrc[8] && 'F' == mSrc[9] )
        {
            // JPEG format
            mFormat = JPEG;
        }
        else if( 'B' == mSrc[0] && 'M' == mSrc[1] )
        {
            // BMP format
            mFormat = BMP;
        }
        else if ( 0 == png_sig_cmp(&mSrc[0], 0, 8) )
        {
            // PNG format
            mFormat = PNG;
        }
        else if( 'D' == mSrc[0] && 'D' == mSrc[1] &&
                 'S' == mSrc[2] && ' ' == mSrc[3] )
        {
            // DDS format
            mFormat = DDS;
        }
        else
        {
            GN_ERROR( "unknown image file format!" );
            return false;
        }

        // read whole file
        sz = i_file.read( &mSrc[HEADER_BYTES], mSrc.size() - HEADER_BYTES );
        if( size_t(-1) == sz ) return false;

        // success
        mState = INITIALIZED;
        return true;

        GN_UNGUARD;
    }

    //!
    //! read image header
    //!
    bool readHeader( ImageDesc & o_desc )
    {
        GN_GUARD;

        if( INITIALIZED != mState )
        {
            GN_ERROR( "image reader is not ready for header reading!" );
            return false;
        }

        GN_ASSERT( !mSrc.empty() );

        #define READ_HEADER( reader ) \
            if( !reader.readHeader( o_desc, &mSrc[0], mSrc.size() ) ) \
            { mState = INVALID; return false; }

        switch( mFormat )
        {
            case JPEG : READ_HEADER( mJpg ); break;
            //case BMP  : READ_HEADER( mBmp ); break;
            case PNG  : READ_HEADER( mPng ); break;
            case DDS  : READ_HEADER( mDds ); break;
            default   :
                GN_ERROR( "unknown or unsupport file format!" );
                mState = INVALID;
                return false;
        }

        #undef READ_HEADER

        // success
        mState = HEADER_READEN;
        return true;

        GN_UNGUARD;
    }

    //!
    //! read image data
    //!
    bool readImage( void * o_data )
    {
        GN_GUARD;

        if( 0 == o_data )
        {
            GN_ERROR( "null output buffer!" );
            return false;
        }

        if( HEADER_READEN != mState )
        {
            GN_ERROR( "image reader is not ready for image reading!" );
            return false;
        }

        #define READ_IMAGE( reader ) \
            if( !reader.readImage( o_data ) ) \
            { mState = INVALID; return false; }

        switch( mFormat )
        {
            case JPEG : READ_IMAGE( mJpg ); break;
            //case BMP  : READ_IMAGE( mBmp ); break;
            case PNG  : READ_IMAGE( mPng ); break;
            case DDS  : READ_IMAGE( mDds ); break;
            default   :
                GN_ERROR( "unknown or unsupport file format!" );
                mState = INVALID;
                return false;
        }

        #undef READ_IMAGE

        // success
        mState = DATA_READEN;
        return true;

        GN_UNGUARD;
    }
};

// *****************************************************************************
// ImageReader
// *****************************************************************************

//
// ctor/dtor
// -----------------------------------------------------------------------------
GN::ImageReader::ImageReader() : mImpl( new Impl ) {}
GN::ImageReader::~ImageReader() { delete mImpl; }

//
// forward call to Impl
// -----------------------------------------------------------------------------
bool GN::ImageReader::reset( File & i_file )
{
    GN_GUARD;
    return mImpl->reset( i_file );
    GN_UNGUARD;
}
//
bool GN::ImageReader::readHeader( ImageDesc & o_desc )
{
    GN_GUARD;
    return mImpl->readHeader( o_desc );
    GN_UNGUARD;
}
//
bool GN::ImageReader::readImage( void * o_data )
{
    GN_GUARD;
    return mImpl->readImage( o_data );
    GN_UNGUARD;
}
