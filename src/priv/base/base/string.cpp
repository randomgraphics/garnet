#include "pch.h"

//
//
// -----------------------------------------------------------------------------
void
GN::StringPrintf( char * buf, size_t bufSize, const char * fmt, ... )
{
    va_list arglist;
    va_start( arglist, fmt );
    StringVarPrintf( buf, bufSize, fmt, arglist );
    va_end( arglist );
}

//
//
// -----------------------------------------------------------------------------
void
GN::StringPrintf( wchar_t * buf, size_t bufSize, const wchar_t * fmt, ... )
{
    va_list arglist;
    va_start( arglist, fmt );
    StringVarPrintf( buf, bufSize, fmt, arglist );
    va_end( arglist );
}

//
//
// -----------------------------------------------------------------------------
void
GN::StringVarPrintf( char * buf, size_t bufSize, const char * fmt, va_list args )
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
GN::StringVarPrintf( wchar_t * buf, size_t bufSize, const wchar_t * fmt, va_list args )
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

//
//
// -----------------------------------------------------------------------------
size_t GN::String2SignedInteger( SInt64 & result, int bits, int base, const char * s )
{
    // check invalid parameters
    if( bits < 2 && bits > 64 ) return 0;
    if( base < 2 ) return 0;
    if( IsStringEmpty(s) ) return 0;

    char * e;
    SInt64 s64 = _strtoi64( s, &e, base );

    if( 0 != errno || 0 == s64 && s == e )
    {
        return 0;
    }

    // check for overflow
    SInt64 maxval = ( 1 << ( bits - 1 ) ) - 1;
    SInt64 minval = ~maxval;
    if( s64 < minval || s64 > maxval ) return 0;

    // success
    result = s64;
    return e - s;
}

//
//
// -----------------------------------------------------------------------------
size_t GN::String2UnsignedInteger( UInt64 & result, int bits, int base, const char * s )
{
    // check invalid parameters
    if( bits < 2 && bits > 64 ) return 0;
    if( base < 2 ) return 0;
    if( IsStringEmpty(s) ) return 0;

    char * e;
    UInt64 u64 = _strtoui64( s, &e, base );

    if( 0 != errno || 0 == u64 && s == e )
    {
        return 0;
    }

    // check for overflow
    UInt64 maxval = ( 1 << bits ) - 1;
    if( u64 > maxval ) return 0;

    // success
    result = u64;
    return e - s;
}

//
//
// -----------------------------------------------------------------------------
size_t GN::String2Float( float & i, const char * s )
{
    double d;
    size_t n = String2Double( d, s );

    if( 0 == n ) return 0;

    if( d < -FLT_MAX ) i = -FLT_MAX;
    else if( d > FLT_MAX ) i = FLT_MAX;
    else i = (float)d;

    return n;
}

//
//
// -----------------------------------------------------------------------------
size_t GN::String2Double( double & i, const char * s )
{
    if( IsStringEmpty(s) ) return 0;

    char * e;
    double d = strtod( s, &e );

    if( 0 != errno || 0 == d && s == e )
    {
        return 0;
    }
    else
    {
        i = d;
        return e - s;
    }
}

//
//
// -----------------------------------------------------------------------------
size_t GN::String2FloatArray( float * buffer, size_t maxCount, const char * str, size_t stringLength )
{
    if( NULL == buffer ) return 0;
    if( IsStringEmpty(str) ) return 0;

    float * bufbegin = buffer;
    float * bufend = buffer + maxCount;
    const char * strend = str + stringLength;

    while( buffer < bufend && str < strend )
    {
        size_t n = String2Float( *buffer, str );

        if( 0 == n ) break;

        // next float
        ++buffer;
        str += n;
    }

    return buffer - bufbegin;
}
