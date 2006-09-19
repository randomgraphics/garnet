#include "pch.h"

//
//
// -----------------------------------------------------------------------------
void GN::putEnv( const char * name, const char * value )
{
    static GN::Logger * sLogger = GN::getLogger("GN.base.putEnv");

#if GN_XENON
    // Xenon does not support putenv()
    GN_UNUSED_PARAM( name );
    GN_UNUSED_PARAM( value );
#else
    if( strEmpty(name) )
    {
        GN_ERROR(sLogger)( "Environment variable name can't be empty!" );
        return;
    }

    if( 0 == value ) value = "";

#if GN_POSIX
    if( 0 != ::setenv( name, value, 1 ) )
    {
        GN_ERROR(sLogger)( "fail to set environment '%s=%s'.", name, value );
    }
#else
    StrA s;
    if( strEmpty(value) )
    {
        s.format( "%s=", name );
    }
    else
    {
        s.format( "%s=%s", name, value );
    }

    if( 0 != _putenv( const_cast<char*>(s.cptr()) ) )
    {
        GN_ERROR(sLogger)( "fail to set environment '%s'.", s.cptr() );
    }
#endif
#endif
}

//
//
// -----------------------------------------------------------------------------
void GN::getEnv( StrA & result, const char * name )
{
#if GN_XENON
    // Xenon does not support getenv()
    result.clear();
#else
    if( strEmpty(name) )
    {
        result.clear();
    }
    else
    {
#if GN_MSVC8
        char * value;
        size_t sz;
        if( 0 == _dupenv_s( &value, &sz, name ) )
        {
            result.assign( value, sz );
            ::free( value );
        }
#else
        result.assign( ::getenv(name) );
#endif
    }
#endif
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
    GN_CASSERT_EX( 0, unimplemented );
#endif
}

//
//
// -----------------------------------------------------------------------------
#if GN_MSVC
#include <crtdbg.h>
void GN::enableCRTMemoryCheck()
{
    int tmpDbgFlag;
    tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
    _CrtSetDbgFlag(tmpDbgFlag);
}
#else
void GN::enableCRTMemoryCheck() {}
#endif
