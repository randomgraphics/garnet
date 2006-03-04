#include "pch.h"
#include "d3dRenderer.h"
#include "d3dTexture.h"

#if GN_XENON

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

    GN_UNIMPL_WARNING();
/*
    // get default render target surface
    GN_ASSERT( 0 == mDefaultRT0 );
    GN_DX_CHECK_RV( mDevice->GetRenderTarget( 0, &mDefaultRT0 ), false );

    // restore render target size to default value
    mCurrentRTSize.set( getDispDesc().width, getDispDesc().height );
    mAutoDepthSize.set( getDispDesc().width, getDispDesc().height );

    // make sure MRT caps does not exceed maximum allowance value
    GN_ASSERT( getCaps(CAPS_MAX_RENDER_TARGETS) <= MAX_RENDER_TARGETS );

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
*/
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

    GN_UNIMPL_WARNING();

    GN_UNGUARD;
}

// *****************************************************************************
// interface functions
// *****************************************************************************

//
//
// ----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::setRenderTarget(
    size_t index, const Texture * tex, uint32_t level, TexFace face )
{
    GN_GUARD_SLOW;

    if( isParameterCheckEnabled() )
    {
        if( index >= getCaps(CAPS_MAX_RENDER_TARGETS) )
        {
            GN_ERROR( "render target index (%d) is too large!", index );
            return;
        }
    }

    RenderTargetTextureDesc & rttd = mCurrentRTs[index];

    // skip redundant call
    if( rttd.equal( tex, level, face ) ) return;

    GN_UNIMPL();

    /* resolve content from EDRAM to current render target texture
    if( rttd.tex )
    {
        const D3DTexture * d3dTex = safeCast<const D3DTexture*>(rttd.tex);

        GN_CASSERT( D3DRESOLVE_RENDERTARGET3 == D3DRESOLVE_RENDERTARGET0+3 );
        GN_CASSERT( D3DRESOLVE_RENDERTARGET2 == D3DRESOLVE_RENDERTARGET0+2 );
        GN_CASSERT( D3DRESOLVE_RENDERTARGET1 == D3DRESOLVE_RENDERTARGET0+1 );
        GN_DX_CHECK( mDevice->Resolve(
            D3DRESOLVE_RENDERTARGET0 + i,
            NULL, // destRect
            d3dTex->getD3DTexture(),
            NULL, // destPoint
            rttd.level,
            rttd.face,
            NULL, .0f, 0, NULL ) );
    }

    // update render target data member
    if( tex ) tex->incref();
    if( rttd.tex ) rttd.tex->decref();
    rttd.tex = tex;
    rttd.level = level;
    rttd.face = face;

    // update render target size
    if( 0 == index )
    {
        if( tex )
        {
            tex->getMipSize( level, &mCurrentRTSize.x, &mCurrentRTSize.y );
        }
        else
        {
            // use default back buffer size
            mCurrentRTSize.x = getDispDesc().width;
            mCurrentRTSize.y = getDispDesc().height;
        }

        // update automatic depth surface
        resizeAutoDepthBuffer( mCurrentRTSize );

        // Because viewport is using relative coordinates based on render target size,
        // so here we have to re-apply the viewport.
        setViewport( getViewport() );
    }*/

    GN_UNGUARD_SLOW;
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::D3DRenderer::setRenderDepth( const Texture * tex, uint32_t level, TexFace face )
{
    GN_GUARD_SLOW;

    // NOTE: Xenon allows to resolve to any texture. so we may skip check of usage flags

    if( mCurrentDepth.equal( tex, level, face ) ) return;

    GN_UNIMPL();

    /* resolve to depth texture
    if( mCurrentDepth.tex )
    {
        const D3DTexture * d3dTex = safeCast<const D3DTexture*>(mCurrentDepth.tex);
        GN_DX_CHECK( mDevice->Resolve(
            D3DRESOLVE_DEPTHSTENCIL,
            NULL, // destRect
            d3dTex->getD3DTexture(),
            NULL, // destPoint
            mCurrentDepth.level,
            mCurrentDepth.face,
            NULL, .0f, 0, NULL ) );
    }

    // set new depth texture
    if( tex ) tex->incref();
    if( mCurrentDepth.tex ) mCurrentDepth.tex->decref();
    mCurrentDepth.tex = tex;
    mCurrentDepth.level = level;
    mCurrentDepth.face = face;*/

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
            mPresentParameters.AutoDepthStencilFormat,
            mPresentParameters.MultiSampleType, mPresentParameters.MultiSampleQuality,
            mPresentParameters.Flags & D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL,
            &mAutoDepth, 0 ) );
    }

    // update D3D depth surface
    GN_DX_CHECK_R( mDevice->SetDepthStencilSurface( mAutoDepth ) );

    GN_UNGUARD_SLOW;
}

#endif
