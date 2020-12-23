#pragma once
#include <garnet/GNgfx2.h>
#include <dxgi1_4.h>
#include <d3d12.h>
#include <d3d12sdklayers.h>
#include <d3dx12.h>

#define ThrowIfFailed(x) if(true) { HRESULT hr__ = (x); if (FAILED(hr__)) GN::breakIntoDebugger(); } else void(0)
#define ReturnIfFailed(x, ...) if(true) { HRESULT hr__ = (x); if (FAILED(hr__)) { GN_ERROR(sLogger)(#x "failed, hr = 0x%08X", hr__); return __VA_ARGS__; } } else void(0)

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

    typedef Win32Handle<(HANDLE)0> Win32Event;

    class D3D12CommandList;

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
        Gpu2::CommandListType _type;
        FenceValue _fenceValue;

        template<Gpu2::CommandListType TYPE>
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

        template<Gpu2::CommandListType TYPE>
        void init(ID3D12Device & device)
        {
            _type = TYPE;
            _fenceValue.type = (unsigned)TYPE;
            _fenceValue.value = 0;
            D3D12_COMMAND_QUEUE_DESC queueDesc = {};
            queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
            queueDesc.Type = d3dtype<TYPE>();
            ThrowIfFailed(device.CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&_q)));
            ThrowIfFailed(device.CreateFence(_fenceValue.value++, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_f)));
            _e.h = CreateEvent(nullptr, FALSE, FALSE, nullptr);
            GN_VERIFY(!_e.empty());
        }

        ID3D12CommandQueue & q() { return *_q; }

        uint64_t mark()
        {
            uint64_t ret = _fenceValue.u64;
            _q->Signal(_f, _fenceValue.value);
            ++_fenceValue.value;
            return ret;
        }

        bool isPending(uint64 fence) const
        {
            FenceValue f = { fence };
            return _f->GetCompletedValue() < f.value;
        }

        void finish(uint64 fence)
        {
            FenceValue f = {fence};
            GN_ASSERT(f.type == (uint64_t)_type);
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
        D3D12CommandQueue _graphicsQueue;
        AutoComPtr<ID3D12RootSignature> _emptyRootSignature;
        AutoComPtr<ID3D12DescriptorHeap> _rtvHeap;
        AutoRef<D3D12CommandList> _present; // command list for presenting
        //AutoComPtr<ID3D12PipelineState> _pipelineState;
        //AutoComPtr<ID3D12GraphicsCommandList> _commandList;
        //D3D12_VIEWPORT _viewport;
        //D3D12_RECT _scissorRect;
        uint32_t _frameIndex = 0;
        uint32_t _frameWidth = 0;
        uint32_t _frameHeight = 0;
        FrameBuffer _frames[BACK_BUFFER_COUNT] = {};

    public:
        
        D3D12Gpu2(const CreationParameters &);

        // member accessor
        ID3D12Device & device() { return *_device; }
        D3D12_CPU_DESCRIPTOR_HANDLE backrtv() { return _frames[_frameIndex].rtv; }
        uint32_t frameWidth() const { return _frameWidth; }
        uint32_t frameHeight() const { return _frameHeight; }
        ID3D12RootSignature * emptyRootSignature() const { return _emptyRootSignature.get(); }

        // interface methods
        DynaArray<uint64_t> createPipelineStates(const PipelineCreationParameters *, size_t) override;
        void deletePipelineStates(const uint64_t *, size_t) override;
        AutoRef<CommandList> createCommandList(const CommandListCreationParameters &) override;
        AutoRef<MemoryBlock> createMemoryBlock(const MemoryBlockCreationParameters &) override;
        AutoRef<Surface> createSurface(const SurfaceCreationParameters &) override;
        AutoRef<Query> createQuery(const QueryCreationParameters &) override { return {}; }
        void kickoff(GN::gfx::Gpu2::CommandList &, uint64_t * fence) override;
        void finish(uint64_t fence) override;
        void present(const PresentParameters &) override;
    };

    class D3D12CommandList : public Gpu2::CommandList
    {
        struct Item
        {
            AutoComPtr<ID3D12CommandAllocator>    allocator;
            AutoComPtr<ID3D12GraphicsCommandList> commandList;
            D3D12CommandQueue *                   queue = nullptr;
            uint64_t                              fence;
            bool                                  closed = false;

            bool pending() const
            {
                return queue && queue->isPending(fence);
            }
        };

        D3D12Gpu2 & _owner;
        std::list<Item> _pool; // command list pool. The first item in the list is always the active command list.

    public:
        
        D3D12CommandList(D3D12Gpu2 &, const Gpu2::CommandListCreationParameters &);
        ~D3D12CommandList() {}

        bool ok() const { return !_pool.empty() && _pool.front().allocator && _pool.front().commandList; }
        uint64_t kickoff(D3D12CommandQueue &);
        ID3D12GraphicsCommandList & active() { return *_pool.front().commandList; };

        void reset(uint64_t initialState) override;
        void clear(const Gpu2::ClearParameters &) override;
        void draw(const Gpu2::DrawParameters &) override;
        void compute(const Gpu2::ComputeParameters &) override { GN_UNIMPL(); }
        void copyBufferRegion(const Gpu2::CopyBufferRegionParameters &) override;

    private:

        bool _closed = false;
    };

    struct D3D12MemoryBlock : public Gpu2::MemoryBlock
    {
        D3D12Gpu2 & owner;
        AutoComPtr<ID3D12Heap> heap;

        D3D12MemoryBlock(D3D12Gpu2 &, const Gpu2::MemoryBlockCreationParameters &);
        ~D3D12MemoryBlock() {}

        bool ok() const { return !heap.empty(); }
    };

    struct D3D12PlacedResource : public Gpu2::Surface
    {
        D3D12Gpu2 & owner;
        AutoComPtr<ID3D12Resource> resource;
        const Gpu2::SurfaceCreationParameters creationParameters;

        D3D12PlacedResource(D3D12Gpu2 & o, const Gpu2::SurfaceCreationParameters & cp) : owner(o), creationParameters(cp) {}

        bool ok() const { return !resource.empty(); }

        void unmap(uint32_t subSurfaceId) override
        {
            resource->Unmap(subSurfaceId, nullptr);
        }
    };

    struct D3D12Buffer : public D3D12PlacedResource
    {
        D3D12Buffer(D3D12Gpu2 &, const Gpu2::SurfaceCreationParameters &);

        Gpu2::MappedSurfaceData map(uint32_t subSurfaceId) override
        {
            Gpu2::MappedSurfaceData result = {};
            result.ptr = nullptr;
            result.rawPitch = creationParameters.b.bytes;
            result.slicePitch = creationParameters.b.bytes;
            result.subSurfaceId = subSurfaceId;
            GN_DX_CHECK_RETURN(resource->Map(subSurfaceId, nullptr, &result.ptr), {});
            return result;
        }

    };

    struct D3D12Texture : public D3D12PlacedResource
    {
        D3D12Texture(D3D12Gpu2 &, const Gpu2::SurfaceCreationParameters &);

        Gpu2::MappedSurfaceData map(uint32_t subSurfaceId) override
        {
            // not implemented.
            subSurfaceId;
            return {};
        }
    };
}}