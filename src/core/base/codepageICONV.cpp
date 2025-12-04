#include "pch.h"
#include "codepageICONV.h"
#include <stdlib.h>

#if HAS_ICONV

static GN::Logger * sLogger = GN::getLogger("GN.base.codepage");

using namespace GN;

// *****************************************************************************
// Local functions
// *****************************************************************************

static const char * sEncodingToLocale(CharacterEncodingConverter::Encoding e) {
    static const char * TABLE[] = {
        "ASCII",      // ASCII
        "ISO-8859-1", // ISO_8859_1
        "UTF-7",      // UTF7
        "UTF-8",      // UTF8
        "UTF-16LE",   // UTF16_LE
        "UTF-16BE",   // UTF16_BE

    #if GN_LITTLE_ENDIAN
        "UTF-16LE", // UTF16
    #else
        "UTF-16BE",            // UTF16
    #endif

        "UTF-32LE", // UTF32_LE
        "UTF-32BE", // UTF32_BE

    #if GN_LITTLE_ENDIAN
        "UTF-32LE", // UTF32
    #else
        "UTF-32BE",            // UTF32
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

        "GBK",  // GBK
        "BIG5", // BIG5
    };
    GN_CASSERT(GN_ARRAY_COUNT(TABLE) == CharacterEncodingConverter::NUM_ENCODINGS);

    if (0 <= e && e < (int) GN_ARRAY_COUNT(TABLE)) {
        return TABLE[e];
    } else {
        GN_ERROR(sLogger)("Invalid character encoding: {}", (int) e);
        return NULL;
    }
}

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::CECImplICONV::init(CharacterEncodingConverter::Encoding from, CharacterEncodingConverter::Encoding to) {
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT();

    const char * fromstr = sEncodingToLocale(from);
    const char * tostr   = sEncodingToLocale(to);
    if (NULL == fromstr || NULL == tostr) return failure();

    mIconv = iconv_open(tostr, fromstr);
    if ((iconv_t) -1 == mIconv) {
        int err = errno;

        switch (err) {
        case EINVAL:
            GN_ERROR(sLogger)("iconv_open() failed: the conversion from {} to {} is not supported by the implementation", fromstr, tostr);
            break;

        default:
            GN_ERROR(sLogger)("iconv_open() failed: errno={} ({}).", err, errno2str(err));
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
void GN::CECImplICONV::quit() {
    GN_GUARD;

    if (mIconv) iconv_close(mIconv), mIconv = 0;

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// Note: iconv() on different platform uses different signature.
// To avoid conditional compiliation, convert system provided
// iconv routine to unified signature.
typedef size_t (*MyICONVFuncType)(iconv_t cd, const char ** inbuf, size_t * inbytesleft, char ** outbuf, size_t * outbytesleft);
static MyICONVFuncType myiconv = (MyICONVFuncType) &iconv;

static size_t calcualteRequiredOutputBufferSize(iconv_t cd, const void * sourceBuffer, size_t sourceBufferSizeInBytes) {
    if (!sourceBuffer || 0 == sourceBufferSizeInBytes) { return 0; }

    std::vector<char> storage;
    #if GN_BUILD_DEBUG_ENABLED
    // In debug build, start from 1 character to excercise the loop code.
    storage.reserve(sourceBufferSizeInBytes * 4); // minimize memory rellocation
    storage.resize(1);
    #else
    // In release build, start from 4 times of input buffer size to minimize the chance of looping.
    storage.resize(sourceBufferSizeInBytes * 4);
    #endif

    for (;;) {
        auto   inBuffer     = (const char *) sourceBuffer;
        auto   inBytesLeft  = sourceBufferSizeInBytes;
        auto   outBuffer    = storage.data();
        auto   outBytesLeft = storage.size();
        size_t result       = myiconv(cd, &inBuffer, &inBytesLeft, &outBuffer, &outBytesLeft);
        if (result == (size_t) -1) {
            if (errno == E2BIG) {
                // Reallocate a larger buffer and try again
                storage.resize(storage.size() * 2);
            } else {
                // Handle other errors
                GN_ERROR(sLogger)("iconv error. error code = {}", errno);
                return 0;
            }
        } else {
            // we are done. now caluclate output buffer size;
            GN_ASSERT(storage.size() >= outBytesLeft);
            auto converted = storage.size() - outBytesLeft;
            GN_ASSERT(outBuffer >= storage.data());
            GN_ASSERT((size_t) (outBuffer - storage.data()) == converted);
            return converted;
        }
    }
}

// *****************************************************************************
// public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
size_t GN::CECImplICONV::convert(void * destBuffer, size_t destBufferSizeInBytes, const void * sourceBuffer, size_t sourceBufferSizeInBytes) {
    // handle size query mode
    if (!destBuffer) { return calcualteRequiredOutputBufferSize(mIconv, sourceBuffer, sourceBufferSizeInBytes); }

    const char * inbuf  = (const char *) sourceBuffer;
    char *       outbuf = (char *) destBuffer;

    size_t converted = myiconv(mIconv, &inbuf, &sourceBufferSizeInBytes, &outbuf, &destBufferSizeInBytes);

    if ((size_t) -1 == converted) {
        const char * reason;
        int          err = errno;
        switch (err) {
        case E2BIG:
            // this is not an error in our case.
            return outbuf - (char *) destBuffer;
        case EILSEQ:
            reason = "an invalid multibyte sequence has been encountered in the input";
            break;
        case EINVAL:
            reason = "an incomplete multibyte sequence has been encountered in the input";
            break;
        default:
            reason = errno2str(err);
            break;
        }
        GN_ERROR(sLogger)("iconv() failed : {}", reason);
        return 0;
    }

    return outbuf - (char *) destBuffer;
}

#endif
