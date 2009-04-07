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
void GN::gfx::D3D10RTMgr::bind(
    const RenderTargetTexture   /*newColorRenderTargets*/[],
    const RenderTargetTexture & /*newDepthStencilRenderTarget*/,
    const RenderTargetTexture   /*oldColorRenderTargets*/[],
    const RenderTargetTexture & /*oldDepthStencilRenderTarget*/,
    bool                        /*skipDirtyCheck*/,
    bool                      & needRebindViewport )
{
    GN_UNIMPL_WARNING();
    needRebindViewport = true;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------

