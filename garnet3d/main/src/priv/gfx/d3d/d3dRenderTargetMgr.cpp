#include "pch.h"
#include "d3dRenderer.h"
#include "d3dTexture.h"

#if !GN_XENON

// *****************************************************************************
// device management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3DRenderer::renderTargetDeviceCreate()
{
    GN_GUARD;

    // check multiple render target support
    if( getCaps(CAPS_MAX_RENDER_TARGETS) > MAX_RENDER_TARGETS )
    {
        GN_ERROR( "Sorry, we currently do not support more then %d simultaneous render targets.", MAX_RENDER_TARGETS );
        return false;
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3DRenderer::renderTargetDeviceRestore()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    // make sure MRT caps does not exceed maximum allowance value
    GN_ASSERT( getCaps(CAPS_MAX_RENDER_TARGETS) <= MAX_RENDER_TARGETS );

    // get default render target surface
    GN_ASSERT( 0 == mBackBuffer );
    GN_DX_CHECK_RV( mDevice->GetRenderTarget( 0, &mBackBuffer ), false );

    // restore render target size to default value
    mCurrentRTSize.set( getDispDesc().width, getDispDesc().height );
    mAutoDepthSize.set( 0, 0 );

    // (re)apply render targets
    RenderTargetTextureDesc desc;
    for( size_t i = 0; i < getCaps(CAPS_MAX_RENDER_TARGETS); ++i )
    {
        desc = mCurrentRTs[i];

        // 将mCurrentRts修改为无效值，以便绕过SetRenderTarget()的重复调用检测。
        // mCurrentDepth同理。
        mCurrentRTs[i].tex = (const Texture*)0xdeadbeef;
        mCurrentRTs[i].face = NUM_TEXFACES;
        setRenderTarget( i, desc.tex, desc.level, desc.face );
    }

    // (re)apply depth texture
    desc = mCurrentDepth;
    mCurrentDepth.tex = (const Texture*)0xdeadbeef;
    mCurrentDepth.face = NUM_TEXFACES;
    setRenderDepth( desc.tex, desc.level, desc.face );

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::renderTargetDeviceDispose()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    // release render target pointers
    safeRelease( mBackBuffer );
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
    size_t index, const Texture * tex, size_t level, size_t face )
{
    GN_GUARD_SLOW;

    if( isParameterCheckEnabled() )
    {
        if( index >= getCaps(CAPS_MAX_RENDER_TARGETS) )
        {
            GN_ERROR( "render target index (%d) is too large!", index );
            return;
        }
        // make sure target texture is a RTT
        if( tex && !(TEXUSAGE_RENDER_TARGET & tex->getDesc().usage) )
        {
            GN_ERROR( "Only texture with TEXUSAGE_RENDER_TARGET usage can be used as render target." );
            return;
        }
    }

    RenderTargetTextureDesc & rttd = mCurrentRTs[index];

    // skip redundant call
    if( rttd.equal( tex, level, face ) ) return;

    // get texture surface
    AutoComPtr<IDirect3DSurface9> surf;
    Vector2<uint32_t> surfSize;
    if( tex )
    {
        // check texture's creation flag
        if( !(TEXUSAGE_RENDER_TARGET & tex->getDesc().usage ) )
        {
            GN_ERROR( "Texture must have usage of TEXUSAGE_RENDER_TARGET!" );
            return;
        }

        // get surface pointer
        const D3DTexture * d3dTex = safeCast<const D3DTexture*>(tex);
        surf.attach( d3dTex->getSurface( face, level ) );
        if( !surf ) return;

        // get surface size
        D3DSURFACE_DESC desc;
        GN_DX_CHECK_R( surf->GetDesc( &desc ) );
        surfSize.x = desc.Width;
        surfSize.y = desc.Height;
    }
    else if( 0 == index )
    {
        GN_ASSERT( mBackBuffer );
        surf.reset( mBackBuffer );
        surfSize.x = getDispDesc().width;
        surfSize.y = getDispDesc().height;
        GN_ASSERT( surf );
    }

    // update D3D render target
    GN_ASSERT( 0 != index || surf );
    GN_DX_CHECK( mDevice->SetRenderTarget( (DWORD)index, surf ) );
    rttd.tex  = tex;
    rttd.level = level;
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
        setViewport( getViewport() );
    }

    GN_UNGUARD_SLOW;
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::setRenderDepth( const Texture * tex, size_t level, size_t face )
{
    GN_GUARD_SLOW;

    // check texture's creation flag
    if( isParameterCheckEnabled() )
    {
        if( tex && !(TEXUSAGE_DEPTH & tex->getDesc().usage) )
        {
            GN_ERROR( "can't set non-depth-texture as depth buffer!" );
            return;
        }
    }

    if( mCurrentDepth.equal( tex, level, face ) ) return;

    if( tex )
    {
        // get surface pointer
        const D3DTexture * d3dTex = safeCast<const D3DTexture*>(tex);
        AutoComPtr<IDirect3DSurface9> surf;
        surf.attach( d3dTex->getSurface( face, level ) );
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
    mCurrentDepth.tex = tex;
    mCurrentDepth.level = level;
    mCurrentDepth.face = face;

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN_INLINE void GN::gfx::D3DRenderer::resizeAutoDepthBuffer( const Vector2<uint32_t> & sz )
{
    GN_GUARD_SLOW;

    // Check surface size
    if( sz.x > mAutoDepthSize.x || sz.y > mAutoDepthSize.y || NULL == mAutoDepth )
    {
        // release old buffer
        safeRelease( mAutoDepth );

        // create a new buffer
        AutoComPtr<IDirect3DSurface9> newSurf;
        GN_DX_CHECK_R( mDevice->CreateDepthStencilSurface(
            sz.x, sz.y,
            D3DFMT_D24S8, // TODO: enumerate avaliable depth format
            mPresentParameters.MultiSampleType, mPresentParameters.MultiSampleQuality,
            mPresentParameters.Flags & D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL,
            &mAutoDepth, 0 ) );
    }

    // update D3D depth surface
    GN_DX_CHECK_R( mDevice->SetDepthStencilSurface( mAutoDepth ) );

    GN_UNGUARD_SLOW;
}

#endif
