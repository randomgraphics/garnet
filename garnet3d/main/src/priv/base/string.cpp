#include "pch.h"

#define VS8 (GN_MSVC&&_MSC_VER>=1400)

//
//
// -----------------------------------------------------------------------------
void
GN::strPrintf( char * buf, size_t bufSize, const char * fmt, va_list args )
{
    if ( buf && bufSize )
    {
#if VS8
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
GN::strPrintf( wchar_t * buf, size_t bufSize, const wchar_t * fmt, va_list args )
{
    if ( buf && bufSize )
    {
#if VS8
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

//
//
// -----------------------------------------------------------------------------
void GN::wcs2mbs( StrA & o, const wchar_t * i, size_t l )
{
    if ( 0 == i ) { o.clear(); return; }
    if ( 0 == l ) l = strLen(i);

    o.setCaps( l + 1 );
#if VS8
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
        o.mLen = l;
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
#if VS8
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
        o.mLen = l;
    }
}
