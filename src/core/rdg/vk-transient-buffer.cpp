#include "pch.h"
#include "vk-transient-buffer.h"
#include <mutex>

static GN::Logger * sLogger = GN::getLogger("GN.rdg");

namespace GN::rdg {

// ---------------------------------------------------------------------------
// TransientBufferVulkan
// ---------------------------------------------------------------------------

TransientBufferVulkan::TransientBufferVulkan(const StrA & name, TransientArenaVulkan * owner, TransientArenaVulkan::BackingBuffer * backing, uint64_t offset,
                                             uint64_t size)
    : TransientBuffer(TYPE_INFO(), name), mOwner(owner), mBacking(backing), mOffset(offset), mSize(size) {
    mBacking->liveCount.fetch_add(1, std::memory_order_relaxed);
}

TransientBufferVulkan::~TransientBufferVulkan() { mBacking->liveCount.fetch_sub(1, std::memory_order_release); }

TransientBuffer::Mapped TransientBufferVulkan::map() {
    uint8_t * ptr = nullptr;
    {
        std::lock_guard<std::mutex> lock(mBacking->mapMutex);
        if (!mBacking->mappedPtr) {
            auto mapped = mBacking->buffer->map({});
            if (!mapped.data) GN_UNLIKELY {
                    GN_ERROR(sLogger)("TransientBufferVulkan::map: failed to map backing buffer");
                    mBacking->mappedCount.fetch_add(1, std::memory_order_relaxed); // so unmap() on destroy is valid
                    return TransientBuffer::Mapped(*this, nullptr, 0);
                }
            mBacking->mappedPtr = mapped.data;
        }
        mBacking->mappedCount.fetch_add(1, std::memory_order_relaxed);
        ptr = mBacking->mappedPtr;
    }
    return TransientBuffer::Mapped(*this, ptr + mOffset, mSize);
}

void TransientBufferVulkan::unmap(const TransientBuffer::Mapped &) {
    std::lock_guard<std::mutex> lock(mBacking->mapMutex);
    auto                        prev = mBacking->mappedCount.fetch_sub(1, std::memory_order_release);
    if (prev == 1) {
        mBacking->buffer->unmap();
        mBacking->mappedPtr = nullptr;
    }
}

bool TransientBufferVulkan::setContent(const void * data, uint64_t size) {
    if (!data || !size) GN_UNLIKELY return true;
    if (size > mSize) {
        GN_ERROR(sLogger)("TransientBufferVulkan::setContent: size {} exceeds allocated {}", size, mSize);
        return false;
    }
    auto m = map();
    if (!m.data()) return false;
    memcpy(m.data(), data, size);
    m.unmap();
    return true;
}

AutoRef<Blob> TransientBufferVulkan::readback() const {
    if (!mSize) return {};
    auto m = const_cast<TransientBufferVulkan *>(this)->map();
    if (!m.data()) return {};
    auto blob = referenceTo(new SimpleBlob<uint8_t>(mSize, static_cast<const uint8_t *>(m.data())));
    m.unmap();
    return blob;
}

// ---------------------------------------------------------------------------
// TransientBufferPoolVulkan
// ---------------------------------------------------------------------------

TransientArenaVulkan::TransientArenaVulkan(const StrA & name, const CreateParameters & params): TransientArena(TYPE_INFO(), name), mParams(params) {
    auto gpu = mParams.context.staticCastTo<GpuContextVulkan>();
    GN_ASSERT(gpu);
    auto props = gpu->globalInfo().physical.getProperties();
    mAlignment = std::max<uint64_t>(props.limits.minUniformBufferOffsetAlignment, 16u);
}

TransientArenaVulkan::~TransientArenaVulkan() {
    for (size_t i = 0; i < mBackingBuffers.size(); ++i) {
        auto * b      = mBackingBuffers[i].get();
        auto   mapped = b->mappedCount.load(std::memory_order_relaxed);
        auto   live   = b->liveCount.load(std::memory_order_relaxed);
        if (mapped != 0 || live != 0) GN_UNLIKELY {
                GN_WARN(sLogger)("TransientArenaVulkan::~TransientArenaVulkan: backing buffer[{}] has mappedCount={}, liveCount={} (expected 0)", i, mapped,
                                 live);
            }
    }
}

static uint64_t alignUp(uint64_t value, uint64_t alignment) { return (value + alignment - 1) & ~(alignment - 1); }

static constexpr uint64_t MB = 1024u * 1024u;
static uint64_t           roundUpToMb(uint64_t value) { return ((value + MB - 1) / MB) * MB; }

bool TransientArenaVulkan::createBackingBuffer(uint64_t minCapacity) {
    // Try to reuse a recyclable backing buffer. Caller holds mAllocateMutex.
    for (size_t i = 0; i < mBackingBuffers.size(); ++i) {
        auto * b = mBackingBuffers[i].get();
        if (b->liveCount.load(std::memory_order_relaxed) == 0 && b->mappedCount.load(std::memory_order_relaxed) == 0 && b->capacity >= minCapacity) {
            b->offset           = 0;
            mActiveBackingIndex = i;
            return true;
        }
    }
    uint64_t capacity = roundUpToMb(std::max({DEFAULT_CAPACITY, minCapacity, mParams.suggestedArenaSize}));
    // Geometric growth: when adding a new backing (current one full), make it at least 2x the previous size
    // rounded to the next MB. Avoids creating many small buffers as usage grows.
    if (!mBackingBuffers.empty()) { capacity = std::max(capacity, roundUpToMb(mBackingBuffers.back()->capacity * 2)); }

    auto gpu = mParams.context.staticCastTo<GpuContextVulkan>();
    GN_ASSERT(gpu);

    rapid_vulkan::Buffer::ConstructParameters cp;
    cp.gi      = &gpu->globalInfo();
    cp.size    = capacity;
    cp.usage   = bufferUsageToVk(mParams.usage) | vk::BufferUsageFlagBits::eTransferSrc | vk::BufferUsageFlagBits::eTransferDst;
    cp.memory  = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent;
    auto vkBuf = rapid_vulkan::Ref<rapid_vulkan::Buffer>::make(cp);
    if (!vkBuf || !vkBuf->desc().handle) GN_UNLIKELY {
            GN_ERROR(sLogger)("TransientBufferPoolVulkan: failed to create backing VkBuffer ({}B)", capacity);
            return false;
        }
    auto back           = std::make_unique<BackingBuffer>();
    back->buffer        = std::move(vkBuf);
    back->capacity      = capacity;
    mActiveBackingIndex = mBackingBuffers.size();
    mBackingBuffers.push_back(std::move(back));
    return true;
}

AutoRef<TransientBuffer> TransientArenaVulkan::allocate(uint64_t size, const char * name_) {
    if (!size) GN_UNLIKELY {
            GN_ERROR(sLogger)("TransientBufferPoolVulkan::allocate: size is 0");
            return {};
        }

    const uint64_t alignedSize = alignUp(size, mAlignment);

    std::lock_guard<std::mutex> lock(mAllocateMutex);

    if (mBackingBuffers.empty() && !createBackingBuffer(alignedSize)) GN_UNLIKELY return {};

    for (;;) {
        auto *         back = mBackingBuffers[mActiveBackingIndex].get();
        const uint64_t cap  = back->capacity;
        const uint64_t off  = alignUp(back->offset, mAlignment); // ensure offset meets minUniformBufferOffsetAlignment for UBO binding
        if (off + alignedSize <= cap) {
            back->offset = off + alignedSize;
            return AutoRef<TransientBufferVulkan>(new TransientBufferVulkan(name_, this, back, off, alignedSize));
        }
        if (!createBackingBuffer(alignedSize)) GN_UNLIKELY return {};
    }
}

AutoRef<TransientArena> createVulkanTransientArena(const StrA & name, const TransientArena::CreateParameters & params) {
    auto gpu = params.context.staticCastTo<GpuContextVulkan>();
    if (!gpu) GN_UNLIKELY {
            GN_ERROR(sLogger)("createVulkanTransientArena: no GPU context, name='{}'", name);
            return {};
        }
    return AutoRef<TransientArena>(new TransientArenaVulkan(name, params));
}

} // namespace GN::rdg
