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
    template <HANDLE NULL_HANDLE_VALUE>
    struct Win32Handle
    {
        HANDLE h = NULL_HANDLE_VALUE;

        ~Win32Handle() { close(); }

        bool empty() const { return NULL_HANDLE_VALUE == h; }

        void close()
        {
            if (NULL_HANDLE_VALUE != h) CloseHandle(h), h = NULL_HANDLE_VALUE;
        }

        operator HANDLE () const { return h; }
    };

    typedef Win32Handle<0> Win32Event;

    struct D3D12CommandList;

    template<Gpu2::CommandListType TYPE>
    class D3D12CommandQueue
    {
        union FenceValue
        {
            uint64_t u64;
            struct
            {
                uint64_t value : 62;
                uint64_t type  : 2;
            };
        };

        AutoComPtr<ID3D12CommandQueue> _q;
        AutoComPtr<ID3D12Fence> _f;
        Win32Event _e;
        FenceValue _fenceValue;

        static constexpr D3D12_COMMAND_LIST_TYPE d3dtype()
        {
            if constexpr (Gpu2::CommandListType::GRAPHICS == TYPE)
                return D3D12_COMMAND_LIST_TYPE_DIRECT;
            else if constexpr (Gpu2::CommandListType::COMPUTE == TYPE)
                return D3D12_COMMAND_LIST_TYPE_COMPUTE;
            else {
                static_assert(TYPE == Gpu2::CommandListType::COPY);
                return D3D12_COMMAND_LIST_TYPE_COPY;
            }
        }

    public:

        void init(ID3D12Device & device)
        {
            _fenceValue.type = (unsigned)TYPE;
            _fenceValue.value = 0;
            D3D12_COMMAND_QUEUE_DESC queueDesc = {};
            queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
            queueDesc.Type = d3dtype();
            ThrowIfFailed(device.CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&_q)));
            ThrowIfFailed(device.CreateFence(_fenceValue.value++, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_f)));
            _e.h = CreateEvent(nullptr, FALSE, FALSE, nullptr);
            GN_VERIFY(!_e.empty());
        }

        ID3D12CommandQueue & q() { return *_q; }

        uint64_t mark()
        {
            _q->Signal(_f, _fenceValue.value);
            ++_fenceValue.value;
            return _fenceValue.u64;
        }

        void finish(uint64 fence)
        {
            FenceValue f = {fence};
            GN_ASSERT(f.type == (unsigned int)TYPE);
            if (_f->GetCompletedValue() < f.value) {
                ThrowIfFailed(_f->SetEventOnCompletion(f.value, _e));
                WaitForSingleObjectEx(_e, INFINITE, FALSE);
            }
        }

        void waitForIdle()
        {
            if (_q && _f) {
                finish(mark());
            }
        }
    };

    class D3D12Gpu2 : public Gpu2
    {
        struct FrameBuffer
        {
            AutoComPtr<ID3D12Resource> rt;
            D3D12_CPU_DESCRIPTOR_HANDLE rtv = {};
            uint64_t fence = 0;
        };

        // Pipeline objects.
        static const uint32_t BACK_BUFFER_COUNT = 2;
        AutoComPtr<IDXGISwapChain3> _swapChain;
        AutoComPtr<ID3D12Device> _device;
        D3D12CommandQueue<CommandListType::GRAPHICS> _graphicsQueue;
        //AutoComPtr<ID3D12RootSignature> _rootSignature;
        AutoComPtr<ID3D12DescriptorHeap> _rtvHeap;
        AutoRef<D3D12CommandList> _present; // command list for presenting
        //AutoComPtr<ID3D12PipelineState> _pipelineState;
        //AutoComPtr<ID3D12GraphicsCommandList> _commandList;
        //D3D12_VIEWPORT _viewport;
        //D3D12_RECT _scissorRect;
        UINT _frameIndex = 0;
        FrameBuffer _frames[BACK_BUFFER_COUNT] = {};

    public:
        
        D3D12Gpu2(const CreationParameters &);

        // member accessor
        ID3D12Device & device() { return *_device; }
        D3D12_CPU_DESCRIPTOR_HANDLE backrtv() { return _frames[_frameIndex].rtv; }

        // interface methods
        DynaArray<uint64_t> createPipelineStates(const PipelineCreationParameters *, size_t) { return {}; }
        void deletePipelineStates(const uint64_t *, size_t) {}
        AutoRef<CommandList> createCommandList(const CommandListCreationParameters &);
        AutoRef<MemPool>  createMemoryPool(const MemPoolCreationParameters &) { return {}; }
        AutoRef<Surface> createSurface(const SurfaceCreationParameters &);
        AutoRef<Query> createQuery(const QueryCreationParameters &) { return {}; }
        void kickoff(GN::gfx::Gpu2::CommandList &, uint64_t * fence);
        void finish(uint64_t fence);
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

        void reset(uint64_t initialState) { commandList->Reset(allocator, (ID3D12PipelineState*)initialState); }
        void clear(const Gpu2::ClearParameters &);
        void draw(const Gpu2::DrawParameters &) { GN_UNIMPL(); }
        void compute(const Gpu2::ComputeParameters &) { GN_UNIMPL(); }
        void copy(const Gpu2::CopyParameters &) { GN_UNIMPL(); }

        uint64_t mark() { GN_UNIMPL(); return 0; }
        void wait(uint64_t) { GN_UNIMPL(); }
    };
}}