#include "pch.h"
#include "garnet/gfx/fatMesh.h"

// ***************************************************************************************
// public functions
// ***************************************************************************************

//
//
// ---------------------------------------------------------------------------------------
void GN::gfx::FatMesh::optimize( const OptimizeOptions & oo )
{
    GN_GUARD;

    // build vertex segments, to make sure index wouldn't be too large
    if( mVertices.size() > 0xFFFF )
    {
        GN_WARN( "do not support mesh with more than 0xFFFF vertices" );
        return;
    }
    DynaArray<DynaArray<size_t> > faces; // indices of faces that belongs to each segments.
    mVtxSegments.resize( 1 );
    mVtxSegments[0].start = 0;
    mVtxSegments[0].count = mVertices.size();
    faces.resize( 1 );
    faces[0].resize( mFaces.size() );
    for( size_t i = 0; i < mFaces.size(); ++i ) faces[0][i] = i;

    // for each vertex segment
    for( size_t vtxSegIdx = 0; vtxSegIdx < mVtxSegments.size(); ++vtxSegIdx )
    {
        // sort by material, generate face segments as well.
        sortByMaterial( vtxSegIdx, faces[vtxSegIdx] );
    }

    // for each face segment
    for( size_t faceSegIdx = 0; faceSegIdx < mFaceSegments.size(); ++faceSegIdx )
    {
        // TODO: do strip and vcache optimization here (try nvtristrip)

        // move indices to 16bit index buffer, if required.
        if( !oo.use32BitIndex )
        {
            FaceSegment & fs = mFaceSegments[faceSegIdx];
            fs.indices16.resize( fs.indices32.size() );
            for( size_t i = 0; i < fs.indices32.size(); ++i )
            {
                GN_ASSERT( fs.indices32[i] <= 0xFFFF );
                fs.indices16[i] = (uint16_t)fs.indices32[i];
            }
            fs.indices32.clear();
        }
    }

    GN_UNGUARD;
}

//
//
// ---------------------------------------------------------------------------------------
void GN::gfx::FatMesh::draw( int material )
{
    GN_GUARD_SLOW;

    if( mFaceSegments.empty() )
    {
        OptimizeOptions oo;
        oo.maxPrimitivesInSingleDraw = 0xFFFF;
        oo.vcache = false;
        oo.strip = true;
        oo.useResetIndex = false;
        oo.use32BitIndex = false;
        optimize( oo );
    }

    for( size_t idx = 0; idx < mFaceSegments.size(); ++idx )
    {
        if( mFaceSegments[idx].material != material ) continue;
        drawFaceSegment( idx );
        break;
    }

    GN_UNGUARD_SLOW;
}

#pragma pack(push,1)
struct FatMeshDesc
{
    uint8_t  hasFaceNormal; // as is
    uint32_t numVtx; // vertex count
    uint32_t numFace; // face count
};
#pragma pack(pop)
//
//
// ---------------------------------------------------------------------------------------
bool GN::gfx::FatMesh::readFrom( File & fp )
{
    GN_GUARD;

    // read file header
    char header[8];
    if( !fp.read( header, sizeof(header) ) )
    {
        GN_ERROR( "Fail to read file header." );
        return false;
    }

    // check header
    static char BIN_TAG[8] = {'F','A','T','M','E','S','H','B'};
    static char TXT_TAG[8] = {'F','A','T','M','E','S','H','T'};
    if( 0 != memcmp( BIN_TAG, header, 8 ) &&
        0 != memcmp( TXT_TAG, header, 8 ) )
    {
        GN_ERROR( "FatMesh file must begin with \"FATMESHB\" or \"FATMESHT\"." );
        return false;
    }

    if( 'B' == header[7] )
    {
        // TODO: read binary file
    }
    else( 'T' == header[7] )
    {
        // TODO: read text file
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// ---------------------------------------------------------------------------------------
bool GN::gfx::FatMesh::writeTo( File & fp, char mode )
{
    GN_GUARD;

    if( 'B' == mode )
    {
    }
    else if( 'T' == mode )
    {
    }
    else
    {
        GN_ERROR( "invalid mode. Must be 'B' or 'T'." );
        return false;
    }

    // success
    return true;

    GN_UNGUARD;
}


// ***************************************************************************************
// private functions
// ***************************************************************************************

//
//
// ---------------------------------------------------------------------------------------
void GN::gfx::FatMesh::sortByMaterial( size_t vtxSegIdx, const DynaArray<size_t> & faces )
{
    GN_GUARD;

    const VtxSegment & vs = mVtxSegments[vtxSegIdx];

    std::map< int,DynaArray<size_t> > matTable;
    for( size_t i = 0; i < faces.size(); ++i )
    {
        size_t faceIdx = faces[i];
        matTable[mFaces[faceIdx].material].append( faceIdx );
    }

    for( std::map< int,DynaArray<size_t> >::const_iterator matIter = matTable.begin(); matIter != matTable.end(); ++matIter )
    {
        // create new face segment
        mFaceSegments.resize( mFaceSegments.size() + 1 );
        FaceSegment & fs = mFaceSegments.back();

        fs.material = matIter->first;
        fs.vtxSegIdx = vtxSegIdx;

        // fill 32-bit index buffer
        const DynaArray<size_t> & m = matIter->second;
        fs.indices32.resize( m.size() * 3 );
        for( size_t faceIdx = 0; faceIdx < m.size(); ++faceIdx )
        {
            const Face & f = mFaces[m[faceIdx]];

            GN_ASSERT( matIter->first == f.material );
            GN_ASSERT( vs.start <= f.i0 && f.i0 < (vs.start+vs.count) );
            GN_ASSERT( vs.start <= f.i1 && f.i1 < (vs.start+vs.count) );
            GN_ASSERT( vs.start <= f.i2 && f.i2 < (vs.start+vs.count) );

#if GN_X64
            GN_ASSERT( f.i0 < 0xFFFFFFFF );
            GN_ASSERT( f.i1 < 0xFFFFFFFF );
            GN_ASSERT( f.i2 < 0xFFFFFFFF );
#endif
            fs.indices32[faceIdx*3+0] = (uint32_t)( f.i0 - vs.start );
            fs.indices32[faceIdx*3+1] = (uint32_t)( f.i1 - vs.start );
            fs.indices32[faceIdx*3+2] = (uint32_t)( f.i2 - vs.start );
        }
    }

    GN_UNGUARD;
}

//
//
// ---------------------------------------------------------------------------------------
inline void GN::gfx::FatMesh::drawFaceSegment( size_t idx )
{
    GN_GUARD_SLOW;

    GN_ASSERT( idx <= mFaceSegments.size() );

    if( mUse32BitIndex )
    {
        GN_WARN( "current renderer does not support 32bit index buffer" );
        return;
    }

    const FaceSegment & s = mFaceSegments[idx];
    if( s.indices16.size() < 3 )
    {
        GN_WARN( "no enough indices (at least 3)" );
        return;
    }

    GN_ASSERT( s.vtxSegIdx < mVtxSegments.size() );
    const VtxSegment & v = mVtxSegments[s.vtxSegIdx];
    GN_ASSERT( v.start < mVertices.size() && (v.start+v.count) <= mVertices.size() );

    gRenderer.drawIndexedUp(
        mUseTriStrip ? TRIANGLE_STRIP : TRIANGLE_LIST,
        mUseTriStrip ? s.indices16.size() - 2 : s.indices16.size() / 3,
        v.count,
        mVertices.cptr() + v.start,
        sizeof(FatVertex),
        s.indices16.cptr() );

    GN_UNGUARD_SLOW;
}
