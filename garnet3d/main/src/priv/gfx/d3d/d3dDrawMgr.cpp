#include "pch.h"
#include "d3dRenderer.h"
#if !GN_ENABLE_INLINE
#include "d3dDrawMgr.inl"
#endif
#include "d3dTexture.h"

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
bool GN::gfx::D3DRenderer::drawDeviceCreate()
{
    GN_GUARD;

    if( !createFont() ) return false;

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

    // restore font
    if( mFont )
    {
        GN_DX_CHECK_RV( mFont->OnResetDevice(), false );
    }

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

    // dispose font
    if( mFont )
    {
        GN_DX_CHECK( mFont->OnLostDevice() );
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::drawDeviceDestroy()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    safeRelease( mFont );

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
        if( !(USAGE_RENDERTARGET & tex->getUsage() ) )
        {
            GNGFX_ERROR( "Texture must have usage of USAGE_RENDERTARGET!" );
            return;
        }

        // get surface pointer
        const D3DTexture * d3dTex = safeCast<const D3DTexture*>(tex);
        AutoComPtr<IDirect3DSurface9> surf;
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
    }

    // update D3D render target
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
        GN_WARN( "TODO: reapply viewport!" );
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
        if( !(USAGE_DEPTH & tex->getUsage()) )
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

    // update vertex buffer state
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

    // update vertex buffer state
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
void GN::gfx::D3DRenderer::drawTextW(
    const wchar_t * text, int x, int y, const Vector4f & color )
{
    GN_GUARD_SLOW;

    GN_ASSERT( mDrawBegan );

    // skip empty string
    if( strEmpty(text) ) return;

    int r;
    RECT rc;
    D3DCOLOR cl = sRgba2D3DCOLOR(color);

    // calculate drawing rect
    rc.left = 0;
    rc.top  = 0;
    r = mFont->DrawTextW( 0, text, -1, &rc, DT_CALCRECT, cl );
    if( 0 == r )
    {
        GNGFX_ERROR( "fail to get text extent!" );
        return;
    }

    // draw text
    OffsetRect( &rc, x, y );
    r = mFont->DrawTextW( 0, text, -1, &rc, DT_LEFT, cl );
    if( 0 == r )
    {
        GNGFX_ERROR( "fail to draw text!" );
    }

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3DRenderer::createFont()
{
    GN_GUARD;

    // Get font description
    LOGFONTW lf;
    ::GetObjectW( GetStockObject(SYSTEM_FIXED_FONT), sizeof(lf), &lf );

    // create d3dx font
    GN_DX_CHECK_RV(
        D3DXCreateFontW(
            mDevice,
            lf.lfHeight,
            lf.lfWidth,
            lf.lfWeight,
            0,
            lf.lfItalic,
            lf.lfCharSet,
            lf.lfOutPrecision,
            lf.lfQuality,
            lf.lfPitchAndFamily,
            lf.lfFaceName,
            &mFont ),
        false );

    HWND hwnd = (HWND)getDispDesc().windowHandle;

    // get window DC
    HDC dc;
    GN_MSW_CHECK_RV( ( dc = ::GetDC( hwnd ) ), false );

    // select default fixed font
    HGDIOBJ oldfont = ::SelectObject( dc, ::GetStockObject(SYSTEM_FIXED_FONT) );

    // get text height
    SIZE sz;
    if( !::GetTextExtentPoint32W(dc, L"你", 1, &sz) )
    {
        GNGFX_ERROR( "Fail to get text height : %s!", getOSErrorInfo() );
        ::SelectObject( dc, oldfont );
        ::ReleaseDC( hwnd, dc );
        return false;
    }
    mFontHeight = sz.cy;

    // release local variables
    ::SelectObject( dc, oldfont );
    ::ReleaseDC( hwnd, dc );

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3DRenderer::handleDeviceLost()
{
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
        GNGFX_INFO( "\nDevice has lost and could NOT be restored by now...\n" );
        ::Sleep( 500 );
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
}
