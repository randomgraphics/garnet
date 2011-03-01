#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::engine;

static GN::Logger * sLogger = GN::getLogger("GN.engine");

// *****************************************************************************
// Local functions
// *****************************************************************************

// TODO: move this function to engine module
extern bool
sLoadModelsFromASE( VisualComponent & comp, GpuResourceDatabase & db, File & file );

// TODO: move this function to engine module
extern bool
sLoadModelsFromXPR( VisualComponent & comp, GpuResourceDatabase & db, File & file );

// *****************************************************************************
// StaticMesh
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::engine::StaticMesh::StaticMesh()
{
    setComponent( &mVisual );
    setComponent( &mSpacial );
}

//
//
// -----------------------------------------------------------------------------
GN::engine::StaticMesh::~StaticMesh()
{
    setComponent( VisualComponent::sGetType(), NULL );
    setComponent( SpacialComponent::sGetType(), NULL );
}

//
//
// -----------------------------------------------------------------------------
bool GN::engine::StaticMesh::loadAllModelsFromFile( const char * fileName )
{
    GN_SCOPE_PROFILER( StaticMesh_loadAllModelsFromFile, "Load models from file into VisualComponent" );

    mVisual.clear();

    // open file
    AutoObjPtr<File> fp( fs::openFile( fileName, "rb" ) );
    if( !fp ) return false;

    // get file extension
    StrA ext = fs::extName( fileName );

    GpuResourceDatabase & gdb = *getGdb();

    // do loading
    if( 0 == stringCompareI( ".ase", ext.cptr() ) )
    {
        return sLoadModelsFromASE( mVisual, gdb, *fp );
    }
    else if( 0 == stringCompareI( ".xpr", ext.cptr() ) ||
             0 == stringCompareI( ".tpr", ext.cptr() ))
    {
        return sLoadModelsFromXPR( mVisual, gdb, *fp );
    }
    else
    {
        GN_ERROR(sLogger)( "Unknown file extension: %s", ext.cptr() );
        return false;
    }
}
