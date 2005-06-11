#include "pch.h"

#if GN_MSVC
#define vsnprintf  _vsnprintf
#define vsnwprintf _vsnwprintf
#endif

//
//
// -----------------------------------------------------------------------------
void
GN::strPrintf( char * buf, size_t bufSize, const char * fmt, va_list args )
{
    if ( buf && bufSize )
    {
        vsnprintf( buf, bufSize, fmt, args );
        buf[bufSize-1] = 0;
    }
}

//
//
// -----------------------------------------------------------------------------
void
GN::strPrintf( wchar_t * buf, size_t bufSize, const wchar_t * fmt, va_list args )
{
    if ( buf && bufSize )
    {
    #if GN_CYGWIN
        buf[0] = 0; // no implementation on cygwin
    #else
        vsnwprintf( buf, bufSize, fmt, args );
        buf[bufSize-1] = 0;
    #endif
    }
}

//
//
// -----------------------------------------------------------------------------
GN::StrA GN::wcs2mbs( const wchar_t * i, size_t l )
{
    StrA o;
    wcs2mbs( o, i, l );
    return o;
}

//
//
// -----------------------------------------------------------------------------
void GN::wcs2mbs( StrA & o, const wchar_t * i, size_t l )
{
    if ( 0 == i ) { o.clear(); return; }
    if ( 0 == l ) l = strLen(i);

    o.setCaps( l + 1 );
    l = ::wcstombs( &o[0], i, l );
    if( (size_t)-1 == l || 0 == l )
    {
        o.clear();
    }
    else
    {
        o[l] = 0;
        o.mLen = l;
    }
}

//
//
// -----------------------------------------------------------------------------
GN::StrW GN::mbs2wcs( const char * i, size_t l )
{
    StrW o;
    mbs2wcs( o, i, l );
    return o;
}

//
//
// -----------------------------------------------------------------------------
void GN::mbs2wcs( StrW & o, const char * i, size_t l )
{
    if ( 0 == i ) { o.clear(); return; }
    if ( 0 == l ) l = strLen(i);

    o.setCaps( l + 1 );
    l = ::mbstowcs( &o[0], i, l );
    if( (size_t)-1 == l || 0 == l )
    {
        o.clear();
    }
    else
    {
        o[l] = 0;
        o.mLen = l;
    }
}
