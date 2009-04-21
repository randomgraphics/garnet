#include "pch.h"
#include "d3d9Renderer.h"
#include "d3d9Line.h"
#include "d3d9IdxBuf.h"

///
/// static primitive map
///
static D3DPRIMITIVETYPE sPrimMap[GN::gfx::PrimitiveType::NUM_PRIMITIVES] =
{
    D3DPT_POINTLIST,
    D3DPT_LINELIST,
    D3DPT_LINESTRIP,
    D3DPT_TRIANGLELIST,
    D3DPT_TRIANGLESTRIP,
#if GN_XENON
    D3DPT_QUADLIST,
    D3DPT_RECTLIST,
#else
    D3DPT_FORCE_DWORD,
    D3DPT_FORCE_DWORD,
#endif
};

///
/// convert Vector4f to D3COLOR
// ------------------------------------------------------------------------
static inline D3DCOLOR sRgba2D3DCOLOR( const GN::Vector4f & c )
{
    D3DCOLOR dc = D3DCOLOR_COLORVALUE(c.r, c.g, c.b, c.a);
    return dc;
}

#if GN_BUILD_RETAIL
#define DUMP_STATE(X)   // disable dump in retail build
#else
#define DUMP_STATE(X) \
    if( mDumpThisFrame && \
        ( mDumpStart <= mDrawCounter && mDrawCounter < mDumpEnd || \
          0 == mDumpStart && 0 == mDumpEnd ) ) \
    { X; } \
    else void(0)
#endif

// *****************************************************************************
// interface functions
// *****************************************************************************

static const bool mFakeDraw = false;

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9Renderer::drawBegin()
{
    GN_GUARD_SLOW;

    GN_ASSERT( getCurrentThreadId() == mThreadId );

    if( mFakeDraw ) return true;

    PIXPERF_FUNCTION_EVENT();

    GN_ASSERT( !mDrawBegan );

#if !GN_XENON
    // check for device lost
    if( !handleDeviceLost() ) return false;
#endif

    // begin scene
    GN_DX9_CHECK_RV( mDevice->BeginScene(), 0 );

    // update per-frame data
    mDrawBegan = true;
    mNumPrims = 0;
    mNumBatches = 0;
    mDrawCounter = 0;

#if !GN_BUILD_RETAIL
    if( mDumpNextFrame )
    {
        mDumpNextFrame = false;
        mDumpThisFrame = true;
    }
#endif

    // success
    return true;

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9Renderer::drawEnd()
{
    GN_GUARD_SLOW;

    GN_ASSERT( getCurrentThreadId() == mThreadId );

    if( mFakeDraw ) return;

    PIXPERF_FUNCTION_EVENT();

    GN_ASSERT( mDrawBegan );
    mDrawBegan = false;
    GN_DX9_CHECK( mDevice->EndScene() );
    GN_DX9_CHECK( mDevice->Present( 0, 0, 0, 0 ) );

    ++mFrameCounter;

#if !GN_BUILD_RETAIL
    mDumpThisFrame = false;
#endif

    // handle render window size move
#if !GN_XENON
    handleRenderWindowSizeMove();
#endif

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9Renderer::clearScreen(
    const GN::Vector4f & c, float z, UInt8 s, BitFields flags )
{
    GN_GUARD_SLOW;

    GN_ASSERT( getCurrentThreadId() == mThreadId );

    if( mFakeDraw ) return;

    // build d3d clear flag
    int d3dflag = (flags & CLEAR_C ? D3DCLEAR_TARGET : 0)
        | (flags & CLEAR_Z ? D3DCLEAR_ZBUFFER : 0)
        | (flags & CLEAR_S ? D3DCLEAR_STENCIL : 0);

    // do clear
    GN_DX9_CHECK( mDevice->Clear( 0, 0, d3dflag, sRgba2D3DCOLOR(c), z, s ) );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9Renderer::drawIndexed(
    PrimitiveType prim,
    size_t        numprim,
    size_t        startvtx,
    size_t        minvtxidx,
    size_t        numvtx,
    size_t        startidx )
{
    GN_GUARD_SLOW;

    GN_ASSERT( getCurrentThreadId() == mThreadId );

    DUMP_STATE( dumpD3D9DrawIndexed(
        sPrimMap[prim],
        (UINT)startvtx,
        (UINT)minvtxidx,
        (UINT)numvtx,
        (UINT)startidx,
        (UINT)numprim ) );

    ++mDrawCounter;

    if( mFakeDraw ) return;

    GN_ASSERT( mDrawBegan );

    //
    // make sure numprim is not too large
    //
    GN_ASSERT_EX( numprim <= getCaps(CAPS_MAX_PRIMITIVES), "too many primitives!" );

    // draw indexed primitives
    GN_ASSERT( prim < PrimitiveType::NUM_PRIMITIVES );
    GN_DX9_CHECK(
        mDevice->DrawIndexedPrimitive(
            sPrimMap[prim],    // primitive type
            (UINT)startvtx,    // start vertex
            (UINT)minvtxidx,   // min vertex index
            (UINT)numvtx,      // num of vertices
            (UINT)startidx,    // base index
            (UINT)numprim ) ); // primitive count

    // success
    mNumPrims += numprim;
    ++mNumBatches;

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9Renderer::draw(
    PrimitiveType prim, size_t numprim, size_t startvtx )
{
    GN_GUARD_SLOW;

    GN_ASSERT( getCurrentThreadId() == mThreadId );

    DUMP_STATE( dumpD3D9Draw(
        sPrimMap[prim],
        (UINT)startvtx,
        (UINT)numprim ) );

    ++mDrawCounter;

    if( mFakeDraw ) return;

    GN_ASSERT( mDrawBegan );

    //
    // make sure numprim is not too large
    //
    GN_ASSERT_EX( numprim <= getCaps(CAPS_MAX_PRIMITIVES), "too many primitives!" );

    // draw indexed primitives
    GN_ASSERT( prim < PrimitiveType::NUM_PRIMITIVES );
    GN_DX9_CHECK(
        mDevice->DrawPrimitive(
            sPrimMap[prim],    // primitive type
            (UINT)startvtx,    // start vertex
            (UINT)numprim ) ); // primitive count

    // success
    mNumPrims += numprim;
    ++mNumBatches;

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9Renderer::drawIndexedUp(
    PrimitiveType    prim,
    size_t           numprim,
    size_t           numvtx,
    const void *     vertexData,
    size_t           strideInBytes,
    const UInt16 * indexData )
{
    GN_GUARD_SLOW;

    GN_ASSERT( getCurrentThreadId() == mThreadId );

    // TODO: dump state

    ++mDrawCounter;

    if( mFakeDraw ) return;

    PIXPERF_FUNCTION_EVENT();

    //
    // make sure numprim is not too large
    //
    GN_ASSERT_EX( numprim <= getCaps(CAPS_MAX_PRIMITIVES), "too many primitives!" );

    // store vertex and index buffer
    AutoComPtr<IDirect3DVertexBuffer9> vb; UINT vbOffset; UINT vbStride;
    AutoComPtr<IDirect3DIndexBuffer9> ib;
    GN_DX9_CHECK( mDevice->GetStreamSource( 0, &vb, &vbOffset, &vbStride ) );
    GN_DX9_CHECK( mDevice->GetIndices( &ib ) );

    GN_DX9_CHECK(
        mDevice->DrawIndexedPrimitiveUP(
            sPrimMap[prim],
            0, // MinVertexIndex
            (UINT)numvtx,
            (UINT)numprim,
            indexData,
            D3DFMT_INDEX16,
            vertexData,
            (UINT)strideInBytes ) );

    // restore vertex and index buffer
    GN_DX9_CHECK( mDevice->SetStreamSource( 0, vb, vbOffset, vbStride ) );
    GN_DX9_CHECK( mDevice->SetIndices( ib ) );

    // success
    mNumPrims += numprim;
    ++mNumBatches;

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9Renderer::drawUp(
    PrimitiveType prim,
    size_t        numprim,
    const void *  vertexData,
    size_t        strideInBytes )
{
    GN_GUARD_SLOW;

    GN_ASSERT( getCurrentThreadId() == mThreadId );

    // TODO: dump state

    ++mDrawCounter;

    if( mFakeDraw ) return;

    //
    // make sure numprim is not too large
    //
    GN_ASSERT_EX( numprim <= getCaps(CAPS_MAX_PRIMITIVES), "too many primitives!" );

    // store vertex and index buffer
    AutoComPtr<IDirect3DVertexBuffer9> vb; UINT vbOffset; UINT vbStride;
    GN_DX9_CHECK( mDevice->GetStreamSource( 0, &vb, &vbOffset, &vbStride ) );

    // do draw
    GN_DX9_CHECK( mDevice->DrawPrimitiveUP(
        sPrimMap[prim],
        (UINT)numprim,
        vertexData,
        (UINT)strideInBytes ) );

    // restore vertex and index buffer
    GN_DX9_CHECK( mDevice->SetStreamSource( 0, vb, vbOffset, vbStride ) );

    // success
    mNumPrims += numprim;
    ++mNumBatches;

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9Renderer::drawLines(
    BitFields options,
    const void * positions,
    size_t stride,
    size_t count,
    UInt32 rgba,
    const Matrix44f & model,
    const Matrix44f & view,
    const Matrix44f & proj )
{
    GN_GUARD_SLOW;

    GN_ASSERT( getCurrentThreadId() == mThreadId );

    // TODO: dump state

    ++mDrawCounter;

    if( mFakeDraw ) return;

    PIXPERF_FUNCTION_EVENT();
    GN_ASSERT( mDrawBegan && mLine );
    mLine->drawLines(
        options,
        (const float*)positions, stride,
        count, rgba,
        model, view, proj );
    PIXPERF_END_EVENT();

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9Renderer::dumpNextFrame( size_t startBatchIndex, size_t numBatches )
{
    GN_ASSERT( getCurrentThreadId() == mThreadId );
    mDumpNextFrame = true;
    mDumpStart = startBatchIndex;
    mDumpEnd   = startBatchIndex + numBatches;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9Renderer::handleDeviceLost()
{
#if GN_XENON
    // There's no device lost on Xenon.
    return true;
#else
    GN_GUARD;

    PIXPERF_FUNCTION_EVENT();

    GN_ASSERT( mDevice );

    HRESULT hr = mDevice->TestCooperativeLevel();
    if( D3DERR_DEVICENOTRESET == hr )
    {
        GN_INFO(sLogger)( "\n============ Restore lost device ===============" );

        // dispose
        deviceDispose();

        // reset d3ddevice
        GN_DX9_CHECK_RV( mDevice->Reset( &mPresentParameters ), false );

        // try restore
        if( !deviceRestore() ) return false;

        GN_INFO(sLogger)( "=================================================\n" );
    }
    else if( D3DERR_DEVICELOST == hr )
    {
        GN_INFO(sLogger)( "\nDevice has lost and could NOT be restored by now.\nWait for 2 seconds to try again...\n" );
        ::Sleep( 2000 );
        return false;
    }
    else if (D3D_OK != hr)
    {
        // fatal error
        GN_ERROR(sLogger)( "TestCooperativeLevel() failed: %s!", ::DXGetErrorString9A(hr) );
        return false;
    }

    // success
    return true;

    GN_UNGUARD;
#endif
}
