#ifndef __GN_BASE_STRING_H__
#define __GN_BASE_STRING_H__
// *****************************************************************************
//! \file    string.h
//! \brief   custom string implementation
//! \author  chenlee (2005.4.17)
// *****************************************************************************

#include <stdarg.h>
#include <string>
#include <boost/format.hpp>

namespace GN
{
    //!
    //! get string length
    //!
    template<typename CHAR>
    inline size_t strLen( const CHAR * s )
    {
        if ( 0 == s ) return 0;
        size_t l = 0;
        while( 0 != *s )
        {
            ++l;
            ++s;
        }
        return l;
    }

    //!
    //! string comparation (case sensitive)
    //!
    template<typename CHAR>
    inline int strCmp( const CHAR * s1, const CHAR * s2 )
    {
        if ( s1 == s2 ) return 0;
        if ( 0 == s1 ) return -1;
        if ( 0 == s2 ) return 1;
        while( *s1 && *s2 )
        {
            if ( *s1 < *s2 ) return -1;
            if ( *s1 > *s2 ) return 1;
            ++s1;
            ++s2;
        }
        if ( 0 != *s1 ) return 1;
        if ( 0 != *s2 ) return -1;
        return 0;
    }

    //!
    //! string comparation (case insensitive)
    //!
    template<typename CHAR>
    inline int strCmpI( const CHAR * s1, const CHAR * s2 )
    {
        if ( s1 == s2 ) return 0;
        if ( 0 == s1 ) return -1;
        if ( 0 == s2 ) return 1;
        int a, b;
        while( *s1 && *s2 )
        {
            a = (int)*s1;
            b = (int)*s2;
            if ( 'a' <= a && a <= 'z' ) a += 'A'-'a'; 
            if ( 'a' <= b && b <= 'z' ) b += 'A'-'a'; 
            if ( a < b ) return -1;
            if ( a > b ) return 1;
            ++s1;
            ++s2;
        }
        if ( 0 != *s1 ) return 1;
        if ( 0 != *s2 ) return -1;
        return 0;
    }

    //!
    //! check for empty string, including NULL.
    //!
    template<typename CHAR>
    inline bool strEmpty( const CHAR * s )
    {
        return 0 == s || 0 == s[0];
    }

    //!
    //! printf-like format string
    //!
    void
    strPrintf(
        char *       buf,
        size_t       bufSizeInChar,
        const char * fmt,
        va_list      args );

    //!
    //! printf-like format string (wide-char)
    //!
    void
    strPrintf(
        wchar_t *       buf,
        size_t          bufSizeInWchar,
        const wchar_t * fmt,
        va_list         args );

    //!
    //! Custom string class
    //!
    template< typename CHAR>
    class Str
    {
    public:

        //!
        //! default constructor
        //!
        Str() : mLen(0), mCaps(1)
        {
            mPtr = alloc(2);
            mPtr[0] = 0;
        }

        //!
        //! copy constructor
        //!
        Str( const Str & s ) : mLen(s.mLen), mCaps(calcCaps(s.mLen))
        {
            mPtr = alloc(mCaps+1);
            ::memcpy( mPtr, s.mPtr, (mLen+1)*sizeof(CHAR) );
        }

        //!
        //! copy constructor from c-style string
        //!
        Str( const CHAR * s, size_t l = 0 )
        {
            if ( 0 == s )
            {
                mCaps = 1;
                mLen = 0;
                mPtr = alloc(2);
                mPtr[0] = 0;
            }
            else
            {
                size_t ll = strLen<CHAR>(s);
                if ( 0 == l ) l = ll;
                else if ( l > ll ) l = ll;
                mCaps = calcCaps(l);
                mPtr = alloc(mCaps+1);
                mLen = l;
                ::memcpy( mPtr, s, l*sizeof(CHAR) );
                mPtr[l] = 0;
            }
        }

        //!
        //! destructor
        //!
        ~Str()
        {
            dealloc(mPtr);
        }

        //!
        //! append to this string
        //!
        void append( const CHAR * s, size_t l = 0 )
        {
            if ( 0 == s ) return;
            size_t ll = strLen<CHAR>(s);
            if ( 0 == l ) l = ll;
            else if ( l > ll ) l = ll;
            setCaps( mLen + l );
            ::memcpy( mPtr+mLen, s, l*sizeof(CHAR) );
            mLen += l;
            mPtr[mLen] = 0;
        }

        //!
        //! append to this string
        //!
        void append( const Str & s )
        {
            append( s.cstr(), s.size() );
        }

        //!
        //! assign value to string class
        //!
        void assign( const CHAR * s, size_t l = 0 )
        {
            if ( 0 == s )
            {
                mPtr[0] = 0;
                mLen = 0;
            }
            else
            {
                size_t ll = strLen<CHAR>(s);
                if ( 0 == l ) l = ll;
                else if ( l > ll ) l = ll;
                setCaps(l);
                mLen = l;
                ::memcpy( mPtr, s, l*sizeof(CHAR) );
                mPtr[l] = 0;
            }
        }

        //!
        //! begin iterator(1)
        //!
        CHAR * begin() { return mPtr; }

        //!
        //! begin iterator(2)
        //!
        const CHAR * begin() const { return mPtr; }

        //!
        //! Clear to empty string
        //!
        void clear()
        {
            mPtr[0] = 0;
            mLen = 0;
        }

        //!
        //! return c-style const char pointer
        //!
        const CHAR * cstr() const { return mPtr; }

        //!
        //! empty string or not?
        //!
        bool empty() const { return 0 == mLen; }

        //!
        //! begin iterator(1)
        //!
        CHAR * end() { return mPtr+mLen; }

        //!
        //! begin iterator(2)
        //!
        const CHAR * end() const { return mPtr+mLen; }

        //!
        //! printf-like string formatting
        //!
        const CHAR * format( const CHAR * fmt, ... )
        {
            va_list arglist;
            va_start( arglist, fmt );
            format( fmt, arglist );
            va_end( arglist );
            return mPtr;
        }

        //!
        //! printf-like string formatting(2)
        //!
        const CHAR * format( const CHAR * fmt, va_list args )
        {
            if( strEmpty(fmt) )
            {
                clear();
            }
            else
            {
                CHAR buf[4096];  // should be enough in most cases
                strPrintf( buf, 4096, fmt, args );
                assign( buf );
            }
            return mPtr;
        }

        //!
        //! get string caps
        //!
        size_t getCaps() const { return mCaps; }

        //!
        //! set string caps
        //!
        void setCaps( size_t newCaps )
        {
            if ( mCaps >= newCaps ) return;
            mCaps = calcCaps(newCaps);
            CHAR * newPtr = alloc(mCaps+1);
            ::memcpy( newPtr, mPtr, sizeof(CHAR)*mLen );
            dealloc(mPtr);
            mPtr = newPtr;
        }

        //!
        //! return string length in character, not including ending zero
        //!
        size_t size() const { return mLen; }

        //!
        //! convert to STL string(1)
        void toSTL( std::basic_string<CHAR> & s ) const
        {
            s.assign( mPtr, mLen );
        }

        //!
        //! convert to STL string(2)
        std::basic_string<CHAR> toSTL() const
        {
            return std::basic_string<CHAR>(mPtr,mLen);
        }

        //!
        //! indexing operator
        //!
        CHAR & operator [] ( size_t i ) { GN_ASSERT( i <= mLen  ); return mPtr[i]; }

        //!
        //! constant indexing operator
        //!
        const CHAR & operator [] ( size_t i ) const { GN_ASSERT( i <= mLen  ); return mPtr[i]; }

        //!
        //! assign operator
        //!
        Str & operator = ( const Str & s )
        {
            assign( s.mPtr, s.mLen );
            return *this;
        }

        //!
        //! assign operator
        //!
        Str & operator = ( const CHAR * s )
        {
            assign( s, strLen<CHAR>(s) );
            return *this;
        }

        //!
        //! += operator
        //!
        Str & operator += ( const Str & s )
        {
            append( s );
            return *this;
        }

        //!
        //! += operator
        //!
        Str & operator += ( const CHAR * s )
        {
            append( s, 0 );
            return *this;
        }

        //!
        //! equality operator(1)
        //!
        friend bool operator == ( const CHAR * s1, const Str & s2 )
        {
            return 0 == strCmp( s1, s2.mPtr );
        }

        //!
        //! equality operator(2)
        //!
        friend bool operator == ( const Str & s1, const CHAR * s2 )
        {
            return 0 == strCmp( s1.mPtr, s2 );
        }

        //!
        //! equality operator(3)
        //!
        friend bool operator == ( const Str & s1, const Str & s2 )
        {
            return 0 == strCmp( s1.mPtr, s2.mPtr ); 
        }

        //!
        //! unequality operator(1)
        //!
        friend bool operator != ( const CHAR * s1, const Str & s2 )
        {
            return 0 != strCmp( s1, s2.mPtr ); 
        }

        //!
        //! unequality operator(2)
        //!
        friend bool operator != ( const Str & s1, const CHAR * s2 )
        {
            return 0 != strCmp( s1.mPtr, s2 ); 
        }

        //!
        //! unequality operator(3)
        //!
        friend bool operator != ( const Str & s1, const Str & s2 )
        {
            return 0 != strCmp( s1.mPtr, s2.mPtr ); 
        }

        //!
        //! less operator(1)
        //!
        friend bool operator < ( const CHAR * s1, const Str & s2 )
        {
            return -1 == strCmp( s1, s2.mPtr ); 
        }

        //!
        //! less operator(2)
        //!
        friend bool operator < ( const Str & s1, const CHAR * s2 )
        {
            return -1 == strCmp( s1.mPtr, s2 ); 
        }

        //!
        //! less operator(3)
        //!
        friend bool operator < ( const Str & s1, const Str & s2 )
        {
            return -1 == strCmp( s1.mPtr, s2.mPtr ); 
        }


        //!
        //! concatnate operator(1)
        //!
        friend Str operator + ( const CHAR * s1, const Str & s2 )
        {
            Str r(s1);
            r.append(s2);
            return r;
        }

        //!
        //! concatnate operator(2)
        //!
        friend Str operator + ( const Str & s1, const CHAR * s2 )
        {
            Str r(s1);
            r.append(s2);
            return r;
        }

        //!
        //! concatnate operator(3)
        //!
        friend Str operator + ( const Str & s1, const Str & s2 )
        {
            Str r(s1);
            r.append(s2);
            return r;
        }

    private:

        size_t calcCaps( size_t caps )
        {
            caps |= caps >> 16;
            caps |= caps >> 8;
            caps |= caps >> 4;
            caps |= caps >> 2;
            caps |= caps >> 1;
            return caps;
        }

        static CHAR * alloc( size_t len )
        {
            return (CHAR*)::malloc( sizeof(CHAR) * len );
        }

        static void dealloc( CHAR * ptr )
        {
            if ( ptr ) ::free( ptr );
        }

        size_t mLen;   //!< string length in charecters, not including pending zero
        size_t mCaps;  //!< string buffer size in characters. Must >= mLen+1.
        CHAR * mPtr;   //!< string buffer pointer.

        friend Str<char> wcs2mbs( const wchar_t *, size_t );
        friend void wcs2mbs( Str<char> &, const wchar_t *, size_t );
        friend Str<wchar_t> mbs2wcs( const char *, size_t );
        friend void mbs2wcs( Str<wchar_t> &, const char *, size_t );
    };

    //!
    //! multi-byte string class
    //!
    typedef Str<char> StrA;

    //!
    //! wide-char string class
    //!
    typedef Str<wchar_t> StrW;

    //!
    //! printf-like string format function
    //!
    inline StrA formatStr( const char * fmt, ... )
    {
        StrA s;
        va_list arglist;
        va_start( arglist, fmt );
        s.format( fmt, arglist );
        va_end( arglist );
        return s;
    }

    //!
    //! printf-like string format function (wide-char version)
    //!
    inline StrW formatStr( const wchar_t * fmt, ... )
    {
        StrW s;
        va_list arglist;
        va_start( arglist, fmt );
        s.format( fmt, arglist );
        va_end( arglist );
        return s;
    }

    //!
    //! convert wide char string to multi-byte string
    //!
    StrA wcs2mbs( const wchar_t *, size_t );

    //!
    //! convert wide char string to multi-byte string
    //!
    void wcs2mbs( StrA &, const wchar_t *, size_t );

    //!
    //! convert multi-byte string to wide char string
    //!
    StrW mbs2wcs( const char *, size_t );

    //!
    //! convert multi-byte string to wide char string
    //!
    void mbs2wcs( StrW &, const char *, size_t );

    //!
    //! convert wide char string to multi-byte string
    //!
    inline StrA
    wcs2mbs( const StrW & i ) { return wcs2mbs( i.cstr(), i.size() ); }

    //!
    //! convert wide char string to multi-byte string
    //!
    inline void
    wcs2mbs( StrA & o, const StrW & i ) { wcs2mbs( o, i.cstr(), i.size() ); }

    //!
    //! convert multi-byte string to wide char string
    //!
    inline StrW
    mbs2wcs( const StrA & i ) { return mbs2wcs( i.cstr(), i.size() ); }

    //!
    //! convert multi-byte string to wide char string
    //!
    inline void
    mbs2wcs( StrW & o, const StrA & i ) { return mbs2wcs( o, i.cstr(), i.size() ); }
}

// *****************************************************************************
//                 End of string.h
// *****************************************************************************
#endif // __GN_BASE_STRING_H__
