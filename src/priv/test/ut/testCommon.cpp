#include "garnet/GNbase.h"

//
// Note:
//  - This function will be called before any test cases.
//  - Return non-zero means failed.
//
int myInit( int /*argc*/, const char * /*argv*/[] )
{
    GN::enableCRTMemoryCheck();

    // setup environment variables to control log behavior.
    if( GN::getEnv( "GN_LOG_QUIET" ).empty() ) GN::putEnv( "GN_LOG_QUIET", "1" );

    return 0;
}
