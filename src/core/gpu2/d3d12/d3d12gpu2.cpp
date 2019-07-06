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

    // create command list for present
    _present = new D3D12CommandList(*this, {});

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
GN::AutoRef<GN::gfx::Gpu2::MemoryBlock> GN::gfx::D3D12Gpu2::createMemoryBlock(const MemoryBlockCreationParameters & cp)
{
    return SafeNew(new D3D12MemoryBlock(*this, cp));
}

//
//
// -----------------------------------------------------------------------------
GN::AutoRef<GN::gfx::Gpu2::Surface> GN::gfx::D3D12Gpu2::createSurface(const SurfaceCreationParameters & cp)
{
    if (SurfaceDimension::BUFFER == cp.dim) {
        return SafeNew(new D3D12Buffer(*this, cp));
    } else {
        GN_ASSERT(SurfaceDimension::TEXTURE == cp.dim);
        return SafeNew(new D3D12Texture(*this, cp));
    }
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
void GN::gfx::D3D12Gpu2::finish(uint64_t fence)
{
    if (0 == fence) {
        _graphicsQueue.waitForIdle();
    } else {
        _graphicsQueue.finish(fence);
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D12Gpu2::present(const PresentParameters &)
{
    // transit frame buffer to present state.
    auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(_frames[_frameIndex].rt, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    _present->commandList->ResourceBarrier(1, &barrier);
    kickoff(*_present, nullptr);
    _present->reset(0);

    // present the frame buffer
    _swapChain->Present(1, 0);

    // transit frame buffer back to render target state.
    barrier = CD3DX12_RESOURCE_BARRIER::Transition(_frames[_frameIndex].rt, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    _present->commandList->ResourceBarrier(1, &barrier);
    kickoff(*_present, nullptr);
    _present->reset(0);

    // insert a fence to mark the end of current frame.
    _frames[_frameIndex].fence = _graphicsQueue.mark();

    // Update the frame index.
    _frameIndex = _swapChain->GetCurrentBackBufferIndex();

    // Wait for next frame to be ready to render to.
    _graphicsQueue.finish(_frames[_frameIndex].fence);
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D12CommandList::D3D12CommandList(D3D12Gpu2 & gpu, const Gpu2::CommandListCreationParameters & cp)
    : owner(gpu)
{
    ReturnIfFailed(gpu.device().CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&allocator)));
    ReturnIfFailed(gpu.device().CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, allocator, (ID3D12PipelineState*)cp.initialPipelineState, IID_PPV_ARGS(&commandList)));
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D12CommandList::clear(const Gpu2::ClearParameters & p)
{
    commandList->ClearRenderTargetView(owner.backrtv(), p.color, 0, nullptr);
}

//
//
// -----------------------------------------------------------------------------
static D3D12_HEAP_TYPE GetHeapType(Gpu2::MemoryType t)
{
    D3D12_HEAP_TYPE heapType;
    switch(t) {
        case Gpu2::MemoryType::UPLOAD : heapType = D3D12_HEAP_TYPE_UPLOAD; break;
        case Gpu2::MemoryType::READBACK : heapType = D3D12_HEAP_TYPE_READBACK; break;
        default: heapType = D3D12_HEAP_TYPE_DEFAULT; break;
    }
    return heapType;
}

//
//
// -----------------------------------------------------------------------------
static ID3D12Heap * GetD3D12Heap(Gpu2::MemoryBlock * mb)
{
    GN_ASSERT(mb);
    return ((D3D12MemoryBlock*)mb)->heap;
}

//
//
// -----------------------------------------------------------------------------
D3D12MemoryBlock::D3D12MemoryBlock(D3D12Gpu2 & o, const Gpu2::MemoryBlockCreationParameters & cp)
 : owner(o)
{
    D3D12_HEAP_DESC desc = {};
    desc.SizeInBytes = cp.sizeInMB * 1024 * 1024;
    desc.Properties = CD3DX12_HEAP_PROPERTIES(GetHeapType(cp.type));
    ReturnIfFailed(o.device().CreateHeap(&desc, IID_PPV_ARGS(&heap)));
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D12Buffer::D3D12Buffer(D3D12Gpu2 & o, const Gpu2::SurfaceCreationParameters & cp) : D3D12PlacedResource(o)
{
    GN_ASSERT(GN::gfx::Gpu2::SurfaceDimension::BUFFER == cp.dim);
    auto desc = CD3DX12_RESOURCE_DESC::Buffer(cp.b.bytes);
    ReturnIfFailed(o.device().CreatePlacedResource(
            GetD3D12Heap(cp.memory),
            cp.offset,
            &desc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&resource)));
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D12Texture::D3D12Texture(D3D12Gpu2 & o, const Gpu2::SurfaceCreationParameters & cp) : D3D12PlacedResource(o)
{
    GN_ASSERT(GN::gfx::Gpu2::SurfaceDimension::TEXTURE == cp.dim);

    DXGI_FORMAT format = (DXGI_FORMAT)colorFormat2DxgiFormat(cp.t.f);
    if (DXGI_FORMAT_UNKNOWN == format) {
        GN_ERROR(sLogger)("Invalid/Unsupported texture format.");
        return;
    }

    D3D12_RESOURCE_DESC desc;
    if (1 == cp.t.d && 1 == cp.t.h) {
        desc = CD3DX12_RESOURCE_DESC::Tex1D(
            format,
            cp.t.w,
            (uint16_t)cp.t.a,
            (uint16_t)cp.t.m
        );
    }
    else if (1 == cp.t.d) {
        desc = CD3DX12_RESOURCE_DESC::Tex2D(
            format,
            cp.t.w,
            cp.t.h,
            (uint16_t)cp.t.a,
            (uint16_t)cp.t.m,
            cp.t.s
        );
    }
    else if (1 == cp.t.a) {
        desc = CD3DX12_RESOURCE_DESC::Tex3D(
            format,
            cp.t.w,
            cp.t.h,
            (uint16_t)cp.t.d,
            (uint16_t)cp.t.m
        );
    } else {
        GN_ERROR(sLogger)("D3D12 does not support 3D array texture.");
        return;
    }

    ReturnIfFailed(o.device().CreatePlacedResource(
        GetD3D12Heap(cp.memory),
        cp.offset,
        &desc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&resource)));
}
