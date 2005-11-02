#include "pch.h"

//
//
// -----------------------------------------------------------------------------
void GN::getEnv( StrA & result, const char * name )
{
    const char * var;
    if( strEmpty(name) || NULL == (var=::getenv(name)) )
    {
        result.clear();
    }
    else
    {
        result.assign( var );
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::sleep( uint32_t microSeconds )
{
#if GN_MSWIN
    ::Sleep( microSeconds );
#elif GN_POSIX
    uint32_t s = microSeconds / 1000;
    uint32_t us = microSeconds % 1000;
    ::sleep( s );
    ::usleep( us );
#else
    GN_CASSERT_EX( 0, "Unimplmented" );
#endif
}
