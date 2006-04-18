#ifndef __GN_BASE_STRING_H__
#define __GN_BASE_STRING_H__
// *****************************************************************************
//! \file    string.h
//! \brief   custom string implementation
//! \author  chenlee (2005.4.17)
// *****************************************************************************

#include <stdarg.h>
#include <string>
#include <ostream>

namespace GN
{
    //!
    //! Get string length.
    //!
    //! if maxLen > 0, then return min(maxLen,realLength).
    //!
    template<typename CHAR>
    inline size_t strLen( const CHAR * s, size_t maxLen = 0 )
    {
        if ( 0 == s ) return 0;
        size_t l = 0;
        if ( maxLen > 0 )
        {
            while( 0 != *s && l < maxLen )
            {
                ++l;
                ++s;
            }
        }
        else
        {
            while( 0 != *s )
            {
                ++l;
                ++s;
            }
        }
        return l;
    }

    //!
    //! string comparison (case sensitive)
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
    //! string comparison (case insensitive)
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
    template<typename CHAR>
    class Str
    {
        CHAR * mPtr;   //!< string buffer pointer.
        size_t mLen;   //!< How many charecters in the string, not including pending zero.
        size_t mCaps;  //!< How many characters can we hold?

        static GN_PUBLIC FixedSizedObjectAllocator<Str<CHAR> > msAllocator; //!< global string allocator

    public:

        //!
        //! new operator
        //!
        void * operator new( size_t ) GN_THROW_BADALLOC() { return msAllocator.alloc(); }

        //!
        //! delete operator
        //!
        void operator delete( void * p ) GN_NOTHROW() { msAllocator.dealloc( p ); }

        //!
        //! placement new operator, for compability with vc71
        //!
        void * operator new( size_t, void * p ) GN_THROW_BADALLOC() { return p; }

        //!
        //! placement delete operator, for compability with vc71
        //!
        void operator delete( void *, void * ) GN_NOTHROW() {}

    public:

        //!
        //! Instance of empty string
        //!
        static Str EMPTYSTR;

        //!
        //! default constructor
        //!
        Str() : mLen(0), mCaps(0)
        {
            mPtr = alloc(mCaps);
            mPtr[0] = 0;
        }

        //!
        //! copy constructor
        //!
        Str( const Str & s ) : mLen(s.mLen), mCaps(calcCaps(s.mLen))
        {
            mPtr = alloc(mCaps);
            ::memcpy( mPtr, s.mPtr, (mLen+1)*sizeof(CHAR) );
        }

        //!
        //! copy constructor from c-style string
        //!
        Str( const CHAR * s, size_t l = 0 )
        {
            if ( 0 == s )
            {
                mCaps = 0;
                mLen = 0;
                mPtr = alloc(mCaps);
                mPtr[0] = 0;
            }
            else
            {
                l = strLen<CHAR>(s,l);
                mCaps = calcCaps(l);
                mPtr = alloc(mCaps);
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
            l = strLen<CHAR>(s,l);
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
            if( s.empty() ) return;
            size_t l = s.size();
            setCaps( mLen + l );
            ::memcpy( mPtr+mLen, s.cptr(), l*sizeof(CHAR) );
            mLen += l;
            mPtr[mLen] = 0;
        }

        //!
        //! append to this string
        //!
        void append( CHAR ch )
        {
            if ( 0 == ch ) return;
            setCaps( mLen + 1 );
            mPtr[mLen] = ch;
            ++mLen;
            mPtr[mLen] = 0;
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
                l = strLen<CHAR>(s,l);
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
        const CHAR * cptr() const { return mPtr; }

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

        static const size_t NOT_FOUND = (size_t)-1; //!< indicate serach failure.

        //!
        //! Searches through a string for the first character that matches any elements in user specified string
        //!
        //! \param s
        //!     User specified search pattern
        //! \param offset, count
        //!     Range of the search
        //!
        size_t findFirstOf( const char * s, size_t offset = 0, size_t count = 0 ) const
        {
            if( 0 == s || 0 == *s ) return NOT_FOUND;
            if( offset >= mLen ) return NOT_FOUND;
            if( 0 == count ) count = mLen;
            if( offset + count > mLen ) count = mLen - offset;
            const char * p = mPtr + offset;
            for( size_t i = 0; i < count; ++i, ++p )
            {
                for( const char * t = s; *t; ++t )
                {
                    GN_ASSERT( *p && *t );
                    if( *p == *t ) return offset + i;
                }
            }
            return NOT_FOUND;
        }

        //!
        //! Searches through a string for the first character that not any elements of user specifed string.
        //!
        size_t findFirstNotOf( const char * s, size_t offset = 0, size_t count = 0 ) const
        {
            if( 0 == s || 0 == *s ) return NOT_FOUND;
            if( offset >= mLen ) return NOT_FOUND;
            if( 0 == count ) count = mLen;
            if( offset + count > mLen ) count = mLen - offset;
            const char * p = mPtr + offset;
            for( size_t i = 0; i < count; ++i, ++p )
            {
                for( const char * t = s; *t; ++t )
                {
                    GN_ASSERT( *p && *t );
                    if( *p != *t ) return offset + i;
                }
            }
            return NOT_FOUND;
        }

        //!
        //! Searches through a string for the first character that matches users predication
        //!
        template<typename PRED>
        size_t findFirstOf( const PRED & pred, size_t offset = 0, size_t count = 0 ) const
        {
            if( offset >= mLen ) return NOT_FOUND;
            if( 0 == count ) count = mLen;
            if( offset + count > mLen ) count = mLen - offset;
            const char * p = mPtr + offset;
            for( size_t i = 0; i < count; ++i, ++p )
            {
                GN_ASSERT( *p );
                if( pred(*p) ) return offset + i;
            }
            return NOT_FOUND;
        }

        //!
        //! Searches through a string for the last character that matches any elements in user specified string
        //!
        size_t findLastOf( const char * s, size_t offset = 0, size_t count = 0 ) const
        {
            if( 0 == s || 0 == *s ) return NOT_FOUND;
            if( offset >= mLen ) return NOT_FOUND;
            if( 0 == count ) count = mLen;
            if( offset + count > mLen ) count = mLen - offset;
            GN_ASSERT( count > 0 );
            const char * p = mPtr + offset + count - 1;
            for( size_t i = count; i > 0; --i, --p )
            {
                for( const char * t = s; *t; ++t )
                {
                    GN_ASSERT( *p && *t );
                    if( *p == *t ) return offset + count - i;
                }
            }
            return NOT_FOUND;
        }

        //!
        //! get first character of the string. If string is empty, return 0.
        //!
        CHAR first() const { return mPtr[0]; }

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
                CHAR buf[16384];  // 16k should be enough in most cases
                strPrintf( buf, 16384, fmt, args );
                buf[16383] = 0;
                assign( buf );
            }
            return mPtr;
        }

        //!
        //! get string caps
        //!
        size_t getCaps() const { return mCaps; }

        //!
        //! Insert a character at specific position
        //!
        void insert( size_t pos, CHAR ch )
        {
            if( 0 == ch ) return;
            if( pos >= mLen )
            {
                append( ch );
            }
            else
            {
                setCaps( mLen + 1 );
                for( size_t i = mLen+1; i > pos; --i )
                {
                    mPtr[i] = mPtr[i-1];
                }
                mPtr[pos] = ch;
                ++mLen;
            }
        }

        //!
        //! get last character of the string. If string is empty, return 0.
        //!
        CHAR last() const { return mLen>0 ? mPtr[mLen-1] : (CHAR)0; }

        //!
        //! Replace specific character with another
        //!
        void replace( CHAR from, CHAR to )
        {
            CHAR * p = mPtr;
            for( size_t i = 0; i < mLen; ++i, ++p )
            {
                if( from == *p ) *p = to;
            }
        }

        //!
        //! set string caps
        //!
        void setCaps( size_t newCaps )
        {
            if ( mCaps >= newCaps ) return;
            mCaps = calcCaps(newCaps);
            CHAR * newPtr = alloc(mCaps);
            ::memcpy( newPtr, mPtr, sizeof(CHAR)*(mLen+1) );
            dealloc(mPtr);
            mPtr = newPtr;
        }

        //!
        //! return string length in character, not including ending zero
        //!
        size_t size() const { return mLen; }

        //!
        //! Get sub string
        //!
        void subString( Str & result, size_t offset, size_t length ) const
        {
            if( offset >= mLen ) { result.clear(); return; }
            if( offset + length > mLen ) length = mLen - offset;
            result.assign( mPtr+offset, length );
        }

        //!
        //! Return sub string
        //!
        Str subString( size_t offset, size_t length ) const
        {
            Str ret;
            subString( ret, offset, length );
            return ret;
        }

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
        //! Trim characters for both side
        //!
        void trim( CHAR ch )
        {
            trimRight( ch );
            trimLeft( ch );
        }

        //!
        //! Trim left characters
        //!
        void trimLeft( CHAR ch )
        {
            CHAR * p = mPtr;
            CHAR * e = mPtr+mLen;
            while( p < e && ch == *p )
            {
                ++p;
            }
            mLen = e - p;
            for( size_t i = 0; i < mLen; ++i )
            {
                mPtr[i] = p[i];
            }
            mPtr[mLen] = 0;
        }

        //!
        //! Trim right characters
        //!
        void trimRight( CHAR ch )
        {
            if( 0 == mLen ) return;
            CHAR * p = mPtr + mLen - 1;
            while( p >= mPtr && ch == *p )
            {
                *p = 0;
                --p;
            }
            mLen = p - mPtr + 1;
        }

        //!
        //! Trim right characters until meet the predication condition.
        //!
        template<typename PRED>
        void trimRightUntil( const PRED & pred )
        {
            if( 0 == mLen ) return;
            CHAR * p = mPtr + mLen - 1;
            while( p >= mPtr && !pred(*p) )
            {
                *p = 0;
                --p;
            }
            mLen = p - mPtr + 1;
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
        //! += operator (1)
        //!
        Str & operator += ( const Str & s )
        {
            append( s );
            return *this;
        }

        //!
        //! += operator (2)
        //!
        Str & operator += ( const CHAR * s )
        {
            append( s, 0 );
            return *this;
        }

        //!
        //! += operator (3)
        //!
        Str & operator += ( CHAR ch )
        {
            append( ch );
            return *this;
        }

        //!
        //! += operator (4)
        //!
        Str & operator += ( std::basic_string<CHAR> & s )
        {
            append( s.c_str(), 0 );
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

        //!
        //! concatnate operator(4)
        //!
        friend Str operator + ( const Str & s1, const std::basic_string<CHAR> & s2 )
        {
            Str r(s1);
            r.append( s2.c_str() );
            return r;
        }

        //!
        //! concatnate operator(5)
        //!
        friend Str operator + ( const std::basic_string<CHAR> & s1, const Str & s2 )
        {
            Str r(s2);
            r.append( s1.c_str() );
            return r;
        }

        //!
        //! Output to ostream
        //!
        friend std::ostream & operator << ( std::ostream & os, const Str & str )
        {
            os << str.cptr();
            return os;
        }

    private:

        size_t calcCaps( size_t len )
        {
            len |= len >> 16;
            len |= len >> 8;
            len |= len >> 4;
            len |= len >> 2;
            len |= len >> 1;
            return len;
        }

        // Allocate a memory buffer that can hold at least 'len' characters, and one extra '\0'.
        static CHAR * alloc( size_t len )
        {
            return (CHAR*)memAlloc( sizeof(CHAR) * (len+1) );
        }

        static void dealloc( CHAR * ptr )
        {
            if ( ptr ) memFree( ptr );
        }

        friend Str<char> wcs2mbs( const wchar_t *, size_t );
        friend void wcs2mbs( Str<char> &, const wchar_t *, size_t );
        friend Str<wchar_t> mbs2wcs( const char *, size_t );
        friend void mbs2wcs( Str<wchar_t> &, const char *, size_t );
    };

    // Implement static data member
    template <typename CHAR> Str<CHAR> Str<CHAR>::EMPTYSTR;

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
    inline StrA strFormat( const char * fmt, ... )
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
    inline StrW strFormat( const wchar_t * fmt, ... )
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
//                 End of string.h
// *****************************************************************************
#endif // __GN_BASE_STRING_H__
