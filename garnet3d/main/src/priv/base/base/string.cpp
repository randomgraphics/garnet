#include "pch.h"
#include <pcrecpp.h>

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
bool GN::str2SInt16( SInt16 & i, const char * s )
{
    if( strEmpty(s) ) return false;
    return 1 == sscanf( s, "%hi", &i );
}

//
//
// -----------------------------------------------------------------------------
bool GN::str2UInt16( UInt16 & i, const char * s )
{
    if( strEmpty(s) ) return false;
    return 1 == sscanf( s, "%hu", &i );
}

//
//
// -----------------------------------------------------------------------------
bool GN::str2SInt32( SInt32 & i, const char * s )
{
    if( strEmpty(s) ) return false;
    return 1 == sscanf( s, "%li", (long*)&i );
}

//
//
// -----------------------------------------------------------------------------
bool GN::str2UInt32( UInt32 & i, const char * s )
{
    if( strEmpty(s) ) return false;
    return 1 == sscanf( s, "%lu", (unsigned long*)&i );
}

//
//
// -----------------------------------------------------------------------------
bool GN::str2SInt64( SInt64 & i, const char * s )
{
    if( strEmpty(s) ) return false;
    long long int lli;
    if( 1 != sscanf( s, "%lli", &lli ) ) return false;
    i = lli;
    return true;
}

//
//
// -----------------------------------------------------------------------------
bool GN::str2UInt64( UInt64 & i, const char * s )
{
    if( strEmpty(s) ) return false;
    unsigned long long int llu;
    if( 1 != sscanf( s, "%llu", &llu ) ) return false;
    i = llu;
    return true;
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

//
//
// -----------------------------------------------------------------------------
size_t GN::str2Floats( float * buffer, size_t count, const char * str, size_t stringLength )
{
    if( strEmpty(str) ) return 0;

    static pcrecpp::RE re( "\\s*([+-]?\\s*([0-9]+(\\.[0-9]*)?|[0-9]*\\.[0-9]+)([eE][+-]?[0-9]+)?)f?\\s*,?\\s*" );

    if( 0 == stringLength ) stringLength = strLen( str );
    pcrecpp::StringPiece text( str, (int)stringLength );

    std::string substring;
    for( size_t i = 0; i < count; ++i )
    {
        if( !re.Consume( &text, &substring ) ||
            !str2Float( *buffer, substring.c_str() ) )
        {
            return i;
        }

        ++buffer; // next float
    }

    // success
    return count;
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

    size_t n = wcs.size() + 1;

    if( o )
    {
        if( os > n )
        {
            memcpy( o, wcs.cptr(), n );
            GN_ASSERT( 0 == o[n-1] );
            return n;
        }
        else if( os > 0 )
        {
            memcpy( o, wcs.cptr(), sizeof(wchar_t) * (os-1) );
            o[os-1] = 0;
            return os;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return n;
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
