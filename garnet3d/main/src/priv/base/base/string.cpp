#include "pch.h"

//
//
// -----------------------------------------------------------------------------
void
GN::strPrintf( char * buf, size_t bufSize, const char * fmt, ... )
{
    va_list arglist;
    va_start( arglist, fmt );
    strVarPrintf( buf, bufSize, fmt, arglist );
    va_end( arglist );
}

//
//
// -----------------------------------------------------------------------------
void
GN::strPrintf( wchar_t * buf, size_t bufSize, const wchar_t * fmt, ... )
{
    va_list arglist;
    va_start( arglist, fmt );
    strVarPrintf( buf, bufSize, fmt, arglist );
    va_end( arglist );
}

//
//
// -----------------------------------------------------------------------------
void
GN::strVarPrintf( char * buf, size_t bufSize, const char * fmt, va_list args )
{
    if ( buf && bufSize )
    {
#if GN_MSVC8
        _vsnprintf_s( buf, bufSize, _TRUNCATE, fmt, args );
#elif GN_MSVC
        _vsnprintf( buf, bufSize, fmt, args );
#else
        vsnprintf( buf, bufSize, fmt, args );
#endif
        buf[bufSize-1] = 0;
    }
}

//
//
// -----------------------------------------------------------------------------
void
GN::strVarPrintf( wchar_t * buf, size_t bufSize, const wchar_t * fmt, va_list args )
{
    if ( buf && bufSize )
    {
#if GN_MSVC8
        _vsnwprintf_s( buf, bufSize, _TRUNCATE, fmt, args );
#elif GN_MSVC
        _vsnwprintf( buf, bufSize, fmt, args );
#elif GN_CYGWIN
        buf[0] = 0; // no implementation on cygwin
#elif GN_POSIX
        vswprintf( buf, bufSize, fmt, args );
#endif
        buf[bufSize-1] = 0;
    }
}

// *****************************************************************************
//
// *****************************************************************************

#if GN_MSVC8
#define sscanf sscanf_s
#endif

//
//
// -----------------------------------------------------------------------------
bool GN::str2Int16( int16_t & i, const char * s )
{
    if( strEmpty(s) ) return false;
    return 1 == sscanf( s, "%hi", &i );
}

//
//
// -----------------------------------------------------------------------------
bool GN::str2Uint16( uint16_t & i, const char * s )
{
    if( strEmpty(s) ) return false;
    return 1 == sscanf( s, "%hu", &i );
}

//
//
// -----------------------------------------------------------------------------
bool GN::str2Int32( int32_t & i, const char * s )
{
    if( strEmpty(s) ) return false;
    return 1 == sscanf( s, "%i", &i );
}

//
//
// -----------------------------------------------------------------------------
bool GN::str2Uint32( uint32_t & i, const char * s )
{
    if( strEmpty(s) ) return false;
    return 1 == sscanf( s, "%u", &i );
}

//
//
// -----------------------------------------------------------------------------
bool GN::str2Int64( int64_t & i, const char * s )
{
    if( strEmpty(s) ) return false;
    return 1 == sscanf( s, "%I64i", &i );
}

//
//
// -----------------------------------------------------------------------------
bool GN::str2Uint64( uint64_t & i, const char * s )
{
    if( strEmpty(s) ) return false;
    return 1 == sscanf( s, "%I64u", &i );
}

//
//
// -----------------------------------------------------------------------------
bool GN::str2Float( float & i, const char * s )
{
    if( strEmpty(s) ) return false;
    return 1 == sscanf( s, "%f", &i );
}

//
//
// -----------------------------------------------------------------------------
bool GN::str2Double( double & i, const char * s )
{
    if( strEmpty(s) ) return false;
    return 1 == sscanf( s, "%lf", &i );
}

// *****************************************************************************
//
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::wcs2mbs( StrA & o, const wchar_t * i, size_t l )
{
    if ( 0 == i ) { o.clear(); return; }
    if ( 0 == l ) l = strLen(i);

    o.setCaps( l + 1 );
#if GN_MSVC8
    size_t ol;
    ::wcstombs_s( &ol, o.mPtr, l+1, i, l );
    l = ol;
#else
    l = ::wcstombs( o.mPtr, i, l );
#endif
    if( (size_t)-1 == l || 0 == l )
    {
        o.clear();
    }
    else
    {
        o.mPtr[l] = 0;
        o.mCount = l;
    }
}

//
//
// -----------------------------------------------------------------------------
size_t GN::mbs2wcs( wchar_t * o, size_t os, const char * i, size_t is )
{
    if( 0 == o || 0 == os )
    {
        // calculate required size
        if( 0 == i ) return 0;
        if( 0 == is ) is = strLen( i );
#if GN_MSVC8
        size_t sz;
        if( 0 != mbstowcs_s( &sz, 0, 0, i, is ) ) return 0;
        return sz;
#else
        size_t sz = mbstowcs( i, is );
        if( (size_t)-1 == sz ) return 0;
        return sz;
#endif
    }
    else
    {
        if( 0 == i )
        {
            *o = 0;
            return 0;
        }
        if( 0 == is ) is = strLen(i);
#if GN_MSVC8
        size_t sz;
        if( 0 != mbstowcs_s( &sz, o, os, i, is ) )
        {
            *o = 0;
            return 0;
        }
        return sz;
#else
        GN_TODO( "not being tested!" );
        std::vector<wchar_t> buf(is+1);
        size_t sz = mbstowcs( &buf[0], i, is );
        if( (size_t)-1 == sz )
        {
            *o = 0;
            return 0;
        }
        if( sz >= os ) sz = os - 1;
        memcpy( o, &buf[0], sz*2 );
        o[sz-1] = 0;
        return sz;
#endif
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::mbs2wcs( StrW & o, const char * i, size_t l )
{
    if ( 0 == i ) { o.clear(); return; }
    if ( 0 == l ) l = strLen(i);

    o.setCaps( l + 1 );
#if GN_MSVC8
    size_t ol;
    ::mbstowcs_s( &ol, o.mPtr, l+1, i, l );
    l = ol;
#else
    l = ::mbstowcs( o.mPtr, i, l );
#endif
    if( (size_t)-1 == l || 0 == l )
    {
        o.clear();
    }
    else
    {
        o.mPtr[l] = 0;
        o.mCount = l;
    }
}
