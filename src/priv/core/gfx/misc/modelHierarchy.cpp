#include "pch.h"
#include "ase.h"

#if GN_MSVC
#pragma warning(disable:4100) // unreferenced formal parameter
#endif
#include <assimp/assimp.hpp>
#include <assimp/aiScene.h>       // Output data structure
#include <assimp/aiPostProcess.h> // Post processing flags
#include <assimp/IOStream.h>
#include <assimp/IOSystem.h>

#ifdef HAS_FBX
# if GN_GCC
#  pragma GCC diagnostic ignored "-Wunused"
# endif
#pragma warning(disable:4996) // 'x' was declared depreciated
#define FBXSDK_SHARED
#include <fbxsdk.h>
#endif

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.util.misc");

#define ROOT_BBOX 1

// *****************************************************************************
// Local stuff
// *****************************************************************************

//
// Check if the string is end with specific suffix
// -----------------------------------------------------------------------------
static bool sStrEndWithI( const char * string, const char * suffix )
{
    if( NULL == suffix ) return true;
    if( NULL == string ) return false;

    size_t n1 = strlen( string );
    size_t n2 = strlen( suffix );

    if( n1 < n2 ) return false;

    string = string + n1 - n2;

    return 0 == str::compareI( string, suffix );
}

// *****************************************************************************
// XPR loader
//
// NOTE: XPR2 format is 3D model format used by various games from Tecomo,
//       like DOA.
//
// The file is structured like this:
//
//     <XPRFileHeader> (16 bytes)
//     <ObjectTable>   (16 bytes * number_of_objects)
//     <Unknown>       (12 bytes)
//     <Data>          (data)
//
// *****************************************************************************

namespace xpr
{

struct XPRFileHeader
{
    FOURCC tag;        ///< must be XPR2
    uint32 size1;      ///< size tag 1
    uint32 size2;      ///< size tag 2 (file size = size1+size2+12)
    uint32 numObjects; ///< number of objects in this file
};

struct XPRObjectHeader
{
    FOURCC type;    ///< object type, could be "USER", "TX2D", "VBUF", "IBUF".
    uint32 offset;  ///< object offset in bytes. The actual offset is this value + 12.
    uint32 size;    ///< object size in bytes
    uint32 unknown; ///< I don't know what this is for.
};

// XPR texture descriptor, 0x28 bytes
struct XPRTex2DDesc
{
    // 10 dwords
    uint32 dwords[10];
};
GN_CASSERT( 0x28 == sizeof(XPRTex2DDesc) );

/// XPR vertex buffer descriptor, 0x14 bytes
struct XPRVBufDesc
{
    uint32 dwords[5];
};
GN_CASSERT( 0x14 == sizeof(XPRVBufDesc) );

/// XPR index buffer descriptor, 0x14 bytes
struct XPRIBufDesc
{
    uint32 dwords[5];
};
GN_CASSERT( 0x14 == sizeof(XPRIBufDesc) );

struct XPRScene
{
    DynaArray<gfx::MeshResourceDesc> meshes;
    DynaArray<XPRTex2DDesc*>         texDescs;
    DynaArray<XPRVBufDesc*>          vbDescs;
    DynaArray<XPRIBufDesc*>          ibDescs;
    DynaArray<void*>                 sceneData; ///< store all texture and mesh data
    Boxf                             bbox;      ///< bounding box
};

static bool
sLoadXprSceneFromFile( XPRScene & xpr, File & file )
{
    size_t readen;

    // read file header
    XPRFileHeader header;
    if( !file.read( &header, sizeof(header), &readen ) || sizeof(header) != readen )
    {
        GN_ERROR(sLogger)( "Fail to read file header." );
        return false;
    }

    // swap header to little endian
    header.size1 = swap8in32( header.size1 );
    header.size2 = swap8in32( header.size2 );
    header.numObjects = swap8in32( header.numObjects );

    // read scene data
    size_t dataSize = header.size1 + header.size2 + 12 - sizeof(header);
    xpr.sceneData.resize( dataSize );
    if( !file.read( xpr.sceneData.rawptr(), dataSize, &readen ) || dataSize != readen )
    {
        GN_ERROR(sLogger)( "Fail to read XPR data." );
        return false;
    }

    // iterate all objects
    XPRObjectHeader * objects = (XPRObjectHeader *)xpr.sceneData.rawptr();
    for( size_t i = 0; i < header.numObjects; ++i )
    {
        XPRObjectHeader & o = objects[i];

        // do endian swap
        o.offset = swap8in32( o.offset );
        o.size   = swap8in32( o.size );
        o.unknown = swap8in32( o.unknown );

        size_t offset = o.offset - sizeof(header) + 12;
        void * desc   = &xpr.sceneData[offset];

        switch( o.type.u32 )
        {
            case GN_MAKE_FOURCC( 'V','B','U','F' ):
            {
                XPRVBufDesc * vbdesc = (XPRVBufDesc*)desc;
                if( sizeof(XPRVBufDesc) != o.size )
                {
                    GN_ERROR(sLogger)( "object size is invalid." );
                    return false;
                }
                swap8in32( vbdesc->dwords, vbdesc->dwords, sizeof(*vbdesc)/4 );
                xpr.vbDescs.append( vbdesc );
                break;
            }

            case GN_MAKE_FOURCC( 'I','B','U','F' ):
            {
                XPRIBufDesc * ibdesc = (XPRIBufDesc*)desc;
                if( sizeof(XPRIBufDesc) != o.size )
                {
                    GN_ERROR(sLogger)( "object size is invalid." );
                    return false;
                }
                swap8in32( ibdesc->dwords, ibdesc->dwords, sizeof(*ibdesc)/4 );
                xpr.ibDescs.append( ibdesc );
                break;
            }

            case GN_MAKE_FOURCC( 'T','X','2','D' ):
            {
                XPRTex2DDesc * texdesc = (XPRTex2DDesc*)desc;
                if( sizeof(XPRTex2DDesc) == o.size )
                {
                    GN_ERROR(sLogger)( "object size is invalid." );
                    return false;
                }
                swap8in32( texdesc->dwords, texdesc->dwords, sizeof(*texdesc)/4 );
                xpr.texDescs.append( texdesc );
                break;
            }

            case GN_MAKE_FOURCC( 'U','S','E','R' ):
                // user data block is silently ignored.
                break;

            default:
                GN_WARN(sLogger)( "unknown object type." );
                return false;
        }
    }

    // success
    return true;
}

//
//
// -----------------------------------------------------------------------------
static bool
sLoadModelHierarchyFromXPR( ModelHierarchyDesc & desc, File & file )
{
    // load XPR file
    XPRScene xpr;
    if( !sLoadXprSceneFromFile( xpr, file ) ) return NULL;

    GN_UNUSED_PARAM( desc );
    GN_UNIMPL_WARNING();
    return false;
}

}

// *****************************************************************************
//
// ASE loader
//
// *****************************************************************************

namespace ase
{

//
//
// -----------------------------------------------------------------------------
static bool sHasPosition( const MeshVertexFormat & vf )
{
    return vf.hasSemantic( "position" )
        || vf.hasSemantic( "pos" );
}

//
//
// -----------------------------------------------------------------------------
static bool sHasNormal( const MeshVertexFormat & vf )
{
    return vf.hasSemantic( "normal" );
}

//
//
// -----------------------------------------------------------------------------
static bool sHasTex0( const MeshVertexFormat & vf )
{
    return vf.hasSemantic( "texcoord" );
}

//
//
// -----------------------------------------------------------------------------
static bool sHasTangent( const MeshVertexFormat & vf )
{
    return vf.hasSemantic( "tangent" );
}

///
/// Determine the best model template that can show the mesh, return NULL for failure
// -----------------------------------------------------------------------------
static const ModelResourceDesc * sDetermineBestModelTemplate( const MeshResourceDesc & m )
{
    const MeshVertexFormat & vf = m.vtxfmt;

    // position is required
    if( !sHasPosition( vf ) )
    {
        GN_ERROR(sLogger)( "The mesh has no position, which is required by the mesh viewer." );
        return NULL;
    }

    if( !sHasNormal( vf ) )
    {
        GN_WARN(sLogger)( "The mesh has no normal." );
        return &SimpleWireframeModel::DESC;
    }

    if( !sHasTex0( vf ) )
    {
        GN_WARN(sLogger)( "The mesh has no texture coordinate." );
        return &SimpleDiffuseModel::DESC;
    }

    // Program reaches here, means that the mesh has position, norml and texcoord.

    if( sHasTangent( vf ) )
    {
        return &SimpleNormalMapModel::DESC;
    }
    else
    {
        return &SimpleDiffuseModel::DESC;
    }
}

//
//
// -----------------------------------------------------------------------------
static bool
sLoadModelHierarchyFromASE( ModelHierarchyDesc & desc, File & file )
{
    // load ASE scene
    AseScene ase;
    if( !ase.loadFromFile(file) ) return false;

    StrA filename = file.name();
    if( filename.empty() )
    {
        GN_WARN(sLogger)( "Can not get filename" );
        return false;
    }
    filename = fs::resolvePath( fs::getCurrentDir(), filename );

#define FULL_MESH_NAME( n ) str::format("%s.%s",filename.rawptr(),n.rawptr())

    // copy meshes. create nodes as well, since in ASE scene, one mesh is one node.
    for( size_t i = 0; i < ase.meshes.size(); ++i )
    {
        const AseMesh & src = ase.meshes[i];

        const StrA    & meshname = FULL_MESH_NAME(src.name);

        // copy mesh
        desc.meshes[meshname] = src;

        // create the node
        ModelHierarchyDesc::NodeDesc & nodeDesc = desc.nodes[meshname];
#if 0
        nodeDesc.parent = src.parent.empty() ? "" : FULL_MESH_NAME(src.parent);
        nodeDesc.position = src.pos;
        nodeDesc.orientation.fromRotation( src.rotaxis, src.rotangle );
#else
        // Note: Ingore hierarchy properties in ASE, since models in ASE have already in "world" space.
        nodeDesc.parent = "";
        nodeDesc.position.set( 0, 0, 0 );
        nodeDesc.orientation.identity();
        nodeDesc.scaling.set( 1, 1, 1 );
#endif
        nodeDesc.bbox = src.selfbbox;
    }

    // copy mesh data
    desc.meshdata = ase.meshdata;

    // create models
    for( size_t i = 0; i < ase.subsets.size(); ++i )
    {
        const AseMeshSubset & subset = ase.subsets[i];

        const AseMesh & asemesh = ase.meshes[subset.meshid];

        // determine the model template
        const ModelResourceDesc * modelTemplate = sDetermineBestModelTemplate( asemesh );
        if( NULL == modelTemplate ) continue;

        // initialize the model descriptor based on the template
        ModelResourceDesc model = *modelTemplate;
        model.mesh = FULL_MESH_NAME(asemesh.name);
        model.subset = subset;

        // associate texture to the model
        const AseMaterial & am = ase.materials[subset.matid];
        if( model.hasTexture("ALBEDO_TEXTURE") && !am.mapdiff.bitmap.empty() )
        {
            model.textures["ALBEDO_TEXTURE"].resourceName = am.mapdiff.bitmap;
        }
        if( model.hasTexture("NORMAL_TEXTURE") && !am.mapbump.bitmap.empty() )
        {
            model.textures["NORMAL_TEXTURE"].resourceName = am.mapbump.bitmap;
        }

        // add the model to model list
        StrA modelname = str::format( "%s.%u", asemesh.name.rawptr(), i );
        GN_ASSERT( NULL == desc.models.find( modelname ) );
        desc.models[modelname] = model;

        // add the model to appropriate node
        GN_ASSERT( NULL != desc.nodes.find( model.mesh ) );
        desc.nodes[model.mesh].models.append( modelname );
    }

#if ROOT_BBOX
    // setup bounding box of the whole scene
    desc.bbox = ase.bbox;
#endif

    return true;
}

}

// *****************************************************************************
//
// FBX loader
//
// *****************************************************************************

namespace fbx
{
#ifdef HAS_FBX

class FbxSdkWrapper
{
public:

    FbxManager * manager;
    FbxGeometryConverter * converter;

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
        manager = FbxManager::Create();
        if( NULL == manager ) return false;

    	// create an IOSettings object
    	FbxIOSettings * ios = FbxIOSettings::Create( manager, IOSROOT );
    	manager->SetIOSettings(ios);

        // create a converter
        converter = new FbxGeometryConverter(manager);

    	// Load plugins from the executable directory
    	FbxString lPath = FbxGetApplicationDirectory();
#if defined(FBXSDK_ENV_WIN)
    	FbxString lExtension = "dll";
#elif defined(FBXSDK_ENV_MAC)
    	FbxString lExtension = "dylib";
#elif defined(FBXSDK_ENV_LINUX)
    	FbxString lExtension = "so";
#endif
    	manager->LoadPluginsDirectory(lPath.Buffer(), lExtension.Buffer());

        return true;
    }

};

//
//
// -----------------------------------------------------------------------------
static const char * sGetTextureFileName( FbxSurfaceMaterial * material, const char * textureType )
{
    FbxProperty prop = material->FindProperty( textureType );
    if( !prop.IsValid() ) return NULL;

    int lLayeredTextureCount = prop.GetSrcObjectCount(FbxLayeredTexture::ClassId);
    if(lLayeredTextureCount > 0 )
    {
        // Layered texture

        for(int j=0; j<lLayeredTextureCount; ++j)
        {
            FbxLayeredTexture *lLayeredTexture = FbxCast <FbxLayeredTexture>(prop.GetSrcObject(FbxLayeredTexture::ClassId, j));

            int lNbTextures = lLayeredTexture->GetSrcObjectCount(FbxTexture::ClassId);

            for(int k =0; k<lNbTextures; ++k)
            {
                FbxFileTexture * lTexture = FbxCast <FbxFileTexture> (lLayeredTexture->GetSrcObject(FbxTexture::ClassId,k));
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
        int lNbTextures = prop.GetSrcObjectCount(FbxTexture::ClassId);
        for(int j =0; j<lNbTextures; ++j)
        {

            FbxFileTexture* lTexture = FbxCast <FbxFileTexture>( prop.GetSrcObject(FbxTexture::ClassId, j) );
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
static inline int sGetLayerElementIndex( const FbxLayerElementTemplate<T> * elements, int index )
{
    if( NULL == elements ) return -1;

    FbxLayerElement::EReferenceMode refmode = elements->GetReferenceMode();

    if( FbxLayerElement::eDirect == refmode )
    {
        return index;
    }
    else if( FbxLayerElement::eIndexToDirect == refmode )
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
    const FbxLayerElementTemplate<T> * elements,
    int vertexIndex,
    int polygonIndex,
    int cornerIndex )
{
    if( NULL == elements ) return -1;

    FbxLayerElement::EMappingMode mapmode = elements->GetMappingMode();

    if( FbxLayerElement::eByControlPoint == mapmode )
    {
        return sGetLayerElementIndex( elements, vertexIndex );
    }
    else if( FbxLayerElement::eByPolygonVertex == mapmode )
    {
        return sGetLayerElementIndex( elements, polygonIndex * 3 + cornerIndex );
    }
    else if( FbxLayerElement::eByPolygon == mapmode )
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
    FbxLayerElementMaterial * materials;

    SortPolygonByMaterial( FbxLayerElementMaterial * m )
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
    4096,
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
    FbxNode                      * fbxnode,
    FbxMesh                      * fbxmesh,
    const char                   * meshName )
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
    FbxLayer * layer0 = fbxmesh->GetLayer(0);
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
    const FbxVector4            * fbxPositions = fbxmesh->GetControlPoints();
    FbxLayerElementUV           * fbxUVs       = layer0->GetUVs();
    FbxLayerElementNormal       * fbxNormals   = layer0->GetNormals();
    FbxLayerElementMaterial     * fbxMaterials = layer0->GetMaterials();
    //FbxLayerElementVertexColor * fbxColors    = layer0->GetVertexColors();
    //FbxLayerElementTangent     * fbxTangents  = layer0->GetTangents();
    //FbxLayerElementBinormal    * fbxBinormals = layer0->GetBinormals();
    int                           numtri       = fbxmesh->GetPolygonCount();
    int                           numidx       = numtri * 3;

    // How many materials are there?
    int nummat;
    if( fbxMaterials &&
        FbxLayerElement::eByPolygon == fbxMaterials->GetMappingMode() &&
        FbxLayerElement::eIndexToDirect == fbxMaterials->GetReferenceMode() )
    {
        // multiple materials
        nummat = fbxnode->GetMaterialCount();
    }
    else
    {
        if( fbxMaterials && FbxLayerElement::eAllSame != fbxMaterials->GetMappingMode() )
        {
            GN_WARN(sLogger)("Unsupported FBX material layer: mapping mode=%d, reference mode=%d. It will be treated as eAllSame.", fbxMaterials->GetMappingMode(), fbxMaterials->GetReferenceMode() );
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
    for( uint32 sortedPolygonIndex = 0; sortedPolygonIndex < sortedPolygons.size(); ++sortedPolygonIndex )
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
            FbxSurfaceMaterial * mat = fbxnode->GetMaterial( matid );
            if( mat )
            {
                StrA dirname = fs::dirName( filename );
                const char * diffuse = sGetTextureFileName( mat, FbxSurfaceMaterial::sDiffuse );
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
            const FbxVector4 & fbxnormal = fbxNormals->GetDirectArray().GetAt(normalIndex);
            key.normal.set( (float)fbxnormal[0], (float)fbxnormal[1], (float)fbxnormal[2] );
            if( fbxUVs )
            {
                const FbxVector2 & fbxUV = fbxUVs->GetDirectArray().GetAt(uvIndex);
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

                const FbxVector4 & fbxvertex = fbxPositions[posIndex];
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
    const MeshVertex * vertices = vertexBlob->array().rawptr();
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

        StrA modelName = str::format( "%s.%d", meshName, i );
        desc.models[modelName] = model;
        gnnode.models.append( modelName );
    }
}

//
//
// -----------------------------------------------------------------------------
static bool
sLoadFbxNodeRecursivly(
    ModelHierarchyDesc & desc,
    const StrA         & filename,
    FbxSdkWrapper      & sdk,
    FbxNode            * node,
    FbxNode            * parent )
{
    if( NULL == node ) return true;

    // the node name should be unique
    // TODO: if the name is not unique, make it unique.
    const char * name = node->GetName();
    if( desc.nodes.find(name) )
    {
        GN_WARN(sLogger)( "Node named %s exists already.", name );
        return true;
    }

    // Get node type
    FbxNodeAttribute* attrib = node->GetNodeAttribute();
    FbxNodeAttribute::EType type = attrib ? attrib->GetAttributeType() : FbxNodeAttribute::eUnknown;

//    if( FbxNodeAttribute::eSkeleton == type )
//    {
//        GN_INFO(sLogger)( "Skeleton animation is not supported yet. So Ignore skeleton node and and its sub nodes: name=%s", name );
//        return true;
//    }

    // we don't support skeleton mesh yet. So ignore skeleton node for now.

    const FbxMatrix & localTransform = node->GetScene()->GetEvaluator()->GetNodeLocalTransform(node);
    FbxVector4    t, s, sh;
    double sign;
    FbxQuaternion q;
    localTransform.GetElements(t, q, sh, s, sign);
    GN_UNUSED_PARAM(sh);
    GN_UNUSED_PARAM(sign);

    ModelHierarchyDesc::NodeDesc & gnnode = desc.nodes[name];
    gnnode.parent = parent ? parent->GetName() : "";
    gnnode.position.set( (float)t[0], (float)t[1], (float)t[2] );
    gnnode.orientation.set( (float)q[0], (float)q[1], (float)q[2], (float)q[3] );
    gnnode.scaling.set( (float)s[0], (float)s[1], (float)s[2] );
    gnnode.bbox.set( 0, 0, 0, 0, 0, 0 );

    if( FbxNodeAttribute::eMesh == type )
    {
        // load mesh node
        StrA fullMeshName = filename + "." + name;
        sLoadFbxMesh( desc, filename, gnnode, sdk, node, (FbxMesh*)attrib, fullMeshName );
    }
    else if(
        // Some nodes are ignored silently.
        FbxNodeAttribute::eNull != type &&
        FbxNodeAttribute::eUnknown != type &&
        FbxNodeAttribute::eLight != type &&
        FbxNodeAttribute::eCamera != type &&
        FbxNodeAttribute::eSkeleton != type )
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
sLoadModelHierarchyFromFBX( ModelHierarchyDesc & desc, File & file )
{
#ifdef HAS_FBX

    FbxSdkWrapper sdk;
    if( !sdk.init() ) return false;
    FbxManager * gSdkManager = sdk.manager;

    // TODO: setup file system.

    // detect file format
    StrA filename = fs::toNativeDiskFilePath( file.name() );
	int lFileFormat = -1;
    if (!gSdkManager->GetIOPluginRegistry()->DetectReaderFileFormat(filename, lFileFormat) )
    {
        // Unrecognizable file format. Try to fall back to FbxImporter::eFBX_BINARY
        lFileFormat = gSdkManager->GetIOPluginRegistry()->FindReaderIDByDescription( "FBX binary (*.fbx)" );;
    }

    // Create the importer.
    FbxImporter* gImporter = FbxImporter::Create(gSdkManager,"");
    if( NULL == gImporter ) return false;
    if(!gImporter->Initialize(filename, lFileFormat))
    {
        GN_ERROR(sLogger)( gImporter->GetStatus().GetErrorString() );
        return false;
    }

    // Import the scene
    FbxScene * gScene = FbxScene::Create( gSdkManager, "" );
    if( NULL == gScene ) return false;
    if(!gImporter->Import(gScene))
    {
        GN_ERROR(sLogger)( gImporter->GetStatus().GetErrorString() );
        return false;
    }

    return sLoadFbxNodeRecursivly( desc, file.name(), sdk, gScene->GetRootNode(), NULL );

#else

    desc.clear();
    GN_ERROR(sLogger)( "Fail to load file %s: FBX is not supported.", file.name().rawptr() );
    return false;

#endif
}

}

// *****************************************************************************
//
// Assimp loader
//
// *****************************************************************************
namespace ai
{

// My own implementation of IOStream
class MyIOStream : public Assimp::IOStream
{
    friend class MyIOSystem;

    AutoObjPtr<File> mFile;

protected:

    // Constructor protected for private usage by MyIOSystem
    MyIOStream( const std::string & filename, const std::string & mode )
    {
        mFile.attach( fs::openFile( filename.c_str(), mode.c_str() ) );
    }

public:

    ~MyIOStream()
    {
        mFile.clear();
    }

    size_t Read( void* pvBuffer, size_t pSize, size_t pCount)
    {
        size_t readen;
        if( mFile && mFile->write( pvBuffer, pSize * pCount, &readen ) )
        {
            return readen;
        }
        else
        {
            return 0;
        }
    }

    size_t Write( const void* pvBuffer, size_t pSize, size_t pCount)
    {
        size_t written;
        if( mFile && mFile->write( pvBuffer, pSize * pCount, &written ) )
        {
            return written;
        }
        else
        {
            return 0;
        }
    }

    aiReturn Seek( size_t pOffset, aiOrigin pOrigin)
    {
        if( mFile )
        {
            FileSeek fs;
            switch( pOrigin )
            {
                case aiOrigin_SET : fs = FileSeek::SET;
                case aiOrigin_CUR : fs = FileSeek::CUR;
                case aiOrigin_END : fs = FileSeek::END;
                default           : return aiReturn_FAILURE;
            }
            return mFile->seek( pOffset, fs ) ? aiReturn_SUCCESS : aiReturn_FAILURE;
        }
        else
        {
            return aiReturn_FAILURE;
        }
    }

    size_t Tell() const
    {
        return mFile ? mFile->tell() : 0;
    }

    size_t FileSize() const
    {
        return mFile ? mFile->size() : 0;
    }

    void Flush ()
    {
    }
};

// Fisher Price - My First Filesystem
class MyIOSystem : public Assimp::IOSystem
{
    MyIOSystem()
    {
    }

    ~MyIOSystem()
    {
    }

    // Check whether a specific file exists
    bool Exists( const std::string & filename ) const
    {
        return GN::fs::pathExist( filename.c_str() );
    }

    // Get the path delimiter character we'd like to see
    char GetOsSeparator() const
    {
        return '/';
    }

    // ... and finally a method to open a custom stream
    Assimp::IOStream * Open( const std::string & file, const std::string& mode )
    {
        return new MyIOStream( file, mode );
    }

    void Close( Assimp::IOStream* pFile)
    {
        delete pFile;
    }
};

}

// *****************************************************************************
//
// Garnet's homebew XML loader
//
// *****************************************************************************

namespace xml
{
//
// post error message
// -----------------------------------------------------------------------------
static void sPostXMLError( const XmlNode & node, const StrA & msg )
{
    GN_UNUSED_PARAM( node );
    const XmlElement * e = node.toElement();
    if( e )
    {
        GN_ERROR(sLogger)( "<%s>: %s", e->name.rawptr(), msg.rawptr() );
    }
    else
    {
        GN_ERROR(sLogger)( "%s", msg.rawptr() );
    }
}

//
//
// -----------------------------------------------------------------------------
static bool
sParseModel( ModelHierarchyDesc & desc, XmlElement & root, const StrA & basedir )
{
    ModelResourceDesc md;

    if( !md.loadFromXml( root, basedir ) ) return false;

    XmlAttrib * modelName = root.findAttrib( "name" );
    if( !modelName || modelName->value.empty() )
    {
        GN_ERROR(sLogger)( "Model name attribute is missing." );
        return false;
    }

    if( NULL == desc.meshes.find( md.mesh ) )
    {
        MeshResourceDesc mesh;
        AutoRef<Blob> blob = mesh.loadFromFile( fs::resolvePath( basedir, md.mesh ) );
        if( !blob ) return false;

        desc.meshes[md.mesh] = mesh;
        desc.meshdata.append( blob );
    }

    desc.models[modelName->value] = md;

    return true;
}

//
//
// -----------------------------------------------------------------------------
static bool
sParseNode( ModelHierarchyDesc & desc, XmlElement & root )
{
    GN_ASSERT( root.name == "node" );

    XmlAttrib * nodeName = root.findAttrib( "name" );
    if( NULL == nodeName )
    {
        sPostXMLError( root, "Entity name attribute is missing." );
        return false;
    }

    ModelHierarchyDesc::NodeDesc node;

    // parse spatial
    XmlAttrib * a = root.findAttrib( "parent" );
    if( a ) node.parent = a->value;

    a = root.findAttrib( "position" );
    if( !a || 3 != str::toFloatArray( (float*)&node.position, 3, a->value ) )
    {
        sPostXMLError( root, "Invalid position" );
        node.position.set( 0, 0, 0 );
    }

    a = root.findAttrib( "orientation" );
    if( !a || 4 != str::toFloatArray( (float*)&node.orientation, 4, a->value ) )
    {
        sPostXMLError( root, "Invalid orientation" );
        node.orientation.set( 0, 0, 0, 1 );
    }

    a = root.findAttrib( "scaling" );
    if( !a || 3 != str::toFloatArray( (float*)&node.scaling, 3, a->value ) )
    {
        sPostXMLError( root, "Invalid scaling" );
        node.scaling.set( 1, 1, 1 );
    }

    a = root.findAttrib( "bbox" );
    if( !a || 6 != str::toFloatArray( (float*)&node.bbox, 6, a->value ) )
    {
        sPostXMLError( root, "Invalid bounding box" );
        node.bbox = Boxf( 0, 0, 0, 0, 0, 0 );
    }

    // parse visual
    XmlElement * visualNode = root.findChildElement( "visual" );
    if( visualNode )
    {
        for( XmlNode * n = visualNode->firstc; n != NULL; n = n->nexts )
        {
            XmlElement * e = n->toElement();
            if( !e ) continue;

            if( "model" == e->name )
            {
                XmlAttrib * a = e->findAttrib( "ref" );
                if( !a )
                {
                    sPostXMLError( *e, "ref attribute is missing." );
                    return false;
                }

                node.models.append( a->value );
            }
            else
            {
                sPostXMLError( *e, str::format( "Unknown element: <%s>", e->name.rawptr() ) );
            }
        }
    }

    // done
    desc.nodes[nodeName->value] = node;
    return true;
}

//
//
// -----------------------------------------------------------------------------
static bool
sLoadModelHierarchyFromXML( ModelHierarchyDesc & desc, File & file )
{
    XmlDocument doc;
    XmlParseResult xpr;
    if( !doc.parse( xpr, file ) )
    {
        static Logger * sLogger = getLogger( "GN.base.xml" );
        GN_ERROR(sLogger)(
            "Fail to parse XML file (%s):\n"
            "    line   : %d\n"
            "    column : %d\n"
            "    error  : %s",
            file.name().rawptr(),
            xpr.errLine,
            xpr.errColumn,
            xpr.errInfo.rawptr() );
        return false;
    }
    GN_ASSERT( xpr.root );

    StrA basedir = fs::dirName( file.name() );

    XmlElement * root = xpr.root->toElement();
    if( !root || "modelHierarchy" != root->name )
    {
        sPostXMLError( *root, "Root element name must be \"<modelHierarchy>\"." );
        return false;
    }

#if ROOT_BBOX
    XmlAttrib * bboxAttr = root->findAttrib( "bbox" );
    if( !bboxAttr || 6 != str::toFloatArray( (float*)&desc.bbox, 6, bboxAttr->value ) )
    {
        sPostXMLError( *root, "Invalid bbox attribute." );
        return false;
    }
#endif

    // parse models
    XmlElement * modelsNode = root->findChildElement( "models" );
    if( NULL == modelsNode )
    {
        sPostXMLError( *root, "Element <models> is missing." );
        return false;
    }
    for( XmlNode * n = modelsNode->firstc; n != NULL; n = n->nexts )
    {
        XmlElement * e = n->toElement();
        if( !e ) continue;

        if( "model" == e->name )
        {
            if( !sParseModel( desc, *e, basedir ) ) return false;
        }
        else
        {
            sPostXMLError( *e, str::format( "Ignore unknowned element: <%s>", e->name.rawptr() ) );
        }
    }

    // parse nodes
    XmlElement * entitiesNode = root->findChildElement( "nodes" );
    if( NULL == entitiesNode )
    {
        sPostXMLError( *root, "Element <nodes> is missing." );
        return false;
    }
    for( XmlNode * n = entitiesNode->firstc; n != NULL; n = n->nexts )
    {
        XmlElement * e = n->toElement();
        if( !e ) continue;

        if( "node" == e->name )
        {
            if( !sParseNode( desc, *e ) ) return false;
        }
        else
        {
            sPostXMLError( *e, str::format( "Ignore unknowned element: <%s>", e->name.rawptr() ) );
        }
    }

    // done
    return true;
}

//
//
// -----------------------------------------------------------------------------
static bool
sSaveModelHierarchyToXML( const ModelHierarchyDesc & desc, const char * filename )
{
    // check dirname
    if( NULL == filename )
    {
        GN_ERROR(sLogger)( "NULL directory name" );
        return false;
    }

    // convert to full path
    StrA fullpath = fs::resolvePath( fs::getCurrentDir(), filename );
    filename = fullpath;
    StrA dirname = fs::dirName( fullpath );
    StrA basename = fs::baseName( fullpath );

    if( !fs::isDir( dirname ) )
    {
        GN_ERROR(sLogger)( "%s is not a directory", dirname.rawptr() );
        return false;
    }

    // write meshes
    int meshindex = 0;
    StringMap<char,StrA> meshNameMapping;
    for( const StringMap<char,MeshResourceDesc>::KeyValuePair * i = desc.meshes.first();
        i != NULL;
        i = desc.meshes.next( i ) )
    {
        const StrA & oldMeshName = i->key;
        const MeshResourceDesc & mesh = i->value;

        StrA newMeshName = str::format( "%s.%d.mesh.bin", basename.rawptr(), meshindex );

        if( !mesh.saveToFile( dirname + "\\" + newMeshName ) ) return false;

        meshNameMapping[oldMeshName] = newMeshName;

        ++meshindex;
    }

    // create a new XML document
    XmlDocument xmldoc;
    XmlElement * root = xmldoc.createElement(NULL);
    root->name = "modelHierarchy";

    // write models
    XmlElement * models = xmldoc.createElement( root );
    models->name = "models";
    for( const StringMap<char,gfx::ModelResourceDesc>::KeyValuePair * i = desc.models.first();
         i != NULL;
         i = desc.models.next( i ) )
    {
        const StrA & modelName  = i->key;
        ModelResourceDesc model = i->value;

        StrA * pNewMeshName = meshNameMapping.find( model.mesh );
        if( NULL != pNewMeshName )
        {
            model.mesh = *pNewMeshName;
        }

        XmlElement * modelNode = model.saveToXml( *models, dirname );
        if( !modelNode ) return false;

        XmlAttrib * a = xmldoc.createAttrib( modelNode );
        a->name = "name";
        a->value = modelName;
    }

    // rename nodes
    int entityIndex = 0;
    StringMap<char,StrA> entityNameMap;
    for( const StringMap<char,ModelHierarchyDesc::NodeDesc>::KeyValuePair * i = desc.nodes.first();
        i != NULL;
        i = desc.nodes.next( i ) )
    {
        const StrA & nodeName = i->key;

        entityNameMap[nodeName] = str::format( "%d", ++entityIndex );
    }

    // write nodes
    XmlElement * nodes = xmldoc.createElement( root );
    nodes->name = "nodes";
    for( const StringMap<char,ModelHierarchyDesc::NodeDesc>::KeyValuePair * i = desc.nodes.first();
        i != NULL;
        i = desc.nodes.next( i ) )
    {
        const StrA                         & nodeName = *entityNameMap.find(i->key);
        const ModelHierarchyDesc::NodeDesc & nodeDesc = i->value;

        XmlElement * node = xmldoc.createElement( nodes );
        node->name = "node";

        XmlAttrib * a = xmldoc.createAttrib( node );
        a->name  = "name";
        a->value = nodeName;

        a = xmldoc.createAttrib( node );
        a->name  = "parent";
        StrA * pParentEntityName = entityNameMap.find(nodeDesc.parent);
        if( NULL != pParentEntityName )
        {
            a->value = *pParentEntityName;
        }
        else if( !nodeDesc.parent.empty() )
        {
            GN_WARN(sLogger)( "Entity %s has invalid parent: %s", i->key, nodeDesc.parent.rawptr() );
        }

        a = xmldoc.createAttrib( node );
        a->name  = "position";
        a->value = str::format( "%f,%f,%f",
            nodeDesc.position.x,
            nodeDesc.position.y,
            nodeDesc.position.z );

        a = xmldoc.createAttrib( node );
        a->name  = "orientation";
        a->value = str::format( "%f,%f,%f,%f",
            nodeDesc.orientation.v.x,
            nodeDesc.orientation.v.y,
            nodeDesc.orientation.v.z,
            nodeDesc.orientation.w );

        a = xmldoc.createAttrib( node );
        a->name  = "scaling";
        a->value = str::format( "%f,%f,%f",
            nodeDesc.scaling.x,
            nodeDesc.scaling.y,
            nodeDesc.scaling.z );

        a = xmldoc.createAttrib( node );
        a->name  = "bbox";
        a->value = str::format( "%f,%f,%f,%f,%f,%f",
            nodeDesc.bbox.x,
            nodeDesc.bbox.y,
            nodeDesc.bbox.z,
            nodeDesc.bbox.w,
            nodeDesc.bbox.h,
            nodeDesc.bbox.d );

        XmlElement * visual = xmldoc.createElement( node );
        visual->name  = "visual";
        for( size_t i = 0; i < nodeDesc.models.size(); ++i )
        {
            XmlElement * modelref = xmldoc.createElement( visual );
            modelref->name = "model";

            XmlAttrib * a = xmldoc.createAttrib( modelref );
            a->name = "ref";
            a->value = nodeDesc.models[i];
        }
    }

#if ROOT_BBOX
    // write scene bounding box
    XmlAttrib * a = xmldoc.createAttrib( root->toElement() );
    a->name  = "bbox";
    a->value = str::format( "%f,%f,%f,%f,%f,%f",
            desc.bbox.x,
            desc.bbox.y,
            desc.bbox.z,
            desc.bbox.w,
            desc.bbox.h,
            desc.bbox.d );
#endif

    // write XML document
    AutoObjPtr<File> fp( fs::openFile( filename, "wt" ) );
    if( !fp ) return false;
    return xmldoc.writeToFile( *fp, *root, false );
}

}

// *****************************************************************************
// Load from garnet mesh binary
// *****************************************************************************

namespace bin
{

//
//
// -----------------------------------------------------------------------------
bool sLoadModelHierarchyFromMeshBinary( ModelHierarchyDesc & desc, File & fp )
{
    desc.clear();

    const StrA & meshname = fp.name();

    MeshResourceDesc mesh;
    AutoRef<Blob> blob = mesh.loadFromFile( fp );
    if( !blob ) return false;

    // determine the model template
    const ModelResourceDesc * modelTemplate = ase::sDetermineBestModelTemplate( mesh );
    if( NULL == modelTemplate ) return false;

    // initialize the model descriptor based on the template
    ModelResourceDesc model = *modelTemplate;
    model.mesh = meshname;

    // add mesh and model to scene
    desc.meshes[meshname] = mesh;
    desc.meshdata.append( blob );
    desc.models[meshname] = model;

    // create a node for the model
    ModelHierarchyDesc::NodeDesc & ed = desc.nodes[meshname];
    ed.position.set( 0, 0, 0 );
    ed.orientation.set( 0, 0, 0, 1 );
    mesh.calculateBoundingBox( ed.bbox );
    ed.models.append( meshname );

#if ROOT_BBOX
    desc.bbox = ed.bbox;
#endif

    // done

    return true;
}

}
// *****************************************************************************
// ModelHierarchyDesc
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN_API bool GN::gfx::ModelHierarchyDesc::loadFromFile( const char * filename )
{
    GN_SCOPE_PROFILER( ModelHierarchyDesc_loadFromFile, "Load models hierarchy from file" );

    GN_INFO(sLogger)( "Load models from file: %s", filename?filename:"<NULL>" );

    clear();

    // open file
    AutoObjPtr<File> fp( fs::openFile( filename, "rb" ) );
    if( !fp ) return false;

    // get file extension
    StrA ext = fs::extName( filename );

    // do loading
    if( sStrEndWithI( filename, ".xml" ) )
    {
        if( !xml::sLoadModelHierarchyFromXML( *this, *fp ) ) return false;
    }
    else if( sStrEndWithI( filename, ".ase" ) )
    {
        if( !ase::sLoadModelHierarchyFromASE( *this, *fp ) ) return false;
    }
    else if( sStrEndWithI( filename, ".fbx" ) )
    {
        if( !fbx::sLoadModelHierarchyFromFBX( *this, *fp ) ) return false;
    }
    else if( sStrEndWithI( filename, ".xpr" ) ||
             sStrEndWithI( filename, ".tpr" ) )
    {
        if( !xpr::sLoadModelHierarchyFromXPR( *this, *fp ) ) return false;
    }
    else if( sStrEndWithI( filename, ".mesh.bin" ) )
    {
        if( !bin::sLoadModelHierarchyFromMeshBinary( *this, *fp ) ) return false;
    }
    else
    {
        GN_ERROR(sLogger)( "Unknown file extension: %s", ext.rawptr() );
        return false;
    }

    size_t totalVerts = 0;
    size_t totalFaces = 0;
    for( const StringMap<char,MeshResourceDesc>::KeyValuePair * p = meshes.first(); p != NULL; p = meshes.next( p ) )
    {
        const MeshResourceDesc & m = p->value;
        totalVerts += m.numvtx;
        totalFaces += m.numidx / 3;
    }
    GN_INFO(sLogger)( "Total vertices: %d, faces: %d", totalVerts, totalFaces );

    return true;
}

///
/// write world description to file
// -----------------------------------------------------------------------------
GN_API bool GN::gfx::ModelHierarchyDesc::saveToFile( const char * filename ) const
{
    GN_SCOPE_PROFILER( ModelHierarchyDesc_saveToFile, "Save models hierarchy to file" );

    GN_INFO(sLogger)( "Write scene to : %s", filename );

    return xml::sSaveModelHierarchyToXML( *this, filename );
}
