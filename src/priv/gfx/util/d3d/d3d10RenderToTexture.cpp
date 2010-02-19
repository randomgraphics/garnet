#include "pch.h"

static GN::Logger * sLogger = GN::GetLogger("GN.d3d10.RenderToTexture");

// *****************************************************************************
// local functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::d3d10::RenderToTexture::Init(
    ID3D10Device * device, const RenderToTextureOption & options )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::d3d10::RenderToTexture, () );

    if( options.count > 8 )
    {
        GN_ERROR(sLogger)( "can not support more then 8 render targets." );
        return Failure();
    }

    D3D10_TEXTURE2D_DESC texdesc = {
        options.width,
        options.height,
        1,
        1,
        options.format,
        constructDXGISampleDesc( *device, options.msaa, options.format ),
        D3D10_USAGE_DEFAULT,
        D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE,
        0, // CPU access flags
        0, // misc flags
    };

    // create color buffers
    mColors.resize( options.count );
    for( size_t i = 0; i < options.count; ++i )
    {
        RenderTargetTexture & rtt = mColors[i];

        // create texture
        ID3D10Texture2D * tex2d;
        GN_DX_CHECK_RETURN( device->CreateTexture2D( &texdesc, NULL, &tex2d ), Failure() );
        rtt.res = tex2d;

        // create RTV
        D3D10_RENDER_TARGET_VIEW_DESC rtvdesc = {
            options.format,
            options.msaa ? D3D10_RTV_DIMENSION_TEXTURE2DMS : D3D10_RTV_DIMENSION_TEXTURE2D,
        };
        rtvdesc.Texture2D.MipSlice = 0;
        GN_DX_CHECK_RETURN( device->CreateRenderTargetView( tex2d, &rtvdesc, &rtt.rtv ), Failure() );

        // create SRV
        D3D10_SHADER_RESOURCE_VIEW_DESC srvdesc = {
            options.format,
            options.msaa ? D3D10_SRV_DIMENSION_TEXTURE2DMS : D3D10_SRV_DIMENSION_TEXTURE2D,
        };
        srvdesc.Texture2D.MostDetailedMip = 0;
        srvdesc.Texture2D.MipLevels = 1;
        GN_DX_CHECK_RETURN( device->CreateShaderResourceView( tex2d, &srvdesc, &rtt.srv ), Failure() );

        mColorViews[i] = rtt.rtv;
    }

    // create depth buffer
    if( options.msaa )
    {
        texdesc.Format = DXGI_FORMAT_R32_TYPELESS;
        texdesc.BindFlags = D3D10_BIND_DEPTH_STENCIL;

        ID3D10Texture2D * tex2d;
        GN_DX_CHECK_RETURN( device->CreateTexture2D( &texdesc, NULL, &tex2d ), Failure() );
        mDepth.res = tex2d;

        // create DSV
        D3D10_DEPTH_STENCIL_VIEW_DESC dsvdesc = {
            DXGI_FORMAT_D32_FLOAT,
            D3D10_DSV_DIMENSION_TEXTURE2DMS,
        };
        GN_DX_CHECK_RETURN( device->CreateDepthStencilView( tex2d, &dsvdesc, &mDepth.dsv ), Failure() );
    }
    else
    {
        texdesc.Format = DXGI_FORMAT_R32_TYPELESS;
        texdesc.BindFlags = D3D10_BIND_DEPTH_STENCIL | D3D10_BIND_SHADER_RESOURCE;

        ID3D10Texture2D * tex2d;
        GN_DX_CHECK_RETURN( device->CreateTexture2D( &texdesc, NULL, &tex2d ), Failure() );
        mDepth.res = tex2d;

        // create DSV
        D3D10_DEPTH_STENCIL_VIEW_DESC dsvdesc = {
            DXGI_FORMAT_D32_FLOAT,
            D3D10_DSV_DIMENSION_TEXTURE2D,
        };
        dsvdesc.Texture2D.MipSlice = 0;
        GN_DX_CHECK_RETURN( device->CreateDepthStencilView( tex2d, &dsvdesc, &mDepth.dsv ), Failure() );

        // create SRV
        D3D10_SHADER_RESOURCE_VIEW_DESC srvdesc = {
            DXGI_FORMAT_R32_FLOAT,
            D3D10_SRV_DIMENSION_TEXTURE2D,
        };
        srvdesc.Texture2D.MostDetailedMip = 0;
        srvdesc.Texture2D.MipLevels = 1;
        GN_DX_CHECK_RETURN( device->CreateShaderResourceView( tex2d, &srvdesc, &mDepth.srv ), Failure() );
    }

    // success
    mDevice = device;
    return Success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::d3d10::RenderToTexture::Quit()
{
    GN_GUARD;

    for( size_t i = 0; i < mColors.size(); ++i )
    {
        mColors[i].Clear();
    }
    mColors.Clear();
    mDepth.Clear();

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
void GN::d3d10::RenderToTexture::clearScreen( float r, float g, float b, float a, float d, UInt8 s )
{
    float color[] = { r, g, b, a };
    for( size_t i = 0; i < mColors.size(); ++i )
    {
        mDevice->ClearRenderTargetView( mColors[i].rtv, color );
    }
    mDevice->ClearDepthStencilView( mDepth.dsv, D3D10_CLEAR_DEPTH, d, s );
}
