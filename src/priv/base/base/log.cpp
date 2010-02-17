#include "pch.h"

//
//
// -----------------------------------------------------------------------------
void GN::Logger::LogHelper::doLog( const char * fmt, ... )
{
    GN_ASSERT( mLogger );
    StrA s;
    va_list arglist;
    va_start( arglist, fmt );
    s.FormatV( fmt, arglist );
    va_end( arglist );
    mLogger->doLog( mDesc, s );
}

//
//
// -----------------------------------------------------------------------------
void GN::Logger::LogHelper::doLog( const wchar_t * fmt, ... )
{
    GN_ASSERT( mLogger );
    StrW s;
    va_list arglist;
    va_start( arglist, fmt );
    s.FormatV( fmt, arglist );
    va_end( arglist );
    mLogger->doLog( mDesc, s );
}
