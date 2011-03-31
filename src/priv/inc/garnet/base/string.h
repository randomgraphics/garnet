#ifndef __GN_BASE_STRING_H__
#define __GN_BASE_STRING_H__
// *****************************************************************************
/// \file
/// \brief   custom string implementation
/// \author  chenlee (2005.4.17)
// *****************************************************************************

#include <stdarg.h>
#include <ostream>
#include <string.h>
namespace GN
{
    /// define enumerations for string compare
    struct StringCompareCase
    {
        enum ENUM
        {
            INSENSITIVE, // case insensitive comparision
            SENSITIVE,   // case sensitive comparision
        };

        GN_DEFINE_ENUM_CLASS_HELPERS( StringCompareCase, ENUM );
    };

    ///
    /// Get string length.
    ///
    /// if maxLen > 0, then return math::getmin(maxLen,realLength).
    ///
    template<typename CHAR>
    inline size_t stringLength( const CHAR * s, size_t maxLen = 0 )
    {
        if( 0 == s ) return 0;
        size_t l = 0;
        if( maxLen > 0 )
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

    ///
    /// string comparison (case sensitive)
    ///
    template<typename CHAR>
    inline int stringCompare( const CHAR * s1, const CHAR * s2 )
    {
        if( s1 == s2 ) return 0;
        if( 0 == s1 ) return -1;
        if( 0 == s2 ) return 1;
        while( *s1 && *s2 )
        {
            if( *s1 < *s2 ) return -1;
            if( *s1 > *s2 ) return 1;
            ++s1;
            ++s2;
        }
        if( 0 != *s1 ) return 1;
        if( 0 != *s2 ) return -1;
        return 0;
    }

    ///
    /// string comparison, case sensitive, with limited length
    ///
    template<typename CHAR>
    inline int stringCompare( const CHAR * s1, const CHAR * s2, size_t maxLength )
    {
        if( s1 == s2 ) return 0;
        if( 0 == s1 ) return -1;
        if( 0 == s2 ) return 1;
        size_t len = 0;
        while( *s1 && *s2 && len < maxLength )
        {
            if( *s1 < *s2 ) return -1;
            if( *s1 > *s2 ) return 1;
            ++s1;
            ++s2;
            ++len;
        }
        if( len == maxLength ) return 0;
        if( 0 != *s1 ) return 1;
        if( 0 != *s2 ) return -1;
        return 0;
    }

    ///
    /// string comparison (case insensitive)
    ///
    template<typename CHAR>
    inline int stringCompareI( const CHAR * s1, const CHAR * s2 )
    {
        if( s1 == s2 ) return 0;
        if( 0 == s1 ) return -1;
        if( 0 == s2 ) return 1;
        int a, b;
        while( *s1 && *s2 )
        {
            a = (int)*s1;
            b = (int)*s2;
            if( 'a' <= a && a <= 'z' ) a += 'A'-'a';
            if( 'a' <= b && b <= 'z' ) b += 'A'-'a';
            if( a < b ) return -1;
            if( a > b ) return 1;
            ++s1;
            ++s2;
        }
        if( 0 != *s1 ) return 1;
        if( 0 != *s2 ) return -1;
        return 0;
    }

    ///
    /// string comparison (case insensitive), with limited length
    ///
    template<typename CHAR>
    inline int stringCompareI( const CHAR * s1, const CHAR * s2, size_t maxLength )
    {
        if( s1 == s2 ) return 0;
        if( 0 == s1 ) return -1;
        if( 0 == s2 ) return 1;
        size_t len = 0;
        int a, b;
        while( *s1 && *s2 && len < maxLength )
        {
            a = (int)*s1;
            b = (int)*s2;
            if( 'a' <= a && a <= 'z' ) a += 'A'-'a';
            if( 'a' <= b && b <= 'z' ) b += 'A'-'a';
            if( a < b ) return -1;
            if( a > b ) return 1;
            ++s1;
            ++s2;
            ++len;
        }
        if( len == maxLength ) return 0;
        if( 0 != *s1 ) return 1;
        if( 0 != *s2 ) return -1;
        return 0;
    }

    ///
    /// check for empty string, including NULL.
    ///
    template<typename CHAR>
    inline bool stringEmpty( const CHAR * s )
    {
        return 0 == s || 0 == s[0];
    }

    ///
    /// safe sprintf. This function always outputs null-terminated string,
    /// like StringCchPrintf(...)
    ///
    void
    stringPrintf(
        char *       buf,
        size_t       bufSizeInChar,
        const char * fmt,
        ... );

    ///
    /// safe sprintf. This function always outputs null-terminated string,
    /// like StringCchPrintf(...)
    ///
    void
    stringPrintf(
        wchar_t *       buf,
        size_t          bufSizeInWchar,
        const wchar_t * fmt,
        ... );

    ///
    /// safe sprintf. This function always outputs null-terminated string,
    /// like StringCchPrintf(...)
    ///
    void
    stringVarPrintf(
        char *       buf,
        size_t       bufSizeInChar,
        const char * fmt,
        va_list      args );

    ///
    /// printf-like format string (wide-char)
    ///
    void
    stringVarPrintf(
        wchar_t *       buf,
        size_t          bufSizeInWchar,
        const wchar_t * fmt,
        va_list         args );

    ///
    /// string hash function
    ///
    /// set to length to 0 to hash NULL terminated string.
    ///
    template<typename CHAR>
    inline uint64 stringHash( const CHAR * s, size_t length = 0 )
    {
        unsigned long hash = 5381;

        if( length > 0 )
        {
            for( size_t i = 0; i < length; ++i )
            {
                hash = ((hash << 5) + hash) + *s; /* hash * 33 + c */
                ++s;
            }
        }
        else
        {
            while( *s )
            {
                hash = ((hash << 5) + hash) + *s; /* hash * 33 + c */
                ++s;
            }
        }

        return hash;
    }

    ///
    /// Custom string class. CHAR type must be POD type.
    ///
    template<typename CHAR, typename RAW_MEMORY_ALLOCATOR = RawHeapMemoryAllocator>
    class Str
    {
        typedef CHAR CharType;

        CharType * mPtr;   ///< string buffer pointer.

    public:

        ///
        /// Instance of empty string
        ///
        static Str EMPTYSTR;

        ///
        /// indicate serach failure.
        ///
        static const size_t NOT_FOUND = (size_t)-1;

        ///
        /// default constructor
        ///
        Str() : mPtr(NULL)
        {
            setCaps( 0 );
            mPtr[0] = 0;
        }

        ///
        /// copy constructor
        ///
        Str( const Str & s ) : mPtr(NULL)
        {
            setCaps( s.size() );
            ::memcpy( mPtr, s.mPtr, (s.size()+1)*sizeof(CharType) );
            setSize( s.size() );
        }

        ///
        /// copy constructor from c-style string
        ///
        Str( const CharType * s, size_t l = 0 ) : mPtr(NULL)
        {
            if( 0 == s )
            {
                setCaps( 0 );
                setSize( 0 );
                mPtr[0] = 0;
            }
            else
            {
                l = stringLength<CharType>(s,l);
                setCaps( l );
                ::memcpy( mPtr, s, l*sizeof(CharType) );
                mPtr[l] = 0;
                setSize( l );
            }
        }

        ///
        /// destructor
        ///
        ~Str()
        {
            dealloc( mPtr );
        }

        ///
        /// append to this string
        ///
        void append( const CharType * s, size_t l = 0 )
        {
            if( 0 == s ) return;
            l = stringLength<CharType>(s,l);
            if( 0 == l ) return;
            size_t oldsize = size();
            size_t newsize = oldsize + l;
            setCaps( newsize );
            setSize( newsize );
            ::memcpy( mPtr+oldsize, s, l*sizeof(CharType) );
            mPtr[newsize] = 0;
        }

        ///
        /// append to this string
        ///
        void append( const Str & s )
        {
            if( s.empty() ) return;
            size_t oldsize = size();
            size_t ssize = s.size();
            size_t newsize = oldsize + ssize;
            setCaps( newsize );
            setSize( newsize );
            ::memcpy( mPtr+oldsize, s, ssize*sizeof(CharType) );
            mPtr[newsize] = 0;
        }

        ///
        /// append to this string
        ///
        void append( CharType ch )
        {
            if( 0 == ch ) return;
            size_t oldsize = size();
            size_t newsize = oldsize + 1;
            setCaps( newsize );
            setSize( newsize );
            mPtr[oldsize] = ch;
            mPtr[newsize] = 0;
        }

        ///
        /// assign value to string class
        ///
        void assign( const CharType * s, size_t l = 0 )
        {
            if( 0 == s )
            {
                setCaps( 0 );
                setSize( 0 );
                mPtr[0] = 0;
            }
            else
            {
                l = stringLength<CharType>(s,l);
                setCaps( l );
                ::memcpy( mPtr, s, l*sizeof(CharType) );
                mPtr[l] = 0;
                setSize( l );
            }
        }

        ///
        /// begin iterator(1)
        ///
        CharType * begin() { return mPtr; }

        ///
        /// begin iterator(2)
        ///
        const CharType * begin() const { return mPtr; }

        ///
        /// Clear to empty string
        ///
        void clear()
        {
            mPtr[0] = 0;
            setSize( 0 );
        }

        ///
        /// return c-style const char pointer
        ///
        const CharType * cptr() const { return mPtr; }

        ///
        /// empty string or not?
        ///
        bool empty() const { return 0 == size(); }

        ///
        /// begin iterator(1)
        ///
        CharType * end() { return mPtr+size(); }

        ///
        /// begin iterator(2)
        ///
        const CharType * end() const { return mPtr+size(); }

        ///
        /// Searches through a string for the first character that matches any elements in user specified string
        ///
        /// \param s
        ///     User specified search pattern
        /// \param offset, count
        ///     Range of the search. (count==0) means to the end of input string.
        /// \return
        ///     Return index of the character of first occurance or NOT_FOUND.
        ///
        size_t findFirstOf( const CharType * s, size_t offset = 0, size_t count = 0 ) const
        {
            if( 0 == s || 0 == *s ) return NOT_FOUND;
            if( offset >= size() ) return NOT_FOUND;
            if( 0 == count ) count = size();
            if( offset + count > size() ) count = size() - offset;
            const CharType * p = mPtr + offset;
            for( size_t i = 0; i < count; ++i, ++p )
            {
                for( const CharType * t = s; *t; ++t )
                {
                    GN_ASSERT( *p && *t );
                    if( *p == *t ) return offset + i;
                }
            }
            return NOT_FOUND;
        }

        ///
        /// Searches through a string for the first character that not any elements of user specifed string.
        ///
        size_t findFirstNotOf( const CharType * s, size_t offset = 0, size_t count = 0 ) const
        {
            if( 0 == s || 0 == *s ) return NOT_FOUND;
            if( offset >= size() ) return NOT_FOUND;
            if( 0 == count ) count = size();
            if( offset + count > size() ) count = size() - offset;
            const CharType * p = mPtr + offset;
            for( size_t i = 0; i < count; ++i, ++p )
            {
                for( const CharType * t = s; *t; ++t )
                {
                    GN_ASSERT( *p && *t );
                    if( *p != *t ) return offset + i;
                }
            }
            return NOT_FOUND;
        }

        ///
        /// Searches through a string for the first character that matches users predication
        ///
        template<typename PRED>
        size_t findFirstOf( PRED pred, size_t offset = 0, size_t count = 0 ) const
        {
            if( offset >= size() ) return NOT_FOUND;
            if( 0 == count ) count = size();
            if( offset + count > size() ) count = size() - offset;
            const char * p = mPtr + offset;
            for( size_t i = 0; i < count; ++i, ++p )
            {
                GN_ASSERT( *p );
                if( pred(*p) ) return offset + i;
            }
            return NOT_FOUND;
        }

        ///
        /// Searches through a string for the last character that matches any elements in user specified string
        ///
        size_t findLastOf( const CharType * s, size_t offset = 0, size_t count = 0 ) const
        {
            if( 0 == s || 0 == *s ) return NOT_FOUND;
            if( offset >= size() ) return NOT_FOUND;
            if( 0 == count ) count = size();
            if( offset + count > size() ) count = size() - offset;
            GN_ASSERT( count > 0 );
            const CharType * p = mPtr + offset + count - 1;
            for( size_t i = count; i > 0; --i, --p )
            {
                for( const CharType * t = s; *t; ++t )
                {
                    GN_ASSERT( *p && *t );
                    if( *p == *t ) return offset + i - 1;
                }
            }
            return NOT_FOUND;
        }

        ///
        /// get first character of the string. If string is empty, return 0.
        ///
        CharType first() const { return mPtr[0]; }

        ///
        /// printf-like string formatting
        ///
        const CharType * format( const CharType * fmt, ... )
        {
            va_list arglist;
            va_start( arglist, fmt );
            formatv( fmt, arglist );
            va_end( arglist );
            return mPtr;
        }

        ///
        /// printf-like string formatting(2)
        ///
        const CharType * formatv( const CharType * fmt, va_list args )
        {
            if( stringEmpty(fmt) )
            {
                clear();
            }
            else
            {
                CharType buf[16384];  // 16k should be enough in most cases
                stringVarPrintf( buf, 16384, fmt, args );
                buf[16383] = 0;
                assign( buf );
            }
            return mPtr;
        }

        ///
        /// get string caps
        ///
        size_t caps() const
        {
            if( NULL == mPtr )
            {
                return 0;
            }
            else
            {
                StringHeader * h = ((StringHeader*)mPtr) - 1;
                return h->caps;
            }
        }

        ///
        /// string hash
        ///
        uint64 hash() const { return stringHash( mPtr, size() ); }

        ///
        /// Insert a character at specific position
        ///
        void insert( size_t pos, CharType ch )
        {
            if( 0 == ch ) return;
            if( pos >= size() )
            {
                append( ch );
            }
            else
            {
                setCaps( size() + 1 );
                for( size_t i = size()+1; i > pos; --i )
                {
                    mPtr[i] = mPtr[i-1];
                }
                mPtr[pos] = ch;
                setSize( size() + 1 );
            }
        }

        ///
        /// get last character of the string. If string is empty, return 0.
        ///
        CharType last() const { return size()>0 ? mPtr[size()-1] : (CharType)0; }

        ///
        /// pop last charater
        ///
        void popback()
        {
            if( size() > 0 )
            {
                setSize( size() - 1 );
                mPtr[size()] = 0;
            }
        }

        ///
        /// Replace specific character with another
        ///
        void replace( CharType from, CharType to )
        {
            CharType * p = mPtr;
            for( size_t i = 0; i < size(); ++i, ++p )
            {
                if( from == *p ) *p = to;
            }
        }

        ///
        /// remove specific charactor at specific location
        ///
        void remove( size_t pos )
        {
            for( size_t i = pos; i < size(); ++i )
            {
                mPtr[i] = mPtr[i+1];
            }
            setSize( size() - 1 );
        }

        ///
        /// set string caps
        ///
        void setCaps( size_t newCaps )
        {
            if( NULL == mPtr )
            {
                newCaps = calcCaps( newCaps );
                mPtr = alloc( newCaps + 1 );
                mPtr[0] = 0;
                StringHeader * h = ((StringHeader*)mPtr) - 1;
                h->caps = newCaps;
                h->size = 0;
            }
            else if( caps() < newCaps )
            {
                GN_ASSERT( size() <= caps() );

                CharType * oldptr = mPtr;
                size_t oldsize = size();

                newCaps = calcCaps( newCaps );
                mPtr = alloc( newCaps + 1 );

                StringHeader * h = ((StringHeader*)mPtr) - 1;
                h->caps = newCaps;
                h->size = oldsize;

                ::memcpy( mPtr, oldptr, (oldsize + 1)*sizeof(CharType) );

                dealloc( oldptr );
            }
        }

        ///
        /// return string length in character, not including ending zero
        ///
        size_t size() const
        {
            GN_ASSERT( mPtr );
            StringHeader * h = ((StringHeader*)mPtr) - 1;
            return h->size;
        }

        ///
        /// Get sub string. (0==length) means to the end of original string.
        ///
        void subString( Str & result, size_t offset, size_t length ) const
        {
            if( offset >= size() ) { result.clear(); return; }
            if( 0 == length ) length = size();
            if( offset + length > size() ) length = size() - offset;
            result.assign( mPtr+offset, length );
        }

        ///
        /// Return sub string
        ///
        Str subString( size_t offset, size_t length ) const
        {
            Str ret;
            subString( ret, offset, length );
            return ret;
        }

        ///
        /// convert to all lower case
        ///
        void toLower()
        {
            CHAR * p = mPtr;
            CHAR * e = mPtr + size();
            for( ; p < e; ++p )
            {
                if( 'A' <= *p && *p <= 'Z' ) *p = (*p) - 'A' + 'a';
            }
        }

        ///
        /// convert to STL string(1)
        ///
        void toSTL( std::basic_string<CharType> & s ) const
        {
            s.assign( mPtr, size() );
        }

        ///
        /// convert to STL string(2)
        ///
        std::basic_string<CharType> toSTL() const
        {
            return std::basic_string<CharType>(mPtr,size());
        }

        ///
        /// convert to all upper case
        ///
        void toUpper()
        {
            CHAR * p = mPtr;
            CHAR * e = mPtr + size();
            for( ; p < e; ++p )
            {
                if( 'a' <= *p && *p <= 'z' ) *p = (*p) - 'a' + 'A';
            }
        }

        ///
        /// Trim characters for both side
        ///
        void trim( const CharType * ch, size_t len = 0 )
        {
            if( 0 == ch ) return;
            if( 0 == len ) len = stringLength( ch );
            trimRight( ch, len );
            trimLeft( ch, len );
        }

        ///
        /// Trim characters for both side
        ///
        void trim( CharType ch ) { trim( &ch, 1 ); }

        ///
        /// Trim left characters
        ///
        void trimLeft( const CharType * ch, size_t len = 0 )
        {
            if( 0 == ch ) return;
            if( 0 == len ) len = stringLength( ch );
            if( 0 == len ) return;
            CharType * p = mPtr;
            CharType * e = mPtr+size();
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
            setSize( e - p );
            for( size_t i = 0; i < size(); ++i )
            {
                mPtr[i] = p[i];
            }
            mPtr[size()] = 0;
        }

        ///
        /// Trim left characters
        ///
        void trimLeft( CharType ch ) { trimLeft( &ch, 1 ); }

        ///
        /// Trim right characters
        ///
        void trimRight( const CharType * ch, size_t len = 0 )
        {
            if( 0 == size() ) return;
            if( 0 == ch ) return;
            if( 0 == len ) len = stringLength( ch );
            if( 0 == len ) return;
            CharType * p = mPtr + size() - 1;
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
            setSize( p - mPtr + 1 );
        }

        ///
        /// Trim right characters
        ///
        void trimRight( CharType ch ) { trimRight( &ch, 1 ); }

        ///
        /// Trim right characters until meet the predication condition.
        ///
        template<typename PRED>
        void trimRightUntil( PRED pred )
        {
            if( 0 == size() ) return;
            CharType * p = mPtr + size() - 1;
            while( p >= mPtr && !pred(*p) )
            {
                *p = 0;
                --p;
            }
            setSize( p - mPtr + 1 );
        }

        ///
        /// type cast to C string
        ///
        operator const CharType *() const { return mPtr; }

        ///
        /// type cast to C string
        ///
        operator CharType *() { return mPtr; }

        ///
        /// assign operator
        ///
        Str & operator = ( const Str & s )
        {
            assign( s.mPtr, s.size() );
            return *this;
        }

        ///
        /// assign operator
        ///
        Str & operator = ( const CharType * s )
        {
            assign( s, stringLength<CharType>(s) );
            return *this;
        }

        ///
        /// += operator (1)
        ///
        Str & operator += ( const Str & s )
        {
            append( s );
            return *this;
        }

        ///
        /// += operator (2)
        ///
        Str & operator += ( const CharType * s )
        {
            append( s, 0 );
            return *this;
        }

        ///
        /// += operator (3)
        ///
        Str & operator += ( CharType ch )
        {
            append( ch );
            return *this;
        }

        ///
        /// += operator (4)
        ///
        Str & operator += ( std::basic_string<CharType> & s )
        {
            append( s.c_str(), 0 );
            return *this;
        }

        ///
        /// equality operator(1)
        ///
        friend bool operator == ( const CharType * s1, const Str & s2 )
        {
            return 0 == stringCompare( s1, s2.mPtr );
        }

        ///
        /// equality operator(2)
        ///
        friend bool operator == ( const Str & s1, const CharType * s2 )
        {
            return 0 == stringCompare( s1.mPtr, s2 );
        }

        ///
        /// equality operator(3)
        ///
        friend bool operator == ( const Str & s1, const Str & s2 )
        {
            return 0 == stringCompare( s1.mPtr, s2.mPtr );
        }

        ///
        /// unequality operator(1)
        ///
        friend bool operator != ( const CharType * s1, const Str & s2 )
        {
            return 0 != stringCompare( s1, s2.mPtr );
        }

        ///
        /// unequality operator(2)
        ///
        friend bool operator != ( const Str & s1, const CharType * s2 )
        {
            return 0 != stringCompare( s1.mPtr, s2 );
        }

        ///
        /// unequality operator(3)
        ///
        friend bool operator != ( const Str & s1, const Str & s2 )
        {
            return 0 != stringCompare( s1.mPtr, s2.mPtr );
        }

        ///
        /// less operator(1)
        ///
        friend bool operator < ( const CharType * s1, const Str & s2 )
        {
            return -1 == stringCompare( s1, s2.mPtr );
        }

        ///
        /// less operator(2)
        ///
        friend bool operator < ( const Str & s1, const CharType * s2 )
        {
            return -1 == stringCompare( s1.mPtr, s2 );
        }

        ///
        /// less operator(3)
        ///
        friend bool operator < ( const Str & s1, const Str & s2 )
        {
            return -1 == stringCompare( s1.mPtr, s2.mPtr );
        }


        ///
        /// concatnate operator(1)
        ///
        friend Str operator + ( const CharType * s1, const Str & s2 )
        {
            Str r(s1);
            r.append(s2);
            return r;
        }

        ///
        /// concatnate operator(2)
        ///
        friend Str operator + ( const Str & s1, const CharType * s2 )
        {
            Str r(s1);
            r.append(s2);
            return r;
        }

        ///
        /// concatnate operator(3)
        ///
        friend Str operator + ( const Str & s1, const Str & s2 )
        {
            Str r(s1);
            r.append(s2);
            return r;
        }

        ///
        /// concatnate operator(4)
        ///
        friend Str operator + ( const Str & s1, const std::basic_string<CharType> & s2 )
        {
            Str r(s1);
            r.append( s2.c_str() );
            return r;
        }

        ///
        /// concatnate operator(5)
        ///
        friend Str operator + ( const std::basic_string<CharType> & s1, const Str & s2 )
        {
            Str r(s2);
            r.append( s1.c_str() );
            return r;
        }

        ///
        /// Output to ostream
        ///
        friend std::ostream & operator << ( std::ostream & os, const Str & str )
        {
            os << str.cptr();
            return os;
        }

        ///
        /// string Hash Functor
        ///
        struct Hash
        {
            uint64 operator()( const Str & s ) const
            {
                return stringHash( s.mPtr, s.size() );
            }
        };

    private:

        struct StringHeader
        {
            size_t caps; //< How many characters can the string hold, not including the null end.
            size_t size; //< How many charecters in the string, not including the null end.
        };

        void setSize( size_t count )
        {
            GN_ASSERT(mPtr);
            StringHeader * h = ((StringHeader*)mPtr) - 1;
            h->size = count;
        }

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
            // ALLOCATOR:sAllocate only allocates raw memory buffer. No constuctors are invoked.
            // This is safe, as long as CharType is POD type.
            StringHeader * ptr = (StringHeader*)RAW_MEMORY_ALLOCATOR::sAllocate( sizeof(StringHeader) + sizeof(CharType) * (count + 1), sizeof(size_t) );
            return (CharType*)(ptr + 1);
        }

        static void dealloc( CharType * ptr )
        {
            // ALLOCATOR:sDeallocate frees memory without calling destructors.
            // This is safe, as long as CharType is POD type.
            if( ptr )
            {
                StringHeader * p = (StringHeader*)ptr;
                RAW_MEMORY_ALLOCATOR::sDeallocate( p - 1 );
            }
        }

        friend void wcs2mbs( Str<char> &, const wchar_t *, size_t );
        friend void mbs2wcs( Str<wchar_t> &, const char *, size_t );
    };

    // Implement static data member
    template <typename CHAR, typename ALLOC> Str<CHAR,ALLOC> Str<CHAR,ALLOC>::EMPTYSTR;

    ///
    /// multi-byte string class
    ///
    typedef Str<char> StrA;

    ///
    /// wide-char string class
    ///
    typedef Str<wchar_t> StrW;

    ///
    /// Fixed sized string that has no runtime memory allocation.
    ///
    template<size_t N, typename CHAR>
    class StackStr
    {
        typedef CHAR CharType;

        size_t mCount;    ///< How many charecters in the string, not including null end.
        CHAR   mBuf[N+1]; ///< Pre-allocated string buffer

        static size_t sValidateLength( size_t len ) { return len < N ? len : N; }

    public:

        /// \name ctor and dtor
        //@{
        StackStr() : mCount(0) { mBuf[0] = 0; }
        StackStr( const CHAR * s, size_t l = 0 ) : mCount(l)
        {
            if( 0 == s )
            {
                mCount = 0;
                mBuf[0] = 0;
            }
            else
            {
                if( 0 == l ) l = stringLength( s );
                memcpy( mBuf, s, sValidateLength(l) * sizeof(CHAR) );
            }
        }
        StackStr( const StackStr & s ) : mCount( s.mCount )
        {
            memcpy( mBuf, s.mBuf, sizeof(CHAR)*s.mCount );
        }
        StackStr( const Str<CHAR> & s )
        {
            memcpy( mBuf, s.cptr(), sizeof(CHAR) * sValidateLength(s.size()) );
        }
        //@}
    };

    //
    // Implmement algorithm described in paper "Fast Algorithms for Sorting and Searching Strings"
    //
    //      http://www.cs.princeton.edu/~rs/strings/)
    //
    // Here are some reference implementations:
    //
    //      http://www.codeproject.com/KB/recipes/tst.aspx
    //      http://meshula.net/wordpress/?p=183
    //
    // TODO: sorted leaf list
    //
    template<class CHAR, class T, StringCompareCase::ENUM CASE_COMPARE = StringCompareCase::SENSITIVE>
    class StringMap
    {
        // *****************************
        // public types
        // *****************************

    public:

        /// public key-value pair type.
        struct KeyValuePair
        {
            const CHAR * const key;   // Note: key is always an const.
            T                  value;

        protected:

            // default constructor
            KeyValuePair(const char * k, const T & v) : key(k), value(v) {}
        };

        // *****************************
        // public methods
        // *****************************

    public:

        /// default constructor
        StringMap() : mRoot(NULL), mCount(0)
        {
        }

        /// copy constructor
        StringMap( const StringMap & sm ) : mRoot(NULL), mCount(0)
        {
            doClone( sm );
        }

        /// destructor
        ~StringMap()
        {
            clear();
        }

        /// get first element in the map
        /// \note elements are _NOT_ sorted yet.
        const KeyValuePair * first() const { return doFirst(); }

        /// get first element in the map
        /// \note elements are _NOT_ sorted yet.
        KeyValuePair * first() { return doFirst(); }

        /// clear whole map
        void clear() { doClear(); }

        /// empty
        bool empty() const { return 0 == mCount; }

        /// Get next item
        /// \note elements are _NOT_ sorted yet.
        const KeyValuePair * next( const KeyValuePair * p ) const { return doNext( p ); }

        /// Get next item
        /// \note elements are _NOT_ sorted yet.
        KeyValuePair * next( const KeyValuePair * p ) { return doNext( p ); }

        /// erase by key
        void remove( const CHAR * text ) { doRemove( text ); }

        /// find
        const T * find( const CHAR * text ) const { return doFind( text ); }

        /// find
        T * find( const CHAR * text ) { return doFind( text ); }

        /// find
        KeyValuePair * findPair( const CHAR * text ) { return doFindPair( text ); }

        /// find
        const KeyValuePair * findPair( const CHAR * text ) const { return doFindPair( text ); }

        /// insert. Return the inserted key value pair or NULL.
        KeyValuePair * insert( const CHAR * text, const T & value ) { bool inserted; KeyValuePair * p = doFindOrInsert( text, value, inserted ); return inserted ? p : NULL; }

        /// return number of items in map
        size_t size() const { return mCount; }

        // *****************************
        // public operators
        // *****************************

    public:

        /// assignment
        StringMap & operator=( const StringMap & rhs ) { doClone( rhs ); return *this; }

        /// indexing operator
        T & operator[]( const CHAR * text ) { bool inserted; KeyValuePair * p = doFindOrInsert( text, T(), inserted ); GN_ASSERT(p); return p->value; }

        /// indexing operator
        const T & operator[]( const CHAR * text ) const { const KeyValuePair * p = doFindPair( text ); GN_ASSERT(p); return p->value; }

        // *****************************
        // private types
        // *****************************

    private:

        struct Leaf : public KeyValuePair
        {
            // double linked list fields
            Leaf * prev;
            Leaf * next;
            void * owner;

            Leaf( const CHAR * text, size_t textlen, const T & v )
                : KeyValuePair( (const char*)HeapMemory::alloc(textlen+1), v )
                , prev(NULL)
                , next(NULL)
                , owner(NULL)
            {
                if( NULL != KeyValuePair::key )
                {
                    memcpy( (char*)KeyValuePair::key, text, textlen+1 );
                }
            }

            ~Leaf()
            {
                HeapMemory::dealloc( (void*)KeyValuePair::key );
            }
        };

        struct Node
        {
            CHAR   splitchar; // 0 means a leaf node
            Node * parent;
            Node * lower;
            Node * equal;
            Node * higher;
            Leaf * leaf;  // valid only when splitchar == 0 (leaf node)
        };

        // *****************************
        // private data
        // *****************************

    private:

        Node * mRoot;
        size_t mCount; // number of items in map
        FixSizedRawMemoryPool<sizeof(Node)> mNodePool;
        ObjectPool<Leaf>                    mLeafPool;
        DoubleLinkedList<Leaf>              mLeafs;

        // *****************************
        // private methods
        // *****************************

    private:

        Node * allocNode()
        {
            return (Node*)mNodePool.alloc();
        }

        void freeNode( Node * n )
        {
            mNodePool.dealloc( n );
        }

        Leaf * allocLeaf( const CHAR * text, size_t textlen, const T & value )
        {
            Leaf * p = mLeafPool.allocUnconstructed();
            if( NULL == p ) return NULL;

            // call in-place new to construct the leaf
            new (p) Leaf( text, textlen, value );
            if( NULL == p->key )
            {
                mLeafPool.freeWithoutDeconstruct( p );
                return NULL;
            }

            return p;
        }

        void freeLeaf( Leaf * l )
        {
            GN_ASSERT( l );
            mLeafs.remove( l );
            mLeafPool.deconstructAndFree( l );
        }

        /// clear the whole map container
        void doClear()
        {
            mRoot = NULL;
            mCount = 0;
            mNodePool.freeAll();
            mLeafPool.deconstructAndFreeAll();

            // I know this is hacky. But it works.
            memset( &mLeafs, 0, sizeof(mLeafs) );
        }

        /// make itself a clone of another map
        void doClone( const StringMap & anotherMap )
        {
            // shortcut for cloning itself.
            if( this == &anotherMap ) return;

            // clear myself
            clear();

            // insert all items in another map to this map.
            for( const KeyValuePair * p = anotherMap.first(); NULL != p; p = anotherMap.next(p) )
            {
                insert( p->key, p->value );
            }
        }

        T * doFind( const CHAR * text ) const
        {
            Leaf * p = doFindPair( text );
            return p ? &p->value : NULL;
        }

        Leaf * doFindPair( const CHAR * text ) const
        {
            // check for NULL text pointer
            if( NULL == text )
            {
                static Logger * sLogger = getLogger("GN.base.StringMap");
                GN_WARN(sLogger)( "StringMap finding warning: NULL text!" );
                return NULL;
            }

            Node * p = mRoot;
            while( p )
            {
                int d;

                if( StringCompareCase::INSENSITIVE == CASE_COMPARE )
                {
                    // conver both to upper case
                    CHAR t = *text;
                    CHAR s = p->splitchar;
                    if( 'a' <= t && t <= 'z' ) t += 'A' - 'a';
                    if( 'a' <= s && s <= 'z' ) s += 'A' - 'a';
                    d = t - s;
                }
                else
                {
                    d = *text - p->splitchar;
                }

                if( 0 == d )
                {
                    if( 0 == *text )
                    {
                        // found!
                        GN_ASSERT( p->leaf );
                        return p->leaf;
                    }
                    else
                    {
                        p = p->equal;
                    }

                    ++text;
                }
                else if( d < 0 )
                {
                    p = p->lower;
                }
                else
                {
                    p = p->higher;
                }
            }

            // not found
            return NULL;
        }

        Leaf * doFindOrInsert( const CHAR * text, const T & value, bool & inserted )
        {
            inserted = false;

            // check for NULL text pointer
            if( NULL == text )
            {
                static Logger * sLogger = getLogger("GN.base.StringMap");
                GN_WARN(sLogger)( "Null text is not allowed!" );
                return NULL;
            }

            // store input text pointer
            const char * inputText = text;

            // search in existing nodes
            Node * parentNode = NULL;
            Node ** pp = &mRoot;
            while( NULL != *pp )
            {
                int d;

                if( StringCompareCase::INSENSITIVE == CASE_COMPARE )
                {
                    // conver both to upper case
                    CHAR t = *text;
                    CHAR s = (*pp)->splitchar;
                    if( 'a' <= t && t <= 'z' ) t += 'A' - 'a';
                    if( 'a' <= s && s <= 'z' ) s += 'A' - 'a';
                    d = t - s;
                }
                else
                {
                    d = *text - (*pp)->splitchar;
                }

                if( 0 == d )
                {
                    if( 0 == *text )
                    {
                        // The text exists already. Insertion failed.
                        GN_ASSERT( (*pp)->leaf );
                        inserted = false;
                        return (*pp)->leaf;
                    }
                    else
                    {
                        parentNode = *pp;
                        pp = &((*pp)->equal);
                    }

                    ++text;
                }
                else if( d < 0 )
                {
                    parentNode = *pp;
                    pp = &((*pp)->lower);
                }
                else
                {
                    parentNode = *pp;
                    pp = &((*pp)->higher);
                }
            }

            // No existing text found. Now insert new nodes for
            // each unmatched characters in input text.
            for(;;)
            {
                // create new node
                Node * newNode = allocNode();
                if( NULL == newNode )
                {
                    static Logger * sLogger = getLogger("GN.base.StringMap");
                    GN_ERROR(sLogger)( "out of memory!" );
                    return NULL;
                }

                // create new leaf if reaching the end of the text
                if( 0 == *text )
                {
                    // we reach the end of the text. Now create a new leaf.
                    newNode->leaf = allocLeaf( inputText, text - inputText, value );
                    if( NULL == newNode->leaf )
                    {
                        static Logger * sLogger = getLogger("GN.base.StringMap");
                        GN_ERROR(sLogger)( "out of memory!" );
                        return NULL;
                    }

                    // TODO: find the real "previous" leaf node
                    Leaf * previousNode = mLeafs.tail();

                    // insert the new leaf into linked list
                    // TODO: sort and insert
                    mLeafs.insertAfter( previousNode, newNode->leaf );

                    ++mCount;
                    inserted = true;
                }
                else
                {
                    newNode->leaf = 0;
                }

                // link new node into node tree
                newNode->parent = parentNode;
                newNode->splitchar = *text;
                newNode->lower = newNode->higher = newNode->equal = 0;
                *pp = newNode;

                // continue with next character or exit
                if( 0 == *text )
                {
                    GN_ASSERT((*pp)->leaf);
                    return (*pp)->leaf;
                }
                else
                {
                    parentNode = *pp;
                    pp = &((*pp)->equal);
                    ++text;
                }
            }
        }

        Leaf * doFirst() const
        {
            return mLeafs.head();
        }

        Leaf * doNext( const KeyValuePair * p ) const
        {
            if( NULL == p ) return NULL;

            Leaf * leaf = (Leaf*)p;

            if( leaf->owner != &mLeafs )
            {
                static Logger * sLogger = getLogger("GN.base.StringMap");
                GN_ERROR(sLogger)( "Input pointer does not belong to this string map." );
                return NULL;
            }

            return leaf->next;
        }

        Node * doRecursiveErase( Node * n, const CHAR * text )
        {
            GN_ASSERT( text );

            if( NULL == n ) return NULL;

            int d = *text - n->splitchar;

            if( d < 0 )
            {
                n->lower = doRecursiveErase( n->lower, text );
            }
            else if( d > 0 )
            {
                n->higher = doRecursiveErase( n->higher, text );
            }
            else if( 0 == *text )
            {
                // We reaches the end of the string. It must contain a leaf node.
                GN_ASSERT( n->leaf );

                // delete leaf node
                freeLeaf( n->leaf );
                n->leaf = NULL;

                --mCount;
            }
            else
            {
                n->equal = doRecursiveErase( n->equal, text+1 );
            }

            // free the node if it contains neither children or leaf
            if( 0 == n->lower && 0 == n->higher && 0 == n->equal && 0 == n->leaf )
            {
                freeNode( n );
                return NULL;
            }
            else
            {
                return n;
            }
        }

        void doRemove( const CHAR * text )
        {
            // check for NULL text pointer
            if( NULL == text )
            {
                static Logger * sLogger = getLogger("GN.base.StringMap");
                GN_WARN(sLogger)( "StringMap erasing warning: NULL text!" );
                return;
            }

            mRoot = doRecursiveErase( mRoot, text );
        }
    }; // End of StringMap class

    /// \name string -> number conversion
    ///
    ///  Returns number of characters that are sucessfully converted. Return 0 for failure.
    //@{

    size_t string2SignedInteger( sint64 & result, int bits, int base, const char * s );
    size_t string2UnsignedInteger( uint64 & result, int bits, int base, const char * s );

    template<typename T> inline size_t string2Integer( T & i, const char * s, int base = 10 )
    {
        size_t n;

        if( SignedType<T>::value )
        {
            sint64 s64;
            n = string2SignedInteger( s64, sizeof(T)*8, base, s );
            if( n > 0 ) i = (T)s64;
        }
        else
        {
            uint64 u64;
            n = string2UnsignedInteger( u64, sizeof(T)*8, base, s );
            if( n > 0 ) i = (T)u64;
        }

        return n;
    }

    template<typename T> T string2Integer( const char * s, T defaultValue, int base = 10 )
    {
        T result;
        if( 0 == string2Integer<T>( result, s, base ) )
        {
            return defaultValue;
        }
        else
        {
            return result;
        }
    }

    size_t string2Float( float & i, const char * s );

    size_t string2Double( double & i, const char * s );

    template<typename T> inline size_t string2Number( T & i, const char * s ) { return string2Integer<T>( i, s, 10 ); }
    template<> inline size_t string2Number<float>( float & i, const char * s ) { return string2Float( i, s ); }
    template<> inline size_t string2Number<double>( double & i, const char * s ) { return string2Double( i, s ); }
    template<typename T> T string2Number( const char * s, T defaultValue )
    {
        T result;
        if( string2Number<T>( result, s ) )
            return result;
        else
            return defaultValue;
    }

    ///
    /// Convert string to float array. String should be in format like:
    ///    float1, float2, float3, ...
    /// or:
    ///    float1 float2 float3 ...
    ///
    /// \return
    ///     Return count of floating filled into target buffer.
    ///
    ///     REVIEW: this function returns number of floats, while other function returns number of characters.
    ///
    size_t string2FloatArray( float * buffer, size_t maxCount, const char * str, size_t stringLength = 0 );

    //@}

    ///
    /// printf-like string format function.
    ///
    /// Similar as stringPrintf(...), but returns a string object
    ///
    inline StrA stringFormat( const char * fmt, ... )
    {
        StrA s;
        va_list arglist;
        va_start( arglist, fmt );
        s.formatv( fmt, arglist );
        va_end( arglist );
        return s;
    }

    ///
    /// printf-like string format function (wide-char version)
    ///
    /// Similar as stringPrintf(...), but returns a string object
    ///
    inline StrW stringFormat( const wchar_t * fmt, ... )
    {
        StrW s;
        va_list arglist;
        va_start( arglist, fmt );
        s.formatv( fmt, arglist );
        va_end( arglist );
        return s;
    }
}

// *****************************************************************************
//                 End of string.h
// *****************************************************************************
#endif // __GN_BASE_STRING_H__
