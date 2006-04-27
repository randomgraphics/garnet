#include "pch.h"
#include "coreResourceDict.h"

//
//
// -----------------------------------------------------------------------------
GN::StrA GN::core::searchResourceFile( const StrA & name )
{
    GN_GUARD;

    if( path::isFile( name ) ) return name;

    if( path::isAbsPath(name) ) return StrA::EMPTYSTR;

    StrA fullPath;

#define CHECK_PATH( X ) do { fullPath = path::join X ; if( path::isFile( fullPath ) ) return fullPath; } while(0)

    // search in startup directory
    CHECK_PATH( ("startup:", name) );
    CHECK_PATH( ("startup:/media", name) );

    // search in application directory
    CHECK_PATH( ("app:", name ) );
    CHECK_PATH( ( "app:/media", name ) );

    // search in garnet binary tree.
    CHECK_PATH( ( getEnv("GARNET_ROOT"), "bin/media", name ) );

    // hack for special cases
    CHECK_PATH( ( "app:../../../../../../media", name ) ); // for temporary build binaries

    // resource not found.
    return StrA::EMPTYSTR;

    GN_UNGUARD;
}
