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
    UINT dxgiflags = 0;
    if (cp.debug) {
        AutoComPtr<ID3D12Debug> debug;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug)))) {
            debug->EnableDebugLayer();
            dxgiflags |= DXGI_CREATE_FACTORY_DEBUG;
        }
    }

    AutoComPtr<IDXGIFactory4> factory;
    ThrowIfFailed(CreateDXGIFactory2(dxgiflags, IID_PPV_ARGS(&factory)));

    AutoComPtr<IDXGIAdapter1> hardwareAdapter;
    GetHardwareAdapter(factory, &hardwareAdapter);

    ThrowIfFailed(D3D12CreateDevice(
        hardwareAdapter,
        D3D_FEATURE_LEVEL_12_0,
        IID_PPV_ARGS(&_device)
        ));

    // create command queue
    _graphicsQueue.init(*_device);

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
        &_graphicsQueue.q(), // Swap chain needs the queue so that it can force a flush on it.
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
    auto rtvDescriptorSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    // Initialize frame buffers
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(_rtvHeap->GetCPUDescriptorHandleForHeapStart());
    for (UINT n = 0; n < BACK_BUFFER_COUNT; n++)
    {
        ThrowIfFailed(_swapChain->GetBuffer(n, IID_PPV_ARGS(&_frames[n].rt)));
        _frames[n].rtv = rtvHandle;
        _device->CreateRenderTargetView(_frames[n].rt, nullptr, _frames[n].rtv);
        rtvHandle.Offset(rtvDescriptorSize);
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
void GN::gfx::D3D12Gpu2::kickoff(GN::gfx::Gpu2::CommandList & cl, uint64_t * fence)
{
    auto ptr = (D3D12CommandList*)&cl;
    ptr->commandList->Close();
    ID3D12CommandList * d3dcl = ptr->commandList;
    _graphicsQueue.q().ExecuteCommandLists(1, &d3dcl);
    if (fence) *fence = _graphicsQueue.mark();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D12Gpu2::wait(uint64_t fence)
{
    _graphicsQueue.wait(fence);
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D12Gpu2::present(const PresentParameters &)
{
    _swapChain->Present(1, 0);

    // insert a fence to mark the end of current frame.
    _frames[_frameIndex].fence = _graphicsQueue.mark();

    // Update the frame index.
    _frameIndex = _swapChain->GetCurrentBackBufferIndex();

    // Wait for next frame to be ready to render to.
    _graphicsQueue.wait(_frames[_frameIndex].fence);
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

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D12CommandList::clear(const Gpu2::ClearParameters & p)
{
    commandList->ClearRenderTargetView(owner.backrtv(), p.color, 0, nullptr);
}
