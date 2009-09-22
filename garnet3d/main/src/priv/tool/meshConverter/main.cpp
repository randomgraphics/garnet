#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::util;

static GN::Logger * sLogger = GN::getLogger("GN.tool.meshConverter");

static const char * inputFile = "media::\\boxes\\boxes.ase";
static const char * outputFile = "app::boxes.scene.xml";

//
// Main entry
// -----------------------------------------------------------------------------
int main( int argc, const char * argv[] )
{
    printf( "\nGarnet mesh converter V0.1.\n" );

    GN_UNUSED_PARAM( argc );
    GN_UNUSED_PARAM( argv );

    SimpleWorldDesc swd;

    GN_INFO(sLogger)( "Load from file     : %s", inputFile );
    if( !swd.loadFromFile( inputFile ) ) return -1;

    GN_INFO(sLogger)( "Write to directory : %s", outputFile );
    if( !swd.saveToFile( outputFile ) ) return -1;

    GN_INFO(sLogger)( "Conversion done successfully." );
    return 0;
}
