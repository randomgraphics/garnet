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
    //! safe sprintf. This function always outputs null-terminated string,
    //! like StringCchPrintf(...)
    //!
    void
    strPrintf(
        char *       buf,
        size_t       bufSizeInChar,
        const char * fmt,
        ... );

    //!
    //! safe sprintf. This function always outputs null-terminated string,
    //! like StringCchPrintf(...)
    //!
    void
    strPrintf(
        wchar_t *       buf,
        size_t          bufSizeInWchar,
        const wchar_t * fmt,
        ... );

    //!
    //! safe sprintf. This function always outputs null-terminated string,
    //! like StringCchPrintf(...)
    //!
    void
    strVarPrintf(
        char *       buf,
        size_t       bufSizeInChar,
        const char * fmt,
        va_list      args );

    //!
    //! printf-like format string (wide-char)
    //!
    void
    strVarPrintf(
        wchar_t *       buf,
        size_t          bufSizeInWchar,
        const wchar_t * fmt,
        va_list         args );

    //!
    //! Custom string class
    //!
    template<typename CHAR, typename ALLOCATOR = MemAllocator<CHAR> >
    class Str
    {
        typedef CHAR CharType;
        typedef ALLOCATOR AllocatorType;

        CharType * mPtr;   //!< string buffer pointer.
        size_t     mCount; //!< How many charecters in the string, not including null end.
        size_t     mCaps;  //!< How many characters can we hold, not including null end?

    public:

        //!
        //! Instance of empty string
        //!
        static Str EMPTYSTR;

        //!
        //! default constructor
        //!
        Str() : mCount(0), mCaps(0)
        {
            mPtr = alloc(mCaps);
            mPtr[0] = 0;
        }

        //!
        //! copy constructor
        //!
        Str( const Str & s ) : mCount(s.mCount), mCaps(calcCaps(s.mCount))
        {
            mPtr = alloc(mCaps);
            ::memcpy( mPtr, s.mPtr, (mCount+1)*sizeof(CharType) );
        }

        //!
        //! copy constructor from c-style string
        //!
        Str( const CharType * s, size_t l = 0 )
        {
            if ( 0 == s )
            {
                mCaps = 0;
                mCount = 0;
                mPtr = alloc(mCaps);
                mPtr[0] = 0;
            }
            else
            {
                l = strLen<CharType>(s,l);
                mCaps = calcCaps(l);
                mPtr = alloc(mCaps);
                mCount = l;
                ::memcpy( mPtr, s, l*sizeof(CharType) );
                mPtr[l] = 0;
            }
        }

        //!
        //! destructor
        //!
        ~Str()
        {
            dealloc( mPtr, mCaps );
        }

        //!
        //! append to this string
        //!
        void append( const CharType * s, size_t l = 0 )
        {
            if ( 0 == s ) return;
            l = strLen<CharType>(s,l);
            setCaps( mCount + l );
            ::memcpy( mPtr+mCount, s, l*sizeof(CharType) );
            mCount += l;
            mPtr[mCount] = 0;
        }

        //!
        //! append to this string
        //!
        void append( const Str & s )
        {
            if( s.empty() ) return;
            size_t l = s.size();
            setCaps( mCount + l );
            ::memcpy( mPtr+mCount, s.cptr(), l*sizeof(CharType) );
            mCount += l;
            mPtr[mCount] = 0;
        }

        //!
        //! append to this string
        //!
        void append( CharType ch )
        {
            if ( 0 == ch ) return;
            setCaps( mCount + 1 );
            mPtr[mCount] = ch;
            ++mCount;
            mPtr[mCount] = 0;
        }

        //!
        //! assign value to string class
        //!
        void assign( const CharType * s, size_t l = 0 )
        {
            if ( 0 == s )
            {
                mPtr[0] = 0;
                mCount = 0;
            }
            else
            {
                l = strLen<CharType>(s,l);
                setCaps(l);
                mCount = l;
                ::memcpy( mPtr, s, l*sizeof(CharType) );
                mPtr[l] = 0;
            }
        }

        //!
        //! begin iterator(1)
        //!
        CharType * begin() { return mPtr; }

        //!
        //! begin iterator(2)
        //!
        const CharType * begin() const { return mPtr; }

        //!
        //! Clear to empty string
        //!
        void clear()
        {
            mPtr[0] = 0;
            mCount = 0;
        }

        //!
        //! return c-style const char pointer
        //!
        const CharType * cptr() const { return mPtr; }

        //!
        //! empty string or not?
        //!
        bool empty() const { return 0 == mCount; }

        //!
        //! begin iterator(1)
        //!
        CharType * end() { return mPtr+mCount; }

        //!
        //! begin iterator(2)
        //!
        const CharType * end() const { return mPtr+mCount; }

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
            if( offset >= mCount ) return NOT_FOUND;
            if( 0 == count ) count = mCount;
            if( offset + count > mCount ) count = mCount - offset;
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
            if( offset >= mCount ) return NOT_FOUND;
            if( 0 == count ) count = mCount;
            if( offset + count > mCount ) count = mCount - offset;
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
            if( offset >= mCount ) return NOT_FOUND;
            if( 0 == count ) count = mCount;
            if( offset + count > mCount ) count = mCount - offset;
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
            if( offset >= mCount ) return NOT_FOUND;
            if( 0 == count ) count = mCount;
            if( offset + count > mCount ) count = mCount - offset;
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
        CharType first() const { return mPtr[0]; }

        //!
        //! printf-like string formatting
        //!
        const CharType * format( const CharType * fmt, ... )
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
        const CharType * format( const CharType * fmt, va_list args )
        {
            if( strEmpty(fmt) )
            {
                clear();
            }
            else
            {
                CharType buf[16384];  // 16k should be enough in most cases
                strVarPrintf( buf, 16384, fmt, args );
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
        void insert( size_t pos, CharType ch )
        {
            if( 0 == ch ) return;
            if( pos >= mCount )
            {
                append( ch );
            }
            else
            {
                setCaps( mCount + 1 );
                for( size_t i = mCount+1; i > pos; --i )
                {
                    mPtr[i] = mPtr[i-1];
                }
                mPtr[pos] = ch;
                ++mCount;
            }
        }

        //!
        //! get last character of the string. If string is empty, return 0.
        //!
        CharType last() const { return mCount>0 ? mPtr[mCount-1] : (CharType)0; }

        //!
        //! Replace specific character with another
        //!
        void replace( CharType from, CharType to )
        {
            CharType * p = mPtr;
            for( size_t i = 0; i < mCount; ++i, ++p )
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
            size_t oldCaps = mCaps;
            mCaps = calcCaps( newCaps );
            CharType * newPtr = alloc( mCaps );
            ::memcpy( newPtr, mPtr, sizeof(CharType)*(mCount+1) );
            dealloc( mPtr, oldCaps );
            mPtr = newPtr;
        }

        //!
        //! return string length in character, not including ending zero
        //!
        size_t size() const { return mCount; }

        //!
        //! Get sub string
        //!
        void subString( Str & result, size_t offset, size_t length ) const
        {
            if( offset >= mCount ) { result.clear(); return; }
            if( offset + length > mCount ) length = mCount - offset;
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
        void toSTL( std::basic_string<CharType> & s ) const
        {
            s.assign( mPtr, mCount );
        }

        //!
        //! convert to STL string(2)
        std::basic_string<CharType> toSTL() const
        {
            return std::basic_string<CharType>(mPtr,mCount);
        }

        //!
        //! Trim characters for both side
        //!
        void trim( const CharType * ch, size_t len = 0 )
        {
            if( 0 == ch ) return;
            if( 0 == len ) len = strLen( ch );
            trimRight( ch, len );
            trimLeft( ch, len );
        }

        //!
        //! Trim characters for both side
        //!
        void trim( CharType ch ) { trim( &ch, 1 ); }

        //!
        //! Trim left characters
        //!
        void trimLeft( const CharType * ch, size_t len = 0 )
        {
            if( 0 == ch ) return;
            if( 0 == len ) len = strLen( ch );
            if( 0 == len ) return;
            CharType * p = mPtr;
            CharType * e = mPtr+mCount;
            while( p < e )
            {
                bool equal = false;
                for( size_t i = 0; i < len; ++i )
                {
                    if( *p == ch[i] )
                    {
                        equal = true;
                        break;
                    }
                }
                if( !equal ) break;
                ++p;
            }
            mCount = e - p;
            for( size_t i = 0; i < mCount; ++i )
            {
                mPtr[i] = p[i];
            }
            mPtr[mCount] = 0;
        }

        //!
        //! Trim left characters
        //!
        void trimLeft( CharType ch ) { trimLeft( &ch, 1 ); }

        //!
        //! Trim right characters
        //!
        void trimRight( const CharType * ch, size_t len = 0 )
        {
            if( 0 == mCount ) return;
            if( 0 == ch ) return;
            if( 0 == len ) len = strLen( ch );
            if( 0 == len ) return;
            CharType * p = mPtr + mCount - 1;
            while( p >= mPtr )
            {
                bool equal = false;
                for( size_t i = 0; i < len; ++i )
                {
                    if( *p == ch[i] )
                    {
                        equal = true;
                        break;
                    }
                }
                if( !equal ) break;
                *p = 0;
                --p;
            }
            mCount = p - mPtr + 1;
        }

        //!
        //! Trim right characters
        //!
        void trimRight( CharType ch ) { trimRight( &ch, 1 ); }

        //!
        //! Trim right characters until meet the predication condition.
        //!
        template<typename PRED>
        void trimRightUntil( const PRED & pred )
        {
            if( 0 == mCount ) return;
            CharType * p = mPtr + mCount - 1;
            while( p >= mPtr && !pred(*p) )
            {
                *p = 0;
                --p;
            }
            mCount = p - mPtr + 1;
        }

        //!
        //! indexing operator
        //!
        CharType & operator [] ( size_t i ) { GN_ASSERT( i <= mCount  ); return mPtr[i]; }

        //!
        //! constant indexing operator
        //!
        const CharType & operator [] ( size_t i ) const { GN_ASSERT( i <= mCount  ); return mPtr[i]; }

        //!
        //! assign operator
        //!
        Str & operator = ( const Str & s )
        {
            assign( s.mPtr, s.mCount );
            return *this;
        }

        //!
        //! assign operator
        //!
        Str & operator = ( const CharType * s )
        {
            assign( s, strLen<CharType>(s) );
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
        Str & operator += ( const CharType * s )
        {
            append( s, 0 );
            return *this;
        }

        //!
        //! += operator (3)
        //!
        Str & operator += ( CharType ch )
        {
            append( ch );
            return *this;
        }

        //!
        //! += operator (4)
        //!
        Str & operator += ( std::basic_string<CharType> & s )
        {
            append( s.c_str(), 0 );
            return *this;
        }

        //!
        //! equality operator(1)
        //!
        friend bool operator == ( const CharType * s1, const Str & s2 )
        {
            return 0 == strCmp( s1, s2.mPtr );
        }

        //!
        //! equality operator(2)
        //!
        friend bool operator == ( const Str & s1, const CharType * s2 )
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
        friend bool operator != ( const CharType * s1, const Str & s2 )
        {
            return 0 != strCmp( s1, s2.mPtr );
        }

        //!
        //! unequality operator(2)
        //!
        friend bool operator != ( const Str & s1, const CharType * s2 )
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
        friend bool operator < ( const CharType * s1, const Str & s2 )
        {
            return -1 == strCmp( s1, s2.mPtr );
        }

        //!
        //! less operator(2)
        //!
        friend bool operator < ( const Str & s1, const CharType * s2 )
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
        friend Str operator + ( const CharType * s1, const Str & s2 )
        {
            Str r(s1);
            r.append(s2);
            return r;
        }

        //!
        //! concatnate operator(2)
        //!
        friend Str operator + ( const Str & s1, const CharType * s2 )
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
        friend Str operator + ( const Str & s1, const std::basic_string<CharType> & s2 )
        {
            Str r(s1);
            r.append( s2.c_str() );
            return r;
        }

        //!
        //! concatnate operator(5)
        //!
        friend Str operator + ( const std::basic_string<CharType> & s1, const Str & s2 )
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

        // align caps to 2^n-1
        size_t calcCaps( size_t count )
        {
            #if GN_X64
            count |= count >> 32;
            #endif
            count |= count >> 16;
            count |= count >> 8;
            count |= count >> 4;
            count |= count >> 2;
            count |= count >> 1;
            return count;
        }

        // Allocate a memory buffer that can hold at least 'count' characters, and one extra '\0'.
        static CharType * alloc( size_t count )
        {
            // TODO: call a.construct() if CharType is class type.
            AllocatorType a;
            return a.allocate( count + 1 );
        }

        static void dealloc( CharType * ptr, size_t count )
        {
            if ( ptr )
            {
                // TODO: call a.destructor(), if CharType is class type.
                AllocatorType a;
                a.deallocate( ptr, count + 1 );
            }
        }

        friend Str<char> wcs2mbs( const wchar_t *, size_t );
        friend void wcs2mbs( Str<char> &, const wchar_t *, size_t );
        friend Str<wchar_t> mbs2wcs( const char *, size_t );
        friend void mbs2wcs( Str<wchar_t> &, const char *, size_t );
    };

    // Implement static data member
    template <typename CHAR, typename ALLOC> Str<CHAR,ALLOC> Str<CHAR,ALLOC>::EMPTYSTR;

    //!
    //! multi-byte string class
    //!
    typedef Str<char> StrA;

    //!
    //! wide-char string class
    //!
    typedef Str<wchar_t> StrW;

    //! \name string -> number conversion
    //@{

    bool str2Int16( int16_t &, const char * );
    bool str2Uint16( uint16_t &, const char * );
    bool str2Int32( int32_t &, const char * );
    bool str2Uint32( uint32_t &, const char * );
    bool str2Int64( int64_t &, const char * );
    bool str2Uint64( uint64_t &, const char * );
    bool str2Float( float &, const char * );
    bool str2Double( double &, const char *  );

    template<typename T> bool str2Int( T & i, const char * s );

    template<> inline bool str2Int<int16_t>( int16_t & i, const char * s ) { return str2Int16( i, s ); }
    template<> inline bool str2Int<uint16_t>( uint16_t & i, const char * s ) { return str2Uint16( i, s ); }
    template<> inline bool str2Int<int32_t>( int32_t & i, const char * s ) { return str2Int32( i, s ); }
    template<> inline bool str2Int<uint32_t>( uint32_t & i, const char * s ) { return str2Uint32( i, s ); }
    template<> inline bool str2Int<int64_t>( int64_t & i, const char * s ) { return str2Int64( i, s ); }
    template<> inline bool str2Int<uint64_t>( uint64_t & i, const char * s ) { return str2Uint64( i, s ); }

    template<> inline bool str2Int<int>( int & i, const char * s )
    {
        return str2Int32( *(int32_t*)&i, s );
    }

    template<> inline bool str2Int<unsigned int>( unsigned int & i, const char * s )
    {
        return str2Uint32( *(uint32_t*)&i, s );
    }

    template<typename T> T str2Int( const char * s, T defaultValue )
    {
        T result;
        if( str2Int( result, s ) )
            return result;
        else
            return defaultValue;
    }

    //@}

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
//                 End of string.h
// *****************************************************************************
#endif // __GN_BASE_STRING_H__
