#include "garnet/GNbase.h"

//
// Note:
//  - This function will be called before any test cases.
//  - Return non-zero means failed.
//
int myInit( int /*argc*/, const char * /*argv*/[] )
{
    // setup environment variables to contro log behavior.
    GN::putEnv( "GN_LOG_FILENAME", "log.txt" );
    GN::putEnv( "GN_LOG_QUIET", "1" );

    return 0;
}
