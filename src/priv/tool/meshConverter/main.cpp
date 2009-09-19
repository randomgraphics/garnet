#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::util;

static GN::Logger * sLogger = GN::getLogger("GN.tool.meshConverter");

static const char * inputFile = "media::\\boxes\\boxes.ase";
static const char * outputDir = "app::";

///
/// write world description to file
// -----------------------------------------------------------------------------
static bool sSaveToDirectory( const SimpleWorldDesc & desc, const char * dirname )
{
    // check dirname
    if( NULL == dirname )
    {
        GN_ERROR(sLogger)( "NULL directory name" );
        return false;
    }
    if( !fs::isDir( dirname ) )
    {
        GN_ERROR(sLogger)( "%s is not a directory", dirname );
        return false;
    }

    // convert to full path
    StrA fulldir = fs::resolvePath( fs::getCurrentDir(), dirname );
    dirname = fulldir;

    // write meshes
    int meshindex = 0;
    std::map<StrA,StrA> meshNameMapping;
    for( std::map<StrA,MeshResourceDesc>::const_iterator i = desc.meshes.begin();
        i != desc.meshes.end();
        ++i )
    {
        const StrA & oldMeshName = i->first;
        const MeshResourceDesc & mesh = i->second;

        StrA newMeshName = strFormat( "%d.mesh.bin", meshindex );

        if( !mesh.saveToFile( fulldir + "\\" + newMeshName ) ) return false;

        meshNameMapping[oldMeshName] = newMeshName;

        ++meshindex;
    }

    // write models

    // write entities

    return true;
}

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

    GN_INFO(sLogger)( "Write to directory : %s", outputDir );
    if( !sSaveToDirectory( swd, outputDir ) ) return -1;

    GN_INFO(sLogger)( "Conversion done successfully." );
    return 0;
}
