#include "pch.h"

//
//
// -----------------------------------------------------------------------------
void GN::PutEnv( const char * name, const char * value )
{
    static GN::Logger * sLogger = GN::GetLogger("GN.base.PutEnv");

#if GN_XENON
    // Xenon does not support putenv()
    GN_UNUSED_PARAM( name );
    GN_UNUSED_PARAM( value );
#else
    if( IsStringEmpty(name) )
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
    if( IsStringEmpty(value) )
    {
        s.Format( "%s=", name );
    }
    else
    {
        s.Format( "%s=%s", name, value );
    }

    if( 0 != _putenv( const_cast<char*>(s.ToRawPtr()) ) )
    {
        GN_ERROR(sLogger)( "fail to set environment '%s'.", s.ToRawPtr() );
    }
#endif
#endif
}

//
//
// -----------------------------------------------------------------------------
void GN::GetEnv( StrA & result, const char * name )
{
#if GN_XENON
    // Xenon does not support getenv()
    result.Clear();
#else
    if( IsStringEmpty(name) )
    {
        result.Clear();
    }
    else
    {
#if GN_MSVC8
        char * value;
        size_t sz;
        if( 0 == _dupenv_s( &value, &sz, name ) )
        {
            result.Assign( value, sz );
            ::free( value );
        }
#else
        result.Assign( ::getenv(name) );
#endif
    }
#endif
}

//
//
// -----------------------------------------------------------------------------
const char * GN::Guid::ToStr() const
{
    // GUID as string: {xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx}
    static char str[1+8+1+4+1+4+1+4+1+12+1];

    StringPrintf(
        str,
        sizeof(str),
        "{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
        data1,
        data2,
        data3,
        data4[0],
        data4[1],
        data4[2],
        data4[3],
        data4[4],
        data4[5],
        data4[6],
        data4[7] );

    return str;
}
