#include "pch.h"

#include <map>
#include <mutex>
#include <atomic>

namespace GN::fx2::bindless {
namespace {

std::atomic<SharedShaderConstants::StreamingId> gNextStreamingId {1};

// SSC storage is deliberately fixed-capacity: allocations never move while recorders
// or submitted work may still refer to them.
class RangePool : public std::enable_shared_from_this<RangePool> {
public:
    struct Allocation {
        std::shared_ptr<RangePool> pool;
        uint64_t                   offset = 0;
        uint64_t                   size   = 0;
        ~Allocation() {
            if (pool) pool->release(offset, size);
        }
    };

    explicit RangePool(uint64_t capacity) { mFree.emplace(0, capacity); }

    std::shared_ptr<Allocation> allocate(uint64_t size, uint64_t alignment) {
        std::lock_guard<std::mutex> lock(mMutex);
        for (auto it = mFree.begin(); it != mFree.end(); ++it) {
            const uint64_t begin = it->first;
            if (begin > uint64_t(-1) - (alignment - 1)) continue;
            const uint64_t aligned = (begin + alignment - 1) / alignment * alignment;
            if (aligned < begin || size > it->second || aligned - begin > it->second - size) continue;
            const uint64_t end = begin + it->second;
            mFree.erase(it);
            if (aligned > begin) mFree.emplace(begin, aligned - begin);
            if (aligned + size < end) mFree.emplace(aligned + size, end - aligned - size);
            auto result    = std::make_shared<Allocation>();
            result->pool   = shared_from_this();
            result->offset = aligned;
            result->size   = size;
            return result;
        }
        return {};
    }

    void release(uint64_t offset, uint64_t size) {
        std::lock_guard<std::mutex> lock(mMutex);
        auto                        next = mFree.lower_bound(offset);
        if (next != mFree.begin()) {
            auto prev = std::prev(next);
            if (prev->first + prev->second == offset) {
                offset = prev->first;
                size += prev->second;
                mFree.erase(prev);
            }
        }
        next = mFree.lower_bound(offset);
        if (next != mFree.end() && offset + size == next->first) {
            size += next->second;
            mFree.erase(next);
        }
        mFree.emplace(offset, size);
    }

private:
    std::mutex                   mMutex;
    std::map<uint64_t, uint64_t> mFree;
};

class UniformStateImpl final : public SharedShaderConstants::UniformState {
public:
    GN_REGISTER_RUNTIME_TYPE(UniformState);
    UniformStateImpl(std::shared_ptr<RangePool::Allocation> allocation, gpu2::GpuResourceView view)
        : UniformState(TYPE_INFO(), "bindless.uniform-state"), mAllocation(std::move(allocation)), mView(std::move(view)) {}
    gpu2::GpuResourceView view() const override { return mView; }

private:
    std::shared_ptr<RangePool::Allocation> mAllocation;
    gpu2::GpuResourceView                  mView;
};

class SharedShaderConstantsImpl final : public SharedShaderConstants {
public:
    GN_REGISTER_RUNTIME_TYPE(SharedShaderConstants);

    explicit SharedShaderConstantsImpl(const CreateParameters & params)
        : SharedShaderConstants(TYPE_INFO(), "bindless.SharedShaderConstants"), mGpu(params.gpu), mUniformCapacity(params.uniformCapacity),
          mStreamCapacity(params.streamingCapacity), mUniformPool(std::make_shared<RangePool>(params.uniformCapacity)),
          mStreamPool(std::make_shared<RangePool>(params.streamingCapacity)) {
        mUniformBuffer = gpu2::Buffer::create("fx2.ssc.uniforms", {.context = mGpu, .size = params.uniformCapacity});
        mStreamBuffer  = gpu2::Buffer::create("fx2.ssc.streaming", {.context = mGpu, .size = params.streamingCapacity, .mappable = true});
        if (mStreamBuffer) mStreamMapping = mStreamBuffer->map();
    }

    bool valid() const { return mUniformBuffer && mStreamBuffer && !mStreamMapping.empty(); }

    AutoRef<UniformState> recordUniformUpdate(gpu2::bindless::CnC & producer, ArrayView<const uint8_t> bytes) override {
        constexpr uint64_t kUniformAlignment = 256;
        constexpr uint64_t kMaxUniformRange  = 16384; // Vulkan guarantees at least this much uniform-buffer range.
        if (bytes.empty() || bytes.size() > kMaxUniformRange || bytes.size() > mUniformCapacity) return {};
        auto allocation = mUniformPool->allocate(bytes.size(), kUniformAlignment);
        if (!allocation) return {};
        gpu2::GpuResourceView view(mUniformBuffer);
        view.setBufferViewType(gpu2::GpuResourceView::BufferView::UNIFORM).setBufferViewOffset(allocation->offset).setBufferViewSize(bytes.size());
        auto state = AutoRef<UniformState>(new UniformStateImpl(allocation, view));
        producer.retainResource(allocation);
        producer.recordUploadBuffer(mUniformBuffer, allocation->offset, bytes);
        return state;
    }

    StreamingBlock allocateStreaming(uint64_t size) override {
        constexpr uint64_t kStorageAlignment = 256;
        if (!size || size > mStreamCapacity) return {};
        auto allocation = mStreamPool->allocate(size, kStorageAlignment);
        if (!allocation) return {};
        StreamingId streamingId;
        {
            std::lock_guard<std::mutex> lock(mMutex);
            streamingId = gNextStreamingId.fetch_add(1, std::memory_order_relaxed);
            if (!streamingId) streamingId = gNextStreamingId.fetch_add(1, std::memory_order_relaxed);
            if (!streamingId) return {};
            mStreaming.emplace(streamingId, allocation);
        }
        auto view = gpu2::GpuResourceView(mStreamBuffer);
        view.setBufferViewType(gpu2::GpuResourceView::BufferView::STORAGE).setBufferViewOffset(allocation->offset).setBufferViewSize(size);
        return {streamingId, {static_cast<uint8_t *>(mStreamMapping.data()) + allocation->offset, static_cast<size_t>(size)}, view};
    }

    void releaseStreaming(StreamingId streamingId) override {
        std::lock_guard<std::mutex> lock(mMutex);
        mStreaming.erase(streamingId);
    }

private:
    AutoRef<gpu2::GpuContext>                                     mGpu;
    uint64_t                                                      mUniformCapacity = 0;
    uint64_t                                                      mStreamCapacity  = 0;
    AutoRef<gpu2::Buffer>                                         mUniformBuffer;
    AutoRef<gpu2::Buffer>                                         mStreamBuffer;
    gpu2::Buffer::Mapped                                          mStreamMapping;
    std::shared_ptr<RangePool>                                    mUniformPool;
    std::shared_ptr<RangePool>                                    mStreamPool;
    std::mutex                                                    mMutex;
    std::map<StreamingId, std::shared_ptr<RangePool::Allocation>> mStreaming;
};

} // namespace

GN_API AutoRef<SharedShaderConstants> SharedShaderConstants::create(const CreateParameters & params) {
    if (!params.gpu || !params.uniformCapacity || !params.streamingCapacity) GN_UNLIKELY return {};
    auto result = AutoRef<SharedShaderConstantsImpl>(new SharedShaderConstantsImpl(params));
    if (!result->valid()) return {};
    return result;
}

} // namespace GN::fx2::bindless
