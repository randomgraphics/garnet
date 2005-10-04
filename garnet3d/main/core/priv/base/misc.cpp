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
