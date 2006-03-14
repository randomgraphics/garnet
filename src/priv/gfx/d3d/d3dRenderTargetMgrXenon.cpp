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

    // make sure MRT caps does not exceed maximum allowance value
    GN_ASSERT( getCaps(CAPS_MAX_RENDER_TARGETS) <= MAX_RENDER_TARGETS );

    // get default render target surface
    GN_ASSERT( 0 == mBackBuffer );
    GN_DX_CHECK_RV( mDevice->GetRenderTarget( 0, &mBackBuffer ), false );

    // store old RT data
    RenderTargetTextureDesc oldRT[MAX_RENDER_TARGETS], oldDepth;
    for( int i = 0; i < MAX_RENDER_TARGETS; ++i )
    {
        oldRT[i] = mCurrentRTs[i];
    }
    oldDepth = mCurrentDepth;

    //set default render target
    mColorBufferSize.set( 0, 0 );
    mDepthBufferSize.set( 0, 0 );
    mCurrentRTSize.set( getDispDesc().width, getDispDesc().height );
    for( int i = 0; i < MAX_RENDER_TARGETS; ++i )
    {
        mCurrentRTs[i].tex = 0;
    }
    setViewport( getViewport() );

    // (re)apply render targets
    for( size_t i = 0; i < getCaps(CAPS_MAX_RENDER_TARGETS); ++i )
    {
        setRenderTarget( i, oldRT[i].tex, oldRT[i].level, oldRT[i].face );
    }
    setRenderDepth( oldDepth.tex, oldDepth.level, oldDepth.face );

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
    for( int i = 0; i < MAX_RENDER_TARGETS; ++i ) safeRelease( mColorBuffers[i] );
    safeRelease( mDepthBuffer );

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
    }

    RenderTargetTextureDesc & rttd = mCurrentRTs[index];

    // skip redundant call
    if( rttd.equal( tex, level, face ) ) return;

    // resolve content from EDRAM to current render target texture
    if( rttd.tex )
    {
        const D3DTexture * d3dTex = safeCast<const D3DTexture*>(rttd.tex);

        D3DRECT rc = { 0, 0 };
        rttd.tex->getMipSize<LONG>( level, &rc.x2, &rc.y2, (LONG*)NULL );

        GN_CASSERT( D3DRESOLVE_RENDERTARGET3 == D3DRESOLVE_RENDERTARGET0+3 );
        GN_CASSERT( D3DRESOLVE_RENDERTARGET2 == D3DRESOLVE_RENDERTARGET0+2 );
        GN_CASSERT( D3DRESOLVE_RENDERTARGET1 == D3DRESOLVE_RENDERTARGET0+1 );
        GN_DX_CHECK( mDevice->Resolve(
            D3DRESOLVE_RENDERTARGET0 + index,
            &rc,
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

    // resize color and depth buffer
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
        resizeColorBuffers( mCurrentRTSize );
        resizeDepthBuffer( mCurrentRTSize );

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

    // NOTE: Xenon allows to resolve to any texture. so we may skip check of usage flags

    if( mCurrentDepth.equal( tex, level, face ) ) return;

    GN_UNIMPL();

    // resolve to depth texture
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
    mCurrentDepth.face = face;

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN_INLINE void GN::gfx::D3DRenderer::resizeColorBuffers( const Vector2<uint32_t> & sz )
{
    GN_GUARD_SLOW;

    // Check surface size
    if( sz.x > mColorBufferSize.x || sz.y > mColorBufferSize.y || NULL == mDepthBuffer )
    {
        uint32_t newX = max( sz.x, mColorBufferSize.x );
        uint32_t newY = max( sz.y, mColorBufferSize.y );

        D3DFORMAT newFormat = D3DFMT_A8R8G8B8; // TODO: let user specify format.

        const D3DPRESENT_PARAMETERS & d3dpp = getPresentParameters();
        D3DMULTISAMPLE_TYPE msaaType = d3dpp.MultiSampleType;
        DWORD msaaQuality = d3dpp.MultiSampleQuality;

        // check for MASS compability
        if( !checkD3DDeviceMsaa( newFormat ) )
        {
            msaaType = D3DMULTISAMPLE_NONE;
            msaaQuality = 0;
        }

        // create new color buffers
        for( int i = 0; i < MAX_RENDER_TARGETS; ++i )
        {
            // release old buffer
            safeRelease( mColorBuffers[i] );

            // create new buffer
            GN_DX_CHECK( mDevice->CreateRenderTarget(
                    newX, newY,
                    newFormat,
                    msaaType, msaaQuality,
                    FALSE, // non-lockable
                    &mColorBuffers[i], 0 ) );

            // bind color buffers
            GN_DX_CHECK( mDevice->SetRenderTarget( i, mColorBuffers[i] ) );
        }

        // update buffer size
        mColorBufferSize.x = newX;
        mColorBufferSize.y = newY;
    }

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
GN_INLINE void GN::gfx::D3DRenderer::resizeDepthBuffer( const Vector2<uint32_t> & sz )
{
    GN_GUARD_SLOW;

    // Check surface size
    if( sz.x > mDepthBufferSize.x || sz.y > mDepthBufferSize.y || NULL == mDepthBuffer )
    {
        uint32_t newX = max( sz.x, mDepthBufferSize.x );
        uint32_t newY = max( sz.y, mDepthBufferSize.y );

        D3DFORMAT newFormat = D3DFMT_D24S8; // TODO: let user specify format.

        const D3DPRESENT_PARAMETERS & d3dpp = getPresentParameters();
        D3DMULTISAMPLE_TYPE msaaType = d3dpp.MultiSampleType;
        DWORD msaaQuality = d3dpp.MultiSampleQuality;

        // check for MASS compability
        if( !checkD3DDeviceMsaa( newFormat ) )
        {
            msaaType = D3DMULTISAMPLE_NONE;
            msaaQuality = 0;
        }
        
        // release old buffer
        safeRelease( mDepthBuffer );

        // create a new buffer
        GN_DX_CHECK_R( mDevice->CreateDepthStencilSurface(
            newX, newY,
            newFormat, // TODO: enumerate avaliable depth format
            msaaType,
            msaaQuality,
            TRUE, // discard
            &mDepthBuffer, 0 ) );

        // bind color and depth buffer
        GN_DX_CHECK( mDevice->SetDepthStencilSurface( mDepthBuffer ) );

        // update buffer size
        mDepthBufferSize.x = newX;
        mDepthBufferSize.y = newY;
    }

    GN_UNGUARD_SLOW;
}

#endif
