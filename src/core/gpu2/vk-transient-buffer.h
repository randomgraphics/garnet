#pragma once

#include "vk-buffer-state.h"
#include "vk-gpu-context.h"
#include "vk-buffer.h"
#include <atomic>
#include <memory>
#include <mutex>
#include <vector>

namespace GN::gpu2 {

class TransientArenaVulkan;

/// A single sub-allocation from a backing VkBuffer in the transient pool.
class TransientBufferVulkan final : public BufferVulkan {
public:
    GN_REGISTER_RUNTIME_TYPE(BufferVulkan);

    struct BackingBuffer {
        rv::Ref<rv::Buffer>       buffer;
        uint8_t *                 mappedPtr     = nullptr;
        uint64_t                  capacity      = 0;
        uint64_t                  offset        = 0;
        vk::DeviceAddress         deviceAddress = 0;
        std::atomic<uint64_t>     liveCount {0};
        std::atomic<uint64_t>     mappedCount {0};
        mutable BufferStateVulkan gpuState {};
    };

    TransientBufferVulkan(const StrA & name, AutoRef<TransientArenaVulkan> owner, BackingBuffer * backing, uint64_t offset, uint64_t size);
    ~TransientBufferVulkan() override;

    uint64_t offset() const { return mOffset; }
    uint64_t size() const { return mSize; }
    uint64_t gpuAddress() const override { return mBacking ? (mBacking->deviceAddress + mOffset) : 0; }
    uint64_t bufferOffset() const override { return mOffset; }

    Mapped               map() override;
    bool                 setContent(ArrayView<const uint8_t> data, size_t offset = 0) override;
    std::vector<uint8_t> readContent(size_t offset = 0, size_t size = (size_t) -1) const override;

    vk::Buffer          nativeBuffer() const { return mBacking && mBacking->buffer ? mBacking->buffer->handle() : vk::Buffer {}; }
    rv::Ref<rv::Buffer> rvBuffer() const { return mBacking ? mBacking->buffer : rv::Ref<rv::Buffer> {}; }
    BackingBuffer *     backing() const { return mBacking; }
    BufferStateVulkan & gpuState() const { return mBacking->gpuState; }

protected:
    void unmap(const Mapped &) override;

private:
    AutoRef<TransientArenaVulkan> mOwner;
    BackingBuffer *               mBacking = nullptr;
    uint64_t                      mOffset  = 0;
    uint64_t                      mSize    = 0;
};

class TransientArenaVulkan final : public RCRT64 {
public:
    GN_REGISTER_RUNTIME_TYPE(RCRT64);

    struct CreateParameters {
        AutoRef<GpuContext> context;
        uint64_t            suggestedArenaSize = 16 * 1024 * 1024; // 16MB default
    };

    using BackingBuffer = TransientBufferVulkan::BackingBuffer;

    TransientArenaVulkan(const StrA & name, const CreateParameters & params);
    ~TransientArenaVulkan() override;

    AutoRef<Buffer> allocate(uint64_t size, uint64_t alignment = 0, const char * bufferName = nullptr);
    void            reset();

    size_t   numBackingBuffers() const { return mBackingBuffers.size(); }
    size_t   activeBackingIndex() const { return mActiveBackingIndex; }
    uint64_t backingCapacity(size_t i) const { return mBackingBuffers[i]->capacity; }
    uint64_t backingOffset(size_t i) const { return mBackingBuffers[i]->offset; }
    uint64_t backingLiveCount(size_t i) const { return mBackingBuffers[i]->liveCount.load(std::memory_order_relaxed); }

private:
    CreateParameters                            mParams;
    AutoRef<GpuContextVulkan2>                  mGpu;
    std::vector<std::unique_ptr<BackingBuffer>> mBackingBuffers;
    size_t                                      mActiveBackingIndex = 0;
    std::mutex                                  mAllocateMutex;
    uint64_t                                    mDefaultAlignment   = 256;
    static constexpr uint64_t                   DEFAULT_CAPACITY    = 16u * 1024u * 1024u; // 16MB

    bool createBackingBuffer(uint64_t minCapacity);
};

} // namespace GN::gpu2
