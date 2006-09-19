#ifndef __GN_BASE_CODEPAGE_H__
#define __GN_BASE_CODEPAGE_H__
// *****************************************************************************
//! \file    base/codepage.h
//! \brief   Code page converter
//! \author  chenlee (2006.5.1)
// *****************************************************************************

namespace GN
{
    //!
    //! convert wide char string to multi-byte string
    //!
    void wcs2mbs( StrA &, const wchar_t *, size_t );

    //!
    //! convert wide char string to multi-byte string
    //!
    inline void
    wcs2mbs( StrA & o, const StrW & i ) { return wcs2mbs( o, i.cptr(), i.size() ); }

    //!
    //! convert wide char string to multi-byte string
    //!
    inline StrA
    wcs2mbs( const wchar_t * i, size_t l ) { StrA o; wcs2mbs( o, i, l ); return o; }

    //!
    //! convert wide char string to multi-byte string
    //!
    inline StrA
    wcs2mbs( const StrW & i ) { return wcs2mbs( i.cptr(), i.size() ); }

    //!
    //! convert multi-byte string to wide char string.
    //!
    //! Normally, it returns number of wide characters written into output buffer, not including the null terminator.
    //! If output wide-char buffer is NULL, it returns required size of wide-char buffer size in words, not including the null terminator.
    //! If anything goes wrong, it'll return 0.
    //! Note that ouput string is always null-terminated.
    //!
    //! \param obuf
    //!     Output buffer. If NULL, this function will return required size of output buffer in words.
    //! \param ocount
    //!     Maximum of wide chars the ouput buffer can hold, including null terminator.
    //! \param ibuf
    //!     Input buffer. If NULL, this function will return zero.
    //! \param icount
    //!     character count in input buffer, not including null terminator. If zero,
    //!     then input buffer must be a null-terminated string.
    //!
    size_t mbs2wcs( wchar_t * obuf, size_t ocount, const char * ibuf, size_t icount );

    //!
    //! convert multi-byte string to wide char string
    //!
    void mbs2wcs( StrW &, const char *, size_t );

    //!
    //! convert multi-byte string to wide char string
    //!
    inline void
    mbs2wcs( StrW & o, const StrA & i ) { return mbs2wcs( o, i.cptr(), i.size() ); }

    //!
    //! convert multi-byte string to wide char string
    //!
    inline StrW
    mbs2wcs( const char * i, size_t l ) { StrW o; mbs2wcs( o, i, l ); return o; }

    //!
    //! convert multi-byte string to wide char string
    //!
    inline StrW
    mbs2wcs( const StrA & i ) { return mbs2wcs( i.cptr(), i.size() ); }
}

// *****************************************************************************
//                           End of codepage.h
// *****************************************************************************
#endif // __GN_BASE_CODEPAGE_H__
