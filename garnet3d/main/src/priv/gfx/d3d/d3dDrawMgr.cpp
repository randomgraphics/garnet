#include "pch.h"
#include "d3dRenderer.h"
#if !GN_ENABLE_INLINE
#include "d3dDrawMgr.inl"
#endif
#include "d3dTexture.h"
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
// device management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3DRenderer::drawDeviceCreate()
{
    GN_GUARD;

    // check multiple render target support
    if( getCaps(CAPS_MAX_RENDER_TARGETS) > 4 )
    {
        GNGFX_ERROR( "Sorry, we currently do not support more then 4 simultaneous render targets." );
        return false;
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3DRenderer::drawDeviceRestore()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    // get default render target surface
    GN_ASSERT( 0 == mDefaultRT0 );
    GN_DX_CHECK_RV( mDevice->GetRenderTarget( 0, &mDefaultRT0 ), false );

    // restore render target size to default value
    mCurrentRTSize.set( getDispDesc().width, getDispDesc().height );
    mAutoDepthSize.set( getDispDesc().width, getDispDesc().height );

    // make sure MRT caps does not exceed maximum allowance value
    GN_ASSERT( getCaps(CAPS_MAX_RENDER_TARGETS) <= 4 );

    // (re)apply render targets
    RenderTargetTextureDesc desc;
    for( size_t i = 0; i < getCaps(CAPS_MAX_RENDER_TARGETS); ++i )
    {
        desc = mCurrentRTs[i];

        // 将mCurrentRts修改为无效值，以便绕过SetRenderTarget()的重复调用检测。
        // mCurrentDepth同理。
        mCurrentRTs[i].tex = (const Texture*)0xdeadbeef; mCurrentRTs[i].face = NUM_TEXFACES;

        setRenderTarget( i, desc.tex, desc.face );
    }

    // (re)apply depth texture
    desc = mCurrentDepth;
    mCurrentDepth.tex = (const Texture*)0xdeadbeef; mCurrentDepth.face = NUM_TEXFACES;
    setRenderDepth( desc.tex, desc.face );

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::drawDeviceDispose()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    // make all draw state dirty
    mDrawState.dirtyFlags.u32 = 0xFFFFFFFF;

    // release render target pointers
    safeRelease( mDefaultRT0 );
    safeRelease( mAutoDepth );

    GN_UNGUARD;
}

// *****************************************************************************
// interface functions
// *****************************************************************************

//
//
// ----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::setRenderTarget(
    size_t index, const Texture * tex, TexFace face )
{
    GN_GUARD_SLOW;

    if( index >= getCaps(CAPS_MAX_RENDER_TARGETS) )
    {
        GN_ERROR( "render target index (%d) is too large!", index );
        return;
    }

    RenderTargetTextureDesc & rttd = mCurrentRTs[index];

    // skip redundant call
    if( rttd.equal( tex, face ) ) return;

    // get texture surface
    AutoComPtr<IDirect3DSurface9> surf;
    Vector2<uint32_t> surfSize;
    if( tex )
    {
        // check texture's creation flag
        if( !(TEXUSAGE_RENDERTARGET & tex->getUsage() ) )
        {
            GNGFX_ERROR( "Texture must have usage of TEXUSAGE_RENDERTARGET!" );
            return;
        }

        // get surface pointer
        const D3DTexture * d3dTex = safeCast<const D3DTexture*>(tex);
        surf.attach( d3dTex->getSurface( face, 0 ) );
        if( !surf ) return;

        // get surface size
        tex->getSize<uint32_t>( &surfSize.x, &surfSize.y, NULL );
    }
    else if( 0 == index )
    {
        GN_ASSERT( mDefaultRT0 );
        surf.reset( mDefaultRT0 );
        surfSize.x = getDispDesc().width;
        surfSize.y = getDispDesc().height;
        GN_ASSERT( surf );
    }

    // update D3D render target
    GN_ASSERT( 0 != index || surf );
    GN_DX_CHECK( mDevice->SetRenderTarget( (DWORD)index, surf ) );
    rttd.tex  = tex;
    rttd.face = face;
 
    // handle RT size change
    if( 0 == index )
    {
        // update render target size
        mCurrentRTSize = surfSize;

        // update automatic depth surface
        if( 0 == mCurrentDepth.tex ) resizeAutoDepthBuffer( surfSize );

        // Because viewport is using relative coordinates based on render target size,
        // so here we have to re-apply the viewport.
        setRenderParameter( RPT_TRANSFORM_VIEWPORT, getRenderParameter( RPT_TRANSFORM_VIEWPORT ).valueFloats, 4 );
    }

    GN_UNGUARD_SLOW;
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::setRenderDepth( const Texture * tex, TexFace face )
{
    GN_GUARD_SLOW;

    if( mCurrentDepth.equal( tex, face ) ) return;

    if( tex )
    {
        // check texture's creation flag
        if( !(TEXUSAGE_DEPTH & tex->getUsage()) )
        {
            GNGFX_ERROR( "can't set non-depth-texture as depth buffer!" );
            return;
        }

        // get surface pointer
        const D3DTexture * d3dTex = safeCast<const D3DTexture*>(tex);
        AutoComPtr<IDirect3DSurface9> surf;
        surf.attach( d3dTex->getSurface( face, 0 ) );
        if( !surf ) return;

        // change D3D depth buffer
        GN_DX_CHECK( mDevice->SetDepthStencilSurface( surf ) );
    }
    else
    {
        // fall back to automatic depth surface
        resizeAutoDepthBuffer( mCurrentRTSize );
    }

    // success
    mCurrentDepth.tex  = tex;
    mCurrentDepth.face = face;

    GN_UNGUARD_SLOW;
}

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
    size_t        baseVtx,
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
            (UINT)baseVtx ,     // start vertex
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
    PrimitiveType prim, size_t numPrims, size_t baseVtx )
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
            (UINT)baseVtx,      // start vertex
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
void GN::gfx::D3DRenderer::drawTextW(
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
