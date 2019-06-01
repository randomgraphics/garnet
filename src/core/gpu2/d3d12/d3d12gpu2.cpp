#include "pch.h"
#include "d3d12gpu2.h"

using namespace GN;
using namespace GN::gfx;

static auto sLogger = GN::getLogger("GN.gfx.gpu2.D3D12");

// Helper function for acquiring the first available hardware adapter that supports Direct3D 12.
// If no such adapter can be found, *ppAdapter will be set to nullptr.
// -----------------------------------------------------------------------------
static void GetHardwareAdapter(IDXGIFactory2* pFactory, IDXGIAdapter1** ppAdapter)
{
    AutoComPtr<IDXGIAdapter1> adapter;
    *ppAdapter = nullptr;

    for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters1(adapterIndex, &adapter); ++adapterIndex)
    {
        DXGI_ADAPTER_DESC1 desc;
        adapter->GetDesc1(&desc);

        if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
        {
            continue;
        }

        // Check to see if the adapter supports Direct3D 12, but don't create the
        // actual device yet.
        if (SUCCEEDED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_0, _uuidof(ID3D12Device), nullptr)))
        {
            break;
        }
    }

    *ppAdapter = adapter.detach();
}

template<typename T>
static AutoRef<T> SafeNew(T * p)
{
    if (p && !p->ok()) delete p, p = nullptr;
    return AutoRef<T>(p);
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D12Gpu2::D3D12Gpu2(const CreationParameters & cp)
{
    GN_VERIFY(cp.window);

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
    GetHardwareAdapter(factory, &hardwareAdapter);

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
    _frameIndex = _swapChain->GetCurrentBackBufferIndex();

    // disable alt-enter transition
    ThrowIfFailed(factory->MakeWindowAssociation((HWND)cp.window->getWindowHandle(), DXGI_MWA_NO_ALT_ENTER));

    // Create RTV descriptor heaps.
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = BACK_BUFFER_COUNT;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    ThrowIfFailed(_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&_rtvHeap)));
    _rtvDescriptorSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    // Create a RTV for each frame.
    {
        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(_rtvHeap->GetCPUDescriptorHandleForHeapStart());
        for (UINT n = 0; n < BACK_BUFFER_COUNT; n++)
        {
            ThrowIfFailed(_swapChain->GetBuffer(n, IID_PPV_ARGS(&_renderTargets[n])));
            _device->CreateRenderTargetView(_renderTargets[n], nullptr, rtvHandle);
            rtvHandle.Offset(1, _rtvDescriptorSize);
        }
    }
}

//
//
// -----------------------------------------------------------------------------
GN::AutoRef<GN::gfx::Gpu2::CommandList> GN::gfx::D3D12Gpu2::createCommandList(const CommandListCreationParameters & cp)
{
     return SafeNew(new D3D12CommandList(*this, cp));
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D12Gpu2::kickoff(GN::gfx::Gpu2::CommandList & cl)
{
    ID3D12GraphicsCommandList * gfxcl = ((D3D12CommandList*)&cl)->commandList;
    gfxcl->Close();
    ID3D12CommandList * d3dcl = gfxcl;
    _commandQueue->ExecuteCommandLists(1, &d3dcl);
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D12Gpu2::present(const PresentParameters &)
{
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D12CommandList::D3D12CommandList(D3D12Gpu2 & gpu, const Gpu2::CommandListCreationParameters & cp)
    : owner(gpu)
{
    ReturnIfFailed(gpu.device().CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&allocator)));
    ReturnIfFailed(gpu.device().CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, allocator, (ID3D12PipelineState*)cp.initialState, IID_PPV_ARGS(&commandList)));
}
