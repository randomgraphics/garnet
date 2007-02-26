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
    GN_CASSERT( D3D10_SDK_VERSION >= 28 );
    DXGI_SWAP_CHAIN_DESC sd;
    ::memset( &sd, 0, sizeof(sd) );
    sd.BufferCount = 1;
    sd.BufferDesc.Width = dd.width;
    sd.BufferDesc.Height = dd.height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = (HWND)dd.windowHandle;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = !ro.fullscreen;

    // create device
    GN_DX10_CHECK_RV(
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

    // setup default viewport
    D3D10_VIEWPORT vp;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    vp.Width = dd.width;
    vp.Height = dd.height;
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

    safeRelease( mSwapChain );
    safeRelease( mDevice );
    safeRelease( mAdapter );

    GN_UNGUARD;
}
