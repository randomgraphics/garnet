#include "garnet/GNbase.h"

bool gSkipRndr = false;

static void sPrintUsage()
{
    printf(
        "Usage: GNut <options>\n"
        "Options:\n"
        "   -h, -?      Print help screen\n"
        "   -v          Verbose mode.\n"
        );
};

//
// Note:
//  - This function will be called before any test cases.
//  - Return non-zero means failed.
//
int myInit( int argc, const char * argv[] )
{
    GN::enableCRTMemoryCheck();

    // parse command line arguments
    bool verbose = false;
    for( int i = 1; i < argc; ++i )
    {
        const char * a = argv[i];

        if( '-' == *a
            #if GN_MSWIN
            || '/' == *a
            #endif
            )
        {
            if( 0 == GN::strCmpI( a+1, "h" ) )
            {
                sPrintUsage();
                exit(-1);
            }
            else if( 0 == GN::strCmpI( a+1, "v" ) )
            {
                verbose = true;
            }
            else
            {
                fprintf( stderr, "Unknown command line option: %s\n\n", a );
                sPrintUsage();
                exit(-1);
            }
        }
    }

    // setup environment variables to control log behavior.
    if( !verbose )
    {
        GN::putEnv( "GN_LOG_QUIET", "1" );
    }

    return 0;
}
