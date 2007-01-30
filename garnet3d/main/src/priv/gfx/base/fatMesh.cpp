#include "pch.h"
#include "garnet/gfx/fatMesh.h"

// ***************************************************************************************
// public functions
// ***************************************************************************************

GN::Logger * GN::gfx::FatMesh::sLogger = GN::getLogger("GN.gfx.base.FatMesh");

//
//
// ---------------------------------------------------------------------------------------
bool GN::gfx::FatVtxFmt::fromStr( const char * str, size_t len )
{
    if( 0 == str )
    {
        static GN::Logger * sLogger = GN::getLogger("GN.gfx.base.FatMesh");
        GN_ERROR(sLogger)( "null string" ); return false;
    }
    if( 0 == len ) len = strLen( str );
    GN_UNIMPL_WARNING();
    return false;
}

//
//
// ---------------------------------------------------------------------------------------
void GN::gfx::FatMesh::optimize( const OptimizeOptions & oo )
{
    GN_GUARD;

    // build vertex segments, to make sure index wouldn't be too large
    if( mVertices.size() > 0xFFFF )
    {
        GN_WARN(sLogger)( "do not support mesh with more than 0xFFFF vertices" );
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
                fs.indices16[i] = (UInt16)fs.indices32[i];
            }
            fs.indices32.clear();
        }
    }

    mUse32BitIndex = oo.use32BitIndex;
    mUseTriStrip = oo.strip;

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
        oo.strip = false; // strip optimization is not implemented yet.
        oo.useResetIndex = false;
        oo.use32BitIndex = false;
        optimize( oo );
    }

    // setup vertex format
    gRenderer.setVtxFmt( gRenderer.createVtxFmt( VtxFmtDesc::XYZ ) );

    for( size_t idx = 0; idx < mFaceSegments.size(); ++idx )
    {
        if( mFaceSegments[idx].material != material ) continue;
        drawFaceSegment( idx );
        break;
    }

    GN_UNGUARD_SLOW;
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
            const FatFace & f = mFaces[m[faceIdx]];

            GN_ASSERT( matIter->first == f.material );
            GN_ASSERT( vs.start <= f.i0 && f.i0 < (vs.start+vs.count) );
            GN_ASSERT( vs.start <= f.i1 && f.i1 < (vs.start+vs.count) );
            GN_ASSERT( vs.start <= f.i2 && f.i2 < (vs.start+vs.count) );

#if GN_X64
            GN_ASSERT( f.i0 < 0xFFFFFFFF );
            GN_ASSERT( f.i1 < 0xFFFFFFFF );
            GN_ASSERT( f.i2 < 0xFFFFFFFF );
#endif
            fs.indices32[faceIdx*3+0] = (UInt32)( f.i0 - vs.start );
            fs.indices32[faceIdx*3+1] = (UInt32)( f.i1 - vs.start );
            fs.indices32[faceIdx*3+2] = (UInt32)( f.i2 - vs.start );
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
        GN_WARN(sLogger)( "current renderer does not support 32bit index buffer" );
        return;
    }

    const FaceSegment & s = mFaceSegments[idx];
    if( s.indices16.size() < 3 )
    {
        GN_WARN(sLogger)( "no enough indices (at least 3)" );
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
        sizeof(FatVtx),
        s.indices16.cptr() );

    GN_UNGUARD_SLOW;
}
