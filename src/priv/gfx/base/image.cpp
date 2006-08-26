#include "pch.h"
#if !GN_ENABLE_INLINE
#include "garnet/gfx/image.inl"
#endif

#include "imageBMP.h"
#include "imageDDS.h"
#include "imageJPG.h"
#include "imagePNG.h"
#include "imageTGA.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.base.image");

// *****************************************************************************
// ImageDesc
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::ImageDesc::valid() const
{
    // check format
    if( format < 0 || format >= NUM_CLRFMTS )
    {
        GN_ERROR(sLogger)( "invalid image format!" );
        return false;
    }

    // check mipmap pointer
    if( numFaces > 0 && numLevels > 0 && 0 == mipmaps )
    {
        GN_ERROR(sLogger)( "Null mipmap array!" );
        return false;
    }

    const GN::gfx::ClrFmtDesc & fd = getClrFmtDesc( format );

    // check mipmaps
    for( size_t f = 0; f < numFaces; ++f )
    for( size_t l = 0; l < numLevels; ++l )
    {
        const MipmapDesc & m = getMipmap( f, l );

        // check image size
        if( 0 == m.width || 0 == m.height || 0 == m.depth )
        {
            GN_ERROR(sLogger)( "mipmaps[%d] size is zero!", l );
            return false;
        }

        // check pitches
        uint32_t w = m.width / fd.blockWidth;
        uint32_t h = m.height / fd.blockHeight;
        if( 0 == w ) w = 1;
        if( 0 == h ) h = 1;
        if( m.rowPitch != w * fd.blockWidth * fd.blockHeight * fd.bits / 8 )
        {
            GN_ERROR(sLogger)( "rowPitch of mipmaps[%d][%d] is incorrect!", f, l );
            return false;
        }
        if( m.slicePitch != m.rowPitch * h )
        {
            GN_ERROR(sLogger)( "slicePitch of mipmaps[%d][%d] is incorrect!", f, l );
            return false;
        }
        if( m.levelPitch != m.slicePitch * m.depth )
        {
            GN_ERROR(sLogger)( "levelPitch of mipmaps[%d][%d] is incorrect!", f, l );
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
class GN::gfx::ImageReader::Impl
{
    //!
    //! file format tag
    //!
    enum FileFormat
    {
        UNKNOWN = 0,
        BMP,
        DDS,
        JPEG,
        PNG,
        TGA,
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

    BMPReader mBMPReader;
    DDSReader mDDSReader;
    JPGReader mJPGReader;
    PNGReader mPNGReader;
    TGAReader mTGAReader;

    FileFormat mFileFormat;

    ReaderState mState;

public:

    //!
    //! default ctor
    //!
    Impl() : mFileFormat(UNKNOWN), mState(INVALID) {}

    //!
    //! reset image reader
    //!
    bool reset( File & i_file )
    {
        GN_GUARD;

        // reset internal states
        mFileFormat = UNKNOWN;
        mState  = INVALID;

        // determine file format
        #define CHECK_FORMAT( reader, format ) \
            if( reader.checkFormat( i_file ) ) mFileFormat = format; else
        CHECK_FORMAT( mBMPReader, BMP )
        CHECK_FORMAT( mDDSReader, DDS )
        CHECK_FORMAT( mJPGReader, JPEG )
        CHECK_FORMAT( mPNGReader, PNG )
        CHECK_FORMAT( mTGAReader, TGA )
        {
            GN_ERROR(sLogger)( "unknown image file format!" );
            return false;
        }
        #undef CHECK_FORMAT

        // read whole file
        size_t sz = i_file.size();
        mSrc.resize( sz );
        if( !i_file.seek( 0, FSEEK_SET ) ) return false;
        if( !i_file.read( &mSrc[0], mSrc.size(), &sz ) ) return false;
        GN_ASSERT( sz <= mSrc.size() );
        if( sz < mSrc.size() ) mSrc.resize( sz );

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
            GN_ERROR(sLogger)( "image reader is not ready for header reading!" );
            return false;
        }

        GN_ASSERT( !mSrc.empty() );

        #define READ_HEADER( reader ) \
            if( !reader.readHeader( o_desc, &mSrc[0], mSrc.size() ) ) \
            { mState = INVALID; return false; }

        switch( mFileFormat )
        {
            case BMP  : READ_HEADER( mBMPReader ); break;
            case DDS  : READ_HEADER( mDDSReader ); break;
            case JPEG : READ_HEADER( mJPGReader ); break;
            case PNG  : READ_HEADER( mPNGReader ); break;
            case TGA  : READ_HEADER( mTGAReader ); break;
            default   :
                GN_ERROR(sLogger)( "unknown or unsupport file format!" );
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
            GN_ERROR(sLogger)( "null output buffer!" );
            return false;
        }

        if( HEADER_READEN != mState )
        {
            GN_ERROR(sLogger)( "image reader is not ready for image reading!" );
            return false;
        }

        #define READ_IMAGE( reader ) \
            if( !reader.readImage( o_data ) ) \
            { mState = INVALID; return false; }

        switch( mFileFormat )
        {
            case BMP  : READ_IMAGE( mBMPReader ); break;
            case DDS  : READ_IMAGE( mDDSReader ); break;
            case JPEG : READ_IMAGE( mJPGReader ); break;
            case PNG  : READ_IMAGE( mPNGReader ); break;
            case TGA  : READ_IMAGE( mTGAReader ); break;
            default   :
                GN_ERROR(sLogger)( "unknown or unsupport file format!" );
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
GN::gfx::ImageReader::ImageReader() : mImpl( new Impl ) {}
GN::gfx::ImageReader::~ImageReader() { delete mImpl; }

//
// forward call to Impl
// -----------------------------------------------------------------------------
bool GN::gfx::ImageReader::reset( File & i_file )
{
    GN_GUARD;
    return mImpl->reset( i_file );
    GN_UNGUARD;
}
//
bool GN::gfx::ImageReader::readHeader( ImageDesc & o_desc )
{
    GN_GUARD;
    return mImpl->readHeader( o_desc );
    GN_UNGUARD;
}
//
bool GN::gfx::ImageReader::readImage( void * o_data )
{
    GN_GUARD;
    return mImpl->readImage( o_data );
    GN_UNGUARD;
}
