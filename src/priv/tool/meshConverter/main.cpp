#include "pch.h"
#include "convert.h"

using namespace GN;

static void sPrintHelp( const char * appname )
{
    printf( "Usage: %s [inputfile] <outputfile>\n", appname );
}

static bool sParseCmdLine( ConvertOptions & co, int argc, const char * argv[] )
{
    if( argc < 2 ) { sPrintHelp( argv[0] ); return false; }

    co.inputFileName = argv[1];

    if( argc > 2 )
    {
        co.outputFileName = argv[2];
    }
    else
    {
        joinPath2(
            co.outputFileName,
            dirName(co.inputFileName),
            baseName(co.inputFileName) );
    }

    return true;
}

int main( int argc, const char * argv[] )
{
    GN_GUARD_ALWAYS;

    // generate options from command line
    ConvertOptions co;
    if( !sParseCmdLine( co, argc, argv ) ) return -1;

    // convert from 3dsmax ASE file
    if( !ase2mesh( co ) ) return -1;

    // success
    GN_INFO(sLogger)( "Convertion Done!" );
    return 0;

    GN_UNGUARD_ALWAYS_NO_THROW;
    return -1;
}
