#include "pch.h"
#include <garnet/gfx/fatModel.h>

using namespace GN;
using namespace GN::gfx;
using namespace GN::engine;

static GN::Logger * sLogger = GN::getLogger("GN.engine");

// *****************************************************************************
// Local stuff
// *****************************************************************************

///
/// Determine the best model template that can show the mesh, return NULL for failure
// -----------------------------------------------------------------------------
static const ModelResourceDesc * sDetermineBestModelTemplate( const MeshVertexFormat & vf )
{
    struct Local
    {
        static bool sHasPosition( const MeshVertexFormat & vf )
        {
            return vf.hasSemantic( "position" )
                || vf.hasSemantic( "pos" );
        }

        static bool sHasNormal( const MeshVertexFormat & vf )
        {
            return vf.hasSemantic( "normal" );
        }

        static bool sHasTex0( const MeshVertexFormat & vf )
        {
            return vf.hasSemantic( "texcoord" )
                || vf.hasSemantic( "texcoord0" );
        }

        static bool sHasTangent( const MeshVertexFormat & vf )
        {
            return vf.hasSemantic( "tangent" );
        }
    };

    // position is required
    if( !Local::sHasPosition( vf ) )
    {
        GN_ERROR(sLogger)( "The mesh has no position, which is required by the mesh viewer." );
        return NULL;
    }

    if( !Local::sHasNormal( vf ) )
    {
        GN_WARN(sLogger)( "The mesh has no normal." );
        return &SimpleWireframeModel::DESC;
    }

    if( !Local::sHasTex0( vf ) )
    {
        GN_WARN(sLogger)( "The mesh has no texture coordinate." );
        return &SimpleDiffuseModel::DESC;
    }

    // Program reaches here, means that the mesh has position, norml and texcoord.

    if( Local::sHasTangent( vf ) )
    {
        return &SimpleNormalMapModel::DESC;
    }
    else
    {
        return &SimpleDiffuseModel::DESC;
    }
}

// *****************************************************************************
// StaticMesh
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static const GN::Guid STATIC_MESH_GUID = { 0x694830b1, 0x49f9, 0x4dd6, { 0x85, 0x56, 0x4d, 0xa5, 0x31, 0xf6, 0xac, 0x2c } };
GN_ENGINE_IMPLEMENT_ENTITY( GN::engine::StaticMesh, STATIC_MESH_GUID );

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

    DynaArray<uint8> vb;

    for( uint32 i = 0; i < fatmodel.meshes.size(); ++i )
    {
        if( NULL == fatmodel.meshes[i] ) continue;

        const FatMesh & fatmesh = *fatmodel.meshes[i];

        StrA meshName = stringFormat( "%s.mesh.%d", fatmodel.name.rawptr(), i );

        // use exising mesh, if possible
        AutoRef<MeshResource> mesh = gdb.findResource<MeshResource>( meshName );
        if( !mesh )
        {
            // setup mesh descriptor
            MeshResourceDesc merd;
            memset( &merd, 0, sizeof(merd) );
            merd.prim = fatmesh.primitive;
            merd.numvtx = fatmesh.vertices.getVertexCount();
            merd.numidx = fatmesh.indices.size();
            merd.idx32 = true; // TODO: use 16-bit index buffer, when possible.
            merd.offsets[0] = 0;
            merd.indices = (void*)fatmesh.indices.rawptr();

            // setup vertex format
            fatmesh.vertices.GenerateMeshVertexFormat( merd.vtxfmt );
            merd.strides[0] = math::alignToPowerOf2<uint16>( merd.vtxfmt.calcStreamStride( 0 ), 16 );

            // copy vertex data
            if( !vb.resize( merd.strides[0] * fatmesh.vertices.getVertexCount() ) ) continue;
            if( !fatmesh.vertices.GenerateVertexStream( merd.vtxfmt, 0, merd.strides[0], vb.rawptr(), vb.size() ) ) continue;
            merd.vertices[0] = vb.rawptr();

            // create GPU mesh resource
            mesh = gdb.createResource<MeshResource>( meshName );
            if( !mesh || !mesh->reset( &merd ) )
            {
                continue;
            }
        }

        GN_ASSERT( mesh );
        const ModelResourceDesc * modelTemplate = sDetermineBestModelTemplate( mesh->getDesc().vtxfmt );
        if( NULL == modelTemplate ) continue;

        // create one model for each mesh subset
        for( size_t s = 0; s < fatmesh.subsets.size(); ++s )
        {
            const FatMeshSubset & fatsubset = fatmesh.subsets[s];
            const FatMaterial   & fatmat    = fatmodel.materials[fatsubset.material];

            // setup model descriptor
            ModelResourceDesc mord = *modelTemplate;
            mord.mesh = meshName;
            mord.subset.basevtx = fatsubset.basevtx;
            mord.subset.numvtx = fatsubset.numvtx;
            mord.subset.startidx = fatsubset.startidx;
            mord.subset.numidx = fatsubset.numidx;

            // associate textures to the model
            if( mord.hasTexture("ALBEDO_TEXTURE") && !fatmat.albedoTexture.empty() )
            {
                mord.textures["ALBEDO_TEXTURE"].resourceName = fatmat.albedoTexture;
            }
            if( mord.hasTexture("NORMAL_TEXTURE") && !fatmat.normalTexture.empty() )
            {
                mord.textures["NORMAL_TEXTURE"].resourceName = fatmat.normalTexture;
            }

            // TODO: associate uniforms to the model

            // create model and add to visual
            AutoRef<ModelResource> model = gdb.createResource<ModelResource>( NULL );
            if( model && model->reset( &mord ) )
            {
                mVisual.addModel( model );
            }
        }
    }

    // update bounding box
    mSpacial.setSelfBoundingBox( fatmodel.bbox );

    return true;
}

//
//
// -----------------------------------------------------------------------------
bool GN::engine::StaticMesh::loadFromFile( const StrA & filename )
{
    FatModel fm;
    if( !fm.loadFromFile( filename ) ) return false;
    return loadFromFatModel( fm );
}
