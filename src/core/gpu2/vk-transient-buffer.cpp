#include "pch.h"
#include "vk-transient-buffer.h"
#include <mutex>

static GN::Logger * sLogger = GN::getLogger("GN.gpu2.vk");

namespace GN::gpu2 {

static inline uint64_t alignUp(uint64_t value, uint64_t alignment) {
    if (alignment <= 1) return value;
    return (value + alignment - 1) & ~(alignment - 1);
}

static constexpr uint64_t MB = 1024u * 1024u;
static inline uint64_t roundUpToMb(uint64_t value) {
    return ((value + MB - 1) / MB) * MB;
}

// -----------------------------------------------------------------------------
// TransientBufferVulkan
// -----------------------------------------------------------------------------

TransientBufferVulkan::TransientBufferVulkan(const StrA & name, AutoRef<TransientArenaVulkan> owner, BackingBuffer * backing, uint64_t offset, uint64_t size)
    : BufferVulkan(TYPE_INFO(), name), mOwner(std::move(owner)), mBacking(backing), mOffset(offset) {
    if (mBacking) {
        mRvBuffer      = mBacking->buffer;
        mSize          = size;
        mDeviceAddress = mBacking->deviceAddress + offset;
        mMappable      = true;
        mBacking->liveCount.fetch_add(1, std::memory_order_relaxed);
    }
}

TransientBufferVulkan::~TransientBufferVulkan() {
    if (mBacking) {
        mBacking->liveCount.fetch_sub(1, std::memory_order_release);
    }
}

Buffer::Mapped TransientBufferVulkan::map() {
    if (!mBacking || !mBacking->mappedPtr) GN_UNLIKELY {
        GN_ERROR(sLogger, "TransientBufferVulkan::map: backing buffer is not mapped");
        return Buffer::Mapped();
    }
    mBacking->mappedCount.fetch_add(1, std::memory_order_relaxed);
    return Buffer::Mapped(this, mBacking->mappedPtr + mOffset, (size_t) mSize);
}

void TransientBufferVulkan::unmap(const Mapped &) {
    if (mBacking) {
        mBacking->mappedCount.fetch_sub(1, std::memory_order_release);
    }
}

bool TransientBufferVulkan::setContent(ArrayView<const uint8_t> data, size_t offset) {
    if (data.empty()) return true;
    if (offset + data.size() > mSize) GN_UNLIKELY {
        GN_ERROR(sLogger, "TransientBufferVulkan::setContent: offset {} + size {} exceeds allocated size {}", offset, data.size(), mSize);
        return false;
    }
    if (!mBacking || !mBacking->mappedPtr) GN_UNLIKELY {
        GN_ERROR(sLogger, "TransientBufferVulkan::setContent: backing buffer is not mapped");
        return false;
    }
    memcpy(mBacking->mappedPtr + mOffset + offset, data.data(), data.size());
    return true;
}

std::vector<uint8_t> TransientBufferVulkan::readContent(size_t offset, size_t size) const {
    if (!mBacking || !mBacking->mappedPtr || offset >= mSize) return {};
    size_t copySize = (size == (size_t) -1 || offset + size > mSize) ? (size_t) (mSize - offset) : size;
    std::vector<uint8_t> result(copySize);
    memcpy(result.data(), mBacking->mappedPtr + mOffset + offset, copySize);
    return result;
}

// -----------------------------------------------------------------------------
// TransientArenaVulkan
// -----------------------------------------------------------------------------

TransientArenaVulkan::TransientArenaVulkan(const StrA & name, const CreateParameters & params)
    : RCRT64(TYPE_INFO(), name), mParams(params) {
    mGpu = mParams.context.staticCastTo<GpuContextVulkan2>();
    if (mGpu && mGpu->vulkanDevice().handle()) {
        auto props = mGpu->vulkanDevice().gi()->physical.getProperties();
        mDefaultAlignment = std::max<uint64_t>(props.limits.minUniformBufferOffsetAlignment, 16u);
    }
}

TransientArenaVulkan::~TransientArenaVulkan() {
    for (size_t i = 0; i < mBackingBuffers.size(); ++i) {
        auto * b      = mBackingBuffers[i].get();
        auto   mapped = b->mappedCount.load(std::memory_order_relaxed);
        auto   live   = b->liveCount.load(std::memory_order_relaxed);
        if (mapped != 0 || live != 0) GN_UNLIKELY {
            GN_WARN(sLogger, "TransientArenaVulkan::~TransientArenaVulkan: backing buffer[{}] has mappedCount={}, liveCount={} (expected 0)", i, mapped, live);
        }
    }
}

bool TransientArenaVulkan::createBackingBuffer(uint64_t minCapacity) {
    for (size_t i = 0; i < mBackingBuffers.size(); ++i) {
        auto * b = mBackingBuffers[i].get();
        if (b->liveCount.load(std::memory_order_relaxed) == 0 && b->mappedCount.load(std::memory_order_relaxed) == 0 && b->capacity >= minCapacity) {
            b->offset           = 0;
            mActiveBackingIndex = i;
            return true;
        }
    }

    uint64_t capacity = roundUpToMb(std::max({DEFAULT_CAPACITY, minCapacity, mParams.suggestedArenaSize}));
    if (!mBackingBuffers.empty()) {
        capacity = std::max(capacity, roundUpToMb(mBackingBuffers.back()->capacity * 2));
    }

    if (!mGpu) GN_UNLIKELY {
        GN_ERROR(sLogger, "TransientArenaVulkan: no Vulkan GPU context");
        return false;
    }

    rv::Buffer::ConstructParameters cp;
    cp.gi     = mGpu->vulkanDevice().gi();
    cp.size   = capacity;
    cp.usage  = vk::BufferUsageFlagBits::eUniformBuffer | vk::BufferUsageFlagBits::eStorageBuffer |
                vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eIndexBuffer |
                vk::BufferUsageFlagBits::eTransferSrc | vk::BufferUsageFlagBits::eTransferDst |
                vk::BufferUsageFlagBits::eShaderDeviceAddress;
    cp.memory = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent;

    auto vkBuf = rv::Ref<rv::Buffer>::make(cp);
    if (!vkBuf || !vkBuf->handle()) GN_UNLIKELY {
        GN_ERROR(sLogger, "TransientArenaVulkan: failed to create backing VkBuffer ({}B)", capacity);
        return false;
    }

    auto back      = std::make_unique<BackingBuffer>();
    back->buffer   = std::move(vkBuf);
    back->capacity = capacity;

    // Persistently map host-visible memory
    auto mapped = back->buffer->map({});
    if (!mapped.data) GN_UNLIKELY {
        GN_ERROR(sLogger, "TransientArenaVulkan: failed to map backing VkBuffer");
        return false;
    }
    back->mappedPtr = mapped.data;

    // Query 64-bit device address
    vk::BufferDeviceAddressInfo info {};
    info.buffer         = back->buffer->handle();
    back->deviceAddress = mGpu->vulkanDevice().handle().getBufferAddress(info);

    mActiveBackingIndex = mBackingBuffers.size();
    mBackingBuffers.push_back(std::move(back));
    return true;
}

AutoRef<Buffer> TransientArenaVulkan::allocate(uint64_t size, uint64_t alignment, const char * bufferName) {
    if (!size) GN_UNLIKELY {
        GN_ERROR(sLogger, "TransientArenaVulkan::allocate: size is 0");
        return {};
    }

    const uint64_t effectiveAlignment = alignment ? alignment : mDefaultAlignment;
    const uint64_t alignedSize        = alignUp(size, effectiveAlignment);

    std::lock_guard<std::mutex> lock(mAllocateMutex);

    if (mBackingBuffers.empty() && !createBackingBuffer(alignedSize)) GN_UNLIKELY return {};

    for (;;) {
        auto *         back = mBackingBuffers[mActiveBackingIndex].get();
        const uint64_t cap  = back->capacity;
        const uint64_t off  = alignUp(back->offset, effectiveAlignment);
        if (off + alignedSize <= cap) {
            back->offset = off + alignedSize;
            return AutoRef<TransientBufferVulkan>(
                new TransientBufferVulkan(bufferName ? bufferName : "transient-buffer", AutoRef<TransientArenaVulkan>(this), back, off, size));
        }
        if (!createBackingBuffer(alignedSize)) GN_UNLIKELY return {};
    }
}

void TransientArenaVulkan::reset() {
    std::lock_guard<std::mutex> lock(mAllocateMutex);
    mActiveBackingIndex = 0;
    for (auto & b : mBackingBuffers) {
        if (b->liveCount.load(std::memory_order_relaxed) == 0 && b->mappedCount.load(std::memory_order_relaxed) == 0) {
            b->offset = 0;
        }
    }
}

} // namespace GN::gpu2
