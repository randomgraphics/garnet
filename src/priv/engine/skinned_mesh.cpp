#include "pch.h"
#include <garnet/gfx/fatModel.h>

using namespace GN;
using namespace GN::gfx;
using namespace GN::engine;

static GN::Logger * sLogger = GN::getLogger("GN.engine");

// *****************************************************************************
// Local stuff
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------

// *****************************************************************************
// SkinnedMesh
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::engine::SkinnedMesh::SkinnedMesh()
{
    clear();
}

//
//
// -----------------------------------------------------------------------------
GN::engine::SkinnedMesh::~SkinnedMesh()
{
    clear();
    setComponent<VisualComponent>( NULL );
    setComponent<SpacialComponent>( NULL );
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::SkinnedMesh::clear()
{
    mJoints.resize( 1 );
    mJoints[0].attach( new SpacialComponent );
    setComponent<SpacialComponent>( mJoints[0] );

    mVisual.attach( new VisualComponent );
    setComponent<VisualComponent>( mVisual );
}

//
//
// -----------------------------------------------------------------------------
bool GN::engine::SkinnedMesh::loadFromFatModel( const GN::gfx::FatModel & fatmodel )
{
    GN_SCOPE_PROFILER( SkinnedMesh_loadFromFatModel, "Load skinned mesh from FatModel" );

    // Clear existing data.
    clear();

    if( fatmodel.skeletons.empty() )
    {
        GN_ERROR(sLogger)( "The fat model does not contain any skeletons." );
        return false;
    }

    GpuResourceDatabase & gdb = *getGdb();

    DynaArray<uint8> vb;

    // Load all meshes
    for( uint32 i = 0; i < fatmodel.meshes.size(); ++i )
    {
        if( NULL == fatmodel.meshes[i] ) continue;

        const FatMesh & fatmesh = *fatmodel.meshes[i];

        StrA meshName = stringFormat( "%s.mesh.%d", fatmodel.name, i );

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
            merd.indices = (void*)fatmesh.indices.cptr();

            // setup vertex format
            fatmesh.vertices.GenerateMeshVertexFormat( merd.vtxfmt );
            merd.strides[0] = math::alignToPowerOf2<uint16>( merd.vtxfmt.calcStreamStride( 0 ), 16 );

            // copy vertex data
            if( !vb.resize( merd.strides[0] * fatmesh.vertices.getVertexCount() ) ) continue;
            if( !fatmesh.vertices.GenerateVertexStream( merd.vtxfmt, 0, merd.strides[0], vb.cptr(), vb.size() ) ) continue;
            merd.vertices[0] = vb.cptr();

            // create GPU mesh resource
            mesh = gdb.createResource<MeshResource>( meshName );
            if( !mesh || !mesh->reset( &merd ) )
            {
                continue;
            }
        }

        GN_ASSERT( mesh );

        // TODO: create special model descriptor for skinned effect.
        const ModelResourceDesc * modelTemplate = &SimpleDiffuseModel::DESC;

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

            // create model and add to visual
            AutoRef<ModelResource> model = gdb.createResource<ModelResource>( NULL );
            if( model && model->reset( &mord ) )
            {
                mVisual->addModel( model );
            }
        }
    }

    // update bounding box
    mJoints[0]->setSelfBoundingBox( fatmodel.bbox );

    return true;
}

//
//
// -----------------------------------------------------------------------------
bool GN::engine::SkinnedMesh::loadFromFile( const StrA & filename )
{
    FatModel fm;
    if( !fm.loadFromFile( filename ) ) return false;
    return loadFromFatModel( fm );
}

