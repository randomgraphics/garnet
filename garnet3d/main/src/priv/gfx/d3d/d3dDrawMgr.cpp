#include "pch.h"
#include "d3dRenderer.h"
#include "d3dFont.h"
#include "d3dQuad.h"
#include "d3dIdxBuf.h"

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
//! convert Vector4f to D3COLOR
// ------------------------------------------------------------------------
static GN_INLINE D3DCOLOR sRgba2D3DCOLOR( const GN::Vector4f & c )
{
    D3DCOLOR dc = D3DCOLOR_COLORVALUE(c.r, c.g, c.b, c.a);
    return dc;
}

// *****************************************************************************
// device management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3DRenderer::drawInit()
{
    GN_GUARD;

    mFont = new D3DFont(*this);
    if( !mFont->init() ) return false;

    mQuad = new D3DQuad(*this);
    if( !mQuad->init() ) return false;

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::drawQuit()
{
    GN_GUARD;

    safeDelete( mFont );
    safeDelete( mQuad );

    GN_UNGUARD;
}

// *****************************************************************************
// interface functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3DRenderer::drawBegin()
{
    GN_GUARD_SLOW;

    GN_ASSERT( !mDrawBegan );

    // handle render window size move
    if( !handleRenderWindowSizeMove() ) return false;

    // check for device lost
    if( !handleDeviceLost() ) return false;

    // begin scene
    GN_DX_CHECK_RV( mDevice->BeginScene(), 0 );

    // success
    mDrawBegan = true;
    mNumPrims = 0;
    mNumDraws = 0;
    return true;

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::drawEnd()
{
    GN_GUARD_SLOW;

    GN_ASSERT( mDrawBegan );
    mDrawBegan = false;
    GN_DX_CHECK( mDevice->EndScene() );
    GN_DX_CHECK( mDevice->Present( 0, 0, 0, 0 ) );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::drawFinish()
{
    GN_GUARD_SLOW;

    GN_ASSERT( mDrawBegan );

    GN_DX_CHECK_R( mDevice->EndScene() );
    GN_DX_CHECK( mDevice->BeginScene() );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::clearScreen(
    const GN::Vector4f & c, float z, uint32_t s, uint32_t flags )
{
    GN_GUARD_SLOW;

    // build d3d clear flag
    int d3dflag = (flags & C_BUFFER ? D3DCLEAR_TARGET : 0)
        | (flags & Z_BUFFER ? D3DCLEAR_ZBUFFER : 0)
        | (flags & S_BUFFER ? D3DCLEAR_STENCIL : 0);

    // do clear
    GN_DX_CHECK( mDevice->Clear( 0, 0, d3dflag, sRgba2D3DCOLOR(c), z, s ) );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::drawIndexed(
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

    // update draw state
    applyDrawState();

    // draw indexed primitives
    GN_ASSERT( prim < NUM_PRIMITIVES );
    GN_DX_CHECK(
        mDevice->DrawIndexedPrimitive(
            sPrimMap[prim],     // primitive type
            (UINT)startVtx ,     // start vertex
            (UINT)minVtxIdx,    // min vertex index
            (UINT)numVtx,       // num of vertices
            (UINT)startIdx,     // base index
            (UINT)numPrims ) ); // primitive count

    // success
    mNumPrims += numPrims;
    ++mNumDraws;

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::draw(
    PrimitiveType prim, size_t numPrims, size_t startVtx )
{
    GN_GUARD_SLOW;

    GN_ASSERT( mDrawBegan );

    //
    // make sure numPrims is not too large
    //
    GN_ASSERT_EX( numPrims <= getCaps(CAPS_MAX_PRIMITIVES), "too many primitives!" );

    // update draw state
    applyDrawState();

    // draw indexed primitives
    GN_ASSERT( prim < NUM_PRIMITIVES );
    GN_DX_CHECK(
        mDevice->DrawPrimitive(
            sPrimMap[prim],     // primitive type
            (UINT)startVtx,      // start vertex
            (UINT)numPrims ) ); // primitive count

    // success
    mNumPrims += numPrims;
    ++mNumDraws;

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::drawIndexedUp(
    PrimitiveType    prim,
    size_t           numPrims,
    size_t           numVertices,
    const void *     vertexData,
    size_t           strideInBytes,
    const uint16_t * indexData )
{
    GN_GUARD_SLOW;

    //
    // make sure numPrims is not too large
    //
    GN_ASSERT_EX( numPrims <= getCaps(CAPS_MAX_PRIMITIVES), "too many primitives!" );

    // update draw state
    applyDrawState();

    GN_DX_CHECK(
        mDevice->DrawIndexedPrimitiveUP(
            sPrimMap[prim],
            0, // MinVertexIndex
            numVertices,
            numPrims,
            indexData,
            D3DFMT_INDEX16,
            vertexData,
            strideInBytes ) );

    // dirty draw state of stream 0
    mDrawState.dirtyFlags.vtxBufs |= 1;
    mDrawState.dirtyFlags.idxBuf |= 1;

    // success
    mNumPrims += numPrims;
    ++mNumDraws;

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::drawUp(
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

    // update draw state
    applyDrawState();

    // do draw
    GN_DX_CHECK( mDevice->DrawPrimitiveUP( sPrimMap[prim], numPrims, vertexData, strideInBytes ) );

    // dirty draw state of stream 0
    mDrawState.dirtyFlags.vtxBufs |= 1;

    // success
    mNumPrims += numPrims;
    ++mNumDraws;

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::drawQuads(
    uint32_t options,
    const void * positions, size_t posStride,
    const void * texcoords, size_t texStride,
    size_t count )
{
    GN_GUARD_SLOW;
    PIXPERF_BEGIN_EVENT( 0, L"GN::gfx::D3DRenderer::drawQuads" );
    GN_ASSERT( mDrawBegan && mQuad );
    applyDrawState();
    mQuad->drawQuads( (const float*)positions, posStride, (const float*)texcoords, texStride, count, options );
    PIXPERF_END_EVENT();
    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::drawDebugTextW(
    const wchar_t * text, int x, int y, const Vector4f & color )
{
    GN_GUARD_SLOW;

    GN_ASSERT( mDrawBegan && mFont );
    mFont->drawTextW( text, x, y, color );

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3DRenderer::handleDeviceLost()
{
#if GN_XENON
    // There's no device lost on Xenon.
    return true;
#else
    GN_GUARD;

    GN_ASSERT( mDevice );

    HRESULT hr = mDevice->TestCooperativeLevel();
    if( D3DERR_DEVICENOTRESET == hr )
    {
        GN_INFO( "\n============ Restore lost device ===============" );

        // dispose
        deviceDispose();

        // reset d3ddevice
        GN_DX_CHECK_RV( mDevice->Reset( &mPresentParameters ), false );

        // try restore
        if( !deviceRestore() ) return false;

        GN_INFO( "=================================================\n" );
    }
    else if( D3DERR_DEVICELOST == hr )
    {
        GN_INFO( "\nDevice has lost and could NOT be restored by now.\nWait for 2 seconds to try again...\n" );
        ::Sleep( 2000 );
        return false;
    }
    else if (D3D_OK != hr)
    {
        // fatal error
        GN_ERROR( "TestCooperativeLevel() failed: %s!", ::DXGetErrorString9A(hr) );
        return false;
    }

    // success
    return true;

    GN_UNGUARD;
#endif
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE void GN::gfx::D3DRenderer::applyDrawState()
{
    GN_GUARD_SLOW;

    PIXPERF_BEGIN_EVENT( 0, L"GN::gfx::D3DRenderer::applyDrawState" );

    // apply textures
    if( getDirtyTextureStages() > 0 ) applyTexture();

    // apply FFP states
    if( 0 != mFfpDirtyFlags.u32 ) applyFfpState();

    // apply other draw states
    if( mDrawState.dirtyFlags.u32 )
    {
        if( mDrawState.dirtyFlags.vtxBinding )
        {
            applyVtxBinding( mDrawState.vtxBinding );
        }

        if( mDrawState.dirtyFlags.vtxBufs )
        {
            applyVtxBuffers();
        }

        if( mDrawState.dirtyFlags.idxBuf )
        {
            const IdxBuf * buf = mDrawState.idxBuf.get();
            GN_DX_CHECK( mDevice->SetIndices( buf ? safeCast<const D3DIdxBuf*>(buf)->getD3DIb() : 0 ) );
        }

        applyShader(
            mDrawState.vtxShader.get(), !!mDrawState.dirtyFlags.vtxShader,
            mDrawState.pxlShader.get(), !!mDrawState.dirtyFlags.pxlShader );

        // clear dirty flags
        mDrawState.dirtyFlags.u32 = 0;
    }

    PIXPERF_END_EVENT();

    GN_UNGUARD_SLOW;
}
