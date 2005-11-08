#include "pch.h"

//
//
// -----------------------------------------------------------------------------
void GN::putEnv( const char * name, const char * value )
{
    if( strEmpty(name) )
    {
        GN_ERROR( "Environment variable name can't be empty!" );
        return;
    }

    StrA s;
    if( strEmpty(value) )
    {
        s.format( "%s=", name );
    }
    else
    {
        s.format( "%s=%s", name, value );
    }

    ::putenv( const_cast<char*>(s.cstr()) );
}

//
//
// -----------------------------------------------------------------------------
void GN::getEnv( StrA & result, const char * name )
{
    if( strEmpty(name) )
        result.clear();
    else
        result.assign( ::getenv(name) );
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
