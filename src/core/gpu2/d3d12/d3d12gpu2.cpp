#include "pch.h"
#include "d3d12gpu2.h"

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D12Gpu2::D3D12Gpu2(const CreationParameters & cp)
{
    // enable debug layer
    if (cp.debug) {
        AutoComPtr<ID3D12Debug> debug;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug)))) {
            debug->EnableDebugLayer();
        }
    }

    AutoComPtr<IDXGIFactory4> factory;
    ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&factory)));

    AutoComPtr<IDXGIAdapter1> hardwareAdapter;
    //TODO: GetHardwareAdapter(factory, &hardwareAdapter);

    ThrowIfFailed(D3D12CreateDevice(
        hardwareAdapter,
        D3D_FEATURE_LEVEL_12_0,
        IID_PPV_ARGS(&_device)
        ));

    // create command queue
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    ThrowIfFailed(_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&_commandQueue)));

    // get current window size
    auto windowSize = cp.window->getClientSize();

    // create swap chain.
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = BACK_BUFFER_COUNT;
    swapChainDesc.BufferDesc.Width = 0 == cp.width ? windowSize.x : cp.width;
    swapChainDesc.BufferDesc.Height = cp.height ? windowSize.y : cp.height;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.OutputWindow = (HWND)cp.window->getWindowHandle();
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.Windowed = TRUE;
    AutoComPtr<IDXGISwapChain> swapChain;
    ThrowIfFailed(factory->CreateSwapChain(
        _commandQueue, // Swap chain needs the queue so that it can force a flush on it.
        &swapChainDesc,
        &swapChain));
    ThrowIfFailed(swapChain.as(&_swapChain));
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D12Gpu2::present(const PresentParameters &)
{
}