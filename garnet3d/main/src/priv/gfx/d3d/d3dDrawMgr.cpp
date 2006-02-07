#include "pch.h"
#include "d3dRenderer.h"
#include "d3dFont.h"
#include "d3dQuad.h"

// static primitive map
static D3DPRIMITIVETYPE sPrimMap[GN::gfx::NUM_PRIMITIVES] =
{
    D3DPT_POINTLIST,
    D3DPT_LINELIST,
    D3DPT_LINESTRIP,
    D3DPT_TRIANGLELIST,
    D3DPT_TRIANGLESTRIP,
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
void GN::gfx::D3DRenderer::drawQuads(
    uint32_t options,
    const void * positions, size_t posStride,
    const void * texcoords, size_t texStride,
    size_t count )
{
    GN_GUARD_SLOW;

    GN_ASSERT( mDrawBegan && mQuad );

    applyDrawState();

    mQuad->drawQuads( (const Vector2f*)positions, posStride, (const Vector2f*)texcoords, texStride, count, options );

    mDrawState.dirtyFlags.vtxShader = 1;
    mDrawState.dirtyFlags.pxlShader = 1;
    mDrawState.dirtyFlags.vtxBufs |= 1;
    mDrawState.dirtyFlags.vtxBinding = 1;

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
        GNGFX_INFO( "\n============ Restore lost device ===============" );

        // dispose
        deviceDispose();

        // reset d3ddevice
        GN_DX_CHECK_RV( mDevice->Reset( &mPresentParameters ), false );

        // try restore
        if( !deviceRestore() ) return false;

        GNGFX_INFO( "=================================================\n" );
    }
    else if( D3DERR_DEVICELOST == hr )
    {
        GNGFX_INFO( "\nDevice has lost and could NOT be restored by now.\nWait for 2 seconds to try again...\n" );
        ::Sleep( 2000 );
        return false;
    }
    else if (D3D_OK != hr)
    {
        // fatal error
        GNGFX_ERROR( "TestCooperativeLevel() failed: %s!", ::DXGetErrorString9A(hr) );
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

    if( 0 == mDrawState.dirtyFlags.u32 ) return;

    if( mDrawState.dirtyFlags.vtxBinding )
    {
        applyVtxBinding( mDrawState.vtxBinding );
    }

    if( mDrawState.dirtyFlags.vtxBufs )
    {
        applyVtxBuffers();
    }

    applyShader(
        mDrawState.vtxShader.get(), !!mDrawState.dirtyFlags.vtxShader,
        mDrawState.pxlShader.get(), !!mDrawState.dirtyFlags.pxlShader );

    // apply FFP states
    if( 0 != mFfpDirtyFlags.u32 ) applyFfpState();

    // clear dirty flags
    mDrawState.dirtyFlags.u32 = 0;

    GN_UNGUARD_SLOW;
}
