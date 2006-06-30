#include "pch.h"
#include "garnet/gfx/fatMesh.h"

// ***************************************************************************************
// public functions
// ***************************************************************************************

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
        optimize( *this, oo );
    }

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
