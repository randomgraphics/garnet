#include "pch.h"
#include "ase.h"
#include <garnet/gfx/fatModel.h>

#if GN_MSVC
#pragma warning(disable:4100) // unreferenced formal parameter
#endif
#include <assimp/assimp.h>
#include <assimp/aiScene.h>       // Output data structure
#include <assimp/aiPostProcess.h> // Post processing flags
#include <assimp/IOStream.h>
#include <assimp/IOSystem.h>

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


#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef INOUT
#define INOUT
#endif

// *****************************************************************************
// Common utilities
// *****************************************************************************

struct JointLocation
{
    uint32 skeletonIndex;
    uint32 jointIndex;
};

//
//
// -----------------------------------------------------------------------------
static void sBuildJointMapFromSkeletons(
    StringMap<char,JointLocation>       & jointMap,
    const DynaArray<FatSkeleton,uint32> & skeletons )
{
    jointMap.clear();
    for( uint32 s = 0; s < skeletons.size(); ++s )
    {
        const FatSkeleton & fatsk = skeletons[s];
        for( uint32 j = 0; j < fatsk.joints.size(); ++j )
        {
            const StrA & jointName = fatsk.joints[j].name;
            JointLocation location = { s, j };
            jointMap.insert( jointName, location );
            //printf( "Insert joint %s to joint map.\n", jointName );
        }
    }
}

//
// Search through all skeletons for joint with specific name.
// -----------------------------------------------------------------------------
static bool
sSearchForNamedJoint(
    OUT uint32                              & skeleton,
    OUT uint32                              & joint,
    IN  const StringMap<char,JointLocation> & jointMap,
    IN  const char                          * jointName )
{
    const JointLocation * location = jointMap.find( jointName );
    if( location )
    {
        skeleton = location->skeletonIndex;
        joint    = location->jointIndex;
        return true;
    }
    else
    {
        return false;
    }
}


//
//
// -----------------------------------------------------------------------------
template<uint32 MAX_JOINTS_PER_VERTEX>
static void sAddNewBone(
    INOUT uint32 * joints,
    INOUT float  * weights,
    IN    uint32   newJoint,
    IN    float    newWeight,
    // Parameters bellow are only used in error logging.
    IN    uint32   vertexIndex,
    IN    uint32   skeletonIndex )
{
    // Add new bone and weight to the skinning structure, and keep the
    // joints in the desending order of weights. We support limited number
    // of joints per vertex. When too many joints are affecting a single
    // vertex, joints with less influence will be dropped, with warning.

    // The loop index "j" is deliberately set to an unsigned integer.
    // When the loop reaches the end, j's value should be -1, which, in term
    // of unsigned int, is actually 0xFFFFFFFF.
    for( uint32 j = MAX_JOINTS_PER_VERTEX - 1; j < MAX_JOINTS_PER_VERTEX; --j )
    {
        if( FatJoint::NO_JOINT == joints[j] ||
            weights[j] < newWeight )
        {
            // The new/current weight is larger than the existing weight in slot #j,
            // or there's no joint in slot #j. We need to move the existing joint
            // to the next slot to make space for the new joint, or drop the existing
            // joint, if the joint array is full already.

            if( (j+1) < MAX_JOINTS_PER_VERTEX )
            {
                joints[j+1]  = joints[j];
                weights[j+1] = weights[j];

                if( 0 == j )
                {
                    // The current weight is larger than any existing weights the
                    // the array. So just store current joint and weight in slot 0.
                    joints[0]  = newJoint;
                    weights[0] = newWeight;
                }
            }
            else if( FatJoint::NO_JOINT != joints[j] )
            {
                // The joint array is full. And the last existing weight in the array
                // is smaller then the new/current weight. So the existing one will be
                // dropped, with warning.
                GN_VERBOSE(sLogger)(
                    "Vertex %d has more than 4 joints attatched. "
                    "Joint %d (weight=%f) in skeleton %d is going to be ignore, "
                    "because it has less influence to the vertex "
                    "then other joints.",
                    vertexIndex,
                    joints[j], weights[j],
                    skeletonIndex );
            }
        }
        else
        {
            // The new weight is smaller then the weight in slot #j. So we need
            // to store the new weight in slot (j+1), or ignore the new weight
            // if the joint array is full already.

            if( (j+1) < MAX_JOINTS_PER_VERTEX )
            {
                // Store new joint at slot (j+1)
                joints[j+1] = newJoint;
                weights[j+1] = newWeight;
            }
            else
            {
                // Drop the new joint, since the joint array is full already.
                GN_VERBOSE(sLogger)(
                    "Vertex %d has more than 4 joints attatched. "
                    "Joint %d (weight=%f) in skeleton %d is going to be ignore, "
                    "because it has less influence to the vertex "
                    "then other joints.",
                    vertexIndex,
                    newJoint, newWeight,
                    skeletonIndex );
            }

            // We've done with the new joint. It has been either stored in the array,
            // or dropped already.
            break;
        }
    }
}

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

    fatmodel.name = filename;

    // copy materials
    fatmodel.materials.resize( (uint32)ase.materials.size() );
    for( uint32 i = 0; i < ase.materials.size(); ++i )
    {
        const AseMaterial & src = ase.materials[i];

        FatMaterial & dst = fatmodel.materials[i];

        dst.clear();

        dst.name = src.name;
        dst.albedoTexture = src.mapdiff.bitmap;
        dst.normalTexture = src.mapbump.bitmap;
        dst.albedoColor.set( src.diffuse, 1.0f );
    }

    // copy meshes
    fatmodel.meshes.resize( (uint32)ase.meshes.size() );
    for( uint32 i = 0; i < ase.meshes.size(); ++i )
    {
        fatmodel.meshes[i] = NULL;

        const AseMesh & src = ase.meshes[i];
        AutoObjPtr<FatMesh> dst( new FatMesh );

        // determine vertex format
        const MeshVertexElement * position = NULL;
        const MeshVertexElement * normal = NULL;
        const MeshVertexElement * texcoord = NULL;
        uint32 vtxfmt = 0;
        for( uint32 e = 0; e < src.vtxfmt.numElements; ++e )
        {
            const MeshVertexElement & mve = src.vtxfmt.elements[e];
            if( 0 == stringCompare( "POSITION", mve.semantic ) )
            {
                position = &mve;
                vtxfmt |= 1 << FatVertexBuffer::POSITION;
            }
            else if( 0 == stringCompare( "NORMAL", mve.semantic ) )
            {
                normal = &mve;
                vtxfmt |= 1 << FatVertexBuffer::NORMAL;
            }
            else if( 0 == stringCompare( "TEXCOORD", mve.semantic ) )
            {
                texcoord = &mve;
                vtxfmt |= 1 << FatVertexBuffer::TEXCOORD0;
            }
        }

        // copy vertex buffer.
        if( !dst->vertices.resize( vtxfmt, src.numvtx ) )
        {
            GN_ERROR(sLogger)( "Out of memory." );
            return false;
        }
        if( position )
        {
            sCopyVertexElement<Vector3f>( dst->vertices.getPosition(), src, *position );
            dst->vertices.setElementFormat( FatVertexBuffer::POSITION, ColorFormat::FLOAT3 );
        }
        if( normal )
        {
            sCopyVertexElement<Vector3f>( dst->vertices.getNormal(), src, *normal );
            dst->vertices.setElementFormat( FatVertexBuffer::NORMAL, ColorFormat::FLOAT3 );
        }
        if( texcoord )
        {
            sCopyVertexElement<Vector2f>( dst->vertices.getTexcoord(0), src, *texcoord );
            dst->vertices.setElementFormat( FatVertexBuffer::TEXCOORD0, ColorFormat::FLOAT2 );
        }

        // copy index buffer
        if( !dst->indices.resize( src.numidx ) )
        {
            GN_ERROR(sLogger)( "Out of memory." );
            return false;
        }
        if( src.idx32 )
        {
            memcpy( dst->indices.rawptr(), src.indices, src.numidx * 4 );
        }
        else
        {
            const uint16 * s = (const uint16*)src.indices;
            uint32 * d = dst->indices.rawptr();
            for( size_t i = 0; i < src.numidx; ++i, ++s, ++d )
            {
                *d = *s;
            }
        }
        dst->primitive = PrimitiveType::TRIANGLE_LIST;

        dst->bbox = src.selfbbox;

        fatmodel.meshes[i] = dst.detach();
    }

    // copy subsets
    for( size_t i = 0; i < ase.subsets.size(); ++i )
    {
        const AseMeshSubset & src = ase.subsets[i];

        if( fatmodel.meshes[src.meshid] )
        {
            FatMeshSubset dst;
            dst.material = src.matid;
            dst.basevtx = src.basevtx;
            dst.numvtx = src.numvtx;
            dst.startidx = src.startidx;
            dst.numidx = src.numidx;
            fatmodel.meshes[src.meshid]->subsets.append( dst );
        }
    }

    // copy bounding box of the whole scene
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

    StringMap<char,JointLocation> jointMap;

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
static const char * sGetTextureFileName( const KFbxSurfaceMaterial * material, const char * textureType )
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

struct Skinning
{
    uint32 joints[4];  //< Joint Index
    float  weights[4]; //< Binding weight;

    bool operator==( const Skinning & rhs ) const
    {
        for( int i = 0; i < 4; ++i )
        {
            if( joints[i]  != rhs.joints[i] ||
                weights[i] != rhs.weights[i] )
            {
                return false;
            }
        }

        return true;
    }
};

struct MeshVertexCache
{
    Vector3f                  pos;
    DynaArray<Vector3f,uint8> normals;
    DynaArray<Vector2f,uint8> tc0;
    DynaArray<Skinning,uint8> skinnings;

    template<typename T>
    uint8 AddAttribute( DynaArray<T,uint8> & array, const T & value )
    {
        for( uint8 i = 0; i < array.size(); ++i )
        {
            if( array[i] == value ) return i;
        }
        array.append( value );
        return array.size() - 1;
    }
};

struct MeshVertexKey
{
    int      pos;      //< Index of the the vertex in FBX mesh control point array.
    uint8    normal;   //< Index into MeshVertexCache.normal.
    uint8    tc0;      //< Index into MeshVertexCache.texcoord0.
    uint8    skinning; //< Index into MeshVertexCache.skinnings;

    MeshVertexKey()
        : pos(-1)
        , normal((uint8)-1)
        , tc0((uint8)-1)
        , skinning((uint8)-1)
    {
    }
};

typedef HashMap<
    MeshVertexKey,
    uint32,
    4096,
    HashMapUtils::HashFunc_MemoryHash<MeshVertexKey>,
    HashMapUtils::EqualFunc_MemoryCompare<MeshVertexKey> > MeshVertexMap;

#if 0
/*
//
// -----------------------------------------------------------------------------
static void sLoadFbxSkin(
    FbxSdkWrapper & sdk,
    KFbxSkin & skin )
{
    // Load bind pose
    int numClusters = skin.GetClusterCount();
    for( int i = 0; i < numClusters; ++i )
    {

    }
}//*/

//
//
// -----------------------------------------------------------------------------
static void
sLoadFbxLimbNodeRecursivly(
    FatSkeleton & fatsk,
    KFbxNode    * fbxnode )
{
    if( NULL == fbxnode ) return;

    KFbxSkeleton * fbxsk = fbxnode->GetSkeleton();
    if( NULL == fbxsk ) return;

    KFbxNode * fbxnode = skeleton->GetNode();
    const StrA & skeletonName = node->GetName();

    int count = node->GetChildCount();
    for( int i = 0; i < count; ++i )
    {
        sLoadFbxLimbRecursivly( fatsk, node->GetChild( i ) );
    }
}

//
//
// -----------------------------------------------------------------------------
static void
sLoadFbxSkeleton(
    FatModel      & fatmodel,
    FbxSdkWrapper & sdk,
    KFbxSkeleton  * fbxsk )
{
    KFbxNode * node = skeleton->GetNode();

    FatSkeleton fatsk;
    fatsk.name = node->GetName();

    fatsk.bindPose.resize( 1 );
    fatsk.bindPose[0].parent = -1;
    fatsk.bindPose[0].transform.setIdentity();

    int count = node->GetChildCount();
    for( int i = 0; i < count; ++i )
    {
        sLoadFbxLimbRecursivly( fatsk, node->GetChild( i ) );
    }
}
#endif

//
// Convert FBX matrix (column-major) to Garnet matrix (row-major)
// -----------------------------------------------------------------------------
Matrix44f & sFbxMatrix2GarnetMatrix( Matrix44f & gnmat, const KFbxXMatrix & fbxmat )
{
    const double * d44 = fbxmat;

    gnmat.set(
        (float)d44[0], (float)d44[4], (float)d44[8],  (float)d44[12],
        (float)d44[1], (float)d44[5], (float)d44[9],  (float)d44[13],
        (float)d44[2], (float)d44[6], (float)d44[10], (float)d44[14],
        (float)d44[3], (float)d44[7], (float)d44[11], (float)d44[15] );

    return gnmat;
}

//
// See if the FBX node contains an eLIMB_NODE skeleton attribute.
// -----------------------------------------------------------------------------
static KFbxSkeleton * sFbxNode2LimbNode( KFbxNode * fbxnode )
{
    if( NULL == fbxnode )
    {
        return NULL;
    }

    KFbxSkeleton * fbxlimb = fbxnode->GetSkeleton();

    if( NULL == fbxlimb )
    {
        return NULL;
    }

    if( fbxlimb->GetSkeletonType() != KFbxSkeleton::eLIMB_NODE )
    {
        return NULL;
    }

    return fbxlimb;
}

//
//
// -----------------------------------------------------------------------------
static void
sLoadFbxLimbNodes(
    FatSkeleton & fatsk,
    uint32        parent,
    uint32        previousSibling,
    KFbxNode    * fbxnode )
{
    if( NULL == fbxnode ) return;

    KFbxSkeleton * limb = sFbxNode2LimbNode(fbxnode);
    if( NULL == limb ) return;

    FatJoint newjoint;

    // Store joint name
    newjoint.name = fbxnode->GetName();

    // Store the parent.
    newjoint.parent = parent;

    // Setup the default child and sibling of the new joint
    newjoint.child   = FatJoint::NO_JOINT;
    newjoint.sibling = FatJoint::NO_JOINT;

    // If the new joint has parent, and the parent has no child yet,
    // then set the new joint as the first child of the parent.
    if( FatJoint::NO_JOINT != parent &&
        FatJoint::NO_JOINT == fatsk.joints[parent].child )
    {
        GN_ASSERT( parent < fatsk.joints.size() );
        fatsk.joints[parent].child = fatsk.joints.size();
    }

    // If the new joint has previous sibling, then set the new
    // joint as its "next" sibling.
    if( FatJoint::NO_JOINT != previousSibling )
    {
        GN_ASSERT( previousSibling < fatsk.joints.size() );
        GN_ASSERT( FatJoint::NO_JOINT == fatsk.joints[previousSibling].sibling );
        fatsk.joints[previousSibling].sibling = fatsk.joints.size();
    }

    // Get the global transform of the node. Note that it is the transformation
    // from joint space to model space.
    sFbxMatrix2GarnetMatrix(
        newjoint.bindPose.model2joint,
        fbxnode->GetScene()->GetEvaluator()->GetNodeGlobalTransform( fbxnode ) );

    // Inverse that to get the bind pose transfomration we want.
    newjoint.bindPose.model2joint.inverse();

    // Now get joint local transformations.
    KFbxXMatrix localTransform = fbxnode->GetScene()->GetEvaluator()->GetNodeLocalTransform( fbxnode );
    KFbxVector4 t = localTransform.GetT(),
                r = localTransform.GetR(),
                s = localTransform.GetS();
    newjoint.bindPose.position.set( (float)t[0], (float)t[1], (float)t[2] );
    newjoint.bindPose.rotation.set( (float)r[0], (float)r[1], (float)r[2], (float)r[3] );
    newjoint.bindPose.scaling.set ( (float)s[0], (float)s[1], (float)s[2] );

    // Add the new joint into the joint array
    fatsk.joints.append( newjoint );

    // recursively load subtrees
    uint32 newParent = fatsk.joints.size() - 1;
    uint32 newPrevSibling = FatJoint::NO_JOINT;
    int count = fbxnode->GetChildCount();
    for( int i = 0; i < count; ++i )
    {
        uint32 oldJointCount = fatsk.joints.size();

        // load subtree
        sLoadFbxLimbNodes( fatsk, newParent, newPrevSibling, fbxnode->GetChild( i ) );

        uint32 newJointCount = fatsk.joints.size();

        // If sLoadFbxLimbNodes() creates more joints, then the first new joint must
        // be the root joint of the subtree, which is the previous sibling of the next
        // subtree.
        if( newJointCount > oldJointCount )
        {
            newPrevSibling = oldJointCount;
        }
    }
}

//
//
// -----------------------------------------------------------------------------
static void
sLoadFbxSkeletons(
    FatModel & fatmodel,
    KFbxNode * fbxnode )
{
    if( sFbxNode2LimbNode( fbxnode ) )
    {
        // We get a new skeleton, load all sub limbs.
        FatSkeleton fatsk;
        sLoadFbxLimbNodes( fatsk, FatJoint::NO_JOINT, FatJoint::NO_JOINT, fbxnode );
        if( !fatsk.joints.empty() )
        {
            fatmodel.skeletons.append( fatsk );
        }
    }
    else if( fbxnode )
    {
        // continue search its subtree for skeletons.
        int count = fbxnode->GetChildCount();
        for( int i = 0; i < count; ++i )
        {
            sLoadFbxSkeletons( fatmodel, fbxnode->GetChild( i ) );
        }
    }

#if 0
    // Print joint hierarchy
    if( fbxnode && fbxnode == fbxnode->GetScene()->GetRootNode() )
    {
        for( uint32 i = 0; i < fatmodel.skeletons.size(); ++i )
        {
            StrA s;
            fatmodel.skeletons[i].printJointHierarchy( s );
            GN_INFO(sLogger)( s );
        }
    }
#endif
}

/*
// Search through all skeletons for joint with specific name.
// -----------------------------------------------------------------------------
static void sLoadFbxAnimations( FatModel & fatmodel, KFbxScene * fbxscene )
{

}*/

//
// Get vertex skinning information for the vertex specified by controlPointIndex.
// -----------------------------------------------------------------------------
static void
sLoadFbxVertexSkinning(
    INOUT uint32         & skeleton, // Index into FatModel::skeleton arrayreturns the skeleton that of the joint
    OUT   Skinning       & sk,
    IN    FbxSdkWrapper  & sdk,
    IN    const KFbxMesh * fbxmesh,
    IN    int              controlPointIndex,
    IN    bool             firstVertex )
{
    // Initialize the global skeleton value for the first vertex.
    if( firstVertex )
    {
        skeleton = FatMesh::NO_SKELETON;
    }

    // set the default binding: bind to nothing.
    for( size_t i = 0; i < GN_ARRAY_COUNT(sk.joints); ++i )
    {
        sk.joints[i]  = FatJoint::NO_JOINT;
        sk.weights[i] = 0;
    }

    // Search through all deformers
    int numdef = fbxmesh->GetDeformerCount();
    for( int iDef = 0; iDef < numdef; ++iDef )
    {
        KFbxDeformer * def = fbxmesh->GetDeformer( iDef );
        KFbxDeformer::EDeformerType deftype = def->GetDeformerType();

        // Ignore the deformer, if it is not a skin.
        if( KFbxDeformer::eSKIN != deftype ) continue;

        KFbxSkin * skin = (KFbxSkin*)def;

        // Search through all clusters
        int numclusters = skin->GetClusterCount();
        for( int iCluster = 0; iCluster < numclusters; ++iCluster )
        {
            KFbxCluster * cl = skin->GetCluster( iCluster );

            KFbxNode * link = cl->GetLink();
            if( NULL == link ) continue;

            uint32 currentSkeleton;
            uint32 currentJoint;
            if( !sSearchForNamedJoint(
                currentSkeleton,
                currentJoint,
                sdk.jointMap,
                link->GetName() ) )
            {
                // The cluster links to an node that is not part of any skeletons.
                // This should not happen for a valid FBX file. But any way, the cluster
                // has to be ignored, with warning.
                GN_ERROR(sLogger)( "Cluter is ignored, because it is linking to a node that is not part of any skeleton." );
                continue;
            }

            // See if the cluster is linking to the same skeleton as what is linking to
            // the first vertex.
            if( firstVertex )
            {
                // This is the first vertex. Update the "global" skeleton.
                skeleton = currentSkeleton;
            }
            else if( skeleton != currentSkeleton )
            {
                // The cluster is linking to a skeleton other than the skeleton linked to vertex #0.
                // We support at most one skeleton per mesh. So this cluster will be ignored, with warning.
                GN_ERROR(sLogger)( "Cluster is ignored, because it is linking to another skeleton." );
                continue;
            }

            // Search through all control points of the current cluster.
            int cpicount = cl->GetControlPointIndicesCount();
            const int * cpi = cl->GetControlPointIndices();
            const double * weights = cl->GetControlPointWeights();
            for( int iCpi = 0; iCpi < cpicount; ++iCpi )
            {
                // Search though until we found the index that we are looking for.
                if( cpi[iCpi] != controlPointIndex ) continue;

                // So, we found it. This cluster is affecting the vertex. But if
                // the weight is zero, we still need to ignore it.
                float currentWeight = (float)weights[iCpi];
                if( 0 == currentWeight ) continue;

                // Add the new binding information to the skinning structure.
                sAddNewBone<GN_ARRAY_COUNT(sk.joints)>( sk.joints, sk.weights, currentJoint, currentWeight, controlPointIndex, skeleton );

                // We've assigned the joint linked to the cluster to the vertex. So break out of this
                // loop and contineu with next cluster.
                break;
            }

            // Done with the current cluster. Loop to next one.
        }

        // Done with the current deformer. Loop to next one.
    }

    // Get sum of all weights.
    float sum = 0.0f;
    for( size_t i = 0; i < GN_ARRAY_COUNT(sk.joints); ++i )
    {
        if( FatJoint::NO_JOINT != sk.joints[i] )
        {
            sum += sk.weights[i];
        }
    }

    // If the vertex does not linked to any joints, or the weight sum is zero,
    // then fallback to the root joint.
    if( FatJoint::NO_JOINT == sk.joints[0] || 0.0f == sum )
    {
        sk.joints[0] = 0;
        sk.weights[0] = 1.0;
    }
    else
    {
        // We might have dropped some joints, because we have limit of 4 joints per vertex.
        // So we need to renormalize the joint weight, to keep sum of all weights 1.0.
        float invsum = 1.0f / sum;
        for( size_t i = 0; i < GN_ARRAY_COUNT(sk.joints); ++i )
        {
            if( FatJoint::NO_JOINT != sk.joints[i] )
            {
                sk.weights[i] *= invsum;
            }
        }
    }
}

//
//
// -----------------------------------------------------------------------------
static bool
sGenerateFatVertices(
    FatMesh               & fatmesh,
    KFbxNode              * fbxnode,
    const MeshVertexCache * vertices,
    const MeshVertexKey   * keys,
    uint32                  numkeys )
{
    // Compute the node's global position.
    KFbxXMatrix globalTransform = fbxnode->GetScene()->GetEvaluator()->GetNodeGlobalTransform(fbxnode);

    // Geometry offset is not inherited by the children.
    KFbxXMatrix geometryOffset;
    KFbxVector4 lT, lR, lS;
    lT = fbxnode->GetGeometricTranslation(KFbxNode::eSOURCE_SET);
    lR = fbxnode->GetGeometricRotation(KFbxNode::eSOURCE_SET);
    lS = fbxnode->GetGeometricScaling(KFbxNode::eSOURCE_SET);
    geometryOffset.SetT(lT);
    geometryOffset.SetR(lR);
    geometryOffset.SetS(lS);
    globalTransform = globalTransform * geometryOffset;

    Matrix44f m44;
    sFbxMatrix2GarnetMatrix( m44, globalTransform );

    // This is used to transform normal vector.
    Matrix44f itm44 = Matrix44f::sInvtrans( m44 );

    // Determine vertex layout
    uint32 layout = FatVertexBuffer::POS_NORMAL_TEX;
    bool skinning = fatmesh.skeleton != FatMesh::NO_SKELETON;
    if( skinning ) layout |= (1<<FatVertexBuffer::JOINT_ID) | (1<<FatVertexBuffer::JOINT_WEIGHT);

    // Allocate fat vertex buffer.
    FatVertexBuffer & fatvb = fatmesh.vertices;
    if( !fatvb.resize( layout, numkeys ) ) return false;

    fatvb.setElementFormat( FatVertexBuffer::POSITION,  ColorFormat::FLOAT3 );
    fatvb.setElementFormat( FatVertexBuffer::NORMAL,    ColorFormat::FLOAT3 );
    fatvb.setElementFormat( FatVertexBuffer::TEXCOORD0, ColorFormat::FLOAT2 );
    if( skinning )
    {
        fatvb.setElementFormat( FatVertexBuffer::JOINT_ID,     ColorFormat::UINT4 );
        fatvb.setElementFormat( FatVertexBuffer::JOINT_WEIGHT, ColorFormat::FLOAT4 );
    }

    Vector4f * pos     = (Vector4f*)fatvb.getPosition();
    Vector4f * nml     = (Vector4f*)fatvb.getNormal();
    Vector4f * uv0     = (Vector4f*)fatvb.getTexcoord(0);
    Vector4i * joints  = (Vector4i*)fatvb.getElementData( FatVertexBuffer::JOINT_ID );
    Vector4f * weights = (Vector4f*)fatvb.getElementData( FatVertexBuffer::JOINT_WEIGHT );

    Vector4f v4;

    for( size_t i = 0; i < numkeys; ++i )
    {
        const MeshVertexKey   & k = keys[i];
        const MeshVertexCache & v = vertices[k.pos];

        // translate position to global space
        v4.set( v.pos, 1.0f );
        *pos = m44 * v4;
        float divw = 1.0f / pos->w;
        pos->x *= divw;
        pos->y *= divw;
        pos->z *= divw;
        pos->w  = 1.0f;

        // translate normal to global space.
        if( k.normal < v.normals.size() )
        {
            v4.set( v.normals[k.normal], 0.0f );
            *nml = itm44 * v4;
            nml->w = 0.0f;
            nml->normalize();
        }
        else
        {
            nml->set( 0, 0, 0, 0 );
        }

        if( k.tc0 < v.tc0.size() )
        {
            uv0->set( v.tc0[k.tc0], 0, 0 );
        }
        else
        {
            uv0->set( 0, 0, 0, 0 );
        }

        if( skinning )
        {
            GN_ASSERT( k.skinning < v.skinnings.size() );
            const Skinning & sk = v.skinnings[k.skinning];
            *joints  = *(Vector4i*)&sk.joints;
            *weights = *(Vector4f*)&sk.weights;
        }

        ++pos;
        ++nml;
        ++uv0;
        ++joints;
        ++weights;
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
static void
sLoadFbxMesh(
    FatModel      & fatmodel,
    const StrA    & filename,
    FbxSdkWrapper & sdk,
    KFbxMesh      * fbxmesh )
{
    KFbxNode * fbxnode = fbxmesh->GetNode();

    if( !fbxmesh->IsTriangleMesh() )
    {
        fbxmesh = sdk.converter->TriangulateMesh( fbxmesh );
        if( NULL == fbxmesh )
        {
            GN_ERROR(sLogger)( "Fail to triangulate fbxmesh node: %s", fbxnode->GetName() );
            return;
        }
    }

    // For now, we supports layer 0 only.
    KFbxLayer * layer0 = fbxmesh->GetLayer(0);
    if( NULL == layer0 )
    {
        GN_ERROR(sLogger)( "The fbxmesh does not have a layer: %s", fbxnode->GetName() );
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

    // add materials to fatmodel.
    DynaArray<size_t> fatmatIndices( nummat );
    for( int i = 0; i < nummat; ++i )
    {
        const KFbxSurfaceMaterial * fbxmat = fbxnode->GetMaterial( i );
        GN_ASSERT( fbxmat );

        FatMaterial fatmat;
        fatmat.name = fbxmat->GetName();

        StrA dirname = fs::dirName( filename );
        const char * texname = sGetTextureFileName( fbxmat, KFbxSurfaceMaterial::sDiffuse );
        if( texname ) fatmat.albedoTexture = fs::resolvePath( dirname, texname );

        texname = sGetTextureFileName( fbxmat, KFbxSurfaceMaterial::sNormalMap );
        if( texname ) fatmat.normalTexture = fs::resolvePath( dirname, texname );

        // TODO: get diffuse color.
        fatmat.albedoColor.set( 1, 1, 1, 1 );

        fatmodel.materials.append( fatmat );
        fatmatIndices[i] = fatmodel.materials.size() - 1;
    }

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

    // Create the mesh object
    AutoObjPtr<FatMesh> fatMeshAutoPtr( new FatMesh );
    if( NULL == fatMeshAutoPtr )
    {
        GN_ERROR(sLogger)( "Fail to load FBX mesh: out of memory." );
        return;
    }
    FatMesh & fatmesh = *fatMeshAutoPtr;

    // Allocate index buffer
    if( !fatmesh.indices.resize( (uint32)numidx ) )
    {
        GN_ERROR(sLogger)( "Fail to load FBX mesh: out of memory." );
        return;
    }

    // Declare vertex cache array
    DynaArray<MeshVertexCache,uint32> vcache;
    int numpos = fbxmesh->GetControlPointsCount();
    if( !vcache.resize( (uint32)numpos ) )
    {
        GN_ERROR(sLogger)( "Fail to load FBX mesh: out of memory." );
        return;
    }
    for( int i = 0; i < numpos; ++i )
    {
        const KFbxVector4 & fbxpos = fbxPositions[i];
        vcache[i].pos.set( (float)fbxpos[0], (float)fbxpos[1], (float)fbxpos[2] );
    }

    // Declare the vertex map
    MeshVertexMap vtxmap( (size_t)numidx * 2 );

    // Allocate another buffer to hold the final sequance of vertex keys
    DynaArray<MeshVertexKey,uint32> vertexKeys;
    if( !vertexKeys.reserve( (uint32)numidx ) )
    {
        GN_ERROR(sLogger)( "Fail to load FBX mesh: out of memory." );
        return;
    }

    // Initialize skeleton index of the mesh to default value.
    fatmesh.skeleton = FatMesh::NO_SKELETON;

    // Split the FBX fbxmesh into multiple models, one material one model.
    int uvIndex = 0;
    int normalIndex = 0;
    int lastMatID = -1;
    FatMeshSubset * lastSubset = NULL;
    for( uint32 sortedPolygonIndex = 0; sortedPolygonIndex < sortedPolygons.size(); ++sortedPolygonIndex )
    {
        int polygonIndex = sortedPolygons[sortedPolygonIndex];

        int matid = nummat > 1 ? fbxMaterials->GetIndexArray().GetAt(polygonIndex) : 0;
        GN_ASSERT( matid < nummat );
        GN_ASSERT( matid >= lastMatID );

        // create new subset for each new material
        if( matid != lastMatID )
        {
            lastMatID = matid;

            // create a new subset
            if( !fatmesh.subsets.resize( fatmesh.subsets.size() + 1 ) )
            {
                GN_ERROR(sLogger)( "Fail to load FBX mesh: out of memory." );
                return;
            }
            FatMeshSubset & subset = fatmesh.subsets.back();
            subset.material = (uint32)fatmatIndices[matid];
            subset.startidx = sortedPolygonIndex*3;
            subset.numidx   = 0;
            subset.basevtx  = 0;
            // Note: final number of vertices are unknown yet.

            lastSubset = &subset;
        }
        lastSubset->numidx += 3;

        // add the polygon to the mesh
        for( int i = 0; i < 3; ++i )
        {
            int posIndex = fbxIndices[polygonIndex*3+i];

            MeshVertexCache & vc = vcache[posIndex];

            // create vetex key
            MeshVertexKey key;
            key.pos = posIndex;

            // get normal
            if( -1 != normalIndex )
            {
                normalIndex = sGetLayerElementIndex( fbxNormals, posIndex, polygonIndex, i );

                if( -1 != normalIndex )
                {
                    const KFbxVector4 & fbxnormal = fbxNormals->GetDirectArray().GetAt(normalIndex);
                    Vector3f normal( (float)fbxnormal[0], (float)fbxnormal[1], (float)fbxnormal[2] );
                    key.normal = vc.AddAttribute( vc.normals, normal );
                }
            }

            // get texcoord0
            if( -1 != uvIndex )
            {
                uvIndex = sGetLayerElementIndex( fbxUVs, posIndex, polygonIndex, i );

                if( -1 != uvIndex )
                {
                    const KFbxVector2 & fbxuv = fbxUVs->GetDirectArray().GetAt(uvIndex);
                    // BUGBUG: for some reason, U coordinates has to be inverted (1.0-v) to make the model look right in the viewer.
                    Vector2f tc0( (float)fbxuv[0], (float)(1.0-fbxuv[1]) );
                    key.tc0 = vc.AddAttribute( vc.tc0, tc0 );
                }
            }

            // get skinning information.
            bool firstVertex = ( 0 == polygonIndex && 0 == i );
            Skinning sk;
            if( firstVertex )
            {
                // Always load skinning information for the first vertex.
                // The function will also determine of the mesh is binding to a skeleton or not, by
                // updating fatmesh.skeleton.
                sLoadFbxVertexSkinning( fatmesh.skeleton, sk, sdk, fbxmesh, posIndex, firstVertex );
            }
            else if( fatmesh.skeleton != FatMesh::NO_SKELETON )
            {
                // Load skinning information for the following vertices, only when the mesh is binding
                // to a skeleton.
                sLoadFbxVertexSkinning( fatmesh.skeleton, sk, sdk, fbxmesh, posIndex, firstVertex );
            }
            if( fatmesh.skeleton != FatMesh::NO_SKELETON )
            {
                // Sanity check
                for( size_t i = 0; i < GN_ARRAY_COUNT(sk.joints); ++i )
                {
                    GN_ASSERT(
                        sk.joints[i] == FatJoint::NO_JOINT ||
                        sk.joints[i] < fatmodel.skeletons[fatmesh.skeleton].joints.size() );
                }

                // Update the skinning key only when the mesh has skeleton.
                key.skinning = vc.AddAttribute( vc.skinnings, sk );
            }

            // If the key exists already, the pair will point to it.
            // If the key does not exisit, the pair will point to the newly inserted one.
            // Either way, pair->value should give us the correct index of the vertex.
            MeshVertexMap::KeyValuePair * pair;
            bool isNewVertex = vtxmap.insert( key, (uint32)vtxmap.size(), &pair );
            uint32 vertexIndex = pair->value;

            if( isNewVertex )
            {
                // If it is a new vertex, append it to the vertex key array.
                vertexKeys.append( key );

                GN_ASSERT( vertexKeys.size() == (vertexIndex + 1) );
            }

            // add the vertex index into the index buffer
            fatmesh.indices[sortedPolygonIndex*3+i] = vertexIndex;
        }
    }

    // Now both vertex and index buffers are filled up. Models' subset information are ready too.
    // We are almost there.

    // Fill up the rest of informations for each subset
    for( size_t i = 0; i < fatmesh.subsets.size(); ++i )
    {
        fatmesh.subsets[i].numvtx = (uint32)vertexKeys.size();
    }

    // Now copy vertex data to fatmesh, and translate position and normal to global space.
    if( !sGenerateFatVertices( fatmesh, fbxnode, vcache.rawptr(), vertexKeys.rawptr(), vertexKeys.size() ) ) return;

    // calculate the bounding box of the mesh
    const Vector4f * vertices = (const Vector4f *)fatmesh.vertices.getPosition();
    calculateBoundingBox(
        fatmesh.bbox,
        &vertices->x, sizeof(vertices[0]),
        &vertices->y, sizeof(vertices[0]),
        &vertices->z, sizeof(vertices[0]),
        fatmesh.vertices.getVertexCount() );

    // It's always triangle list.
    fatmesh.primitive = PrimitiveType::TRIANGLE_LIST;

    GN_INFO(sLogger)( "Load FBX mesh %s: %d vertices, %d faces",
        fbxnode->GetName(),
        fatmesh.vertices.getVertexCount(),
        fatmesh.indices.size() );

    // finally, add the fatmesh to fatmodel. And we are done!
    fatmodel.meshes.append( fatMeshAutoPtr.detach() );
}

//
//
// -----------------------------------------------------------------------------
static void
sLoadFbxMeshes(
    FatModel      & fatmodel,
    const StrA    & filename,
    FbxSdkWrapper & sdk,
    KFbxScene     & scene )
{
    // Load meshes
    int meshCount = KFbxGetSrcCount<KFbxMesh>(&scene);
    for( int i=0; i<meshCount; i++ )
    {
        KFbxMesh * fbxmesh = KFbxGetSrc<KFbxMesh>(&scene, i);
        sLoadFbxMesh( fatmodel, filename, sdk, fbxmesh );
    }
}

#endif // HAS_FBX

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

    // Convert Axis System to right handed system, if needed
    KFbxAxisSystem SceneAxisSystem = gScene->GetGlobalSettings().GetAxisSystem();
    KFbxAxisSystem OurAxisSystem(KFbxAxisSystem::YAxis, KFbxAxisSystem::ParityOdd, KFbxAxisSystem::RightHanded);
    if( SceneAxisSystem != OurAxisSystem )
    {
        OurAxisSystem.ConvertScene(gScene);
    }

    // Convert Unit System to 1.0, if needed
    KFbxSystemUnit SceneSystemUnit = gScene->GetGlobalSettings().GetSystemUnit();
    if( SceneSystemUnit.GetScaleFactor() != 1.0 )
    {
        KFbxSystemUnit OurSystemUnit(1.0);
        OurSystemUnit.ConvertScene(gScene);
    }

    // preallocate material array.
    int nummat = KFbxGetSrcCount<KFbxSurfaceMaterial>(gScene);
    fatmodel.materials.reserve( (size_t)nummat );
    //fatmodel.materials.resize( 1 );
    //fatmodel.materials[0].name = "=[DEFAULT]=";
    //fatmodel.materials[0].albedoTexture = "";
    //fatmodel.materials[0].normalTexture = "";
    //fatmodel.materials[0].albedoColor.set( 1, 1, 1, 1 );

    // Load skeletons
    sLoadFbxSkeletons( fatmodel, gScene->GetRootNode() );

    // Build joint map to accelerate joint searching.
    sBuildJointMapFromSkeletons( sdk.jointMap, fatmodel.skeletons );

    // TODO: Load animations
    //sLoadFbxAnimations( fatmodel, gScene->GetRootNode() );

    // Load meshes
    sLoadFbxMeshes( fatmodel, filename, sdk, *gScene );

    // calculate the final bounding box
    fatmodel.calcBoundingBox();

    // done!
    return true;

#else // HAS_FBX

    fatmodel.clear();
    GN_UNUSED_PARAM( file );
    GN_ERROR(sLogger)( "Fail to load file %s: FBX is not supported.", filename.rawptr() );
    return false;

#endif // HAS_FBX
}

static void
sPrintFBXNodeHierarchy( StrA & hierarchy, const StrA & filename )
{
#ifdef HAS_FBX

    FbxSdkWrapper sdk;
    if( !sdk.init() )
    {
        hierarchy = "ERROR: fail to initialize FBX sdk.";
        return;
    }

    KFbxSdkManager * gSdkManager = sdk.manager;

    // detect file format
	int lFileFormat = -1;
    if (!gSdkManager->GetIOPluginRegistry()->DetectReaderFileFormat(filename, lFileFormat) )
    {
        // Unrecognizable file format. Try to fall back to KFbxImporter::eFBX_BINARY
        lFileFormat = gSdkManager->GetIOPluginRegistry()->FindReaderIDByDescription( "FBX binary (*.fbx)" );;
    }

    // Create the importer.
    KFbxImporter* gImporter = KFbxImporter::Create(gSdkManager,"");
    if( NULL == gImporter )
    {
        hierarchy = "ERROR: out of memory.";
        return;
    }
    if(!gImporter->Initialize(filename, lFileFormat))
    {
        hierarchy = gImporter->GetLastErrorString();
        return;
    }

    // Import the scene
    KFbxScene * gScene = KFbxScene::Create( gSdkManager, "" );
    if( NULL == gScene )
    {
        hierarchy = "ERROR: out of memory.";
        return;
    }
    if(!gImporter->Import(gScene))
    {
        hierarchy = gImporter->GetLastErrorString();
        return;
    }

    struct Local
    {
        static void sPrintNodeRecursivly( StrA & hierarchy, KFbxNode * node, int depth )
        {
            if( NULL == node ) return;

            for( int i = 0; i < depth; ++i )
            {
                hierarchy += "  ";
            }

            hierarchy += stringFormat( "(%d) ", depth );

            const char * name = node->GetName();

            hierarchy += stringEmpty(name) ? "[UNNAMED]" : name;

            hierarchy += " : ";

            KFbxNodeAttribute * a = node->GetNodeAttribute();
            if( a )
            {
                static const char * sAttributeTypeNames[] =
                {
                    "eUNIDENTIFIED",
                    "eNULL",
                    "eMARKER",
                    "eSKELETON",
                    "eMESH",
                    "eNURB",
                    "ePATCH",
                    "eCAMERA",
                    "eCAMERA_STEREO",
                    "eCAMERA_SWITCHER",
                    "eLIGHT",
                    "eOPTICAL_REFERENCE",
                    "eOPTICAL_MARKER",
                    "eNURBS_CURVE",
                    "eTRIM_NURBS_SURFACE",
                    "eBOUNDARY",
                    "eNURBS_SURFACE",
                    "eSHAPE",
                    "eLODGROUP",
                    "eSUBDIV",
                    "eCACHED_EFFECT",
                };

                KFbxNodeAttribute::EAttributeType atype = a->GetAttributeType();

                if( KFbxNodeAttribute::eSKELETON == atype )
                {
                    KFbxSkeleton::ESkeletonType stype = ((KFbxSkeleton*)a)->GetSkeletonType();
                    switch(stype)
                    {
                        case KFbxSkeleton::eROOT      : hierarchy += "eSKELETON : eROOT"; break;
                        case KFbxSkeleton::eLIMB      : hierarchy += "eSKELETON : eLIMB"; break;
                        case KFbxSkeleton::eLIMB_NODE : hierarchy += "eSKELETON : eLIMB_NODE"; break;
                        case KFbxSkeleton::eEFFECTOR  : hierarchy += "eSKELETON : eEFFECTOR"; break;
                        default                       : hierarchy += "eSKELETON : [INVALID TYPE]"; break;
                    }
                }
                else if( 0 <= atype && atype < (int)GN_ARRAY_COUNT(sAttributeTypeNames) )
                {
                    hierarchy += sAttributeTypeNames[atype];
                }
                else
                {
                    hierarchy += stringFormat("[INVALID:%d]", atype);
                }
            }
            else
            {
                hierarchy += "[NULL]";
            }

            hierarchy += "\n";

            int count = node->GetChildCount();

            for( int i = 0; i < count; ++i )
            {
                sPrintNodeRecursivly( hierarchy, node->GetChild( i ), depth + 1 );
            }
        }
    };

    Local::sPrintNodeRecursivly( hierarchy, gScene->GetRootNode(), 0 );

    if( hierarchy.empty() ) hierarchy = "[EMPTY FILE]";

#else

    GN_UNUSED_PARAM( filename );
    hierarchy = "No FBX support.";

#endif
}

}

// *****************************************************************************
// Assimp loader
// *****************************************************************************

namespace ai
{

using namespace Assimp;

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

//
//
// -----------------------------------------------------------------------------
static void
sLoadAiJointHierarchy( FatSkeleton & fatsk, uint32 parentJointIndex, const aiNode * ainode )
{
    if( NULL == ainode ) return;

    // Store node name.
    const StrA & name = ainode->mName.data;

    // Search through joints for a joint with the same name as node name.
    uint32 currentJointIndex = FatJoint::NO_JOINT;
    for( uint32 i = 0; i < fatsk.joints.size(); ++i )
    {
        if( fatsk.joints[i].name == name )
        {
            currentJointIndex = i;
            break;
        }
    }

    if( FatJoint::NO_JOINT != currentJointIndex )
    {
        // We found the joint. It must be one of the children of the parent joint.

        // Reference the current joint
        FatJoint & currentJoint = fatsk.joints[currentJointIndex];

        // Store parent index
        currentJoint.parent = parentJointIndex;

        if( FatJoint::NO_JOINT != parentJointIndex )
        {
            // Reference the parent joint.
            FatJoint & parentJoint = fatsk.joints[parentJointIndex];

            // Add the current joint to parent joint's children list. We do prepending
            // instead of appending, because the childen list is a single-linked-list.
            // The prepending operation is much cheaper than appending operation.
            currentJoint.sibling = parentJoint.child;
            parentJoint.child = currentJointIndex;
        }

        // Search through the subtree for more joints.
        for( uint32 i = 0; i < ainode->mNumChildren; ++i )
        {
            sLoadAiJointHierarchy( fatsk, currentJointIndex, ainode->mChildren[i] );
        }
    }
    else if( FatJoint::NO_JOINT == parentJointIndex )
    {
        // The current joint does not match any joints. Since the parent joint index,
        // is NO_JOINT too, it means that we havn't searched deep enough to even find
        // the root node of the skeleton. So keep searching...
        for( uint32 i = 0; i < ainode->mNumChildren; ++i )
        {
            sLoadAiJointHierarchy( fatsk, currentJointIndex, ainode->mChildren[i] );
        }
    }
    else
    {
        // If there's no joint in the skeleton that matches the current node.
        // It means the parent joint must be a leaf joint. So there's no need
        // to search through the sub tree.
        GN_ASSERT( FatJoint::NO_JOINT == fatsk.joints[parentJointIndex].child );
    }
}

//
//
// -----------------------------------------------------------------------------
static bool
sSortJointHierarchy( FatSkeleton & fatsk )
{
    // In an valid joint hierarchy, there should be one joint
    // and one joint only that has no parent.
    uint32 jointArraySize = fatsk.joints.size();
    uint32 root = FatJoint::NO_JOINT;
    for( uint32 i = 0; i < jointArraySize; ++i )
    {
        FatJoint & j = fatsk.joints[i];

        // make sure parent/child/sibling are in vaild range
        if( ( j.parent  >= jointArraySize && FatJoint::NO_JOINT != j.parent  ) ||
            ( j.child   >= jointArraySize && FatJoint::NO_JOINT != j.child   ) ||
            ( j.sibling >= jointArraySize && FatJoint::NO_JOINT != j.sibling ) )
        {
            GN_ERROR(sLogger)( "Invalid joint herarchy: joint %d contains invalid joint index.", i );
            return false;
        }

        // If the joint has no parent, it should be the root joint.
        if( FatJoint::NO_JOINT == j.parent )
        {
            if( root != FatJoint::NO_JOINT )
            {
                // There's more than one joint in the tree that has no parent.
                GN_ERROR(sLogger)( "Invalid joint hierarchy: multiple root joints." );
                return false;
            }

            // Remember index of the root joint. Then continue the loop, to see if
            // there's another root joint.
            root = i;
        }
    }
    if( root == FatJoint::NO_JOINT )
    {
        GN_ERROR(sLogger)( "Invalid joint hierarchy: root joint not found." );
        return false;
    }

    // Move root joint to slot 0, if it is not already there.
    if( 0 != root )
    {
        // Swap joint[0] with joint[root]
        FatJoint tmp = fatsk.joints[root];
        fatsk.joints[root] = fatsk.joints[0];
        fatsk.joints[0] = tmp;

        // Now update indices in all joints:
        //   - if the index points to joint[root], reset it to joint[0]; or,
        //   - if the index points to joint[0], reset it to joint[root].
        for( uint32 i = 0; i < fatsk.joints.size(); ++i )
        {
            FatJoint & j = fatsk.joints[i];

            if( 0 == j.parent ) j.parent = root;
            else if( root == j.parent ) j.parent = 0;

            if( 0 == j.child ) j.child = root;
            else if( root == j.child ) j.child = 0;

            if( 0 == j.sibling ) j.sibling = root;
            else if( root == j.sibling ) j.sibling = 0;
        }
    }

#if 0
    // Count number of joints in the hierarchy recursivly.
    // It should equal size of the joint array.
    struct Local
    {
        static void sCountJointRecursivly( const FatJoint * joints, uint32 arraySize, uint32 & counter, uint32 parent )
        {
            GN_UNIMPL();
        }
    };
    uint32 counter = 0;
    Local::sCountJointRecursivly( fatsk.joints.rawptr(), fatsk.joints.size(), counter, 0 );
    if( counter != fatsk.joints.size() )
    {
        GN_ERROR(sLogger)( "Invalid joint hierarchy!" );
        return false;
    }
#endif

#if 0
    // Print joint hierarchy
    StrA s;
    fatsk.printJointHierarchy( s );
    GN_INFO(sLogger)( s );
#endif

    return true;
}

//
//
// -----------------------------------------------------------------------------
void sLoadAiMeshSkeleton( FatModel & fatmodel, FatMesh & fatmesh, const aiScene & aiscene, const aiMesh & aimesh )
{
    // No skeleton by default.
    fatmesh.skeleton = FatMesh::NO_SKELETON;

    if( 0 == aimesh.mNumBones ) return;

    FatSkeleton fatsk;
    fatsk.name = aimesh.mName.data;

    if( !fatsk.joints.resize( aimesh.mNumBones ) )
    {
        GN_ERROR(sLogger)( "Out of memory." );
        return;
    }

    for( uint32 i = 0; i < aimesh.mNumBones; ++i )
    {
        const aiBone & aibone = *aimesh.mBones[i];

        FatJoint & fatjoint = fatsk.joints[i];

        fatjoint.name = aibone.mName.data;

        fatjoint.bindPose.model2joint = *(Matrix44f*)&aibone.mOffsetMatrix;

        // Setup default hierarchy
        fatjoint.parent  = FatJoint::NO_JOINT;
        fatjoint.child   = FatJoint::NO_JOINT;
        fatjoint.sibling = FatJoint::NO_JOINT;
    }

    // Load joint hierarchy from AI scene
    sLoadAiJointHierarchy( fatsk, FatJoint::NO_JOINT, aiscene.mRootNode );

    // Then sort the hierarchy based on depth, to make sure that the root joint stays at index 0.
    if( !sSortJointHierarchy( fatsk ) ) return;

    // Build local transformations of each node.
    for( uint32 i = 0; i < aimesh.mNumBones; ++i )
    {
        FatJoint & fatjoint = fatsk.joints[i];

        Matrix44f local2parent;
        if( FatJoint::NO_JOINT != fatjoint.parent )
        {
            FatJoint & parentJoint = fatsk.joints[fatjoint.parent];
            local2parent = parentJoint.bindPose.model2joint * Matrix44f::sInverse(fatjoint.bindPose.model2joint);
        }
        else
        {
            local2parent = Matrix44f::sInverse( fatjoint.bindPose.model2joint );
        }

        // TODO: implement Matrix44::decompose(...).
        aiVector3D s;
        aiQuaternion r;
	    aiVector3D t;
        ((aiMatrix4x4*)&local2parent)->Decompose( s, r, t );

        fatjoint.bindPose.position.set( t.x, t.y, t.z );
        fatjoint.bindPose.rotation.set( r.x, r.y, r.z, r.w );
        fatjoint.bindPose.scaling.set( s.x, s.y, s.z );
    }

    // Add the new skeleton to fat model.
    if( !fatmodel.skeletons.append(fatsk) )
    {
        GN_ERROR(sLogger)( "Out of memory." );
        return;
    }

    fatmesh.skeleton = fatmodel.skeletons.size() - 1;
}

//
//
// -----------------------------------------------------------------------------
static uint32 sDetermineFatVertexLayout( const aiMesh * aimesh )
{
    uint32 layout = 1<<FatVertexBuffer::POSITION;

    if( aimesh->mNormals ) layout |= 1<<FatVertexBuffer::NORMAL;

    uint32 maxtc = math::getmin<uint32>( GN_ARRAY_COUNT(aimesh->mTextureCoords), FatVertexBuffer::MAX_TEXCOORDS );
    for( uint32 t = 0; t < maxtc; ++t )
    {
        if( aimesh->mTextureCoords[t] ) layout |= 1<<(FatVertexBuffer::TEXCOORD0 + t);
    }

    if( aimesh->mNumBones > 0 ) layout |= (1<<FatVertexBuffer::JOINT_ID) | (1<<FatVertexBuffer::JOINT_WEIGHT);

    return layout;
}

//
//
// -----------------------------------------------------------------------------
static void
sLoadAiVertexSkinning(
    uint32         joints[],
    float          weights[],
    uint32         vertexIndex,
    uint32         skeletonIndex,
    const aiMesh & aimesh )
{
    const uint32 MAX_JOINTS_PER_VERTEX = 4;

    // setup default skinning: no bone, zero weight.
    for( uint32 i = 0; i < MAX_JOINTS_PER_VERTEX; ++i )
    {
        joints[i] = FatJoint::NO_JOINT;
        weights[i] = 0.0f;
    }

    // Search through all bones for the bone that affects the vertex specified by "vertexIndex".
    for( uint32 b = 0; b < aimesh.mNumBones; ++b )
    {
        const aiBone & aibone = *aimesh.mBones[b];

        for( uint32 w = 0; w <= aibone.mNumWeights; ++w )
        {
            const aiVertexWeight & aivw = aibone.mWeights[w];

            if( aivw.mVertexId != vertexIndex ) continue;

            // OK, we found the bone that affects the vertex. But if the weight
            // is zero, we still need to ignore it.
            if( 0.0 == aivw.mWeight ) continue;

            // Add the new joint binding information into joint and weight array.
            sAddNewBone<MAX_JOINTS_PER_VERTEX>( joints, weights, b, aivw.mWeight, vertexIndex, skeletonIndex );
        }
    }
}

//
//
// -----------------------------------------------------------------------------
static bool sLoadAiVertices(
    FatMesh           & fatmesh,
    const aiScene     * aiscene,
    const aiMesh      * aimesh,
    const aiMatrix4x4 & transform )
{
    FatVertexBuffer & fatvb = fatmesh.vertices;

    uint32 fatlayout = sDetermineFatVertexLayout( aimesh );

    if( !fatvb.resize( fatlayout, aimesh->mNumVertices ) ) return false;

    Vector4f * fatpos = (Vector4f*)fatvb.getPosition();
    if( fatpos ) fatvb.setElementFormat( FatVertexBuffer::POSITION, ColorFormat::FLOAT3 );

    Vector4f * fatnormal = (Vector4f*)fatvb.getNormal();
    if( fatnormal ) fatvb.setElementFormat( FatVertexBuffer::NORMAL, ColorFormat::FLOAT3 );

    Vector4f * fattc0 = (Vector4f*)fatvb.getTexcoord(0);
    // TODO: get texcood format from aimesh.
    if( fattc0 ) fatvb.setElementFormat( FatVertexBuffer::TEXCOORD0, ColorFormat::FLOAT2 );

    Vector4<uint32> * fatjoint = (Vector4<uint32>*)fatvb.getElementData( FatVertexBuffer::JOINT_ID );
    if( fatjoint ) fatvb.setElementFormat( FatVertexBuffer::JOINT_ID, ColorFormat::UINT4 );

    Vector4f * fatweight = (Vector4f*)fatvb.getElementData( FatVertexBuffer::JOINT_WEIGHT );
    if( fatweight ) fatvb.setElementFormat( FatVertexBuffer::JOINT_WEIGHT, ColorFormat::FLOAT4 );

    aiMatrix4x4 normalTransform = transform;
    normalTransform.Transpose();
    normalTransform.Inverse();

    aiVector3D bbmin, bbmax;
	bbmin.x = bbmin.y = bbmin.z =  1e10f;
	bbmax.x = bbmax.y = bbmax.z = -1e10f;

    for( uint32 i = 0; i < aimesh->mNumVertices; ++i )
    {
        if( fatpos )
        {
            aiVector3D aipos = aimesh->mVertices[i];
            aiTransformVecByMatrix4( &aipos, &transform );

			bbmin.x = math::getmin<>(bbmin.x,aipos.x);
			bbmin.y = math::getmin<>(bbmin.y,aipos.y);
			bbmin.z = math::getmin<>(bbmin.z,aipos.z);

			bbmax.x = math::getmax<>(bbmax.x,aipos.x);
			bbmax.y = math::getmax<>(bbmax.y,aipos.y);
			bbmax.z = math::getmax<>(bbmax.z,aipos.z);

            fatpos->set( *(const Vector3f*)&aipos, 1.0f );

            ++fatpos;
        }

        if( fatnormal )
        {
            aiVector3D ainormal = aimesh->mNormals[i];
            aiTransformVecByMatrix4( &ainormal, &normalTransform );
            fatnormal->set( *(const Vector3f*)&ainormal, 0.0f );

            ++fatnormal;
        }

        if( fattc0 )
        {
            const aiVector3D & aitc0 = aimesh->mTextureCoords[0][i];
            // BUGBUG: why invert V?
            fattc0->set( aitc0.x, 1.0f-aitc0.y, aitc0.z, 0.0f );

            ++fattc0;
        }

        if( fatjoint )
        {
            GN_ASSERT( fatweight );

            sLoadAiVertexSkinning( *fatjoint, *fatweight, i, fatmesh.skeleton, *aimesh );

            ++fatjoint;
            ++fatweight;
        }
    }

    aiVector3D bbsize = bbmax - bbmin;
    fatmesh.bbox.set( bbmin.x, bbmin.y, bbmin.z, bbsize.x, bbsize.y, bbsize.z );

    return true;
}

//
//
// -----------------------------------------------------------------------------
static bool sLoadAiIndices( FatMesh& fatmesh, const aiMesh * aimesh )
{
    uint32 numidx = 0;
    uint32 idxppt = 0; // indices per primitive.
    switch( aimesh->mPrimitiveTypes )
    {
        case aiPrimitiveType_POINT:
            numidx = aimesh->mNumFaces;
            idxppt = 1;
            fatmesh.primitive = PrimitiveType::POINT_LIST;
            break;

        case aiPrimitiveType_LINE:
            numidx = aimesh->mNumFaces * 2;
            idxppt = 2;
            fatmesh.primitive = PrimitiveType::LINE_LIST;
            break;

        case aiPrimitiveType_TRIANGLE:
            numidx = aimesh->mNumFaces * 3;
            idxppt = 3;
            fatmesh.primitive = PrimitiveType::TRIANGLE_LIST;
            break;

        default:
            // Unsupported primitive.
            GN_ERROR(sLogger)( "Unsupported primitive: %d", aimesh->mPrimitiveTypes );
            return false;
    }

    if( !fatmesh.indices.resize( numidx ) )
    {
        GN_ERROR(sLogger)( "Out of memory." );
        return false;
    }

    for( uint32 f = 0; f < aimesh->mNumFaces; ++f )
    {
        const aiFace & aif = aimesh->mFaces[f];

        for( uint32 i = 0; i < idxppt; ++i )
        {
            fatmesh.indices[f * idxppt + i] = aif.mIndices[i];
        }
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
static void sLoadAiNodeRecursivly(
    FatModel & fatmodel,
    const aiScene * aiscene,
    const aiNode * ainode,
    const aiMatrix4x4 & parentTransform )
{
    if( NULL == ainode ) return;

    aiMatrix4x4 myTransform = parentTransform;
	aiMultiplyMatrix4(&myTransform,&ainode->mTransformation);

    for( uint32 i = 0; i < ainode->mNumMeshes; ++i )
    {
        const aiMesh * aimesh = aiscene->mMeshes[ainode->mMeshes[i]];

        // create a new fat mesh instance
        AutoObjPtr<FatMesh> fatmeshAutoPtr( new FatMesh );
        FatMesh & fatmesh = *fatmeshAutoPtr;

        // Load skeleton
        sLoadAiMeshSkeleton( fatmodel, fatmesh, *aiscene, *aimesh );

        // Load mesh vertices and indices
        if( !sLoadAiIndices( fatmesh, aimesh ) ) continue;
        if( !sLoadAiVertices( fatmesh, aiscene, aimesh, myTransform ) ) continue;

        // Setup mesh subset
        fatmesh.subsets.resize( 1 );
        FatMeshSubset & subset = fatmesh.subsets[0];
        memset( &subset, 0, sizeof(subset) );
        subset.material = aimesh->mMaterialIndex;

        // Add the mesh to fat model.
        fatmodel.meshes.append( &fatmesh );
        fatmeshAutoPtr.detach();
    }

    for( uint32 i = 0; i < ainode->mNumChildren; ++i )
    {
        sLoadAiNodeRecursivly( fatmodel, aiscene, ainode->mChildren[i], myTransform );
    }
}

//
//
// -----------------------------------------------------------------------------
static bool sLoadAiJointAnimation(
    FatModel                            & fatmodel,
    FatAnimation                        & fatanim,
    const StringMap<char,JointLocation> & jointMap,
    const aiAnimation                   & aianim )
{
    // Preallocate animation buffer.
    if( !fatanim.skeletonAnimations.resize( fatmodel.skeletons.size() ) )
    {
        GN_ERROR(sLogger)( "Out of memory." );
        return false;
    }
    for( uint32 i = 0; i < fatanim.skeletonAnimations.size(); ++i )
    {
        if( !fatanim.skeletonAnimations[i].resize( fatmodel.skeletons[i].joints.size() ) )
        {
            GN_ERROR(sLogger)( "Out of memory." );
            return false;
        }
    }

    // Certain combinations of file format and exporter don't always store
    // this information in the exported file. In this case, mTicksPerSecond
    // is set to 0 to indicate the lack of knowledge. And we'll use 1000
    // in this case, which means 1000 tick is 1 second.
    double secondsPerTick;
    if( 0 == aianim.mTicksPerSecond )
    {
        secondsPerTick = 0.001; // 1 / 1000
    }
    else
    {
        secondsPerTick = 1.0 / aianim.mTicksPerSecond;
    }

    // Declara a new Fat animation object.
    fatanim.name = aianim.mName.data;
    fatanim.duration = aianim.mDuration * secondsPerTick;

    // Go through each animation channels. Each channel controls
    // one node in the scene.
    for( uint32 c = 0; c < aianim.mNumChannels; ++c )
    {
        const aiNodeAnim & aina = *aianim.mChannels[c];

        // See if there's a joint affected by the current node animation.
        uint32 skeletonIndex, jointIndex;
        if( !sSearchForNamedJoint(
            skeletonIndex,
            jointIndex,
            jointMap,
            aina.mNodeName.data ) )
        {
            // The animation channel does not link to a joint node.
            // Ignore it, since we care about skeleton animation only.
            continue;
        }
        GN_ASSERT( skeletonIndex < fatmodel.skeletons.size() );
        GN_ASSERT( jointIndex < fatmodel.skeletons[skeletonIndex].joints.size() );

        // Reference the fat joint animation object that was preallocated at the beginning
        // of the function.
        FatJointAnimation & fatJointAnim = fatanim.skeletonAnimations[skeletonIndex][jointIndex];

        // Allocate fat key frames
        if( !fatJointAnim.positions.resize(aina.mNumPositionKeys) ||
            !fatJointAnim.rotations.resize(aina.mNumRotationKeys) ||
            !fatJointAnim.scalings.resize(aina.mNumScalingKeys) )
        {
            // We are running out of memory. Stop loading.
            GN_ERROR(sLogger)( "Out of memory." );
            return false;
        }

        // Load position key frames
        for( uint32 k = 0; k < aina.mNumPositionKeys; ++k )
        {
            const aiVectorKey & aikey = aina.mPositionKeys[k];
            FatKeyFrame<Vector3f> & fatkey = fatJointAnim.positions[k];
            fatkey.time = (float)(aikey.mTime * secondsPerTick);
            fatkey.value = *(Vector3f*)&aikey.mValue;
        }

        // Load rotation key frames
        for( uint32 k = 0; k < aina.mNumRotationKeys; ++k )
        {
            const aiQuatKey & aikey = aina.mRotationKeys[k];
            FatKeyFrame<Quaternionf> & fatkey = fatJointAnim.rotations[k];
            fatkey.time = (float)(aikey.mTime * secondsPerTick);
            fatkey.value.v.x = aikey.mValue.x;
            fatkey.value.v.y = aikey.mValue.y;
            fatkey.value.v.z = aikey.mValue.z;
            fatkey.value.w   = aikey.mValue.w;
        }

        // Load scaling key frames
        for( uint32 k = 0; k < aina.mNumScalingKeys; ++k )
        {
            const aiVectorKey & aikey = aina.mScalingKeys[k];
            FatKeyFrame<Vector3f> & fatkey = fatJointAnim.scalings[k];
            fatkey.time = (float)(aikey.mTime * secondsPerTick);
            fatkey.value = *(Vector3f*)&aikey.mValue;
        }
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
static void sLoadAiAnimations( FatModel & fatmodel, const aiScene & aiscene )
{
    // Build a joint map to accelerate joint searching by name.
    StringMap<char,JointLocation> jointMap;
    sBuildJointMapFromSkeletons( jointMap, fatmodel.skeletons );

    // Preallocate animation array
    if( !fatmodel.animations.reserve( aiscene.mNumAnimations ) )
    {
        GN_ERROR(sLogger)( "Fail to preallocate fat animation array: out of memory." );
        return;
    }

    // Go through the animation list. Load them one by one. And for now,
    // we load node/joint animations only. Mesh animations are not supported yet.
    for( uint32 a = 0; a < aiscene.mNumAnimations; ++a )
    {
        // Reference the Assimp animation object
        const aiAnimation & aianim = *aiscene.mAnimations[a];

        // Create a new Fat animation object.
        fatmodel.animations.resize( fatmodel.animations.size() + 1 );
        FatAnimation & fatanim = fatmodel.animations.back();

        if( !sLoadAiJointAnimation( fatmodel, fatanim, jointMap, aianim ) )
        {
            // Animation loading failed. Destroy the fat animation object
            // that was just created.
            fatmodel.animations.popBack();
        }
    } // end of for each animation
}

//
//
// -----------------------------------------------------------------------------
static bool sLoadFromAssimp( FatModel & fatmodel, const StrA & filename )
{
    const aiScene * scene = aiImportFile( filename, aiProcessPreset_TargetRealtime_Quality );
    if( NULL == scene ) return false;

    // Load materials
    StrA dirname = fs::dirName( filename );
    fatmodel.materials.resize( scene->mNumMaterials );
    for( uint32 i = 0; i < fatmodel.materials.size(); ++i )
    {
        const aiMaterial * aimat = scene->mMaterials[i];

        FatMaterial & fatmat = fatmodel.materials[i];

        fatmat.clear();

        aiString path;

        if( aimat->GetTextureCount(aiTextureType_DIFFUSE) > 0 &&
            aiReturn_SUCCESS == aimat->GetTexture( aiTextureType_DIFFUSE, 0, &path ) )
        {
            fatmat.albedoTexture = fs::resolvePath( dirname, path.data );
        }
    }

    // Load meshes recursively
    aiMatrix4x4 rootTransform;
    aiIdentityMatrix4(&rootTransform);
    sLoadAiNodeRecursivly( fatmodel, scene, scene->mRootNode, rootTransform );

    // Load animations.
    sLoadAiAnimations( fatmodel, *scene );

    // calculate the final bounding box
    fatmodel.calcBoundingBox();

    aiReleaseImport( scene );
    return true;
}

static bool
sPrintAiNodeHierarchy( StrA & hierarchy, const StrA & filename )
{
    const aiScene * scene = aiImportFile( filename, aiProcessPreset_TargetRealtime_Quality );
    if( NULL == scene ) return false;

    struct Local
    {
        static void sPrintRecursivly( StrA & hierarchy, const aiNode * node, int depth )
        {
            if( NULL == node ) return;

            for( int i = 0; i < depth; ++i )
            {
                hierarchy += "  ";
            }

            hierarchy += stringFormat( "(%d) ", depth );

            const char * name = node->mName.data;

            hierarchy += stringEmpty(name) ? "[UNNAMED]" : name;

            hierarchy += "\n";

            for( uint32 i = 0; i < node->mNumChildren; ++i )
            {
                sPrintRecursivly( hierarchy, node->mChildren[i], depth + 1 );
            }
        }
    };

    Local::sPrintRecursivly( hierarchy, scene->mRootNode, 0 );

    if( hierarchy.empty() ) hierarchy = "[EMPTY FILE]";

    aiReleaseImport( scene );
    return true;
}

};

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

    bool noerr = true;

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

    // by default the fat model name would be the file.
    this->name = fullFileName;

    switch( ff )
    {
        case FF_ASE:
            noerr = ase::sLoadFromASE( *this, *file, fullFileName );
            break;

        case FF_FBX:
            noerr = fbx::sLoadFromFBX( *this, *file, fullFileName );
            break;

        case FF_GARNET_XML:
            noerr = false;
            break;

        case FF_GARNET_BIN:
            noerr = false;
            break;

        default:
            if( !ai::sLoadFromAssimp( *this, fullFileName ) )
            {
                GN_ERROR(sLogger)( "Unknown file format: %s", filename.rawptr() );
                noerr = false;
            }
            break;
    }

    if( noerr )
    {
        size_t totalVerts = 0;
        size_t totalFaces = 0;
        for( uint32 i = 0; i < this->meshes.size(); ++i )
        {
            const FatMesh * m = this->meshes[i];
            if( m )
            {
                totalVerts += m->vertices.getVertexCount();
                totalFaces += m->indices.size() / 3;
            }
        }
        GN_INFO(sLogger)( "Total vertices: %d, faces: %d", totalVerts, totalFaces );
    }
    else
    {
        clear();
    }

    return noerr;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::printModelFileNodeHierarchy( StrA & hierarchy, const StrA & filename )
{
    StrA fullFileName = fs::resolvePath( fs::getCurrentDir(), filename );

    FileFormat ff = sDetermineFileFormatByFileName( fullFileName );
    if( FF_FBX == ff )
    {
        fbx::sPrintFBXNodeHierarchy( hierarchy, fullFileName );
    }
    else if( !ai::sPrintAiNodeHierarchy( hierarchy, fullFileName ) )
    {
        hierarchy = "ERROR: unsupported or unknown file format.";
    }

}
