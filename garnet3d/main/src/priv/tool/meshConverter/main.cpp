#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::util;

static GN::Logger * sLogger = GN::GetLogger("GN.tool.meshConverter");

static const char * outputFile = "app::boxes.scene.xml";

void printHelp( const char * exepath )
{
    printf( "Usage: %s inputfile <outputfile>\n",
        fs::BaseName( exepath ).ToRawPtr() );
}

//
// Main entry
// -----------------------------------------------------------------------------
int main( int argc, const char * argv[] )
{
    printf( "\nGarnet mesh converter V0.1.\n" );

    if( argc < 2 )
    {
        printHelp( argv[0] );
        return -1;
    }

    const char * inputFile = argv[1];

    StrA outputFile;
    if( argc > 2 )
    {
        outputFile = argv[2];
    }
    else
    {
        outputFile = StringFormat( "startup::%s.scene.xml", fs::BaseName( inputFile ).ToRawPtr() );
    }

    SimpleWorldDesc swd;

    if( !swd.LoadFromFile( inputFile ) ) return -1;

    if( !swd.SaveToFile( outputFile ) ) return -1;

    GN_INFO(sLogger)( "Conversion done successfully." );
    return 0;
}
