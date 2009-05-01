#ifndef __GN_BASE_CODEPAGE_H__
#define __GN_BASE_CODEPAGE_H__
// *****************************************************************************
/// \file
/// \brief   Code page converter
/// \author  chenlee (2006.5.1)
// *****************************************************************************

namespace GN
{
    ///
    /// ANSI Code page enumeration
    ///
    struct CharacterEncoding
    {
        enum Enum
        {
            // ISO
            ISO_8859_1,     ///< Western Europe

            // Unicode
            UTF_7,          ///< 7-bit Unicode Transformation Format (used in internet e-mail messages)
            UTF_8,          ///< 8-bit UCS/Unicode Transformation Format)
            UTF16_LE,       ///< 16-bit Unicode Transformation Format (little endian)
            UTF16_BE,       ///< 16-bit Unicode Transformation Format (big endian)

            // Chinese
            GBK,            ///< Chinese
            BIG5,           ///< Taiwan Chinese
        };

        GN_DEFINE_ENUM_CLASS_HELPERS( CharacterEncoding, Enum );
    };

    ///
    /// Universal character encoding converter
    ///
    /// \param destBuffer, destBufferSizeInBytes
    ///         Specify destination buffer. If both are zero, then the function returns
    ///         Number of bytes required to store conversion result.
    ///
    /// \param destEncoding
    ///         Specify destination encoding type.
    ///
    /// \param sourceBuffer, sourceBufferInBytes
    ///         Specify source buffer.
    ///
    /// \param sourceEncoding
    ///         Specify source buffer encoding type.
    ///
    /// \return
    ///         Return number of bytes copied to destination buffer, or number of bytes required
    ///         to store destination buffer.
    ///         Return 0 on error.
    size_t
    ConvertCharacterEncoding(
        void            * destBuffer,
        size_t            destBufferSizeInBytes,
        CharacterEncoding destEncoding,
        const void      * sourceBuffer,
        size_t            sourceBufferSizeInBytes,
        CharacterEncoding sourceEncoding
        );

    ///
    /// Universal character encoding converter
    ///
    /// Simiary as the function above. But destination buffer is allocated by the function from heap,
    /// and must be freed by GN::heapFree() after use. Return NULL on error.
    ///
    void *
    ConvertCharacterEncoding(
        size_t           * destBufferSizeInBytes, // return destination buffer size in bytes (optional)
        CharacterEncoding  destEncoding,
        const void       * sourceBuffer,
        size_t             sourceBufferSizeInBytes,
        CharacterEncoding  sourceEncoding
        );

    ///
    /// convert wide char string to multi-byte string in current system codepage
    ///
    void wcs2mbs( StrA &, const wchar_t *, size_t );

    ///
    /// convert wide char string to multi-byte string in current system codepage
    ///
    inline void
    wcs2mbs( StrA & o, const StrW & i ) { return wcs2mbs( o, i.cptr(), i.size() ); }

    ///
    /// convert wide char string to multi-byte string in current system codepage
    ///
    inline StrA
    wcs2mbs( const wchar_t * i, size_t l ) { StrA o; wcs2mbs( o, i, l ); return o; }

    ///
    /// convert wide char string to multi-byte string in current system codepage
    ///
    inline StrA
    wcs2mbs( const StrW & i ) { return wcs2mbs( i.cptr(), i.size() ); }

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
    size_t mbs2wcs( wchar_t * obuf, size_t ocount, const char * ibuf, size_t icount );

    ///
    /// convert multi-byte string in current system code page to wide char string
    ///
    void mbs2wcs( StrW &, const char *, size_t );

    ///
    /// convert multi-byte string in current system code page to wide char string
    ///
    inline void
    mbs2wcs( StrW & o, const StrA & i ) { return mbs2wcs( o, i.cptr(), i.size() ); }

    ///
    /// convert multi-byte string in current system code page to wide char string
    ///
    inline StrW
    mbs2wcs( const char * i, size_t l ) { StrW o; mbs2wcs( o, i, l ); return o; }

    ///
    /// convert multi-byte string in current system code page to wide char string
    ///
    inline StrW
    mbs2wcs( const StrA & i ) { return mbs2wcs( i.cptr(), i.size() ); }
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_CODEPAGE_H__
