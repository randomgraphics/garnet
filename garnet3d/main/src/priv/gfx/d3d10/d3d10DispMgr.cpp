#include "pch.h"
#include "d3d10Renderer.h"

// *****************************************************************************
// device management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D10Renderer::dispDeviceCreate()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    const RendererOptions & ro = getOptions();
    const DispDesc & dd = getDispDesc();

    UINT flags = 0;
#if GN_DEBUG_BUILD
    flags |= D3D10_CREATE_DEVICE_DEBUG;
#endif
    if( !ro.multithread ) flags |= D3D10_CREATE_DEVICE_SINGLETHREADED;

    // setup swap chain descriptor
    DXGI_SWAP_CHAIN_DESC sd;
    ::memset( &sd, sizeof(sd), 0 );
    sd.BackBufferCount = 1;
    sd.BackBufferDesc.Width = dd.width;
    sd.BackBufferDesc.Height = dd.height;
    sd.BackBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BackBufferDesc.RefreshRate.Numerator = 60;
    sd.BackBufferDesc.RefreshRate.Denominator = 1;
    sd.BackBufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = (HWND)dd.windowHandle;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = !ro.fullscreen;

    // create device
    GN_D3D10_CHECK_RV(
        D3D10CreateDeviceAndSwapChain(
            mAdapter,
            ro.reference ? D3D10_DRIVER_TYPE_REFERENCE : D3D10_DRIVER_TYPE_HARDWARE,
            NULL, // software module handle
            flags,
            D3D10_SDK_VERSION,
            &sd,
            &mSwapChain,
            &mDevice ),
        false );

    // initialize render target view
    AutoComPtr<ID3D10Texture2D> backBuffer;
    GN_D3D10_CHECK_RV( mSwapChain->GetBackBuffer( 0, __uuidof( ID3D10Texture2D ), (void**)&backBuffer ), false );
    GN_D3D10_CHECK_RV( mDevice->CreateRenderTargetView( backBuffer, NULL, &mRTView ), false );
    mDevice->OMSetRenderTargets( 1, &mRTView, NULL );

    // setup viewport
    D3D10_VIEWPORT vp;
    vp.Width = (float)dd.width;
    vp.Height = (float)dd.height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0.0f;
    vp.TopLeftY = 0.0f;
    mDevice->RSSetViewports( 1, &vp );

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D10Renderer::dispDeviceDestroy()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    safeRelease( mRTView );
    safeRelease( mSwapChain );
    safeRelease( mDevice );
    safeRelease( mAdapter );

    GN_UNGUARD;
}
