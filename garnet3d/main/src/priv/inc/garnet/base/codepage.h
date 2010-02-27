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
    /// Multi-Byte character encoding class (represents language specific encodings)
    ///
    class CharacterEncodingConverter
    {
    public:

        ///
        /// Define character encoding enumeration
        ///
        enum Encoding
        {
            //@{

            // ISO
            ASCII,          ///< ASCII code
            ISO_8859_1,     ///< Western Europe

            // UNICODE
            UTF7,
            UTF8,
            UTF16_LE,
            UTF16_BE,
            UTF16,          ///< UTF16_LE on little endian system; UTF16_BE on big endian system.
            UTF32_LE,
            UTF32_BE,       ///< UTF32_LE on little endian system; UTF32_BE on big endian system.
            UTF32,
            WIDECHAR,       ///< UTF16, if sizeof(wchar_t)==2; or UTF32, if sizeof(wchar_t)==4

            // Chinese
            GBK,            ///< Chinese
            BIG5,           ///< Taiwan Chinese

            NUM_ENCODINGS,  ///< number of encodings.

            //@}
        };

    public:

        ///
        /// constructor
        ///
        CharacterEncodingConverter( Encoding from, Encoding to );

        ///
        /// destructor
        ///
        ~CharacterEncodingConverter();

        ///
        /// Convert from source encoding to destination encoding
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
        size_t
        Convert(
            void       * destBuffer,
            size_t       destBufferSizeInBytes,
            const void * sourceBuffer,
            size_t       sourceBufferSizeInBytes );

        /// helper operators
        //@{

        size_t
        operator()(
            void       * destBuffer,
            size_t       destBufferSizeInBytes,
            const void * sourceBuffer,
            size_t       sourceBufferSizeInBytes )
        {
            return Convert(
                destBuffer,
                destBufferSizeInBytes,
                sourceBuffer,
                sourceBufferSizeInBytes );
        }

        template<typename DEST_TYPE, size_t DEST_SIZE>
        size_t
        operator()(
            DEST_TYPE  (& destBuffer)[DEST_SIZE],
            const void  * sourceBuffer,
            size_t        sourceBufferSizeInBytes )
        {
            return Convert(
                destBuffer,
                DEST_SIZE * sizeof(DEST_TYPE),
                sourceBuffer,
                sourceBufferSizeInBytes );
        }

        template<
            typename DEST_TYPE, size_t DEST_SIZE,
            typename SRC_TYPE, size_t SRC_SIZE>
        size_t
        operator()(
            DEST_TYPE (& destBuffer)[DEST_SIZE],
            SRC_TYPE  (& sourceBuffer)[SRC_SIZE] )
        {
            return Convert(
                destBuffer,
                DEST_SIZE * sizeof(DEST_TYPE),
                sourceBuffer,
                SRC_SIZE * sizeof(SRC_TYPE) );
        }

        //@}

    private:

        void * mImpl; ///< implementation instance
    };

    ///
    /// get current system encoding
    ///
    CharacterEncodingConverter::Encoding GetCurrentSystemEncoding();

    ///
    /// convert wide char string to multi-byte string in current system encoding
    ///
    void Wcs2Mbs( StrA &, const wchar_t *, size_t );

    ///
    /// convert wide char string to multi-byte string in current system encoding
    ///
    inline void
    Wcs2Mbs( StrA & o, const StrW & i ) { return Wcs2Mbs( o, i.ToRawPtr(), i.Size() ); }

    ///
    /// convert wide char string to multi-byte string in current system encoding
    ///
    inline StrA
    Wcs2Mbs( const wchar_t * i, size_t l ) { StrA o; Wcs2Mbs( o, i, l ); return o; }

    ///
    /// convert wide char string to multi-byte string in current system encoding
    ///
    inline StrA
    Wcs2Mbs( const StrW & i ) { return Wcs2Mbs( i.ToRawPtr(), i.Size() ); }

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
    size_t Mbs2Wcs( wchar_t * obuf, size_t ocount, const char * ibuf, size_t icount );

    ///
    /// convert multi-byte string in current system code page to wide char string
    ///
    void Mbs2Wcs( StrW &, const char *, size_t );

    ///
    /// convert multi-byte string in current system code page to wide char string
    ///
    inline void
    Mbs2Wcs( StrW & o, const StrA & i ) { return Mbs2Wcs( o, i.ToRawPtr(), i.Size() ); }

    ///
    /// convert multi-byte string in current system code page to wide char string
    ///
    inline StrW
    Mbs2Wcs( const char * i, size_t l ) { StrW o; Mbs2Wcs( o, i, l ); return o; }

    ///
    /// convert multi-byte string in current system code page to wide char string
    ///
    inline StrW
    Mbs2Wcs( const StrA & i ) { return Mbs2Wcs( i.ToRawPtr(), i.Size() ); }
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_CODEPAGE_H__
