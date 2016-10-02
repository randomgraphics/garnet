#include "pch.h"

//
//
// -----------------------------------------------------------------------------
void GN::Logger::LogHelper::DoLog( const char * fmt, ... )
{
    GN_ASSERT( mLogger );
    StrA s;
    va_list arglist;
    va_start( arglist, fmt );
    s.formatv( fmt, arglist );
    va_end( arglist );
    mLogger->DoLog( mDesc, s );
}

//
//
// -----------------------------------------------------------------------------
void GN::Logger::LogHelper::DoLog( const wchar_t * fmt, ... )
{
    GN_ASSERT( mLogger );
    StrW s;
    va_list arglist;
    va_start( arglist, fmt );
    s.formatv( fmt, arglist );
    va_end( arglist );
    mLogger->DoLog( mDesc, s );
}
