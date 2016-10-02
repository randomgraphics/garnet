#include "pch.h"

#ifdef HAS_FBX
#pragma warning(disable:4100) // unreferenced formal parameter
# if GN_GCC
#  pragma GCC diagnostic ignored "-Wunused"
# endif
#include <fbxsdk.h>
#include <fbxfilesdk/kfbxio/kfbximporter.h>
#include <fbxfilesdk/kfbxplugins/kfbxgeometryconverter.h>
#include <fbxfilesdk/fbxfilesdk_nsuse.h>
#endif

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::GetLogger("GN.tool.meshConverter");

void printHelp( const char * exepath )
{
    printf( "Usage: %s inputfile <outputfile>\n",
        fs::baseName( exepath ).rawptr() );
}

#ifdef HAS_FBX

//
// Convert FBX binary file to FBX ascii file
// -----------------------------------------------------------------------------
int FbxBin2Ascii( int argc, const char * argv[] )
{
    struct Local
    {
        static void Local::sPrintHelp( const char * exepath )
        {
            printf( "Usage: -fbx inputfile outputfile\n",
                fs::baseName( exepath ).rawptr() );
        }

        KFbxSdkManager * sdk;
        KFbxImporter * importer;
        KFbxExporter * exporter;
        KFbxScene * scene;

        Local()
            : sdk(NULL)
            , importer(NULL)
            , exporter(NULL)
            , scene(NULL)
        {
        }

        ~Local()
        {
            if( exporter ) exporter->Destroy(), exporter = NULL;
            if( importer ) importer->Destroy(), importer = NULL;
            if( scene ) scene->Destroy(), scene = NULL;
            if( sdk ) sdk->Destroy(), sdk = NULL;
        }
    };

    printf( "\nConvert FBX binary file to ascii file.\n" );

    if( argc < 3 )
    {
        Local::sPrintHelp( argv[0] );
        return -1;
    }

    Local l;

    const char * inputFileName = argv[2];
    const char * outputFileName = argv[3];

    // create SDK
    if( NULL == (l.sdk = KFbxSdkManager::Create()) )
    {
        fprintf( stderr, "ERROR: Out of memory.\n" );
        return -1;
    }

    // create other components
    if( NULL == (l.importer = KFbxImporter::Create( l.sdk, "" )) ||
        NULL == (l.exporter = KFbxExporter::Create( l.sdk, "" )) ||
        NULL == (l.scene = KFbxScene::Create( l.sdk, "" )) )
    {
        fprintf( stderr, "ERROR: Out of memory.\n" );
        return -1;
    }

    // Load file
    if( !l.importer->Initialize( inputFileName ) )
    {
        fprintf( stderr, "ERROR: call to KFbxImporter::Initialize( %s ) failed: %s\n", inputFileName, l.importer->GetLastErrorString() );
        return -1;
    }

    // Load scene
    if( !l.importer->Import( l.scene ) )
    {
        fprintf( stderr, "ERROR: call to KFbxScene::Import() failed: %s\n", l.importer->GetLastErrorString() );
        return -1;
    }

    // Initialize exporter (1 means FBX ascii format)
    if( !l.exporter->Initialize(outputFileName, 1) )
    {
        fprintf( stderr, "ERROR: call to KFbxExporter::Initialize() failed: %s\n", l.exporter->GetLastErrorString() );
        return -1;
    }

    // Save scene
    if( !l.exporter->Export( l.scene ) )
    {
        fprintf( stderr, "ERROR: call to KFbxExporter::Export() failed: %s\n", l.exporter->GetLastErrorString() );
        return -1;
    }

    // done
    return 0;
}

#endif

//
// Main entry
// -----------------------------------------------------------------------------
int main( int argc, const char * argv[] )
{
#ifdef HAS_FBX
    if( argc > 2 && 0 == str::compareI( argv[1], "-fbx" ) )
    {
        return FbxBin2Ascii( argc, argv );
    }
#endif

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
        outputFile = str::format( "startup::%s.scene.xml", fs::baseName( inputFile ).rawptr() );
    }

    ModelHierarchyDesc mhd;

    if( !mhd.loadFromFile( inputFile ) ) return -1;

    if( !mhd.saveToFile( outputFile ) ) return -1;

    GN_INFO(sLogger)( "Conversion done successfully." );
    return 0;
}
