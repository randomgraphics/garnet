#pragma once
#include <garnet/GNgfx2.h>
#include <dxgi1_4.h>
#include <d3d12.h>
#include <d3d12sdklayers.h>
#include <d3dx12.h>

#define ThrowIfFailed(x) if(true) { HRESULT hr__ = (x); if (FAILED(hr__)) GN::breakIntoDebugger(); } else void(0)

namespace GN { namespace gfx
{

    class D3D12CommandList;

    class D3D12Gpu2 : public Gpu2
    {
        // Pipeline objects.
        static const uint32_t BACK_BUFFER_COUNT = 2;
        AutoComPtr<IDXGISwapChain3> _swapChain;
        AutoComPtr<ID3D12Device> _device;
        AutoComPtr<ID3D12Resource> _renderTargets[BACK_BUFFER_COUNT];
        AutoComPtr<ID3D12CommandAllocator> _commandAllocator;
        AutoComPtr<ID3D12CommandQueue> _commandQueue;
        AutoComPtr<ID3D12RootSignature> _rootSignature;
        AutoComPtr<ID3D12DescriptorHeap> _rtvHeap;
        AutoComPtr<ID3D12PipelineState> _pipelineState;
        AutoComPtr<ID3D12GraphicsCommandList> _commandList;
        UINT _rtvDescriptorSize = 0;
        UINT _frameIndex = 0; // index of current back buffer
        D3D12_VIEWPORT _viewport;
        D3D12_RECT _scissorRect;

    public:
        D3D12Gpu2(const CreationParameters &);
        std::vector<Pipeline> createPipelineObjects(const PipelineCreationParameters *, size_t) { return {}; }
        AutoRef<CommandList> createCommandList(const CommandListCreationParameters &);
        AutoRef<MemPool>  createMemoryPool(const MemPoolCreationParameters &) { return {}; }
        std::vector<uint8> compileProgram(const ProgramSource &) { return {}; }
        AutoRef<Surface> createSurface(const SurfaceCreationParameters &) { return {}; }
        AutoRef<Query> createQuery(const QueryCreationParameters &) { return {}; }
        void kickoff(CommandList &) {}
        void present(const PresentParameters &);
    };

    class D3D12CommandList : public Gpu2::CommandList
    {
        bool _ok = false;
    public:
        D3D12CommandList(const Gpu2::CommandListCreationParameters & ) { _ok = true; }
        ~D3D12CommandList() {}
        bool ok() const { return _ok; }
        void copy(const Gpu2::CopyParameters &) { GN_UNIMPL(); }
        void draw(const Gpu2::DrawParameters *) { GN_UNIMPL(); }
        uint64_t mark() { GN_UNIMPL(); return 0; }
        void wait(uint64_t) { GN_UNIMPL(); }
    };
}}