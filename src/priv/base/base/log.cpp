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
    s.format( fmt, arglist );
    va_end( arglist );
    mLogger->doLog( mDesc, s );
}
