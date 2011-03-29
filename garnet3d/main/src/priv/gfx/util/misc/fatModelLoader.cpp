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
    fatmodel.materials.resize( ase.materials.size() );
    for( size_t i = 0; i < ase.materials.size(); ++i )
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
    fatmodel.meshes.resize( ase.meshes.size() );
    for( size_t i = 0; i < ase.meshes.size(); ++i )
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
            memcpy( dst->indices.cptr(), src.indices, src.numidx * 4 );
        }
        else
        {
            const uint16 * s = (const uint16*)src.indices;
            uint32 * d = dst->indices.cptr();
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
    HashMapUtils::HashFunc_MemoryHash<MeshVertexKey>,
    HashMapUtils::EqualFunc_MemoryCompare<MeshVertexKey> > MeshVertexHashMap;

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

    // Get the global transform of the node, which is the transformation
    // from local/joint space to model space.
    sFbxMatrix2GarnetMatrix( newjoint.bindPose, fbxnode->GetScene()->GetEvaluator()->GetNodeGlobalTransform( fbxnode ) );

    // Inverse that to get the bind pose transfomration we want.
    newjoint.bindPose.inverse();

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

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef INOUT
#define INOUT
#endif

//
// Search through all skeletons for joint with specific name.
// -----------------------------------------------------------------------------
static bool
sSearchForNamedJoint(
    OUT uint32         & skeleton,
    OUT uint32         & joint,
    IN  const FatModel & fatmodel,
    IN  const char     * jointName )
{
    for( uint32 i = 0; i < fatmodel.skeletons.size(); ++i )
    {
        // Reference the skeleton
        const FatSkeleton & sk = fatmodel.skeletons[i];

        for( uint32 j = 0; j < sk.joints.size(); ++j )
        {
            if( sk.joints[j].name == jointName )
            {
                skeleton = i;
                joint = j;
                return true;
            }
        }
    }

    return false;
}

//
// Get vertex skinning information for the vertex specified by controlPointIndex.
// -----------------------------------------------------------------------------
static void
sLoadVertexSkinning(
    INOUT uint32         & skeleton, // Index into FatModel::skeleton arrayreturns the skeleton that of the joint
    OUT   Skinning       & sk,
    IN    const FatModel & fatmodel,
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
    for( int i = 0; i < GN_ARRAY_COUNT(sk.joints); ++i )
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
                fatmodel,
                link->GetName() ) )
            {
                // The cluster links to an node that is not part of any skeletons.
                // This should not happen for a valid FBX file. But any way, the cluster
                // has to be ignored, with warning.
                GN_ERROR(sLogger)( "Cluter is ignored, because it is linking to a node that is not part of any skeleton." );
                continue;
            }
            GN_ASSERT( currentSkeleton < fatmodel.skeletons.size() );
            GN_ASSERT( currentJoint < fatmodel.skeletons[currentSkeleton].joints.size() );

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

                // Add it to the skinning structure, and keep the joints
                // in the desending order of weights. We support at most
                // 4 joints per vertex. When too many joints are affecting
                // a single vertex, joints with less influence will be
                // dropped.

                int maxJoints = (int)GN_ARRAY_COUNT(sk.joints);
                for( int iJoint = maxJoints - 1; iJoint >= 0; --iJoint )
                {
                    if( FatJoint::NO_JOINT == sk.joints[iJoint] ||
                        sk.weights[iJoint] < currentWeight )
                    {
                        // The new/current weight is larger than the existing weight in slot #iJoint,
                        // or there's no joint in slot #iJoint. We need to move the existing joint
                        // to the next slot to make space for the new joint, or drop the existing
                        // joint, if the joint array is full already.

                        if( (iJoint+1) < maxJoints )
                        {
                            sk.joints[iJoint+1]  = sk.joints[iJoint];
                            sk.weights[iJoint+1] = sk.weights[iJoint];

                            if( 0 == iJoint )
                            {
                                // The current weight is larger than any existing weights the
                                // the array. So just store current joint and weight in slot 0.
                                sk.joints[0]  = currentJoint;
                                sk.weights[0] = currentWeight;
                            }
                        }
                        else if( FatJoint::NO_JOINT != sk.joints[iJoint] )
                        {
                            // The joint array is full. And the last existing weight in the array
                            // is smaller then the new/current weight. So the existing one will be
                            // dropped, with warning.
                            GN_ERROR(sLogger)(
                                "Vertex %d has more than 4 joints attatched. "
                                "Joint %d (weight=%f) in skeleton %d is going to be ignore, "
                                "because it has less influence to the vertex "
                                "then other joints.",
                                controlPointIndex,
                                sk.joints[iJoint], sk.weights[iJoint],
                                skeleton );
                        }
                    }
                    else
                    {
                        // The new weight is smaller then the weight in slot #iJoint. So we need
                        // to store the new weight in slot (iJoint+1), or ignore the new weight
                        // if the joint array is full already.

                        if( (iJoint+1) < maxJoints )
                        {
                            // Store new joint at slot (iJoint+1)
                            sk.joints[iJoint+1] = currentJoint;
                            sk.weights[iJoint+1] = currentWeight;
                        }
                        else
                        {
                            // Drop the new joint, since the joint array is full already.
                            GN_ERROR(sLogger)(
                                "Vertex %d has more than 4 joints attatched. "
                                "Joint %d (weight=%f) in skeleton %d is going to be ignore, "
                                "because it has less influence to the vertex "
                                "then other joints.",
                                controlPointIndex,
                                currentJoint, currentWeight,
                                skeleton );
                        }

                        // We've done with the new joint. It has been either stored in the array,
                        // or dropped already.
                        break;
                    }
                }

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
    for( int i = 0; i < GN_ARRAY_COUNT(sk.joints); ++i )
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
        for( int i = 0; i < GN_ARRAY_COUNT(sk.joints); ++i )
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
    if( !fatvb.resize( FatVertexBuffer::POS_NORMAL_TEX_SKINNING, numkeys ) ) return false;

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

    // Declare the vertex hash table
    MeshVertexHashMap vhash( (size_t)numidx * 2 );

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
                sLoadVertexSkinning( fatmesh.skeleton, sk, fatmodel, fbxmesh, posIndex, firstVertex );
            }
            else if( fatmesh.skeleton != FatMesh::NO_SKELETON )
            {
                // Load skinning information for the following vertices, only when the mesh is binding
                // to a skeleton.
                sLoadVertexSkinning( fatmesh.skeleton, sk, fatmodel, fbxmesh, posIndex, firstVertex );
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
            MeshVertexHashMap::KeyValuePair * pair;
            bool isNewVertex = vhash.insert( key, (uint32)vhash.size(), &pair );
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
    if( !sGenerateFatVertices( fatmesh, fbxnode, vcache.cptr(), vertexKeys.cptr(), vertexKeys.size() ) ) return;

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

    // Load meshes
    sLoadFbxMeshes( fatmodel, filename, sdk, *gScene );

    // calculate the final bounding box
    fatmodel.calcBoundingBox();

    // done!
    return true;

#else // HAS_FBX

    fatmodel.clear();
    GN_UNUSED_PARAM( file );
    GN_ERROR(sLogger)( "Fail to load file %s: FBX is not supported.", filename.cptr() );
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
                else if( 0 <= atype && atype < GN_ARRAY_COUNT(sAttributeTypeNames) )
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
static uint32 sDetermineFatVertexLayout( const aiMesh * aimesh )
{
    uint32 layout = 1<<FatVertexBuffer::POSITION;

    if( aimesh->mNormals ) layout |= 1<<FatVertexBuffer::NORMAL;

    uint32 maxtc = math::getmin<uint32>( GN_ARRAY_COUNT(aimesh->mTextureCoords), FatVertexBuffer::MAX_TEXCOORDS );
    for( uint32 t = 0; t < maxtc; ++t )
    {
        if( aimesh->mTextureCoords[t] ) layout |= 1<<(FatVertexBuffer::TEXCOORD0 + t);
    }

    return layout;
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

        AutoObjPtr<FatMesh> fatmeshAutoPtr( new FatMesh );
        FatMesh & fatmesh = *fatmeshAutoPtr;

        if( !sLoadAiIndices( fatmesh, aimesh ) ) continue;
        if( !sLoadAiVertices( fatmesh, aiscene, aimesh, myTransform ) ) continue;

        fatmesh.subsets.resize( 1 );
        FatMeshSubset & subset = fatmesh.subsets[0];
        memset( &subset, 0, sizeof(subset) );
        subset.material = aimesh->mMaterialIndex;

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
static bool sLoadFromAssimp( FatModel & fatmodel, const StrA & filename )
{
    const aiScene * scene = aiImportFile( filename, aiProcessPreset_TargetRealtime_Quality );
    if( NULL == scene ) return false;

    // Load AI materials
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

    // Load AI nodes
	aiMatrix4x4 rootTransform;
	aiIdentityMatrix4(&rootTransform);
    sLoadAiNodeRecursivly( fatmodel, scene, scene->mRootNode, rootTransform );

    // calculate the final bounding box
    fatmodel.calcBoundingBox();

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
                GN_ERROR(sLogger)( "Unknown file format: %s", filename.cptr() );
                noerr = false;
            }
            break;
    }

    if( noerr )
    {
        size_t totalVerts = 0;
        size_t totalFaces = 0;
        for( size_t i = 0; i < this->meshes.size(); ++i )
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
    if( FF_FBX != ff )
    {
        hierarchy = "ERROR: FBX file only.";
        return;
    }

    fbx::sPrintFBXNodeHierarchy( hierarchy, fullFileName );
}
