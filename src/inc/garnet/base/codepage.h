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
        WIDECHAR, ///< Platform dependent wide character mode. UTF16, if sizeof(wchar_t)==2; or UTF32, if sizeof(wchar_t)==4

        // Chinese
        GBK,  ///< Chinese
        BIG5, ///< Taiwan Chinese

        NUM_ENCODINGS, ///< number of encodings.

        //@}
    };

public:
    ///
    /// constructor
    ///
    CharacterEncodingConverter(Encoding from, Encoding to);

    ///
    /// destructor
    ///
    ~CharacterEncodingConverter();

    ///
    /// Convert from source encoding to destination encoding. Note that this method will _NOT_ treatment null
    /// terminator any differently. It means:
    ///     - If the source buffer is not null terminated, then the destination buffer is not too.
    ///     - If the source buffer contains null terminator in the middle, the conversion will not stop at where
    ///       the terminator is.
    ///
    /// \param destBuffer, destBufferSizeInBytes
    ///     Specify destination buffer and size. Set destBuffer to null to activate size query mode.
    ///     In this mode, the function will return number of bytes need to hold the conversion output, or 0, if error
    ///     is encountered. In this mode, the destBufferSizeInBytes parameter is ignored.
    ///
    ///     If the destination buffer is not large enough, the function will fill it as much as possible,
    ///     return the converted bytes in return value.
    ///
    /// \param sourceBuffer, sourceBufferSizeInBytes
    ///     Specify source buffer and size. Note that the conversion will _NOT_ automatically stop at null terminators.
    ///
    /// \return
    ///     1) Return 0 for failure. Check log for error details.
    ///     2) If destBuffer is NULL, return number of bytes required
    ///        to store conversion result. In this case, value of
    ///        destBufferSizeInBytes is ignored.
    ///     3) Return number of bytes filled into destination buffer.
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

    ///
    /// Convert Encoding enum to string
    ///
    static const char * sEncoding2Str(Encoding);

    /// helpers to convert between char and wchar_t
    //@{
    static size_t ascii2wcs(wchar_t * os, size_t on, const char * i, size_t in) {
        static CharacterEncodingConverter cec(CharacterEncodingConverter::ASCII, CharacterEncodingConverter::WIDECHAR);
        return cec.convert(os, on * sizeof(wchar_t), i, in);
    }
    static size_t wcs2ascii(char * os, size_t on, const wchar_t * i, size_t in) {
        static CharacterEncodingConverter cec(CharacterEncodingConverter::WIDECHAR, CharacterEncodingConverter::ASCII);
        return cec.convert(os, on, i, in * sizeof(wchar_t));
    }
    //@}

private:
    void * mImpl; ///< implementation instance
};

/// Helper functions to convert between widechar and utf-8 string. Differ from the raw CharacterEncodingConverter,
/// these helper functions will respect null terminators in the source buffer. It'll also always return a null
/// terminated string, regardless if source buffer is null terminated or not.
//@{
GN_API GN::StrA wcs2utf8(const wchar_t * ibuf, size_t icount);
GN_API GN::StrW utf82wcs(const char * ibuf, size_t icount);
GN_API GN::StrA wcs2mbs(const wchar_t *, size_t);
inline GN::StrA wcs2mbs(const StrW & i) { return wcs2mbs(i.data(), i.size()); }
GN_API GN::StrW mbs2wcs(const char *, size_t);
inline GN::StrW mbs2wcs(const StrA & i) { return mbs2wcs(i.data(), i.size()); }
//@}

} // namespace GN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_CODEPAGE_H__
