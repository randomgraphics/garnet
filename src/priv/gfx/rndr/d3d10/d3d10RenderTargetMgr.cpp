#include "pch.h"
#include "d3d10Renderer.h"
#include "d3d10RenderTargetMgr.h"
#include "d3d10Texture.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.D3D10");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D10RTMgr::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3D10RTMgr, () );

    ID3D10Device & dev = mRenderer.getDeviceRefInlined();

    // create default rener target view
    AutoComPtr<ID3D10Texture2D> backBuffer;
    GN_DX10_CHECK_RV( mRenderer.getSwapChainRef().GetBuffer( 0, __uuidof( ID3D10Texture2D ), (void**)&backBuffer ), failure() );
    GN_DX10_CHECK_RV( dev.CreateRenderTargetView( backBuffer, NULL, &mAutoColor0 ), failure() );
    GN_ASSERT( mAutoColor0 );

    // create depth texture
    const DispDesc & dd = mRenderer.getDispDesc();
    D3D10_TEXTURE2D_DESC td;
    td.Width              = dd.width;
    td.Height             = dd.height;
    td.MipLevels          = 1;
    td.ArraySize          = 1;
    td.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
    td.SampleDesc.Count   = 1;
    td.SampleDesc.Quality = 0;
    td.Usage              = D3D10_USAGE_DEFAULT;
    td.BindFlags          = D3D10_BIND_DEPTH_STENCIL;
    td.CPUAccessFlags     = 0;
    td.MiscFlags          = 0;
    GN_DX10_CHECK_RV( dev.CreateTexture2D( &td, NULL, &mAutoDepthTexture ), failure() );

    // create depth stencil view
    D3D10_DEPTH_STENCIL_VIEW_DESC dsvd;
    dsvd.Format             = td.Format;
    dsvd.ViewDimension      = D3D10_DSV_DIMENSION_TEXTURE2D;
    dsvd.Texture2D.MipSlice = 0;
    GN_DX10_CHECK_RV( dev.CreateDepthStencilView( mAutoDepthTexture, &dsvd, &mAutoDepth ), failure() );

    // bind these views to device.
    dev.OMSetRenderTargets( 1, &mAutoColor0, mAutoDepth );

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10RTMgr::quit()
{
    GN_GUARD;

    safeRelease( mAutoColor0 );
    safeRelease( mAutoDepthTexture );
    safeRelease( mAutoDepth );

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D10RTMgr::bind(
    const RenderTargetTexture   newColorRenderTargets[],
    const RenderTargetTexture & newDepthStencilRenderTarget,
    const RenderTargetTexture   oldColorRenderTargets[],
    const RenderTargetTexture & oldDepthStencilRenderTarget,
    bool                        skipDirtyCheck,
    bool                      & needRebindViewport )
{
    // check for redundant render target settings.
    if( !skipDirtyCheck )
    {
        bool differ = false;

        // compare color render targets
        for( size_t i = 0; i < RendererContext::MAX_COLOR_RENDER_TARGETS; ++i )
        {
            if( newColorRenderTargets[i] != oldColorRenderTargets[i] )
            {
                differ = true;
                break;
            }
        }

        // compare depth render target
        differ |= newDepthStencilRenderTarget != oldDepthStencilRenderTarget;

        // return if old and new render target settings are identical
        if( !differ )
        {
            needRebindViewport = false;
            return true;
        }
    }

    // compose RTV array
    mNumColors = 0;
    for( size_t i = 0; i < RendererContext::MAX_COLOR_RENDER_TARGETS; ++i )
    {
        const RenderTargetTexture & rtt = newColorRenderTargets[i];

        D3D10Texture * tex = (D3D10Texture*)rtt.texture.get();

        if( tex )
        {
            mColors[i] = tex->getRTView( rtt.face, rtt.level, rtt.slice );
            if( NULL == mColors[i] )
            {
                return false;
            }

            ++mNumColors;
        }
        else if( 0 == i )
        {
            GN_ASSERT( mAutoColor0 );
            mColors[i] = mAutoColor0;

            ++mNumColors;
        }
        else
        {
            // stop after meeting the first NULL render target (except render target 0)
            break;
        }
    }
    // pad RTV array with NULLs
    for( size_t i = mNumColors; i < RendererContext::MAX_COLOR_RENDER_TARGETS; ++i )
    {
        mColors[i] = NULL;
    }

    // Get RSV pointer
    D3D10Texture * dstex = (D3D10Texture*)newDepthStencilRenderTarget.texture.get();
    if( dstex )
    {
        mDepth = dstex->getDSView(
            newDepthStencilRenderTarget.face,
            newDepthStencilRenderTarget.level,
            newDepthStencilRenderTarget.slice );
        if( NULL == mDepth )
        {
            return false;
        }
    }
    else
    {
        GN_ASSERT( mAutoDepth );
        mDepth = mAutoDepth;
    }

    // bind to D3D device
    mRenderer.getDeviceRefInlined().OMSetRenderTargets(
        (UINT)RendererContext::MAX_COLOR_RENDER_TARGETS,
        mColors,
        mDepth );

    // update mRenderTargetSize, according to render target 0 size
    Vector2<UInt32> newRtSize;
    if( newColorRenderTargets[0].texture )
    {
        const Vector3<UInt32> size = newColorRenderTargets[0].texture->getBaseSize();
        newRtSize.x = size.x;
        newRtSize.y = size.y;
    }
    else
    {
        const DispDesc & dd = mRenderer.getDispDesc();
        newRtSize.x = dd.width;
        newRtSize.y = dd.height;
    }

    // need to rebind viewport when render target size is changed.
    needRebindViewport = ( newRtSize != mRenderTargetSize );

    // store new render target size
    mRenderTargetSize = newRtSize;

    // done
    return true;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------

