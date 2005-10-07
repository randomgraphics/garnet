#include "pch.h"

//
//
// -----------------------------------------------------------------------------
bool GN::getEnv( StrA & result, const char * name )
{
    const char * var;
    if( strEmpty(name) || NULL == (var=::getenv(name)) )
    {
        result.clear();
        return false;
    }
    else
    {
        result.assign( var );
        return true;
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::sleep( uint32_t microSeconds )
{
#if GN_WINNT
    ::Sleep( microSeconds );
#elif GN_POSIX
    ::usleep( microSeconds );
#else
    GN_CASSERT_EX( 0, "Unimplmented" );
#endif
}
