#include "pch.h"
#include "garnet/gfx/fatModel.h"

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.FatModel");

// *****************************************************************************
// FatVertexBuffer
// *****************************************************************************

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
    for( int i = 0; i < NUM_SEMANTICS; ++i )
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
    for( int i = 0; i < NUM_SEMANTICS; ++i )
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
        for( int i = 0; i < NUM_SEMANTICS; ++i )
        {
            safeHeapDealloc( vertices[i] );
        }
        GN_ERROR(sLogger)( "Fail to resize fat vertex buffer: out of memory." );
        return false;
    }

    // update data pointer and format
    for( int i = 0; i < NUM_SEMANTICS; ++i )
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
    for( int i = 0; i < NUM_SEMANTICS; ++i )
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
            GN_WARN(sLogger)( "unsupport semantic: %s", mvf.elements[i].semantic );
        }
    }

    // Copy vertex data
    for( size_t j = 0; j < mvf.numElements; ++j )
    {
        if( semantics[j] == INVALID ) continue;

        const MeshVertexElement & e = mvf.elements[j];
        SafeArrayAccessor<const uint8> src( (const uint8*)mElements[semantics[j]], mCount * ELEMENT_SIZE );
        SafeArrayAccessor<uint8>       dst( (uint8*)buffer + e.offset, (mCount * stride) - e.offset );
        size_t size = e.format.getBytesPerBlock();

        GN_ASSERT( ( e.offset + size ) <= stride );

        for( size_t i = 0; i < mCount; ++i )
        {
            memcpy( dst.subrange(0,size), src.subrange(0,size), size );
            src += ELEMENT_SIZE;
            dst += stride;
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
        s += stringFormat( "(%d) %s\n", depth, joints[root].name );

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
        return sPrintFatJointRecursivly( s, joints.cptr(), joints.size(), 0, 0 );
    }
}

// *****************************************************************************
// FatModel
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::FatModel::calcBoundingBox()
{
    bbox.clear();
    for( size_t i = 0; i < meshes.size(); ++i )
    {
        GN_ASSERT( meshes[i] );
        Boxf::sGetUnion( bbox, bbox, meshes[i]->bbox );
    }
}
