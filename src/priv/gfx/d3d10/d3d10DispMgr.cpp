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
    ::memset( &sd, 0, sizeof(sd) );
#if D3D10_SDK_VERSION >= 28
    sd.BufferCount = 1;
    sd.BufferDesc.Width = dd.width;
    sd.BufferDesc.Height = dd.height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
#else
    sd.BackBufferCount = 1;
    sd.BackBufferDesc.Width = dd.width;
    sd.BackBufferDesc.Height = dd.height;
    sd.BackBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BackBufferDesc.RefreshRate.Numerator = 60;
    sd.BackBufferDesc.RefreshRate.Denominator = 1;
    sd.BackBufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
#endif
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
#if D3D10_SDK_VERSION >= 28
    GN_D3D10_CHECK_RV( mSwapChain->GetBuffer( 0, __uuidof( ID3D10Texture2D ), (void**)&backBuffer ), false );
#else
    GN_D3D10_CHECK_RV( mSwapChain->GetBackBuffer( 0, __uuidof( ID3D10Texture2D ), (void**)&backBuffer ), false );
#endif
    GN_D3D10_CHECK_RV( mDevice->CreateRenderTargetView( backBuffer, NULL, &mRTView ), false );
    mDevice->OMSetRenderTargets( 1, &mRTView, NULL );

    // setup viewport
    D3D10_VIEWPORT vp;
#if D3D10_SDK_VERSION >= 28
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    vp.Width = dd.width;
    vp.Height = dd.height;
#else
    vp.TopLeftX = 0.0f;
    vp.TopLeftY = 0.0f;
    vp.Width = (float)dd.width;
    vp.Height = (float)dd.height;
#endif
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
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
