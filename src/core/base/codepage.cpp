#include "pch.h"
#include "codepageICONV.h"
#include "codepageMSWIN.h"
#include "codepageXenon.h"
#include <stdlib.h>
#include <cstdlib>

static GN::Logger * sLogger = GN::getLogger("GN.base.codepage");

using namespace GN;

// *****************************************************************************
// class CharacterEncodingConverter implementation class
// *****************************************************************************

#if GN_WINPC

typedef GN::CECImplMSWIN CECImpl;

#elif HAS_ICONV

typedef GN::CECImplICONV CECImpl;

#else

    #error "ICONV not found. Make sure you have iconv library installed."

#endif

// *****************************************************************************
// class CharacterEncodingConverter
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN_API const char * GN::CharacterEncodingConverter::sEncoding2Str(Encoding e) {
    switch (e) {
    case ASCII:
        return "ascii";
    case ISO_8859_1:
        return "iso-8859-1";
    case UTF7:
        return "utf-7";
    case UTF8:
        return "utf-8";
    case UTF16_LE:
        return "utf-16-le";
    case UTF16_BE:
        return "utf-16-be";
    case UTF16:
        return "utf-16";
    case UTF32_LE:
        return "utf-32-le";
    case UTF32_BE:
        return "utf-32-be";
    case UTF32:
        return "utf-32";
    case WIDECHAR:
        return 2 == sizeof(wchar_t) ? "utf-16" : "utf-32";
    case GBK:
        return "gbk";
    case BIG5:
        return "big5";
    default:
        return "=[unknown]=";
    }
}

//
//
// -----------------------------------------------------------------------------
GN_API GN::CharacterEncodingConverter::CharacterEncodingConverter(Encoding from, Encoding to): mImpl(new CECImpl) {
    CECImpl * p = (CECImpl *) mImpl;

    if (!p->init(from, to)) {
        delete p;
        mImpl = NULL;
    }
}

//
//
// -----------------------------------------------------------------------------
GN_API GN::CharacterEncodingConverter::~CharacterEncodingConverter() { delete (CECImpl *) mImpl; }

//
//
// -----------------------------------------------------------------------------
GN_API size_t GN::CharacterEncodingConverter::convert(void * destBuffer, size_t destBufferSizeInBytes, const void * sourceBuffer,
                                                      size_t sourceBufferSizeInBytes) {
    if (!mImpl) {
        GN_ERROR(sLogger)("CharacterEncodingConverter is not correctly initialized.");
        return 0;
    }

    CECImpl * p = (CECImpl *) mImpl;

    return p->convert((wchar_t *) destBuffer, destBufferSizeInBytes, (const char *) sourceBuffer, sourceBufferSizeInBytes);
}

// *****************************************************************************
// public utilities
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN_API GN::StrA GN::wcs2utf8(const wchar_t * ibuf, size_t icount) {
    static auto cec      = CharacterEncodingConverter(CharacterEncodingConverter::WIDECHAR, CharacterEncodingConverter::UTF8);
    auto        inBytes  = icount * sizeof(wchar_t);
    auto        outBytes = cec.convert(nullptr, 0, ibuf, inBytes);
    if (0 == outBytes) return {};
    DynaArray<char> buffer(outBytes + 1);
    cec.convert(buffer.data(), buffer.size(), ibuf, inBytes);
    buffer[outBytes] = 0; // ensure null termination.
    return buffer.data();
}

//
//
// -----------------------------------------------------------------------------
GN_API GN::StrW GN::utf82wcs(const char * ibuf, size_t icount) {
    static auto cec      = CharacterEncodingConverter(CharacterEncodingConverter::UTF8, CharacterEncodingConverter::WIDECHAR);
    auto        outBytes = cec.convert(nullptr, 0, ibuf, icount);
    if (outBytes < sizeof(wchar_t)) return {};
    DynaArray<wchar_t> buffer(outBytes / sizeof(wchar_t) + 1);
    auto               outSize = buffer.size();
    cec.convert(buffer.data(), outSize * sizeof(wchar_t), ibuf, icount);
    buffer[outSize - 1] = 0; // ensure null termination
    return (wchar_t *) buffer.data();
}

//
//
// -----------------------------------------------------------------------------
GN_API GN::StrA GN::wcs2mbs(const wchar_t * i, size_t l) {
    GN::StrA o;
    if (0 == i || 0 == l) return o;

    o.resize(l);
#if GN_MSVC
    if (::wcstombs_s(&l, o.data(), l + 1, i, l)) {
        o.clear();
        return o;
    }
    --l; // For MVCS (at least up to VS2022), l includes the null terminator.
#else
    l = std::wcstombs(o.data(), i, l);
    if (static_cast<std::size_t>(-1) == l) {
        o.clear();
        return o;
    }
#endif
    o.resize(l);
    o[l] = 0;
    return o;
}

//
//
// -----------------------------------------------------------------------------
GN_API GN::StrW GN::mbs2wcs(const char * i, size_t l) {
    GN::StrW o;

    if (0 == i || 0 == l) return o;

    // N multi-bytes characters converts to at most N wide characters. So we resize the output string to N first,
    // to ensure there's enough space for conversion.
    o.resize(l);

#if GN_MSVC
    if (::mbstowcs_s(&l, o.data(), l + 1, i, l)) {
        // the function returns non-zero value, indicating conversion failed. Clear the output buffer and bail out.
        o.clear();
        return o;
    }
    // MSVC and the rest of the world, don't agree with the meaning of l.
    // For MSVC, l includes the null terminator, while C++ standard says otherwise.
    --l;
#else
    l = std::mbstowcs(o.data(), i, l);
    if (static_cast<std::size_t>(-1) == l) {
        o.clear();
        return o;
    }
#endif

    // done
    o.resize(l);
    o[l] = 0;
    return o;
}
