#include "pch.h"
#include "d3d9Renderer.h"
#include "d3d9Font.h"
#include "d3d9Quad.h"
#include "d3d9Line.h"
#include "d3d9IdxBuf.h"

// static primitive map
static D3DPRIMITIVETYPE sPrimMap[GN::gfx::NUM_PRIMITIVES] =
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

//
/// convert Vector4f to D3COLOR
// ------------------------------------------------------------------------
static GN_INLINE D3DCOLOR sRgba2D3DCOLOR( const GN::Vector4f & c )
{
    D3DCOLOR dc = D3DCOLOR_COLORVALUE(c.r, c.g, c.b, c.a);
    return dc;
}

// *****************************************************************************
// interface functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D9Renderer::drawBegin()
{
    GN_GUARD_SLOW;

    PIXPERF_FUNCTION_EVENT();

    GN_ASSERT( !mDrawBegan );

#if !GN_XENON
    // handle render window size move
    if( !handleRenderWindowSizeMove() ) return false;

    // check for device lost
    if( !handleDeviceLost() ) return false;
#endif

    // begin scene
    GN_DX9_CHECK_RV( mDevice->BeginScene(), 0 );

    // success
    mDrawBegan = true;
    mNumPrims = 0;
    mNumBatches = 0;
    return true;

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9Renderer::drawEnd()
{
    GN_GUARD_SLOW;

    PIXPERF_FUNCTION_EVENT();

    GN_ASSERT( mDrawBegan );
    mDrawBegan = false;
    GN_DX9_CHECK( mDevice->EndScene() );
    GN_DX9_CHECK( mDevice->Present( 0, 0, 0, 0 ) );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9Renderer::clearScreen(
    const GN::Vector4f & c, float z, UInt32 s, BitFields flags )
{
    GN_GUARD_SLOW;

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
    size_t        numPrims,
    size_t        startVtx,
    size_t        minVtxIdx,
    size_t        numVtx,
    size_t        startIdx )
{
    GN_GUARD_SLOW;

    GN_ASSERT( mDrawBegan );

    //
    // make sure numPrims is not too large
    //
    GN_ASSERT_EX( numPrims <= getCaps(CAPS_MAX_PRIMITIVES), "too many primitives!" );

    // draw indexed primitives
    GN_ASSERT( prim < NUM_PRIMITIVES );
    GN_DX9_CHECK(
        mDevice->DrawIndexedPrimitive(
            sPrimMap[prim],     // primitive type
            (UINT)startVtx ,     // start vertex
            (UINT)minVtxIdx,    // min vertex index
            (UINT)numVtx,       // num of vertices
            (UINT)startIdx,     // base index
            (UINT)numPrims ) ); // primitive count

    // success
    mNumPrims += numPrims;
    ++mNumBatches;

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9Renderer::draw(
    PrimitiveType prim, size_t numPrims, size_t startVtx )
{
    GN_GUARD_SLOW;

    GN_ASSERT( mDrawBegan );

    //
    // make sure numPrims is not too large
    //
    GN_ASSERT_EX( numPrims <= getCaps(CAPS_MAX_PRIMITIVES), "too many primitives!" );

    // draw indexed primitives
    GN_ASSERT( prim < NUM_PRIMITIVES );
    GN_DX9_CHECK(
        mDevice->DrawPrimitive(
            sPrimMap[prim],     // primitive type
            (UINT)startVtx,      // start vertex
            (UINT)numPrims ) ); // primitive count

    // success
    mNumPrims += numPrims;
    ++mNumBatches;

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9Renderer::drawIndexedUp(
    PrimitiveType    prim,
    size_t           numPrims,
    size_t           numVertices,
    const void *     vertexData,
    size_t           strideInBytes,
    const UInt16 * indexData )
{
    GN_GUARD_SLOW;

    PIXPERF_FUNCTION_EVENT();

    //
    // make sure numPrims is not too large
    //
    GN_ASSERT_EX( numPrims <= getCaps(CAPS_MAX_PRIMITIVES), "too many primitives!" );

    // store vertex and index buffer
    AutoComPtr<IDirect3DVertexBuffer9> vb; UINT vbOffset; UINT vbStride;
    AutoComPtr<IDirect3DIndexBuffer9> ib;
    GN_DX9_CHECK( mDevice->GetStreamSource( 0, &vb, &vbOffset, &vbStride ) );
    GN_DX9_CHECK( mDevice->GetIndices( &ib ) );

    GN_DX9_CHECK(
        mDevice->DrawIndexedPrimitiveUP(
            sPrimMap[prim],
            0, // MinVertexIndex
            (UINT)numVertices,
            (UINT)numPrims,
            indexData,
            D3DFMT_INDEX16,
            vertexData,
            (UINT)strideInBytes ) );

    // restore vertex and index buffer
    GN_DX9_CHECK( mDevice->SetStreamSource( 0, vb, vbOffset, vbStride ) );
    GN_DX9_CHECK( mDevice->SetIndices( ib ) );

    // success
    mNumPrims += numPrims;
    ++mNumBatches;

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9Renderer::drawUp(
    PrimitiveType prim,
    size_t        numPrims,
    const void *  vertexData,
    size_t        strideInBytes )
{
    GN_GUARD_SLOW;

    //
    // make sure numPrims is not too large
    //
    GN_ASSERT_EX( numPrims <= getCaps(CAPS_MAX_PRIMITIVES), "too many primitives!" );

    // store vertex and index buffer
    AutoComPtr<IDirect3DVertexBuffer9> vb; UINT vbOffset; UINT vbStride;
    GN_DX9_CHECK( mDevice->GetStreamSource( 0, &vb, &vbOffset, &vbStride ) );

    // do draw
    GN_DX9_CHECK( mDevice->DrawPrimitiveUP(
        sPrimMap[prim],
        (UINT)numPrims,
        vertexData,
        (UINT)strideInBytes ) );

    // restore vertex and index buffer
    GN_DX9_CHECK( mDevice->SetStreamSource( 0, vb, vbOffset, vbStride ) );

    // success
    mNumPrims += numPrims;
    ++mNumBatches;

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9Renderer::drawQuads(
    BitFields options,
    const void * positions, size_t posStride,
    const void * texcoords, size_t texStride,
    const void * colors, size_t clrStride,
    size_t count )
{
    GN_GUARD_SLOW;
    PIXPERF_FUNCTION_EVENT();
    GN_ASSERT( mDrawBegan && mQuad );
    mQuad->drawQuads(
        options,
        (const float*)positions, posStride,
        (const float*)texcoords, texStride,
        (const UInt32*)colors, clrStride,
        count );
    PIXPERF_END_EVENT();
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
    UInt32 color,
    const Matrix44f & model,
    const Matrix44f & view,
    const Matrix44f & proj )
{
    GN_GUARD_SLOW;
    PIXPERF_FUNCTION_EVENT();
    GN_ASSERT( mDrawBegan && mLine );
    mLine->drawLines(
        options,
        (const float*)positions, stride,
        count, color,
        model, view, proj );
    PIXPERF_END_EVENT();
    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D9Renderer::drawDebugText(
    const char * text, int x, int y, const Vector4f & color )
{
    GN_GUARD_SLOW;

    PIXPERF_FUNCTION_EVENT();

    GN_ASSERT( mDrawBegan && mFont );
    mFont->drawText( text, x, y, color );

    GN_UNGUARD_SLOW;
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
