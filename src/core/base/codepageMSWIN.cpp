#include "pch.h"

#if GN_WINPC
    #include "codepageMSWIN.h"
    #include <stdlib.h>
    #include <windows.h>

static GN::Logger * sLogger = GN::getLogger("GN.base.codepage");

using namespace GN;

// *****************************************************************************
// Local functions
// *****************************************************************************

static int sEncodingToCodePage(CharacterEncodingConverter::Encoding e) {
    static INT TABLE[] = {
        437,   // ASCII
        1252,  // ISO_8859_1
        65000, // UTF7
        65001, // UTF8
        0,     // UTF16_LE
        1201,  // UTF16_BE
        0,     // UTF16
        12000, // UTF32_LE
        12001, // UTF32_BE
        12000, // UTF32
        0,     // WIDECHAR
        936,   // GBK
        950,   // BIG5
    };
    GN_CASSERT(GN_ARRAY_COUNT(TABLE) == CharacterEncodingConverter::NUM_ENCODINGS);

    if (0 <= e && e < GN_ARRAY_COUNT(TABLE)) {
        return TABLE[e];
    } else {
        GN_ERROR(sLogger)("Invalid character encoding: %d", e);
        return -1;
    }
}

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::CECImplMSWIN::init(CharacterEncodingConverter::Encoding from, CharacterEncodingConverter::Encoding to) {
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT();

    mCodePageFrom = sEncodingToCodePage(from);
    mCodePageTo   = sEncodingToCodePage(to);
    if (-1 == mCodePageFrom || -1 == mCodePageTo) {
        GN_ERROR(sLogger)("Invalid or unsupported encoding.");
        return failure();
    }

    // success
    mEncodingFrom = from;
    mEncodingTo   = to;
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::CECImplMSWIN::quit() {
    GN_GUARD;

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
size_t GN::CECImplMSWIN::convert(void * destBuffer, size_t destBufferSizeInBytes, const void * sourceBuffer, size_t sourceBufferSizeInBytes) {
    size_t converted = 0;

    // convert from source encoding to widechar encoding
    DynaArray<wchar_t> tempBuffer;
    if (0 != mCodePageFrom) {
        tempBuffer.resize(sourceBufferSizeInBytes);

        converted = (size_t)::MultiByteToWideChar(mCodePageFrom, 0, (const char *) sourceBuffer, (int) sourceBufferSizeInBytes, tempBuffer.rawptr(),
                                                  (int) (tempBuffer.size() * sizeof(wchar_t)));
        if (0 == converted) {
            GN_ERROR(sLogger)("fail to convert input buffer to UNICODE.");
            return 0;
        }

        sourceBuffer            = tempBuffer.rawptr();
        sourceBufferSizeInBytes = converted * sizeof(wchar_t);
    }

    // convert from widechar encoding to target encoding
    if (0 != mCodePageTo) {
        converted = (size_t)::WideCharToMultiByte(mCodePageTo, 0, (const wchar_t *) sourceBuffer, (int) (sourceBufferSizeInBytes / sizeof(wchar_t)),
                                                  (char *) destBuffer, (int) destBufferSizeInBytes, NULL, NULL);
        if (0 == converted) { GN_ERROR(sLogger)("fail to convert from UNICODE to target encoding."); }
        return converted;
    } else if (mEncodingTo == CharacterEncodingConverter::UTF16 || mEncodingTo == CharacterEncodingConverter::UTF16_LE ||
               mEncodingTo == CharacterEncodingConverter::WIDECHAR) {
        // Target encoding is UNICODE too. A memory copy is enough.

        if (0 == destBuffer) {
            // return number of bytes required to store conversion result
            return sourceBufferSizeInBytes;
        }

        if (0 == sourceBuffer) {
            GN_ERROR(sLogger)("NULL source buffer pointer!");
            return 0;
        }

        if (sourceBufferSizeInBytes > destBufferSizeInBytes) {
            GN_ERROR(sLogger)("There's no enough space in destination buffer.");
            return 0;
        }

        memcpy(destBuffer, sourceBuffer, sourceBufferSizeInBytes);

        return sourceBufferSizeInBytes;
    } else {
        GN_ERROR(sLogger)("Conversion to encoding \"%d\" is not supported yet.", mEncodingTo);
        return 0;
    }
}

#endif
