#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::engine;

//static GN::Logger * sLogger = GN::getLogger("GN.engine");

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
bool GN::engine::StaticMesh::loadFromModelHierarchy( const gfx::ModelHierarchyDesc & mhd )
{
    GN_SCOPE_PROFILER( StaticMesh_loadFromModelHierarchy, "Load models from mesh hierarchy" );

    mVisual.clear();

    GpuResourceDatabase & gdb = *getGdb();

    // create mesh list. TODO: load mesh on-demand.
    DynaArray<AutoRef<MeshResource> > meshes;
    {
        GN_SCOPE_PROFILER( sLoadModelsFromASE_GenerateMeshList, "Load ASE into VisualComponent: generating mesh list" );

        meshes.resize( mhd.meshes.size() );

        size_t i = 0;
        for( const StringMap<char,gfx::MeshResourceDesc>::KeyValuePair * p = mhd.meshes.first();
             p != NULL; p = mhd.meshes.next( p ), ++i )
        {
            // TODO: avoid reloading existing mesh
            const char * meshName = p->key;
            const MeshResourceDesc & meshDesc = p->value;

            meshes[i] = gdb.findResource<MeshResource>( meshName );
            if( meshes[i] ) continue; // use exising mesh.

            meshes[i] = gdb.createResource<MeshResource>( meshName );
            if( !meshes[i] ) return false;
            if( !meshes[i]->reset( &meshDesc ) ) return false;
        }
    }

    // create models
    for( const StringMap<char,gfx::ModelResourceDesc>::KeyValuePair * p = mhd.models.first();
         p != NULL; p = mhd.models.next( p ) )
    {
        const ModelResourceDesc & modelDesc = p->value;
        AutoRef<ModelResource> model = gdb.createResource<ModelResource>( NULL );
        if( !model->reset( &modelDesc ) ) continue;

        mVisual.addModel( model );
    }

    // update bounding box
    mSpacial.setSelfBoundingBox( mhd.bbox );

    // TODO: handle node hierarchy

    return true;
}
