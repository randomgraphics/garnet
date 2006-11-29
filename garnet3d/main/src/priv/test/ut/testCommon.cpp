#include "garnet/GNbase.h"

bool gSkipRndr = false;

//
// Note:
//  - This function will be called before any test cases.
//  - Return non-zero means failed.
//
int myInit( int argc, const char * argv[] )
{
    GN::enableCRTMemoryCheck();

    // setup environment variables to control log behavior.
    if( GN::getEnv( "GN_LOG_QUIET" ).empty() ) GN::putEnv( "GN_LOG_QUIET", "1" );

    // parse command line arguments
    for( int i = 1; i < argc; ++i )
    {
        const char * a = argv[i];

        if( 0 == GN::strCmpI( a, "-h" ) )
        {
            printf( "usage: ..." );
            exit(-1);
        }
        else if( 0 == GN::strCmpI( a, "-norndr" ) )
        {
            gSkipRndr = true;
        }
    }

    return 0;
}
