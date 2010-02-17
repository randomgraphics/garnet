#include "pch.h"
#include "codepageICONV.h"
#include <stdlib.h>

#if HAS_ICONV

static GN::Logger * sLogger = GN::getLogger("GN.base.codepage");

using namespace GN;

// *****************************************************************************
// Local functions
// *****************************************************************************

static const char * sEncodingToLocal( CharacterEncodingConverter::Encoding e )
{
    static const char * TABLE[] =
    {
        "ASCII",        // ASCII
        "ISO-8859-1",   // ISO_8859_1
        "UTF-7",        // UTF7
        "UTF-8",        // UTF8
        "UTF-16LE",     // UTF16_LE
        "UTF-16BE",     // UTF16_BE

#if GN_LITTLE_ENDIAN
        "UTF-16LE",     // UTF16
#else
        "UTF-16BE",     // UTF16
#endif

        "UTF-32LE",    // UTF32_LE
        "UTF-32BE",    // UTF32_BE

#if GN_LITTLE_ENDIAN
        "UTF-32LE",     // UTF32
#else
        "UTF-32BE",     // UTF32
#endif

#if GN_LITTLE_ENDIAN
        (2 == sizeof(wchar_t)) // WIDECHAR
        ? "UTF-16LE"
        : "UTF-32LE",
#else
        (2 == sizeof(wchar_t)) // WIDECHAR
        ? "UTF-16BE"
        : "UTF-32BE",
#endif

        "GBK",         // GBK
        "BIG5",        // BIG5
    };
    GN_CASSERT( GN_ARRAY_COUNT(TABLE) == CharacterEncodingConverter::NUM_ENCODINGS );

    if( 0 <= e && e < (int)GN_ARRAY_COUNT(TABLE) )
    {
        return TABLE[e];
    }
    else
    {
        GN_ERROR(sLogger)( "Invalid character encoding: %d", e );
        return NULL;
    }
}

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::CECImplICONV::init(
    CharacterEncodingConverter::Encoding from,
    CharacterEncodingConverter::Encoding to )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::CECImplICONV, () );

    const char * fromstr = sEncodingToLocal( from );
    const char * tostr   = sEncodingToLocal( to );
    if( NULL == fromstr || NULL == tostr  ) return failure();

    mIconv = iconv_open( tostr, fromstr );
    if( (iconv_t)-1 == mIconv )
    {
        int err = errno;

        switch( err )
        {
            case EINVAL:
                GN_ERROR(sLogger)( "iconv_open() failed: the conversion from %s to %s is not supported by the implementation",
                    fromstr, tostr );
                break;

            default:
                GN_ERROR(sLogger)( "iconv_open() failed: errno=%d (%s).", err, Errno2Str(err) );
                break;
        };

        return failure();
    }

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::CECImplICONV::quit()
{
    GN_GUARD;

    if( mIconv ) iconv_close( mIconv ), mIconv = 0;

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
size_t
GN::CECImplICONV::convert(
    void       * destBuffer,
    size_t       destBufferSizeInBytes,
    const void * sourceBuffer,
    size_t       sourceBufferSizeInBytes )
{
    const char * inbuf  = (const char *)sourceBuffer;
    char       * outbuf = (char*)destBuffer;

    // Note: iconv() on different platform uses different signature.
    // To avoid conditional compiliation, convert system provided
    // iconv routine to unified signature.
    typedef size_t (*MyICONVFuncType)(
        iconv_t cd,
        const char ** inbuf,
        size_t *inbytesleft,
        char* * outbuf,
        size_t *outbytesleft );
    MyICONVFuncType myiconv = (MyICONVFuncType)&iconv;

    size_t converted = myiconv(
        mIconv,
        &inbuf,
        &sourceBufferSizeInBytes,
        &outbuf,
        &destBufferSizeInBytes );

    if( (size_t)-1 == converted )
    {
        const char * reason;

        int err = errno;

        switch( err )
        {
            case E2BIG :
                reason = "there is not sufficient room at destination buffer";
                break;
            case EILSEQ :
                reason = "an invalid multibyte sequence has been encountered in the input";
                break;
            case EINVAL :
                reason = "an incomplete multibyte sequence has been encountered in the input";
                break;

            default :
                reason = Errno2Str( err );
                break;
        }

        GN_ERROR(sLogger)( "iconv() failed : %s", reason );

        return 0;
    }

    return outbuf - (char*)destBuffer;
}

#endif
