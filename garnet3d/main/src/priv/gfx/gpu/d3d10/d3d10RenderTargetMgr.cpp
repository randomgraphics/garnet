#include "pch.h"
#include "d3d10Gpu.h"
#include "d3d10RenderTargetMgr.h"
#include "d3d10Texture.h"

static GN::Logger * sLogger = GN::GetLogger("GN.gfx.gpu.D3D10");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D10RTMgr::Init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::D3D10RTMgr, () );

    ID3D10Device & dev = mGpu.getDeviceRefInlined();

    // create default rener target view
    AutoComPtr<ID3D10Texture2D> backBuffer;
    GN_DX_CHECK_RETURN( mGpu.getSwapChainRef().GetBuffer( 0, __uuidof( ID3D10Texture2D ), (void**)&backBuffer ), Failure() );
    GN_DX_CHECK_RETURN( dev.CreateRenderTargetView( backBuffer, NULL, &mAutoColor0 ), Failure() );
    GN_ASSERT( mAutoColor0 );

    // create depth texture
    const DispDesc & dd = mGpu.getDispDesc();
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
    GN_DX_CHECK_RETURN( dev.CreateTexture2D( &td, NULL, &mAutoDepthTexture ), Failure() );

    // create depth stencil view
    D3D10_DEPTH_STENCIL_VIEW_DESC dsvd;
    dsvd.Format             = td.Format;
    dsvd.ViewDimension      = D3D10_DSV_DIMENSION_TEXTURE2D;
    dsvd.Texture2D.MipSlice = 0;
    GN_DX_CHECK_RETURN( dev.CreateDepthStencilView( mAutoDepthTexture, &dsvd, &mAutoDepth ), Failure() );

    // bind these views to device.
    dev.OMSetRenderTargets( 1, &mAutoColor0, mAutoDepth );

    // success
    return Success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10RTMgr::Quit()
{
    GN_GUARD;

    SafeRelease( mAutoColor0 );
    SafeRelease( mAutoDepthTexture );
    SafeRelease( mAutoDepth );

    // standard Quit procedure
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
    const RenderTargetDesc & oldrt,
    const RenderTargetDesc & newrt,
    bool                     skipDirtyCheck,
    bool                   & renderTargetSizeChanged )
{
    renderTargetSizeChanged = false;

    // make new render target description is valid.
    GN_ASSERT( oldrt.valid() );
    if( !newrt.valid() ) return false;

    // check for redundancy
    if( !skipDirtyCheck && oldrt == newrt )
    {
        return true;
    }

    if( 0 == newrt.colortargets.Size() && 0 == newrt.depthstencil.texture )
    {
        // separate code path for rendering to back buffer
        memset( mColors, 0, sizeof(mColors) );
        mColors[0] = mAutoColor0;
        mDepth     = mAutoDepth;
        mNumColors = 1;
    }
    else
    {
        // build RTV array
        for( size_t i = 0; i < newrt.colortargets.Size(); ++i )
        {
            const RenderTargetTexture & rtt = newrt.colortargets[i];

            D3D10Texture * tex = (D3D10Texture*)rtt.texture.Get();

            GN_ASSERT( tex );

            mColors[i] = tex->getRTView( rtt.face, rtt.level, rtt.slice );

            if( NULL == mColors[i] )
            {
                return false;
            }
        }
        // fill remained items in RTV array with NULLs
        for( size_t i = newrt.colortargets.Size(); i < GpuContext::MAX_COLOR_RENDER_TARGETS; ++i )
        {
            mColors[i] = NULL;
        }

        mNumColors = newrt.colortargets.Size();

        // Get depth stencil view
        D3D10Texture * dstex = (D3D10Texture*)newrt.depthstencil.texture.Get();
        if( dstex )
        {
            mDepth = dstex->getDSView(
                newrt.depthstencil.face,
                newrt.depthstencil.level,
                newrt.depthstencil.slice );
            if( NULL == mDepth )
            {
                return false;
            }
        }
        else
        {
            mDepth = NULL;
        }
    }

    // bind to D3D device
    mGpu.getDeviceRefInlined().OMSetRenderTargets(
        (UINT)GpuContext::MAX_COLOR_RENDER_TARGETS,
        mColors,
        mDepth );

    // update mRenderTargetSize
    Vector2<UInt32> newRtSize;
    if( newrt.colortargets.Size() > 0 )
    {
        newrt.colortargets[0].texture->getMipSize( newrt.colortargets[0].level, &newRtSize.x, &newRtSize.y );
    }
    else if( newrt.depthstencil.texture )
    {
        newrt.depthstencil.texture->getMipSize( newrt.depthstencil.level, &newRtSize.x, &newRtSize.y );
    }
    else
    {
        const DispDesc & dd = mGpu.getDispDesc();
        newRtSize.x = dd.width;
        newRtSize.y = dd.height;
    }

    // need to rebind viewport when render target size is changed.
    renderTargetSizeChanged = ( newRtSize != mRenderTargetSize );

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

