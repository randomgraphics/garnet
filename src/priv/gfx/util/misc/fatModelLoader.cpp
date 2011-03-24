#include "pch.h"
#include <garnet/gfx/fatModel.h>
#include "ase.h"

#if GN_MSVC
#pragma warning(disable:4100) // unreferenced formal parameter
#endif
#include <assimp/assimp.hpp>
#include <assimp/aiScene.h>       // Output data structure
#include <assimp/aiPostProcess.h> // Post processing flags
#include <assimp/IOStream.h>
#include <assimp/IOSystem.h>

// Disable FBX for now
#ifdef HAS_FBX
#undef HAS_FBX
#endif

#ifdef HAS_FBX
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

#define ROOT_BBOX 1

static GN::Logger * sLogger = GN::getLogger("GN.gfx.FatModel");

// *****************************************************************************
// ASE loader
// *****************************************************************************

namespace ase
{

//
//
// -----------------------------------------------------------------------------
template<typename SRC_VERTEX_TYPE>
static void sCopyVertexElement( void * dst, const MeshResourceDesc & src, const MeshVertexElement & e )
{
    Vector4f * d = (Vector4f*)dst;
    const uint8 * s = (const uint8*)src.vertices[e.stream] + src.offsets[e.stream] + e.offset;
    for( size_t i = 0; i < src.numvtx; ++i )
    {
        if( IsSameType<SRC_VERTEX_TYPE, Vector3f>::value )
        {
            d->set( *(const Vector3f*)s, 0.0f );
        }
        else if( IsSameType<SRC_VERTEX_TYPE, Vector2f>::value )
        {
            d->set( *(const Vector2f*)s, 0.0f, 0.0f );
        }
        s += src.strides[e.stream];
        ++d;
    }
}

//
//
// -----------------------------------------------------------------------------
static bool
sLoadFromASE( FatModel & fatmodel, File & file, const StrA & filename )
{
    // load ASE scene
    AseScene ase;
    if( !ase.loadFromFile(file) ) return false;

    // copy materials
    for( size_t i = 0; i < ase.materials.size(); ++i )
    {
        const AseMaterial & src = ase.materials[i];

        FatMaterial & dst = fatmodel.materials[src.name];

        dst.clear();

        dst.name = src.name;
        dst.albedoTexture = src.mapdiff.bitmap;
        dst.normalTexture = src.mapbump.bitmap;
        dst.albedoColor.set( src.diffuse, 1.0f );
    }

    // copy meshes
    fatmodel.meshes.resize( ase.meshes.size() );
    for( size_t i = 0; i < ase.meshes.size(); ++i )
    {
        const AseMesh & src = ase.meshes[i];
        FatMesh & dst = fatmodel.meshes[i];

        // determine vertex format
        const MeshVertexElement * position = NULL;
        const MeshVertexElement * normal = NULL;
        const MeshVertexElement * texcoord = NULL;
        uint32 vtxfmt = 0;
        for( uint32 i = 0; i < src.vtxfmt.numElements; ++i )
        {
            const MeshVertexElement & e = src.vtxfmt.elements[i];
            if( 0 == stringCompare( "POSITION", e.semantic ) )
            {
                position = &e;
                vtxfmt |= 1 << FatVertexBuffer::POSITION;
            }
            else if( 0 == stringCompare( "NORMAL", e.semantic ) )
            {
                normal = &e;
                vtxfmt |= 1 << FatVertexBuffer::NORMAL;
            }
            else if( 0 == stringCompare( "TEXCOORD", e.semantic ) )
            {
                texcoord = &e;
                vtxfmt |= 1 << FatVertexBuffer::TEXCOORD0;
            }
        }

        // copy vertex buffer.
        if( !dst.vertices.resize( vtxfmt, src.numvtx ) )
        {
            GN_ERROR(sLogger)( "Out of memory." );
            return false;
        }
        if( position )
        {
            sCopyVertexElement<Vector3f>( dst.vertices.getPosition(), src, *position );
        }
        if( normal )
        {
            sCopyVertexElement<Vector3f>( dst.vertices.getNormal(), src, *normal );
        }
        if( texcoord )
        {
            sCopyVertexElement<Vector2f>( dst.vertices.getTexcoord(0), src, *texcoord );
        }

        // copy index buffer
        if( !dst.indices.resize( src.numidx ) )
        {
            GN_ERROR(sLogger)( "Out of memory." );
            return false;
        }
        if( src.idx32 )
        {
            memcpy( dst.indices.cptr(), src.indices, src.numidx * 4 );
        }
        else
        {
            const uint16 * s = (const uint16*)src.indices;
            uint32 * d = dst.indices.cptr();
            for( size_t i = 0; i < src.numidx; ++i, ++s, ++d )
            {
                *d = *s;
            }
        }

        dst.bbox = src.selfbbox;
    }

    // copy subsets
    for( size_t i = 0; i < ase.subsets.size(); ++i )
    {
        const AseMeshSubset & src = ase.subsets[i];

        FatMeshSubset dst;
        dst.material = ase.materials[src.matid].name;
        dst.basevtx = src.basevtx;
        dst.numvtx = src.numvtx;
        dst.startidx = src.startidx;
        dst.numidx = dst.numidx;

        fatmodel.meshes[src.meshid].subsets.append( dst );
    }

    // setup bounding box of the whole scene
    fatmodel.bbox = ase.bbox;

    return true;
}

}


// *****************************************************************************
// FBX loader
// *****************************************************************************

namespace fbx
{
#ifdef HAS_FBX

class FbxSdkWrapper
{
public:

    KFbxSdkManager * manager;
    KFbxGeometryConverter * converter;

    FbxSdkWrapper() : manager(NULL), converter(NULL)
    {

    }

    ~FbxSdkWrapper()
    {
        delete converter;
        converter = NULL;

        // Delete the FBX SDK manager. All the objects that have been allocated
        // using the FBX SDK manager and that haven't been explicitly destroyed
        // are automatically destroyed at the same time.
        if (manager) manager->Destroy();
        manager = NULL;
    }

    bool init()
    {
        manager = KFbxSdkManager::Create();
        if( NULL == manager ) return false;

    	// create an IOSettings object
    	KFbxIOSettings * ios = KFbxIOSettings::Create( manager, IOSROOT );
    	manager->SetIOSettings(ios);

        // create a converter
        converter = new KFbxGeometryConverter(manager);

    	// Load plugins from the executable directory
    	KString lPath = KFbxGetApplicationDirectory();
#if defined(KARCH_ENV_WIN)
    	KString lExtension = "dll";
#elif defined(KARCH_ENV_MACOSX)
    	KString lExtension = "dylib";
#elif defined(KARCH_ENV_LINUX)
    	KString lExtension = "so";
#endif
    	manager->LoadPluginsDirectory(lPath.Buffer(), lExtension.Buffer());

        return true;
    }

};

//
//
// -----------------------------------------------------------------------------
static const char * sGetTextureFileName( KFbxSurfaceMaterial * material, const char * textureType )
{
    KFbxProperty prop = material->FindProperty( textureType );
    if( !prop.IsValid() ) return NULL;

    int lLayeredTextureCount = prop.GetSrcObjectCount(KFbxLayeredTexture::ClassId);
    if(lLayeredTextureCount > 0 )
    {
        // Layered texture

        for(int j=0; j<lLayeredTextureCount; ++j)
        {
            KFbxLayeredTexture *lLayeredTexture = KFbxCast <KFbxLayeredTexture>(prop.GetSrcObject(KFbxLayeredTexture::ClassId, j));

            int lNbTextures = lLayeredTexture->GetSrcObjectCount(KFbxTexture::ClassId);

            for(int k =0; k<lNbTextures; ++k)
            {
                KFbxTexture * lTexture = KFbxCast <KFbxTexture> (lLayeredTexture->GetSrcObject(KFbxTexture::ClassId,k));
                if(lTexture)
                {
                    return (const char *)lTexture->GetRelativeFileName();
                }
            }
        }
    }
    else
    {
        // Simple texture
        int lNbTextures = prop.GetSrcObjectCount(KFbxTexture::ClassId);
        for(int j =0; j<lNbTextures; ++j)
        {

            KFbxTexture* lTexture = KFbxCast <KFbxTexture>( prop.GetSrcObject(KFbxTexture::ClassId, j) );
            if(lTexture)
            {
                return (const char *)lTexture->GetRelativeFileName();
            }
        }
    }

    return NULL;
}

//
//
// -----------------------------------------------------------------------------
template<typename T>
static inline int sGetLayerElementIndex( const KFbxLayerElementTemplate<T> * elements, int index )
{
    if( NULL == elements ) return -1;

    KFbxLayerElement::EReferenceMode refmode = elements->GetReferenceMode();

    if( KFbxLayerElement::eDIRECT == refmode )
    {
        return index;
    }
    else if( KFbxLayerElement::eINDEX_TO_DIRECT == refmode )
    {
        return elements->GetIndexArray().GetAt(index);
    }
    else
    {
        GN_ERROR(sLogger)( "Unsupport reference mode: %d", refmode );
        return -1;
    }
}

//
//
// -----------------------------------------------------------------------------
template<typename T>
static inline int sGetLayerElementIndex(
    const KFbxLayerElementTemplate<T> * elements,
    int vertexIndex,
    int polygonIndex,
    int cornerIndex )
{
    if( NULL == elements ) return -1;

    KFbxLayerElement::EMappingMode mapmode = elements->GetMappingMode();

    if( KFbxLayerElement::eBY_CONTROL_POINT == mapmode )
    {
        return sGetLayerElementIndex( elements, vertexIndex );
    }
    else if( KFbxLayerElement::eBY_POLYGON_VERTEX == mapmode )
    {
        return sGetLayerElementIndex( elements, polygonIndex * 3 + cornerIndex );
    }
    else if( KFbxLayerElement::eBY_POLYGON == mapmode )
    {
        return sGetLayerElementIndex( elements, polygonIndex );
    }
    else
    {
        GN_ERROR(sLogger)( "Invalid layer mapping mode: %d", mapmode );
        return -1;
    }
}

struct SortPolygonByMaterial
{
    KFbxLayerElementMaterial * materials;

    SortPolygonByMaterial( KFbxLayerElementMaterial * m )
        : materials(m)
    {
    }

    bool operator()( const int & a, const int & b ) const
    {
        int ma = materials->GetIndexArray().GetAt(a);
        int mb = materials->GetIndexArray().GetAt(b);
        return ma < mb;
    }
};

struct MeshVertex
{
    Vector3f pos;
    Vector3f normal;
    Vector2f uv;

    static MeshVertexFormat sGetVertexFormat()
    {
        return MeshVertexFormat::XYZ_NORM_UV();
    }
};

struct MeshVertexKey
{
    int      pos;
    Vector3f normal;
    Vector2f uv;
};

typedef HashMap<
    MeshVertexKey,
    uint32,
    HashMapUtils::HashFunc_MemoryHash<MeshVertexKey>,
    HashMapUtils::EqualFunc_MemoryCompare<MeshVertexKey> > MeshVertexHashMap;

//
//
// -----------------------------------------------------------------------------
static void
sLoadFbxMesh(
    ModelHierarchyDesc           & desc,
    const StrA                   & filename,
    ModelHierarchyDesc::NodeDesc & gnnode,
    FbxSdkWrapper                & sdk,
    KFbxNode                     * fbxnode,
    KFbxMesh                     * fbxmesh,
    const char *                   meshName )
{
    if( !fbxmesh->IsTriangleMesh() )
    {
        fbxmesh = sdk.converter->TriangulateMesh( fbxmesh );
        if( NULL == fbxmesh )
        {
            GN_ERROR(sLogger)( "Fail to triangulate fbxmesh node: %s", meshName );
            return;
        }
    }

    // For now, we supports layer 0 only.
    KFbxLayer * layer0 = fbxmesh->GetLayer(0);
    if( NULL == layer0 )
    {
        GN_ERROR(sLogger)( "The fbxmesh does not have a layer: %s", meshName );
        return;
    }
    if( NULL == layer0->GetNormals() )
    {
        fbxmesh->ComputeVertexNormals();
    }

    // Get basic fbxmesh properties
    int                         * fbxIndices   = fbxmesh->GetPolygonVertices();
    const KFbxVector4           * fbxPositions = fbxmesh->GetControlPoints();
    KFbxLayerElementUV          * fbxUVs       = layer0->GetUVs();
    KFbxLayerElementNormal      * fbxNormals   = layer0->GetNormals();
    KFbxLayerElementMaterial    * fbxMaterials = layer0->GetMaterials();
    //KFbxLayerElementVertexColor * fbxColors    = layer0->GetVertexColors();
    //KFbxLayerElementTangent     * fbxTangents  = layer0->GetTangents();
    //KFbxLayerElementBinormal    * fbxBinormals = layer0->GetBinormals();
    int                           numtri       = fbxmesh->GetPolygonCount();
    int                           numidx       = numtri * 3;

    // How many materials are there?
    int nummat;
    if( fbxMaterials &&
        KFbxLayerElement::eBY_POLYGON == fbxMaterials->GetMappingMode() &&
        KFbxLayerElement::eINDEX_TO_DIRECT == fbxMaterials->GetReferenceMode() )
    {
        // multiple materials
        nummat = fbxnode->GetMaterialCount();
    }
    else
    {
        if( fbxMaterials && KFbxLayerElement::eALL_SAME != fbxMaterials->GetMappingMode() )
        {
            GN_WARN(sLogger)("Unsupported FBX material layer: mapping mode=%d, reference mode=%d. It will be treated as eALL_SAME.", fbxMaterials->GetMappingMode(), fbxMaterials->GetReferenceMode() );
        }

        // one material
        nummat = 1;
    }

    // Declare the hash table for vertices
    MeshVertexHashMap vhash( (size_t)numidx * 2 );

    // sort polygon by material
    DynaArray<int> sortedPolygons;
    if( !sortedPolygons.resize( numtri ) )
    {
        GN_ERROR(sLogger)( "Fail to load FBX mesh: out of memory." );
        return;
    }
    for( size_t i = 0; i < sortedPolygons.size(); ++i )
    {
        sortedPolygons[i] = (int)i;
    }
    if( nummat > 1 )
    {
        std::sort(
            sortedPolygons.begin(),
            sortedPolygons.end(),
            SortPolygonByMaterial( fbxMaterials ) );
    }

    // Create vertex blob that stores the final vertex buffer.
    AutoRef<DynaArrayBlob<MeshVertex> > vertexBlob( new DynaArrayBlob<MeshVertex> );
    if( !vertexBlob->array().reserve( numidx ) )
    {
        GN_ERROR(sLogger)( "Fail to load FBX mesh: out of memory." );
        return;
    }

    // Create index blob that stores the index buffer (assume 32-bit indices)
    AutoRef<SimpleBlob> indexBlob( new SimpleBlob(numidx * sizeof(uint32) ) );
    if( 0 == indexBlob->size() )
    {
        GN_ERROR(sLogger)( "Fail to load FBX mesh: out of memory." );
        return;
    }

    // Create one model for each material. Note that there might be materials
    // that are not referenced in this FBX mesh. So the actual number of models
    // could be less than number of the materials.
    DynaArray<ModelResourceDesc> models;
    if( !models.resize( (size_t)nummat ) ) return;

    // Split the FBX fbxmesh into multiple models, one material one model.
    int uvIndex = 0;
    int normalIndex = 0;
    int lastMatID = -1;
    for( size_t sortedPolygonIndex = 0; sortedPolygonIndex < sortedPolygons.size(); ++sortedPolygonIndex )
    {
        int polygonIndex = sortedPolygons[sortedPolygonIndex];

        int matid = nummat > 1 ? fbxMaterials->GetIndexArray().GetAt(polygonIndex) : 0;
        GN_ASSERT( matid >= lastMatID );

        // update mesh subset of each model
        ModelResourceDesc & model = models[matid];
        if( matid != lastMatID )
        {
            lastMatID = matid;
            model = SimpleDiffuseModel::DESC;
            //model = SimpleWireframeModel::DESC;
            model.subset.startidx = sortedPolygonIndex*3;
            model.subset.numidx   = 0;
            model.subset.basevtx  = 0;
            // Final number of vertices are known yet.
            //model.subset.numvert = ?;

            // get the texture associated with the material.
            KFbxSurfaceMaterial * mat = fbxnode->GetMaterial( matid );
            if( mat )
            {
                StrA dirname = fs::dirName( filename );
                const char * diffuse = sGetTextureFileName( mat, KFbxSurfaceMaterial::sDiffuse );
                if( model.hasTexture("ALBEDO_TEXTURE") && diffuse )
                {
                    model.textures["ALBEDO_TEXTURE"].resourceName = fs::resolvePath( dirname, diffuse );
                }
            }
        }
        model.subset.numidx += 3;

        // TODO: check polygon winding

        // add the polygon to the model
        for( int i = 0; i < 3; ++i )
        {
            int posIndex = fbxIndices[polygonIndex*3+i];

            // get index into UV elements
            if( -1 != uvIndex )
            {
                uvIndex = sGetLayerElementIndex( fbxUVs, posIndex, polygonIndex, i );
            }

            // get index into normal elements.
            if( -1 != normalIndex )
            {
                normalIndex = sGetLayerElementIndex( fbxNormals, posIndex, polygonIndex, i );
            }

            // create vetex key
            MeshVertexKey key;
            key.pos = posIndex;
            const KFbxVector4 & fbxnormal = fbxNormals->GetDirectArray().GetAt(normalIndex);
            key.normal.set( (float)fbxnormal[0], (float)fbxnormal[1], (float)fbxnormal[2] );
            if( fbxUVs )
            {
                const KFbxVector2 & fbxUV = fbxUVs->GetDirectArray().GetAt(uvIndex);
                // BUGBUG: for some reason, U coordinates has to be inverted (1.0-v) to make the
                // model look right in the viewer.
                key.uv.set( (float)fbxUV[0], (float)(1.0-fbxUV[1]) );
            }
            else
            {
                key.uv.set( FLT_MAX, FLT_MAX );
            }

            // If the key exists already, the pair will point to it.
            // If the key does not exisit, the pair will point to the newly inserted one.
            // Either way, pair->value should give us the correct index of the vertex.
            MeshVertexHashMap::KeyValuePair * pair;
            bool isNewVertex = vhash.insert( key, (uint32)vhash.size(), &pair );
            uint32 vertexIndex = pair->value;

            if( isNewVertex )
            {
                // If it is a new vertex, append it to the vertex blob.

                MeshVertex vertex;

                const KFbxVector4 & fbxvertex = fbxPositions[posIndex];
                vertex.pos.set( (float)fbxvertex[0], (float)fbxvertex[1], (float)fbxvertex[2] );
                vertex.normal = key.normal;
                vertex.uv = key.uv;

                vertexBlob->array().append( vertex );

                GN_ASSERT( vertexBlob->array().size() == (vertexIndex + 1) );
            }

            // add the vertex index into the final index buffer
            uint32 * indices = (uint32*)indexBlob->data();
            indices[sortedPolygonIndex*3+i] = vertexIndex;
        }
    }

    // Now both vertex and index buffers are filled up. Models' subset information are ready too.
    // We are almost there.

    // Compress index buffer to 16 bits, if possible.
    if( vertexBlob->array().size() <= 0x10000 )
    {
        AutoRef<SimpleBlob> ib16( new SimpleBlob (numidx * sizeof(uint16) ) );
        if( 0 == ib16->size() )
        {
            GN_ERROR(sLogger)( "Fail to load FBX mesh: out of memory." );
            return;
        }

        const uint32 * i32 = (const uint32*)indexBlob->data();
        uint16       * i16 = (uint16 *)ib16->data();
        for( size_t i = 0; i < (size_t)numidx; ++i )
        {
            i16[i] = (uint16)i32[i];
        }

        indexBlob = ib16;
    }

    // calculate the bounding box of the mesh
    const MeshVertex * vertices = vertexBlob->array().cptr();
    Boxf boundingBox;
    calculateBoundingBox(
        boundingBox,
        &vertices->pos.x, sizeof(MeshVertex),
        &vertices->pos.y, sizeof(MeshVertex),
        &vertices->pos.z, sizeof(MeshVertex),
        vertexBlob->array().size() );

    // Fill up the rest of informations for each models.
    for( size_t i = 0; i < models.size(); ++i )
    {
        models[i].mesh           = meshName;
        models[i].subset.numvtx  = (uint32)vertexBlob->array().size();
    }

    // Now copy everthing to the output descriptor. And we are done!
    MeshResourceDesc & gnmesh = desc.meshes[meshName];
    gnmesh.clear();
    gnmesh.prim        = PrimitiveType::TRIANGLE_LIST;
    gnmesh.numvtx      = (uint32)vertexBlob->array().size();
    gnmesh.numidx      = numidx;
    gnmesh.idx32       = gnmesh.numvtx > 0x10000;
    gnmesh.vtxfmt      = MeshVertex::sGetVertexFormat();
    gnmesh.vertices[0] = (void*)vertices;
    gnmesh.strides[0]  = sizeof(MeshVertex);
    gnmesh.offsets[0]  = 0;
    gnmesh.indices     = indexBlob->data();
    GN_INFO(sLogger)( "Load FBX mesh %s: %d vertices, %d faces", meshName, gnmesh.numvtx, gnmesh.numidx / 3 );

    desc.meshdata.append( vertexBlob );
    desc.meshdata.append( indexBlob );

    gnnode.bbox = boundingBox;

    for( size_t i = 0; i < models.size(); ++i )
    {
        ModelResourceDesc & model = models[i];

        // skip empty models.
        if( model.effect.empty() ) continue;

        StrA modelName = stringFormat( "%s.%d", meshName, i );
        desc.models[modelName] = model;
        gnnode.models.append( modelName );
    }
}

//
//
// -----------------------------------------------------------------------------
static bool
sLoadFbxNodeRecursivly(
    FatModel           & fatmodel,
    const StrA         & filename,
    FbxSdkWrapper      & sdk,
    KFbxNode           * node,
    KFbxNode           * parent )
{
    if( NULL == node ) return true;

    // Get node type
    KFbxNodeAttribute* attrib = node->GetNodeAttribute();
    KFbxNodeAttribute::EAttributeType type = attrib ? attrib->GetAttributeType() : KFbxNodeAttribute::eUNIDENTIFIED;

    const KFbxXMatrix & localTransform = node->GetScene()->GetEvaluator()->GetNodeLocalTransform(node);
    KFbxVector4    t = localTransform.GetT();
    KFbxQuaternion q = localTransform.GetQ();
    KFbxVector4    s = localTransform.GetS();

    ModelHierarchyDesc::NodeDesc & gnnode = desc.nodes[name];
    gnnode.parent = parent ? parent->GetName() : "";
    gnnode.position.set( (float)t[0], (float)t[1], (float)t[2] );
    gnnode.orientation.set( (float)q[0], (float)q[1], (float)q[2], (float)q[3] );
    gnnode.scaling.set( (float)s[0], (float)s[1], (float)s[2] );
    gnnode.bbox.set( 0, 0, 0, 0, 0, 0 );

    if( KFbxNodeAttribute::eMESH == type )
    {
        // load mesh node
        StrA fullMeshName = filename + "." + name;
        sLoadFbxMesh( desc, filename, gnnode, sdk, node, (KFbxMesh*)attrib, fullMeshName );
    }
    else if(
        // Some nodes are ignored silently.
        KFbxNodeAttribute::eNULL != type &&
        KFbxNodeAttribute::eUNIDENTIFIED != type &&
        KFbxNodeAttribute::eLIGHT != type &&
        KFbxNodeAttribute::eCAMERA != type &&
        KFbxNodeAttribute::eSKELETON != type )
    {
        GN_WARN(sLogger)( "Ignore unsupported node: type=%d, name=%s", type, name );
    }

    // load children
    for( int i = 0; i < node->GetChildCount(); ++i )
    {
        if( !sLoadFbxNodeRecursivly( desc, filename, sdk, node->GetChild( i ), node ) )
        {
            return false;
        }
    }

    // done
    return true;
}

#endif

//
//
// -----------------------------------------------------------------------------
static bool
sLoadFromFBX( FatModel & fatmodel, File & file, const StrA & filename )
{
#ifdef HAS_FBX

    FbxSdkWrapper sdk;
    if( !sdk.init() ) return false;
    KFbxSdkManager * gSdkManager = sdk.manager;

    // TODO: setup file system.

    // detect file format
    StrA filename = fs::toNativeDiskFilePath( file.name() );
	int lFileFormat = -1;
    if (!gSdkManager->GetIOPluginRegistry()->DetectReaderFileFormat(filename, lFileFormat) )
    {
        // Unrecognizable file format. Try to fall back to KFbxImporter::eFBX_BINARY
        lFileFormat = gSdkManager->GetIOPluginRegistry()->FindReaderIDByDescription( "FBX binary (*.fbx)" );;
    }

    // Create the importer.
    KFbxImporter* gImporter = KFbxImporter::Create(gSdkManager,"");
    if( NULL == gImporter ) return false;
    if(!gImporter->Initialize(filename, lFileFormat))
    {
        GN_ERROR(sLogger)( gImporter->GetLastErrorString() );
        return false;
    }

    // Import the scene
    KFbxScene * gScene = KFbxScene::Create( gSdkManager, "" );
    if( NULL == gScene ) return false;
    if(!gImporter->Import(gScene))
    {
        GN_ERROR(sLogger)( gImporter->GetLastErrorString() );
        return false;
    }

    return sLoadFbxNodeRecursivly( fatmodel, filename, sdk, gScene->GetRootNode(), NULL );

#else

    fatmodel.clear();
    GN_UNUSED_PARAM( file );
    GN_ERROR(sLogger)( "Fail to load file %s: FBX is not supported.", filename.cptr() );
    return false;

#endif
}

}
// *****************************************************************************
// XML loader
// *****************************************************************************

// *****************************************************************************
// BIN loader
// *****************************************************************************

// *****************************************************************************
// FatModel
// *****************************************************************************

enum FileFormat
{
    FF_UNKNOWN = 0,
    FF_FBX,
    FF_ASE,
    FF_GARNET_XML,
    FF_GARNET_BIN,
};

//
// Check if the string is end with specific suffix
// -----------------------------------------------------------------------------
static bool sCheckFileExtension( const char * filename, const char * extension )
{
    if( NULL == extension ) return true;
    if( NULL == filename ) return false;

    size_t n1 = strlen( filename );
    size_t n2 = strlen( extension );

    if( n1 < n2 ) return false;

    filename = filename + n1 - n2;

    return 0 == stringCompareI( filename, extension );
}

//
//
// -----------------------------------------------------------------------------
static FileFormat sDetermineFileFormatByContent( File & file )
{
    // not implemented yet.
    return FF_UNKNOWN;
}

//
//
// -----------------------------------------------------------------------------
static FileFormat sDetermineFileFormatByFileName( const StrA & filename )
{
    if( sCheckFileExtension( filename, ".xml" ) )
    {
        return FF_GARNET_XML;
    }
    else if( sCheckFileExtension( filename, ".mesh.bin" ) )
    {
        return FF_GARNET_BIN;
    }
    else if( sCheckFileExtension( filename, ".ase" ) )
    {
        return FF_ASE;
    }
    else if( sCheckFileExtension( filename, ".fbx" ) )
    {
        return FF_FBX;
    }
    else
    {
        return FF_UNKNOWN;
    }
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::FatModel::loadFromFile( const StrA & filename )
{
    clear();

    // Open the file.
    AutoObjPtr<File> file( fs::openFile( filename, "rb" ) );
    if( NULL == file ) return false;

    // determine file format
    FileFormat ff = sDetermineFileFormatByContent( *file );
    if( FF_UNKNOWN == ff )
    {
        ff = sDetermineFileFormatByFileName( filename );
    }

    StrA fullFileName = fs::resolvePath( fs::getCurrentDir(), filename );

    bool noerr;
    switch( ff )
    {
        case FF_ASE:
            noerr = ase::sLoadFromASE( *this, *file, filename );
            break;

        case FF_FBX:
            noerr = fbx::sLoadFromFBX( *this, *file, filename );
            break;

        case FF_GARNET_XML:
            noerr = false;
            break;

        case FF_GARNET_BIN:
            noerr = false;
            break;

        default:
            GN_ERROR(sLogger)( "Unknown file format: %s", filename.cptr() );
            noerr = false;
            break;
    }

    if( noerr )
    {
        size_t totalVerts = 0;
        size_t totalFaces = 0;
        for( size_t i = 0; i < this->meshes.size(); ++i )
        {
            const FatMesh & m = this->meshes[i];
            totalVerts += m.vertices.getVertexCount();
            totalFaces += m.indices.size() / 3;
        }
        GN_INFO(sLogger)( "Total vertices: %d, faces: %d", totalVerts, totalFaces );
    }
    else
    {
        clear();
    }

    return noerr;
}
