#include "pch.h"

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void
GN::detail::LogHelper::log( const char * fmt, ... )
{
    StrA s;
    va_list arglist;
    va_start( arglist, fmt );
    s.format( fmt, arglist );
    va_end( arglist );
    GN::doLog( mDesc, s.cptr() );
}

//
//
// -----------------------------------------------------------------------------
void
GN::detail::LogHelper::logc( const char * cate, const char * fmt, ... )
{
    StrA s;
    va_list arglist;
    va_start( arglist, fmt );
    s.format( fmt, arglist );
    va_end( arglist );
    mDesc.cate = cate ? cate : "";
    GN::doLog( mDesc, s.cptr() );
}

//
//
// -----------------------------------------------------------------------------
void
GN::detail::LogHelper::loglc(
    int level, const char * cate, const char * fmt, ... )
{
    StrA s;
    va_list arglist;
    va_start( arglist, fmt );
    s.format( fmt, arglist );
    va_end( arglist );
    mDesc.level = level;
    mDesc.cate = cate ? cate : "";
    GN::doLog( mDesc, s.cptr() );
}
