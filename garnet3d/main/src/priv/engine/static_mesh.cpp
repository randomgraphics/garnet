#include "pch.h"
#include <garnet/gfx/fatModel.h>

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

    // create mesh list.
    DynaArray<AutoRef<MeshResource> > meshes;
    {
        GN_SCOPE_PROFILER( sLoadModelsFromASE_GenerateMeshList, "generating mesh list" );

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

//
//
// -----------------------------------------------------------------------------
bool GN::engine::StaticMesh::loadFromFatModel( const GN::gfx::FatModel & fatmodel )
{
    GN_SCOPE_PROFILER( StaticMesh_loadFromFatModel, "Load models from FatModel" );

    mVisual.clear();

    GpuResourceDatabase & gdb = *getGdb();

    DynaArray<AutoRef<MeshResource> > meshes;
    meshes.resize( fatmodel.meshes.size() );
    for( size_t i = 0; i < fatmodel.meshes.size(); ++i )
    {
        const FatMesh & fatmesh = fatmodel.meshes[i];

        StrA meshName = stringFormat( "%s.mesh.%d", fatmodel.name, i );

        // use exising mesh, if possible
        meshes[i] = gdb.findResource<MeshResource>( meshName );
        if( meshes[i] ) continue;

        MeshResourceDesc mrd;
        mrd.prim = PrimitiveType::TRIANGLE_LIST;
        mrd.numvtx = fatmesh.vertices.getVertexCount();

        GN_UNIMPL();

        meshes[i] = gdb.createResource<MeshResource>( meshName );
        if( !meshes[i] ) return false;
        if( !meshes[i]->reset( &mrd ) ) return false;
    }

    return true;
}
