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
bool GN::str2SInt8( SInt8 & i, const char * s )
{
    if( strEmpty(s) ) return false;
    int ii;
    if( 1 != sscanf( s, "%i", &ii ) ) return false;
    i = (SInt8)ii;
    return true;
}

//
//
// -----------------------------------------------------------------------------
bool GN::str2UInt8( UInt8 & i, const char * s )
{
    if( strEmpty(s) ) return false;
    unsigned int ii;
    if( 1 != sscanf( s, "%u", &ii ) ) return false;
    i = (UInt8)ii;
    return true;
}


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
