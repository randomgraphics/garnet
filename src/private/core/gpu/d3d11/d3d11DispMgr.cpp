#include "pch.h"
#include "d3d11Gpu.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.D3D11");

// *****************************************************************************
// device management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::D3D11Gpu::dispInit()
{
    GN_GUARD;

    const GpuOptions & ro = getOptions();
    const DispDesc & dd = getDispDesc();

    // setup swap chain descriptor
    GN_CASSERT( D3D11_SDK_VERSION >= 7 );
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
    sd.Windowed = ro.displayMode.mode != DisplayMode::FULL_SCREEN;

    // Get the highest feature level
    D3D_FEATURE_LEVEL featureLevel;
    GN_RETURN_FALSE_ON_HR_FAILED(D3D11CreateDevice(
        NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, NULL, 0,
        D3D11_SDK_VERSION, NULL, &featureLevel, NULL));

    // create device
    UINT flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
    if( ro.debug ) flags |= D3D11_CREATE_DEVICE_DEBUG;
    HRESULT firstTry = D3D11CreateDeviceAndSwapChain(
            mAdapter,
            ro.reference ? D3D_DRIVER_TYPE_REFERENCE : D3D_DRIVER_TYPE_HARDWARE,
            NULL, // software module handle
            flags,
            &featureLevel,
            1,
            D3D11_SDK_VERSION,
            &sd,
            &mSwapChain,
            &mDevice,
            NULL, // feature level
            &mDeviceContext );
    if (FAILED(firstTry) && ro.debug)
    {
        // remove debug flag and try again
        flags &= ~D3D11_CREATE_DEVICE_DEBUG;
        GN_RETURN_FALSE_ON_HR_FAILED( D3D11CreateDeviceAndSwapChain(
            mAdapter,
            ro.reference ? D3D_DRIVER_TYPE_REFERENCE : D3D_DRIVER_TYPE_HARDWARE,
            NULL, // software module handle
            flags,
            &featureLevel,
            1,
            D3D11_SDK_VERSION,
            &sd,
            &mSwapChain,
            &mDevice,
            NULL, // feature level
            &mDeviceContext ) );
        GN_WARN(sLogger)("D3D device created w/o debug layer.");
    }

    mDevice->QueryInterface( IID_ID3D11Debug, (void**)&mD3D11Debug );

    // customize D3D11 debug output
    AutoComPtr<ID3D11InfoQueue> iq;
    if( S_OK == mDevice->QueryInterface( IID_ID3D11InfoQueue, (void**)&iq ) )
    {
        // Break into debugger on D3D errors
		iq->SetBreakOnSeverity( D3D11_MESSAGE_SEVERITY_CORRUPTION, true );
		iq->SetBreakOnSeverity( D3D11_MESSAGE_SEVERITY_ERROR, true );
		iq->SetBreakOnSeverity( D3D11_MESSAGE_SEVERITY_WARNING, false );
		iq->SetBreakOnSeverity( D3D11_MESSAGE_SEVERITY_INFO, false );

        // ignore some expected errors
        D3D11_MESSAGE_ID disabledWarnings[] =
        {
            D3D11_MESSAGE_ID_DEVICE_OMSETRENDERTARGETS_HAZARD,
            D3D11_MESSAGE_ID_DEVICE_PSSETSHADERRESOURCES_HAZARD,
        };
        D3D11_INFO_QUEUE_FILTER filter;
        memset( &filter, 0, sizeof(filter) );
        filter.DenyList.NumIDs  = GN_ARRAY_COUNT( disabledWarnings );
        filter.DenyList.pIDList = disabledWarnings;
        iq->AddStorageFilterEntries( &filter );
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D11Gpu::dispQuit()
{
    GN_GUARD;

    // switch back to windowed mode, before destroy the swap chain.
    if( mSwapChain && getOptions().displayMode.mode == DisplayMode::FULL_SCREEN )
    {
        mSwapChain->SetFullscreenState( FALSE, NULL );
    }

    safeRelease( mD3D11Debug );
    safeRelease( mDeviceContext );
    safeRelease( mSwapChain );
    safeRelease( mDevice );
    safeRelease( mAdapter );

    GN_UNGUARD;
}
