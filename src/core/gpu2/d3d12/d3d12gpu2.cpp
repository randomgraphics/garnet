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

//
//
// -----------------------------------------------------------------------------
template<typename T>
static AutoRef<T> SafeNew(T * p)
{
    if (p && !p->ok()) delete p, p = nullptr;
    return AutoRef<T>(p);
}

//
//
// -----------------------------------------------------------------------------
static ID3D12Resource * promote(const Gpu2::Surface * p)
{
    if (!p) return nullptr;
    auto r = (const D3D12PlacedResource *)p;
    return r->resource.get();
}

//
//
// -----------------------------------------------------------------------------
static D3D12_PRIMITIVE_TOPOLOGY tod3d(PrimitiveType p)
{
    static const D3D12_PRIMITIVE_TOPOLOGY table[] = {
        D3D_PRIMITIVE_TOPOLOGY_POINTLIST, //    POINT_LIST,     ///< point list
        D3D_PRIMITIVE_TOPOLOGY_LINELIST, //    LINE_LIST,      ///< line list
        D3D_PRIMITIVE_TOPOLOGY_LINESTRIP, //    LINE_STRIP,     ///< line strip
        D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, //    TRIANGLE_LIST,  ///< triangle list
        D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, //    TR;IANGLE_STRIP, ///< triangle strip
    };
    if (0 <= p && p < GN_ARRAY_COUNT(table))
        return table[p];
    else {
        GN_ERROR(sLogger)("invalid primitive type: %d", p);
        return D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
    }
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
            auto debug1 = debug.as<ID3D12Debug1>();
            debug1->SetEnableGPUBasedValidation(true);
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
    swapChainDesc.BufferDesc.Width = cp.width ? cp.width : windowSize.x;
    swapChainDesc.BufferDesc.Height = cp.height ? cp.height: windowSize.y;
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
    _frameWidth = swapChainDesc.BufferDesc.Width;
    _frameHeight = swapChainDesc.BufferDesc.Height;

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

    // put the back buffer into RT state
    auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(_frames[_frameIndex].rt, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_RENDER_TARGET);
    _present->commandList->ResourceBarrier(1, &barrier);
    kickoff(*_present, nullptr);
    _present->reset(0);

    // Create an empty root signature.
    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
    rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
    AutoComPtr<ID3DBlob> signature;
    AutoComPtr<ID3DBlob> error;
    ThrowIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error));
    ThrowIfFailed(_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&_emptyRootSignature)));
}

//
//
// -----------------------------------------------------------------------------
static std::vector<D3D12_INPUT_ELEMENT_DESC> GetD3D12InputElements(const Gpu2::InputElement * elements, uint32_t count)
{
    std::vector<D3D12_INPUT_ELEMENT_DESC> result;

    for(size_t i = 0; i < count; ++i) {
        const auto & e = elements[i];
        result.push_back({
            e.semantic,
            e.index,
            (DXGI_FORMAT)colorFormat2DxgiFormat(e.format),
            e.slot,
            e.offset,
            e.instanceData ? D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA : D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
            e.instanceRate
        });
    }

    return result;
}

//
//
// -----------------------------------------------------------------------------
DynaArray<uint64_t> GN::gfx::D3D12Gpu2::createPipelineStates(const PipelineCreationParameters * parameters, size_t n)
{
    DynaArray<uint64_t> r;
    for(size_t i = 0; i < n; ++i) {
        const auto & cp = parameters[i];
        AutoComPtr<ID3D12PipelineState> pso;
        D3D12_GRAPHICS_PIPELINE_STATE_DESC d = {};
        d.pRootSignature = _emptyRootSignature;
        d.VS.pShaderBytecode = cp.vs.ptr;
        d.VS.BytecodeLength = cp.vs.sizeInBytes;
        d.PS.pShaderBytecode = cp.ps.ptr;
        d.PS.BytecodeLength = cp.ps.sizeInBytes;
        
        d.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

        d.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
        d.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

        d.SampleMask = UINT_MAX;

        auto elements = GetD3D12InputElements(cp.inputElements, cp.numInputElements);
        d.InputLayout.NumElements = cp.numInputElements;
        d.InputLayout.pInputElementDescs = elements.data();
        d.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

        d.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
        d.NumRenderTargets = 1;
        d.SampleDesc.Count = 1;
        
        ThrowIfFailed(_device->CreateGraphicsPipelineState(&d, IID_PPV_ARGS(&pso)));
        r.append((uint64_t)(intptr_t)pso.detach());
    }
    return r;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D12Gpu2::deletePipelineStates(const uint64_t * psolist, size_t count)
{
    for (size_t i = 0; i < count; ++i) {
        auto pso = psolist[i];
        if (pso) {
            auto d3dpso = (ID3D12PipelineState *)pso;
            d3dpso->Release();
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
    ptr->close();
    ID3D12GraphicsCommandList * d3dcl = ptr->commandList;
    _graphicsQueue.q().ExecuteCommandLists(1, (ID3D12CommandList**)&d3dcl);
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
    _present->reset(0);
    auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(_frames[_frameIndex].rt, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    _present->commandList->ResourceBarrier(1, &barrier);
    kickoff(*_present, nullptr);

    // present the frame buffer
    _swapChain->Present(1, 0);

    // insert a fence to mark the end of current frame.
    _frames[_frameIndex].fence = _graphicsQueue.mark();

    // Update the frame index to point to the next frame in the queue
    _frameIndex = _swapChain->GetCurrentBackBufferIndex();

    // Then wait for it to be ready to render to.
    // TODO: move this out of prensent. to give app a chance to do other CPU related work while waiting for the frame buffer to be ready.
    _graphicsQueue.finish(_frames[_frameIndex].fence);

    // transit it back to render target state.
    _present->reset(0);
    barrier = CD3DX12_RESOURCE_BARRIER::Transition(_frames[_frameIndex].rt, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    _present->commandList->ResourceBarrier(1, &barrier);
    kickoff(*_present, nullptr);
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D12CommandList::D3D12CommandList(D3D12Gpu2 & gpu, const Gpu2::CommandListCreationParameters & cp)
    : owner(gpu)
{
    ThrowIfFailed(gpu.device().CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&allocator)));
    ThrowIfFailed(gpu.device().CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, allocator, (ID3D12PipelineState*)cp.initialPipelineState, IID_PPV_ARGS(&commandList)));
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D12CommandList::close()
{
    if (!_closed) {
        commandList->Close();
        _closed = true;
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D12CommandList::reset(uint64_t initialState)
{
    close();
    commandList->Reset(allocator, (ID3D12PipelineState*)initialState);
    _closed = false;

    // hack hack: setup default render states
    auto w = owner.frameWidth();
    auto h = owner.frameHeight();
    auto viewport = CD3DX12_VIEWPORT(0.0f, 0.0f, static_cast<float>(w), static_cast<float>(h));
    auto scissor = CD3DX12_RECT(0, 0, static_cast<LONG>(w), static_cast<LONG>(h));
    auto rtv = owner.backrtv();
    commandList->SetGraphicsRootSignature(owner.emptyRootSignature());
    commandList->OMSetRenderTargets(1, &rtv, false, nullptr);
    commandList->RSSetViewports(1, &viewport);
    commandList->RSSetScissorRects(1, &scissor);
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
void GN::gfx::D3D12CommandList::draw(const Gpu2::DrawParameters & p)
{
    if (p.pso)
        commandList->SetPipelineState((ID3D12PipelineState*)p.pso);
    else
        commandList->ClearState(nullptr);

    // setup vertex buffer views
    D3D12_VERTEX_BUFFER_VIEW vb[16];
    uint32_t offsets[16];
    GN_ASSERT(p.vertexBufferCount <= GN_ARRAY_COUNT(vb));
    for(size_t i = 0;  i < p.vertexBufferCount; ++i) {
        vb[i].BufferLocation = promote(p.vertexBuffers[i].surface)->GetGPUVirtualAddress();
        vb[i].SizeInBytes = ((D3D12PlacedResource*)p.vertexBuffers[i].surface)->creationParameters.b.bytes; // TODO: maybe pass in size?
        vb[i].StrideInBytes = p.vertexBuffers[i].stride;
        offsets[i] = p.vertexBuffers[i].offset;
    }
    commandList->IASetVertexBuffers(0, p.vertexBufferCount, vb);
    commandList->IASetPrimitiveTopology(tod3d(p.prim));

    if (p.indexBuffer) {
        // TODO: setup index buffer view
        commandList->DrawIndexedInstanced(p.vertexOrIndexCount, 1, p.baseindex, p.basevertex, 0);
    } else {
        commandList->DrawInstanced(p.vertexOrIndexCount, 1, p.basevertex, 0);
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::D3D12CommandList::copyBufferRegion(const Gpu2::CopyBufferRegionParameters & p)
{
    // hack hack: set target buffer to writeable.
    auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(promote(p.dest), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_COPY_DEST);
    commandList->ResourceBarrier(1, &barrier);

    auto sourceBytes = p.sourceBytes;
    if (0 == sourceBytes) sourceBytes = ((D3D12PlacedResource*)p.source)->creationParameters.b.bytes - p.sourceOffset;

    commandList->CopyBufferRegion(promote(p.dest), p.destOffset, promote(p.source), p.sourceOffset, sourceBytes);

    // hack hack: restore default resource state
    barrier = CD3DX12_RESOURCE_BARRIER::Transition(promote(p.dest), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
    commandList->ResourceBarrier(1, &barrier);
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
GN::gfx::D3D12Buffer::D3D12Buffer(D3D12Gpu2 & o, const Gpu2::SurfaceCreationParameters & cp) : D3D12PlacedResource(o, cp)
{
    GN_ASSERT(GN::gfx::Gpu2::SurfaceDimension::BUFFER == cp.dim);
    if (cp.memory) {
        // create placed resource
        auto desc = CD3DX12_RESOURCE_DESC::Buffer(cp.b.bytes);
        ReturnIfFailed(o.device().CreatePlacedResource(
            GetD3D12Heap(cp.memory),
            cp.offset,
            &desc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&resource)));
    }
    else {
        // test code path: create comitted resource
        GN_ASSERT(0 == cp.offset);
        auto prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
        auto desc = CD3DX12_RESOURCE_DESC::Buffer(cp.b.bytes);
        ReturnIfFailed(o.device().CreateCommittedResource(
            &prop,
            D3D12_HEAP_FLAG_NONE,
            &desc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&resource)));
    }
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::D3D12Texture::D3D12Texture(D3D12Gpu2 & o, const Gpu2::SurfaceCreationParameters & cp) : D3D12PlacedResource(o, cp)
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
