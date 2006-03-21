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
#if GN_DEBUG
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
