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
    StrW wcs;
    mbs2wcs( wcs, i, is );
    if( o )
    {
        if( os > wcs.size() )
        {
            memcpy( o, wcs.cptr(), sizeof(wchar_t)*wcs.size() );
            o[wcs.size()] = 0;
            return wcs.size();
        }
        else if( os > 0 )
        {
            --os;
            memcpy( o, wcs.cptr(), sizeof(wchar_t)*os );
            o[os] = 0;
            return os;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return wcs.size();
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
    if( 0 != ::mbstowcs_s( &ol, o.mPtr, l+1, i, l ) )
    {
        o.clear();
    }
    else
    {
        o.mPtr[ol] = 0;
        while( ol > 0 && 0 == o.mPtr[ol] ) --ol;
        o.mCount = ol+1;
    }
#else
    l = ::mbstowcs( o.mPtr, i, l );
    if( (size_t)-1 == l || 0 == l )
    {
        o.clear();
    }
    else
    {
        o.mPtr[l] = 0;
        o.mCount = l;
    }
#endif
}
