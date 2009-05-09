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
    /// Unicode encoding enumerations
    ///
    struct UnicodeEncoding
    {
        enum Enum
        {
            UTF7,
            UTF8,
            UTF16_LE,
            UTF16_BE,
        };

        GN_DEFINE_ENUM_CLASS_HELPERS( UnicodeEncoding, Enum );
    };

    ///
    /// Multi-Byte character encoding class (represents language specific encodings)
    ///
    class MultiByteCharacterEncoding
    {
    public:

        enum Enum
        {
            // ISO
            ISO_8859_1,     ///< Western Europe

            // Chinese
            GBK,            ///< Chinese
            BIG5,           ///< Taiwan Chinese
        };

    public:

        ///
        /// constructor
        ///
        MultiByteCharacterEncoding( Enum e );

        ///
        /// destructor
        ///
        ~MultiByteCharacterEncoding();

        ///
        /// convert from multibyte encoding to unicode encoding
        ///
        /// \param destEncoding
        ///     Specify destination encoding. Must be one of UnicodeEncoding enumration.
        ///
        /// \param destBuffer, destBufferSizeInBytes
        ///     Specify destination buffer and size. destBuffer could be NULL.
        ///
        /// \param sourceBuffer, sourceBufferSizeInBytes
        ///     Specify source buffer and size
        ///
        /// \return
        ///     1) Return 0 for failure.
        ///     2) If destBuffer is NULL, return number of bytes required
        ///        to store convertion result. In this case, value of
        ///        destBufferSizeInBytes is ignored.
        ///     3) Return number of bytes filled into destination buffer.
        ///
        size_t
        toUnicode(
            UnicodeEncoding   destEncoding,
            void            * destBuffer,
            size_t            destBufferSizeInBytes,
            const void      * sourceBuffer,
            size_t            sourceBufferSizeInBytes );

        ///
        /// convert from unicode encoding to multibyte encoding
        ///
        /// \param destBuffer, destBufferSizeInBytes
        ///     Specify destination buffer and size. destBuffer could be NULL.
        ///
        /// \param sourceEncoding
        ///     Specify destination encoding. Must be one of UnicodeEncoding enumration.
        ///
        /// \param sourceBuffer, sourceBufferSizeInBytes
        ///     Specify source buffer and size
        ///
        /// \return
        ///     1) Return 0 for failure.
        ///     2) If destBuffer is NULL, return number of bytes required
        ///        to store convertion result. In this case, value of
        ///        destBufferSizeInBytes is ignored.
        ///     3) Return number of bytes filled into destination buffer.
        ///
        size_t
        toUnicode(
            void            * destBuffer,
            size_t            destBufferSizeInBytes,
            UnicodeEncoding   sourceEncoding,
            const void      * sourceBuffer,
            size_t            sourceBufferSizeInBytes );

    private:
        void * mImpl; ///< implementation instance
    };

    ///
    /// get current system encoding
    ///
    MultiByteCharacterEncoding::Enum getCurrentSystemEncoding();

    ///
    /// convert wide char string to multi-byte string in current system encoding
    ///
    void wcs2mbs( StrA &, const wchar_t *, size_t );

    ///
    /// convert wide char string to multi-byte string in current system encoding
    ///
    inline void
    wcs2mbs( StrA & o, const StrW & i ) { return wcs2mbs( o, i.cptr(), i.size() ); }

    ///
    /// convert wide char string to multi-byte string in current system encoding
    ///
    inline StrA
    wcs2mbs( const wchar_t * i, size_t l ) { StrA o; wcs2mbs( o, i, l ); return o; }

    ///
    /// convert wide char string to multi-byte string in current system encoding
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
