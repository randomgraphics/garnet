#ifndef __GN_BASE_CODEPAGE_H__
#define __GN_BASE_CODEPAGE_H__
// *****************************************************************************
/// \file
/// \brief   Code page converter
/// \author  chenlee (2006.5.1)
// *****************************************************************************

namespace GN {
///
/// Multi-Byte character encoding class (represents language specific encodings)
///
class GN_API CharacterEncodingConverter {
public:
    ///
    /// Define character encoding enumeration
    ///
    enum Encoding {
        //@{

        // ISO
        ASCII,      ///< ASCII code
        ISO_8859_1, ///< Western Europe

        // UNICODE
        UTF7,
        UTF8,
        UTF16_LE,
        UTF16_BE,
        UTF16, ///< UTF16_LE on little endian system; UTF16_BE on big endian system.
        UTF32_LE,
        UTF32_BE,
        UTF32,    ///< UTF32_LE on little endian system; UTF32_BE on big endian system.
        WIDECHAR, ///< UTF16, if sizeof(wchar_t)==2; or UTF32, if sizeof(wchar_t)==4

        // Chinese
        GBK,  ///< Chinese
        BIG5, ///< Taiwan Chinese

        NUM_ENCODINGS, ///< number of encodings.

        //@}
    };

public:
    ///
    /// Convert Encoding enum to string
    ///
    static const char * sEncoding2Str(Encoding);

    ///
    /// constructor
    ///
    CharacterEncodingConverter(Encoding from, Encoding to);

    ///
    /// destructor
    ///
    ~CharacterEncodingConverter();

    ///
    /// convert from source encoding to destination encoding
    ///
    /// \param destBuffer, destBufferSizeInBytes
    ///     Specify destination buffer and size. destBuffer could be NULL.
    ///     Note that destBuffer is null terminated, only if source buffer
    ///     is null terminated too.
    ///
    /// \param sourceBuffer, sourceBufferSizeInBytes
    ///     Specify source buffer and size
    ///
    /// \return
    ///     1) Return 0 for failure.
    ///     2) If destBuffer is NULL, return number of bytes required
    ///        to store convertion result. In this case, value of
    ///        destBufferSizeInBytes is ignored.
    ///     3) Return number of bytes filled into destination buffer,
    ///        including null terminator.
    ///
    size_t convert(void * destBuffer, size_t destBufferSizeInBytes, const void * sourceBuffer, size_t sourceBufferSizeInBytes);

    /// helper operators
    //@{

    size_t operator()(void * destBuffer, size_t destBufferSizeInBytes, const void * sourceBuffer, size_t sourceBufferSizeInBytes) {
        return convert(destBuffer, destBufferSizeInBytes, sourceBuffer, sourceBufferSizeInBytes);
    }

    template<typename DEST_TYPE, size_t DEST_SIZE>
    size_t operator()(DEST_TYPE (&destBuffer)[DEST_SIZE], const void * sourceBuffer, size_t sourceBufferSizeInBytes) {
        return convert(destBuffer, DEST_SIZE * sizeof(DEST_TYPE), sourceBuffer, sourceBufferSizeInBytes);
    }

    template<typename DEST_TYPE, size_t DEST_SIZE, typename SRC_TYPE, size_t SRC_SIZE>
    size_t operator()(DEST_TYPE (&destBuffer)[DEST_SIZE], SRC_TYPE (&sourceBuffer)[SRC_SIZE]) {
        return convert(destBuffer, DEST_SIZE * sizeof(DEST_TYPE), sourceBuffer, SRC_SIZE * sizeof(SRC_TYPE));
    }

    //@}

private:
    void * mImpl; ///< implementation instance
};

/// conversion between utf-16 and utf-8
//@{
GN_API size_t wcs2utf8(char * obuf, size_t ocount, const wchar_t * ibuf, size_t icount);
GN_API size_t utf82wcs(wchar_t * obuf, size_t ocount, const char * ibuf, size_t icount);
GN_API StrA   wcs2utf8(const wchar_t * ibuf, size_t icount);
GN_API StrW   utf82wcs(const char * ibuf, size_t icount);
//@}

///
/// convert wide char string to multi-byte string in current system encoding
///
GN_API void wcs2mbs(StrA &, const wchar_t *, size_t);

///
/// convert wide char string to multi-byte string in current system encoding
///
inline void wcs2mbs(StrA & o, const StrW & i) { return wcs2mbs(o, i.rawptr(), i.size()); }

///
/// convert wide char string to multi-byte string in current system encoding
///
inline StrA wcs2mbs(const wchar_t * i, size_t l) {
    StrA o;
    wcs2mbs(o, i, l);
    return o;
}

///
/// convert wide char string to multi-byte string in current system encoding
///
inline StrA wcs2mbs(const StrW & i) { return wcs2mbs(i.rawptr(), i.size()); }

///
/// convert multi-byte string in current system code page to wide char string.
///
/// Normally, it returns number of wide characters written into output buffer, including the null terminator.
/// If output wide-char buffer is NULL, it returns required size of wide-char buffer size in words, including the null terminator.
/// If anything goes wrong, it'll return 0.
///
/// Note that ouput string is always null-terminated.
///
/// \param obuf
///     Output buffer. If NULL, this function will return required size of output buffer in words.
/// \param ocount
///     Maximum of wide chars the ouput buffer can hold, including null terminator.
/// \param ibuf
///     Input buffer. If NULL, this function will return zero.
/// \param icount
///     character count in input buffer, not including null terminator. If zero,
///     then input buffer must be a null-terminated string.
///
GN_API size_t mbs2wcs(wchar_t * obuf, size_t ocount, const char * ibuf, size_t icount);

///
/// convert multi-byte string in current system code page to wide char string
///
GN_API void mbs2wcs(StrW &, const char *, size_t);

///
/// convert multi-byte string in current system code page to wide char string
///
inline void mbs2wcs(StrW & o, const StrA & i) { return mbs2wcs(o, i.rawptr(), i.size()); }

///
/// convert multi-byte string in current system code page to wide char string
///
inline StrW mbs2wcs(const char * i, size_t l) {
    StrW o;
    mbs2wcs(o, i, l);
    return o;
}

///
/// convert multi-byte string in current system code page to wide char string
///
inline StrW mbs2wcs(const StrA & i) { return mbs2wcs(i.rawptr(), i.size()); }
} // namespace GN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_CODEPAGE_H__
