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
    /// if maxLen > 0, then return math::GetMin(maxLen,realLength).
    ///
    template<typename CHAR>
    inline size_t StringLength( const CHAR * s, size_t maxLen = 0 )
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
    inline int StringCompare( const CHAR * s1, const CHAR * s2 )
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
    inline int StringCompare( const CHAR * s1, const CHAR * s2, size_t maxLength )
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
    inline int StringCompareI( const CHAR * s1, const CHAR * s2 )
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
    inline int StringCompareI( const CHAR * s1, const CHAR * s2, size_t maxLength )
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
    inline bool IsStringEmpty( const CHAR * s )
    {
        return 0 == s || 0 == s[0];
    }

    ///
    /// safe sprintf. This function always outputs null-terminated string,
    /// like StringCchPrintf(...)
    ///
    void
    StringPrintf(
        char *       buf,
        size_t       bufSizeInChar,
        const char * fmt,
        ... );

    ///
    /// safe sprintf. This function always outputs null-terminated string,
    /// like StringCchPrintf(...)
    ///
    void
    StringPrintf(
        wchar_t *       buf,
        size_t          bufSizeInWchar,
        const wchar_t * fmt,
        ... );

    ///
    /// safe sprintf. This function always outputs null-terminated string,
    /// like StringCchPrintf(...)
    ///
    void
    StringVarPrintf(
        char *       buf,
        size_t       bufSizeInChar,
        const char * fmt,
        va_list      args );

    ///
    /// printf-like format string (wide-char)
    ///
    void
    StringVarPrintf(
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
    inline UInt64 StringHash( const CHAR * s, size_t length = 0 )
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
    /// Custom string class
    ///
    template<typename CHAR, typename ALLOCATOR = StlAllocator<CHAR> >
    class Str
    {
        typedef CHAR CharType;
        typedef ALLOCATOR AllocatorType;

        CharType * mPtr;   ///< string buffer pointer.
        size_t     mCount; ///< How many charecters in the string, not including null End.
        size_t     mCaps;  ///< How many characters can we hold, not including null End?

    public:

        ///
        /// Instance of Empty string
        ///
        static Str EMPTYSTR;

        ///
        /// indicate serach failure.
        ///
        static const size_t NOT_FOUND = (size_t)-1;

        ///
        /// default constructor
        ///
        Str() : mCount(0), mCaps(0)
        {
            mPtr = alloc(mCaps);
            mPtr[0] = 0;
        }

        ///
        /// copy constructor
        ///
        Str( const Str & s ) : mCount(s.mCount), mCaps(CalculateCaps(s.mCount))
        {
            mPtr = alloc(mCaps);
            ::memcpy( mPtr, s.mPtr, (mCount+1)*sizeof(CharType) );
        }

        ///
        /// copy constructor from c-style string
        ///
        Str( const CharType * s, size_t l = 0 )
        {
            if( 0 == s )
            {
                mCaps = 0;
                mCount = 0;
                mPtr = alloc(mCaps);
                mPtr[0] = 0;
            }
            else
            {
                l = StringLength<CharType>(s,l);
                mCaps = CalculateCaps(l);
                mPtr = alloc(mCaps);
                mCount = l;
                ::memcpy( mPtr, s, l*sizeof(CharType) );
                mPtr[l] = 0;
            }
        }

        ///
        /// destructor
        ///
        ~Str()
        {
            dealloc( mPtr, mCaps );
        }

        ///
        /// Append to this string
        ///
        void Append( const CharType * s, size_t l = 0 )
        {
            if( 0 == s ) return;
            l = StringLength<CharType>(s,l);
            SetCaps( mCount + l );
            ::memcpy( mPtr+mCount, s, l*sizeof(CharType) );
            mCount += l;
            mPtr[mCount] = 0;
        }

        ///
        /// Append to this string
        ///
        void Append( const Str & s )
        {
            if( s.Empty() ) return;
            size_t l = s.Size();
            SetCaps( mCount + l );
            ::memcpy( mPtr+mCount, s.ToRawPtr(), l*sizeof(CharType) );
            mCount += l;
            mPtr[mCount] = 0;
        }

        ///
        /// Append to this string
        ///
        void Append( CharType ch )
        {
            if( 0 == ch ) return;
            SetCaps( mCount + 1 );
            mPtr[mCount] = ch;
            ++mCount;
            mPtr[mCount] = 0;
        }

        ///
        /// Assign value to string class
        ///
        void Assign( const CharType * s, size_t l = 0 )
        {
            if( 0 == s )
            {
                mPtr[0] = 0;
                mCount = 0;
            }
            else
            {
                l = StringLength<CharType>(s,l);
                SetCaps(l);
                mCount = l;
                ::memcpy( mPtr, s, l*sizeof(CharType) );
                mPtr[l] = 0;
            }
        }

        ///
        /// Begin iterator(1)
        ///
        CharType * Begin() { return mPtr; }

        ///
        /// Begin iterator(2)
        ///
        const CharType * Begin() const { return mPtr; }

        ///
        /// Clear to Empty string
        ///
        void Clear()
        {
            mPtr[0] = 0;
            mCount = 0;
        }

        ///
        /// Empty string or not?
        ///
        bool Empty() const { return 0 == mCount; }

        ///
        /// Begin iterator(1)
        ///
        CharType * End() { return mPtr+mCount; }

        ///
        /// Begin iterator(2)
        ///
        const CharType * End() const { return mPtr+mCount; }

        ///
        /// Searches through a string for the GetFirst character that matches any elements in user specified string
        ///
        /// \param s
        ///     User specified search pattern
        /// \param offset, count
        ///     Range of the search. (count==0) means to the End of input string.
        /// \return
        ///     Return index of the character of GetFirst occurance or NOT_FOUND.
        ///
        size_t FindFirstOf( const CharType * s, size_t offset = 0, size_t count = 0 ) const
        {
            if( 0 == s || 0 == *s ) return NOT_FOUND;
            if( offset >= mCount ) return NOT_FOUND;
            if( 0 == count ) count = mCount;
            if( offset + count > mCount ) count = mCount - offset;
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
        /// Searches through a string for the GetFirst character that not any elements of user specifed string.
        ///
        size_t FindFirstNotOf( const CharType * s, size_t offset = 0, size_t count = 0 ) const
        {
            if( 0 == s || 0 == *s ) return NOT_FOUND;
            if( offset >= mCount ) return NOT_FOUND;
            if( 0 == count ) count = mCount;
            if( offset + count > mCount ) count = mCount - offset;
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
        /// Searches through a string for the GetFirst character that matches users predication
        ///
        template<typename PRED>
        size_t FindFirstOf( PRED pred, size_t offset = 0, size_t count = 0 ) const
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

        ///
        /// Searches through a string for the GetLast character that matches any elements in user specified string
        ///
        size_t FindLastOf( const CharType * s, size_t offset = 0, size_t count = 0 ) const
        {
            if( 0 == s || 0 == *s ) return NOT_FOUND;
            if( offset >= mCount ) return NOT_FOUND;
            if( 0 == count ) count = mCount;
            if( offset + count > mCount ) count = mCount - offset;
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
        /// printf-like string formatting
        ///
        const CharType * Format( const CharType * fmt, ... )
        {
            va_list arglist;
            va_start( arglist, fmt );
            FormatV( fmt, arglist );
            va_end( arglist );
            return mPtr;
        }

        ///
        /// printf-like string formatting(2)
        ///
        const CharType * FormatV( const CharType * fmt, va_list args )
        {
            if( IsStringEmpty(fmt) )
            {
                Clear();
            }
            else
            {
                CharType buf[16384];  // 16k should be enough in most cases
                StringVarPrintf( buf, 16384, fmt, args );
                buf[16383] = 0;
                Assign( buf );
            }
            return mPtr;
        }

        ///
        /// get string caps
        ///
        size_t GetCaps() const { return mCaps; }

        ///
        /// get GetFirst character of the string. If string is Empty, return 0.
        ///
        CharType GetFirst() const { return mPtr[0]; }

        ///
        /// get GetLast character of the string. If string is Empty, return 0.
        ///
        CharType GetLast() const { return mCount>0 ? mPtr[mCount-1] : (CharType)0; }

        ///
        /// string Hash
        ///
        UInt64 Hash() const { return StringHash( mPtr, mCount ); }

        ///
        /// Insert a character at specific position
        ///
        void Insert( size_t pos, CharType ch )
        {
            if( 0 == ch ) return;
            if( pos >= mCount )
            {
                Append( ch );
            }
            else
            {
                SetCaps( mCount + 1 );
                for( size_t i = mCount+1; i > pos; --i )
                {
                    mPtr[i] = mPtr[i-1];
                }
                mPtr[pos] = ch;
                ++mCount;
            }
        }

        ///
        /// pop GetLast charater
        ///
        void PopBack()
        {
            if( mCount > 0 )
            {
                --mCount;
                mPtr[mCount] = 0;
            }
        }

        ///
        /// Replace specific character with another
        ///
        void Replace( CharType from, CharType to )
        {
            CharType * p = mPtr;
            for( size_t i = 0; i < mCount; ++i, ++p )
            {
                if( from == *p ) *p = to;
            }
        }

        ///
        /// Remove specific charactor at specific location
        ///
        void Remove( size_t pos )
        {
            for( size_t i = pos; i < mCount; ++i )
            {
                mPtr[i] = mPtr[i+1];
            }
            --mCount;
        }

        ///
        /// set string caps
        ///
        void SetCaps( size_t newCaps )
        {
            if( mCaps >= newCaps ) return;
            size_t oldCaps = mCaps;
            mCaps = CalculateCaps( newCaps );
            CharType * newPtr = alloc( mCaps );
            ::memcpy( newPtr, mPtr, sizeof(CharType)*(mCount+1) );
            dealloc( mPtr, oldCaps );
            mPtr = newPtr;
        }

        ///
        /// return string length in character, not including ending zero
        ///
        size_t Size() const { return mCount; }

        ///
        /// Get sub string. (0==length) means to the End of original string.
        ///
        void SubString( Str & result, size_t offset, size_t length ) const
        {
            if( offset >= mCount ) { result.Clear(); return; }
            if( 0 == length ) length = mCount;
            if( offset + length > mCount ) length = mCount - offset;
            result.Assign( mPtr+offset, length );
        }

        ///
        /// Return sub string
        ///
        Str SubString( size_t offset, size_t length ) const
        {
            Str ret;
            SubString( ret, offset, length );
            return ret;
        }

        ///
        /// convert to all lower case
        ///
        void ToLowerCase()
        {
            CHAR * p = mPtr;
            CHAR * e = mPtr + mCount;
            for( ; p < e; ++p )
            {
                if( 'A' <= *p && *p <= 'Z' ) *p = (*p) - 'A' + 'a';
            }
        }

        ///
        /// return c-style const char pointer
        ///
        const CharType * ToRawPtr() const { return mPtr; }

        ///
        /// convert to all upper case
        ///
        void ToUpperCase()
        {
            CHAR * p = mPtr;
            CHAR * e = mPtr + mCount;
            for( ; p < e; ++p )
            {
                if( 'a' <= *p && *p <= 'z' ) *p = (*p) - 'a' + 'A';
            }
        }

        ///
        /// Trim characters for both side
        ///
        void Trim( const CharType * ch, size_t len = 0 )
        {
            if( 0 == ch ) return;
            if( 0 == len ) len = StringLength( ch );
            TrimRight( ch, len );
            TrimLeft( ch, len );
        }

        ///
        /// Trim characters for both side
        ///
        void Trim( CharType ch ) { Trim( &ch, 1 ); }

        ///
        /// Trim left characters
        ///
        void TrimLeft( const CharType * ch, size_t len = 0 )
        {
            if( 0 == ch ) return;
            if( 0 == len ) len = StringLength( ch );
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

        ///
        /// Trim left characters
        ///
        void TrimLeft( CharType ch ) { TrimLeft( &ch, 1 ); }

        ///
        /// Trim right characters
        ///
        void TrimRight( const CharType * ch, size_t len = 0 )
        {
            if( 0 == mCount ) return;
            if( 0 == ch ) return;
            if( 0 == len ) len = StringLength( ch );
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

        ///
        /// Trim right characters
        ///
        void TrimRight( CharType ch ) { TrimRight( &ch, 1 ); }

        ///
        /// Trim right characters until meet the predication condition.
        ///
        template<typename PRED>
        void TrimRightUntil( PRED pred )
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

        ///
        /// type cast to C string
        ///
        operator const CharType *() const { return mPtr; }

        ///
        /// type cast to C string
        ///
        operator CharType *() { return mPtr; }

        ///
        /// Assign operator
        ///
        Str & operator = ( const Str & s )
        {
            Assign( s.mPtr, s.mCount );
            return *this;
        }

        ///
        /// Assign operator
        ///
        Str & operator = ( const CharType * s )
        {
            Assign( s, StringLength<CharType>(s) );
            return *this;
        }

        ///
        /// += operator (1)
        ///
        Str & operator += ( const Str & s )
        {
            Append( s );
            return *this;
        }

        ///
        /// += operator (2)
        ///
        Str & operator += ( const CharType * s )
        {
            Append( s, 0 );
            return *this;
        }

        ///
        /// += operator (3)
        ///
        Str & operator += ( CharType ch )
        {
            Append( ch );
            return *this;
        }

        ///
        /// += operator (4)
        ///
        Str & operator += ( std::basic_string<CharType> & s )
        {
            Append( s.c_str(), 0 );
            return *this;
        }

        ///
        /// equality operator(1)
        ///
        friend bool operator == ( const CharType * s1, const Str & s2 )
        {
            return 0 == StringCompare( s1, s2.mPtr );
        }

        ///
        /// equality operator(2)
        ///
        friend bool operator == ( const Str & s1, const CharType * s2 )
        {
            return 0 == StringCompare( s1.mPtr, s2 );
        }

        ///
        /// equality operator(3)
        ///
        friend bool operator == ( const Str & s1, const Str & s2 )
        {
            return 0 == StringCompare( s1.mPtr, s2.mPtr );
        }

        ///
        /// unequality operator(1)
        ///
        friend bool operator != ( const CharType * s1, const Str & s2 )
        {
            return 0 != StringCompare( s1, s2.mPtr );
        }

        ///
        /// unequality operator(2)
        ///
        friend bool operator != ( const Str & s1, const CharType * s2 )
        {
            return 0 != StringCompare( s1.mPtr, s2 );
        }

        ///
        /// unequality operator(3)
        ///
        friend bool operator != ( const Str & s1, const Str & s2 )
        {
            return 0 != StringCompare( s1.mPtr, s2.mPtr );
        }

        ///
        /// less operator(1)
        ///
        friend bool operator < ( const CharType * s1, const Str & s2 )
        {
            return -1 == StringCompare( s1, s2.mPtr );
        }

        ///
        /// less operator(2)
        ///
        friend bool operator < ( const Str & s1, const CharType * s2 )
        {
            return -1 == StringCompare( s1.mPtr, s2 );
        }

        ///
        /// less operator(3)
        ///
        friend bool operator < ( const Str & s1, const Str & s2 )
        {
            return -1 == StringCompare( s1.mPtr, s2.mPtr );
        }


        ///
        /// concatnate operator(1)
        ///
        friend Str operator + ( const CharType * s1, const Str & s2 )
        {
            Str r(s1);
            r.Append(s2);
            return r;
        }

        ///
        /// concatnate operator(2)
        ///
        friend Str operator + ( const Str & s1, const CharType * s2 )
        {
            Str r(s1);
            r.Append(s2);
            return r;
        }

        ///
        /// concatnate operator(3)
        ///
        friend Str operator + ( const Str & s1, const Str & s2 )
        {
            Str r(s1);
            r.Append(s2);
            return r;
        }

        ///
        /// concatnate operator(4)
        ///
        friend Str operator + ( const Str & s1, const std::basic_string<CharType> & s2 )
        {
            Str r(s1);
            r.Append( s2.c_str() );
            return r;
        }

        ///
        /// concatnate operator(5)
        ///
        friend Str operator + ( const std::basic_string<CharType> & s1, const Str & s2 )
        {
            Str r(s2);
            r.Append( s1.c_str() );
            return r;
        }

        ///
        /// Output to ostream
        ///
        friend std::ostream & operator << ( std::ostream & os, const Str & str )
        {
            os << str.ToRawPtr();
            return os;
        }

    private:

        // align caps to 2^n-1
        size_t CalculateCaps( size_t count )
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
            AllocatorType a;
            return a.allocate( count + 1 );
        }

        static void dealloc( CharType * ptr, size_t count )
        {
            AllocatorType a;
            a.deallocate( ptr, count + 1 );
        }

        friend Str<char> Wcs2Mbs( const wchar_t *, size_t );
        friend void Wcs2Mbs( Str<char> &, const wchar_t *, size_t );
        friend Str<wchar_t> Mbs2Wcs( const char *, size_t );
        friend void Mbs2Wcs( Str<wchar_t> &, const char *, size_t );
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
                if( 0 == l ) l = StringLength( s );
                memcpy( mBuf, s, sValidateLength(l) * sizeof(CHAR) );
            }
        }
        StackStr( const StackStr & s ) : mCount( s.mCount )
        {
            memcpy( mBuf, s.mBuf, sizeof(CHAR)*s.mCount );
        }
        StackStr( const Str<CHAR> & s )
        {
            memcpy( mBuf, s.ToRawPtr(), sizeof(CHAR) * sValidateLength(s.Size()) );
        }
        //@}
    };

    /// \name string -> number conversion
    ///
    ///  Returns number of characters that are sucessfully scanned. Return 0 for failure.
    //@{

    size_t String2SignedInteger( SInt64 & result, int bits, int base, const char * s );
    size_t String2UnsignedInteger( UInt64 & result, int bits, int base, const char * s );

    template<typename T> size_t String2Integer( T & i, const char * s, int base = 10 )
    {
        size_t n;

        if( SignedType<T>::value )
        {
            SInt64 s64;
            n = String2SignedInteger( s64, sizeof(T)*8, base, s );
            if( n > 0 ) i = (T)s64;
        }
        else
        {
            UInt64 u64;
            n = String2UnsignedInteger( u64, sizeof(T)*8, base, s );
            if( n > 0 ) i = (T)u64;
        }

        return n;
    }

    template<typename T> T String2Integer( const char * s, T defaultValue, int base = 10 )
    {
        T result;
        if( 0 == String2Integer<T>( result, s, base ) )
        {
            return defaultValue;
        }
        else
        {
            return result;
        }
    }

    size_t String2Float( float & i, const char * s );

    size_t String2Double( double & i, const char * s );

    template<typename T> size_t String2Number( T & i, const char * s ) { return String2Integer<T>( i, s, 10 ); }
    template<> inline size_t String2Number<float>( float & i, const char * s ) { return String2Float( i, s ); }
    template<> inline size_t String2Number<double>( double & i, const char * s ) { return String2Double( i, s ); }

    ///
    /// Convert string to float array. String should be in format like:
    ///    float1, float2, float3, ...
    /// or:
    ///    float1 float2 float3 ...
    ///
    /// \return
    ///     Return count of floating filled into target buffer.
    ///
    size_t String2FloatArray( float * buffer, size_t maxCount, const char * str, size_t stringLength = 0 );

    //@}

    ///
    /// printf-like string format function.
    ///
    /// Similar as StringPrintf(...), but returns a string object
    ///
    inline StrA StringFormat( const char * fmt, ... )
    {
        StrA s;
        va_list arglist;
        va_start( arglist, fmt );
        s.FormatV( fmt, arglist );
        va_end( arglist );
        return s;
    }

    ///
    /// printf-like string format function (wide-char version)
    ///
    /// Similar as StringPrintf(...), but returns a string object
    ///
    inline StrW StringFormat( const wchar_t * fmt, ... )
    {
        StrW s;
        va_list arglist;
        va_start( arglist, fmt );
        s.FormatV( fmt, arglist );
        va_end( arglist );
        return s;
    }
}

// *****************************************************************************
//                 End of string.h
// *****************************************************************************
#endif // __GN_BASE_STRING_H__
