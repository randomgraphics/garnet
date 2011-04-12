#include "pch.h"
#include "garnet/gfx/fatModel.h"
#include <set>

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.FatModel");

// *****************************************************************************
// FatVertexBuffer
// *****************************************************************************

const uint32 GN::gfx::FatVertexBuffer::MAX_TEXCOORDS = (uint32)(GN::gfx::FatVertexBuffer::TEXCOORD_LAST - GN::gfx::FatVertexBuffer::TEXCOORD0);
const uint32 GN::gfx::FatVertexBuffer::POS_NORMAL_TEX = (1<<GN::gfx::FatVertexBuffer::POSITION) | (1<<GN::gfx::FatVertexBuffer::NORMAL) | (1<<GN::gfx::FatVertexBuffer::TEXCOORD0);
const uint32 GN::gfx::FatVertexBuffer::POS_NORMAL_TEX_SKINNING = (1<<GN::gfx::FatVertexBuffer::POSITION) | (1<<GN::gfx::FatVertexBuffer::NORMAL) | (1<<GN::gfx::FatVertexBuffer::TEXCOORD0) | (1<<GN::gfx::FatVertexBuffer::JOINT_ID) | (1<<GN::gfx::FatVertexBuffer::JOINT_WEIGHT);

//
//
// -----------------------------------------------------------------------------
const char * const GN::gfx::FatVertexBuffer::SEMANTIC_NAMES[] =
{
    "POSITION",
    "NORMAL",
    "TANGENT",
    "BINORMAL",
    "TEXCOORD0",
    "TEXCOORD1",
    "TEXCOORD2",
    "TEXCOORD3",
    "TEXCOORD4",
    "TEXCOORD5",
    "TEXCOORD6",
    "TEXCOORD7",
    "ALBEDO",
    "JOINT_ID",
    "JOINT_WEIGHT",
    "CUSTOM0",
    "CUSTOM1",
    "CUSTOM2",
    "CUSTOM3",
    "CUSTOM4",
    "CUSTOM5",
    "CUSTOM6",
    "CUSTOM7",
};
GN_CASSERT( GN_ARRAY_COUNT(GN::gfx::FatVertexBuffer::SEMANTIC_NAMES) == GN::gfx::FatVertexBuffer::NUM_SEMANTICS );

//
//
// -----------------------------------------------------------------------------
GN::gfx::FatVertexBuffer::Semantic GN::gfx::FatVertexBuffer::sString2Semantic( const char * s )
{
    for( int i = 0; i < (int)NUM_SEMANTICS; ++i )
    {
        if( 0 == stringCompare( s, SEMANTIC_NAMES[i] ) )
        {
            return (Semantic)i;
        }
    }
    return INVALID;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::FatVertexBuffer::resize( uint32 layout, uint32 count )
{
    if( 0 == layout || 0 == count )
    {
        clear();
        return true;
    }

    // allocate memory
    void * vertices[NUM_SEMANTICS];
    memset( vertices, 0, sizeof(vertices) );
    bool outofmem = false;
    for( int i = 0; i < (int)NUM_SEMANTICS; ++i )
    {
        if( (1<<i) & layout )
        {
            vertices[i] = HeapMemory::alignedAlloc( count * 128, ELEMENT_SIZE );
            if( NULL == vertices[i] )
            {
                outofmem = true;
                break;
            }
        }
    }
    if( outofmem )
    {
        for( int i = 0; i < (int)NUM_SEMANTICS; ++i )
        {
            safeHeapDealloc( vertices[i] );
        }
        GN_ERROR(sLogger)( "Fail to resize fat vertex buffer: out of memory." );
        return false;
    }

    // update data pointer and format
    for( int i = 0; i < (int)NUM_SEMANTICS; ++i )
    {
        if( (1<<i) & layout )
        {
            GN_ASSERT( vertices[i] );
            memcpy( vertices[i], mElements[i], math::getmin<>(count,mCount) );
            safeHeapDealloc( mElements[i] );
            mElements[i] = vertices[i];
        }
        else
        {
            safeHeapDealloc( mElements[i] );
        }

        // clear both new and unused formats
        if( (0 == ((1<<i) & layout)) || (0 == ((1<<i) & mLayout)) )
        {
            mFormats[i] = ColorFormat::UNKNOWN;
        }
    }

    mLayout = layout;
    mCount = count;

    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::FatVertexBuffer::GenerateMeshVertexFormat( MeshVertexFormat & mvf ) const
{
    mvf.clear();

    uint8 offset = 0;
    for( int i = 0; i < (int)NUM_SEMANTICS; ++i )
    {
        if( (1<<i) & mLayout )
        {
            MeshVertexElement & e = mvf.elements[mvf.numElements];

            e.format = mFormats[i];
            e.stream = 0;
            e.offset = offset;
            e.setSemantic( SEMANTIC_NAMES[i] );

            offset += e.format.getBytesPerBlock();
            ++mvf.numElements;
        }
    }
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::FatVertexBuffer::GenerateVertexStream(
    const MeshVertexFormat & mvf,
    size_t                   stream,
    size_t                   stride,
    void                   * buffer,
    size_t                   bufferSize ) const
{
    // Check parameters
    uint32 numStreams = mvf.calcNumStreams();
    if( stream >= numStreams )
    {
        GN_ERROR(sLogger)( "Invalid stream." );
        return false;
    }

    uint32 minStride = mvf.calcStreamStride( (uint32)stream );
    if( 0 == stride )
    {
        stride = minStride;
    }
    else if( stride < minStride )
    {
        GN_ERROR(sLogger)( "Stride is too small." );
        return false;
    }

    if( NULL == buffer )
    {
        GN_ERROR(sLogger)( "Null buffer pointer." );
        return false;
    }

    size_t minBufferSize = stride * mCount;
    if( bufferSize < minBufferSize )
    {
        GN_ERROR(sLogger)( "Buffer size is too small." );
        return false;
    }

    Semantic semantics[MeshVertexFormat::MAX_VERTEX_ELEMENTS];
    for( size_t i = 0; i < mvf.numElements; ++i )
    {
        const char * s = mvf.elements[i].semantic;
        if( stringEmpty(s) )
        {
            s = "[EMPTY]";
            semantics[i] = INVALID;
        }
        else
        {
            semantics[i] = sString2Semantic(s);
        }
        if( INVALID == semantics[i] )
        {
            GN_WARN(sLogger)( "unsupport semantic: %s", s );
        }
    }

    // Copy vertex data
    for( size_t j = 0; j < mvf.numElements; ++j )
    {
        const MeshVertexElement & e = mvf.elements[j];

        size_t size = e.format.getBytesPerBlock();
        if( 0 == size ) continue;

        GN_ASSERT( ( e.offset + size ) <= stride );

        SafeArrayAccessor<uint8> dst( (uint8*)buffer + e.offset, (mCount * stride) - e.offset );

        if( semantics[j] != INVALID )
        {
            SafeArrayAccessor<const uint8> src( (const uint8*)mElements[semantics[j]], mCount * ELEMENT_SIZE );

            for( size_t i = 0; i < mCount; ++i )
            {
                memcpy( dst.subrange(0,size), src.subrange(0,size), size );
                src += ELEMENT_SIZE;
                dst += stride;
            }
        }
        else
        {
            for( size_t i = 0; i < mCount; ++i )
            {
                memset( dst.subrange(0,size), 0, size );
                dst += stride;
            }
        }
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::FatVertexBuffer::copyFrom( const FatVertexBuffer & other )
{
    GN_UNUSED_PARAM( other );
    GN_UNIMPL();

    /*clear();

    if( !resize( other.getLayout(), other.getVertexCount() ) ) return false;

    for( int i = 0; i < NUM_SEMANTICS; ++i )
    {
        if( other.mElements[i] )
        {
            memcpy( mElements[i], other.mElements[i], mCount * ELEMENT_SIZE );
        }

        mFormats[i] = other.mFormats[i];
    }*/

    return true;
}

// *****************************************************************************
// FatSkeleton
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static void sPrintFatJointRecursivly( StrA & s, const FatJoint * joints, uint32 count, uint32 root, uint32 depth )
{
    for( uint32 i = 0; i < depth; ++i )
    {
        s += "  ";
    }

    if( root < count )
    {
        s += stringFormat( "(%d) %s\n", depth, joints[root].name.rawptr() );

        for( uint32 i = joints[root].child; i != FatJoint::NO_JOINT; i = joints[i].sibling )
        {
            sPrintFatJointRecursivly( s, joints, count, i, depth + 1 );
        }
    }
    else
    {
        s += stringFormat( "(%d) ERROR: joint index out of range: %d\n", depth, root );
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::FatSkeleton::printJointHierarchy( StrA & s ) const
{
    if( joints.empty() )
    {
        s = "[Empty skeleton]";
    }
    else
    {
        return sPrintFatJointRecursivly( s, joints.rawptr(), joints.size(), rootJointIndex, 0 );
    }
}

// *****************************************************************************
// FatModel
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static void sJointSet2JointArray( DynaArray<uint32,uint32> & jarray, const std::set<uint32> & jset )
{
    jarray.clear();
    for( std::set<uint32>::const_iterator i = jset.begin(); i != jset.end(); ++i )
    {
        jarray.append( *i );
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::FatModel::calcBoundingBox()
{
    bbox.clear();
    for( uint32 i = 0; i < meshes.size(); ++i )
    {
        GN_ASSERT( meshes[i] );
        Boxf::sGetUnion( bbox, bbox, meshes[i]->bbox );
    }
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::FatModel::splitSkinnedMesh( uint32 maxJointsPerSubset )
{
    if( maxJointsPerSubset < 12 )
    {
        GN_ERROR(sLogger)( "The minimal MAX_JOINTS_PER_SUBSET is 12." );
        maxJointsPerSubset = 12;
    }

    //
    // Loop through all meshes
    //
    for( uint32 i = 0; i < meshes.size(); ++i )
    {
        FatMesh & mesh = *meshes[i];

        // ignore mesh without skeleton
        if( FatMesh::NO_SKELETON == mesh.skeleton ) continue;

        // ignore mesh without skin information in vertex.
        const uint32 * joints  = (uint32*)mesh.vertices.getElementData( FatVertexBuffer::JOINT_ID );
        float  * weights = (float*)mesh.vertices.getElementData( FatVertexBuffer::JOINT_WEIGHT );
        if( NULL == joints || NULL == weights ) continue;

        // cache mesh index buffer (might be NULL)
        uint32 * indices = mesh.indices.rawptr();

        // Temporary array to hold newly created subset.
        DynaArray<FatMeshSubset> newSubsets;

        //
        // Loop through all subsets in the mesh. See if any of them need to be
        // split into smaller pieces to meet the joint number threshold. All
        // newly created subsets are stored in array "newSubsets".
        //
        for( uint32 i = 0; i < mesh.subsets.size(); ++i )
        {
            // This is the subset that we're currently looping through
            FatMeshSubset & subset = mesh.subsets[i];

            // This is to see if we have created a new subset.
            FatMeshSubset * newsub = NULL;

            // Loop through each triangle in the subset. Accumulate joints on the way.
            // Whenever number of joints exceeds the threshold, creates a new subset
            // and reset the joint count.

            std::set<uint32> accumulatedJoints;

            uint32 faceCount = (indices ? subset.numidx : subset.numvtx) / 3;

            for( uint32 i = 0; i < faceCount; ++i )
            {
                // Collect joint of this face (triangle)
                std::set<uint32> newJoints;
                for( uint32 j = 0; j < 3; ++j )
                {
                    uint32 vertexIndex = subset.basevtx + ( indices ? indices[subset.startidx+i*3+j] : (i*3+j) );
                    const uint32 * vertexJoints = joints + vertexIndex;
                    if( FatJoint::NO_JOINT != vertexJoints[0] ) newJoints.insert( vertexJoints[0] );
                    if( FatJoint::NO_JOINT != vertexJoints[1] ) newJoints.insert( vertexJoints[1] );
                    if( FatJoint::NO_JOINT != vertexJoints[2] ) newJoints.insert( vertexJoints[2] );
                    if( FatJoint::NO_JOINT != vertexJoints[3] ) newJoints.insert( vertexJoints[3] );
                }

                // Merge current joints and new joints into a temporary set.
                std::set<uint32> tempJointSet( accumulatedJoints );
                tempJointSet.insert( newJoints.begin(), newJoints.end() );

                // See if the size of the merged joint set exceeds the limit.
                if( tempJointSet.size() > maxJointsPerSubset )
                {
                    //
                    // There's no eough space to hold the new face in current subset. We have to
                    // create a new subset and put the newJoints into it. And if it is the first
                    // time we split, we also need to remember the current face index, which we
                    // will use later to adjust the original subset.
                    //

                    if( NULL == newsub )
                    {
                        // This is the first time we split. We need to adjust the original subset
                        // to remove all faces that will been moved to new subsets. We have
                        // remembered the orignal face count in variable "faceCount". So this should
                        // not affect the looping.
                        if( indices )
                        {
                            GN_ASSERT( ( i * 3 ) > subset.startidx );
                            subset.numidx = i * 3 - subset.startidx;
                        }
                        else
                        {
                            GN_ASSERT( ( i * 3 ) > subset.basevtx );
                            subset.numvtx = i * 3 - subset.basevtx;
                        }

                        // The accumualted joint set should contain joints and only joints that are
                        // used by the subset
                        sJointSet2JointArray( subset.joints, accumulatedJoints );
                    }
                    else
                    {
                        // We have split the subset before. It's time to finalize the previous split
                        // subset by filling in the number of vertices or indices in the subset.
                        if( indices )
                        {
                            newsub->numidx = i * 3 - newsub->startidx;
                        }
                        else
                        {
                            newsub->numvtx = i * 3 - newsub->basevtx;
                        }

                        // copy accumulated joints to the new subset.
                        sJointSet2JointArray( newsub->joints, accumulatedJoints );
                    }

                    // Now, create a new subset.
                    newSubsets.resize( newSubsets.size() + 1 );
                    newsub = &newSubsets.back();

                    // Initialize the new subset. Note that we don't know the final vertex and
                    // index count yet.
                    newsub->material = subset.material;
                    if( indices )
                    {
                        newsub->basevtx = subset.basevtx;
                        newsub->numvtx = subset.numvtx;
                        newsub->startidx = i * 3;
                        newsub->numidx = 0xbadbeef; // don't know this yet.
                    }
                    else
                    {
                        newsub->basevtx = i * 3;
                        newsub->numvtx = 0xbadbeef; // don't know this yet.
                        newsub->startidx = 0;
                        newsub->numidx = 0;
                    }

                    // We just split a new subset. So we need to reset the joint set to start
                    // a new round of accumulation towards the joint count threshold.
                    accumulatedJoints = newJoints;
                }
                else
                {
                    // Not yet. Let's add the new joints into accumulated joint set then continue.
                    accumulatedJoints = tempJointSet;
                }
            }

            // If we have split the subset at least once, ...
            if( NULL != newsub )
            {
                // So we need to finalize the last new subset, in the same way we finalize other
                // subsets.
                if( indices )
                {
                    newsub->numidx = faceCount * 3 - newsub->startidx;
                }
                else
                {
                    newsub->numvtx = faceCount * 3 - newsub->basevtx;
                }

                // copy accumulated joints to the new subset.
                sJointSet2JointArray( newsub->joints, accumulatedJoints );
            }
        }
        // end loo: for each subset

        // We have gone through all subsets in the mesh and make sure every single of them meets
        // the joint threshold (anyone that doesn't has been split). Now is time to add all newly
        // created subsets back to the mesh.
        mesh.subsets.append( newSubsets );

        // TODOs:
        // 1. remap vertex joint ID, split vertex if needed.
        // 2. sort subsets to minimize joint matrix upload.
    }
    // end loop: for each mesh

    // We have gone through all meshes. Job is well done.
    return true;
}
