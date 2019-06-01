#pragma once
#include <garnet/GNgfx2.h>
#include <dxgi1_4.h>
#include <d3d12.h>
#include <d3d12sdklayers.h>
#include <d3dx12.h>

#define ThrowIfFailed(x) if(true) { HRESULT hr__ = (x); if (FAILED(hr__)) GN::breakIntoDebugger(); } else void(0)
#define ReturnIfFailed(x) if(true) { HRESULT hr__ = (x); if (FAILED(hr__)) { GN_ERROR(sLogger)(#x "failed, hr = 0x%08X", hr__); return; } } else void(0)

namespace GN { namespace gfx
{

    struct D3D12CommandList;

    class D3D12Gpu2 : public Gpu2
    {
        // Pipeline objects.
        static const uint32_t BACK_BUFFER_COUNT = 2;
        AutoComPtr<IDXGISwapChain3> _swapChain;
        AutoComPtr<ID3D12Device> _device;
        AutoComPtr<ID3D12Resource> _renderTargets[BACK_BUFFER_COUNT];
        AutoComPtr<ID3D12CommandQueue> _commandQueue;
        AutoComPtr<ID3D12RootSignature> _rootSignature;
        AutoComPtr<ID3D12DescriptorHeap> _rtvHeap;
        //AutoComPtr<ID3D12PipelineState> _pipelineState;
        //AutoComPtr<ID3D12GraphicsCommandList> _commandList;
        UINT _rtvDescriptorSize = 0;
        UINT _frameIndex = 0; // index of current back buffer
        //D3D12_VIEWPORT _viewport;
        //D3D12_RECT _scissorRect;

    public:
        D3D12Gpu2(const CreationParameters &);

        // member accessor
        ID3D12Device & device() { return *_device; }

        // interface methods
        DynaArray<uint64_t> createPipelineStates(const PipelineCreationParameters *, size_t) { return {}; }
        void deletePipelineStates(const uint64_t *, size_t) {}
        AutoRef<CommandList> createCommandList(const CommandListCreationParameters &);
        AutoRef<MemPool>  createMemoryPool(const MemPoolCreationParameters &) { return {}; }
        std::vector<uint8> compileProgram(const ProgramSource &) { return {}; }
        AutoRef<Surface> createSurface(const SurfaceCreationParameters &) { return {}; }
        AutoRef<Query> createQuery(const QueryCreationParameters &) { return {}; }
        void kickoff(GN::gfx::Gpu2::CommandList &);
        void present(const PresentParameters &);
    };

    struct D3D12CommandList : public Gpu2::CommandList
    {
        D3D12Gpu2 & owner;
        AutoComPtr<ID3D12CommandAllocator> allocator;
        AutoComPtr<ID3D12GraphicsCommandList> commandList;
        
        D3D12CommandList(D3D12Gpu2 &, const Gpu2::CommandListCreationParameters &);
        ~D3D12CommandList() {}
        
        bool ok() const { return nullptr != commandList; }
        
        void copy(const Gpu2::CopyParameters &) { GN_UNIMPL(); }
        void draw(const Gpu2::DrawParameters &) { GN_UNIMPL(); }
        void compute(const Gpu2::ComputeParameters &) { GN_UNIMPL(); }

        uint64_t mark() { GN_UNIMPL(); return 0; }
        void wait(uint64_t) { GN_UNIMPL(); }
    };
}}