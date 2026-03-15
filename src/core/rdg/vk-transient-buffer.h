#pragma once

#include "vk-persistent-buffer.h"
#include <atomic>
#include <memory>
#include <mutex>
#include <vector>
#include <vulkan/vulkan_enums.hpp>

namespace GN::rdg {

/// Grow-only pool of host-visible VkBuffers for transient allocations.
///
/// Backing buffers are created on demand with geometric growth.  Old buffers
/// are kept alive as long as live sub-allocations reference them (via
/// TransientBufferVulkan → AutoRef to the rapid_vulkan::Buffer).
///
/// TODO: implement a shrink policy — retire large under-used backing buffers
///       once all their sub-allocations are freed.
class TransientArenaVulkan : public TransientArena {
public:
    struct BackingBuffer {
        rapid_vulkan::Ref<rapid_vulkan::Buffer> buffer;
        uint8_t *             mappedPtr = nullptr; ///< Non-null only while map() is active. Lazy-mapped on first map, unmapped when last unmap.
        uint64_t              capacity  = 0;
        uint64_t              offset    = 0;   ///< Next free byte (bump allocator). Protected by mAllocateMutex.
        std::atomic<uint64_t> mappedCount {0}; ///< Mapped still outstanding. map()/unmap(). handle()/rapid() warn if > 0.
        std::atomic<uint64_t> liveCount {0};   ///< Live TransientBufferVulkan. When 0, recyclable. ctor/dtor.
        std::mutex            mapMutex;        ///< Protects lazy map/unmap of backing buffer.
    };

    TransientArenaVulkan(const StrA & name, const TransientArena::CreateParameters & params);

    ~TransientArenaVulkan() override;

    GpuContext & gpu() const override { return *mParams.context; }

    AutoRef<TransientBuffer> allocate(uint64_t size, const char * name) override;

    /// For testing: read-only access to arena state. Not thread-safe with concurrent allocate.
    size_t   numBackingBuffers() const { return mBackingBuffers.size(); }
    size_t   activeBackingIndex() const { return mActiveBackingIndex; }
    uint64_t backingCapacity(size_t i) const { return mBackingBuffers[i]->capacity; }
    uint64_t backingOffset(size_t i) const { return mBackingBuffers[i]->offset; }
    uint64_t backingLiveCount(size_t i) const { return mBackingBuffers[i]->liveCount.load(std::memory_order_relaxed); }
    uint64_t backingMappedCount(size_t i) const { return mBackingBuffers[i]->mappedCount.load(std::memory_order_relaxed); }

private:
    CreateParameters                            mParams;
    std::vector<std::unique_ptr<BackingBuffer>> mBackingBuffers;         // unique_ptr so resize doesn't move non-movable BackingBuffer
    size_t                                      mActiveBackingIndex = 0; ///< Index of backing buffer used for next allocation. Protected by mAllocateMutex.
    std::mutex                                  mAllocateMutex;          ///< Protects entire allocate() path.
    uint64_t                                    mAlignment       = 256;  ///< Sub-allocation alignment (from device limits).
    static constexpr uint64_t                   DEFAULT_CAPACITY = 16u * 1024u * 1024u; // 16MB

    bool createBackingBuffer(uint64_t minCapacity);
};

/// A single sub-allocation from a backing VkBuffer in the transient pool.
class TransientBufferVulkan : public TransientBuffer {
    TransientArenaVulkan *                mOwner;
    TransientArenaVulkan::BackingBuffer * mBacking;
    uint64_t                              mOffset;
    uint64_t                              mSize;

public:
    TransientBufferVulkan(const StrA & name, TransientArenaVulkan * owner, TransientArenaVulkan::BackingBuffer * backing, uint64_t offset, uint64_t size);

    ~TransientBufferVulkan() override;

    GpuContext & gpu() const override { return mOwner->gpu(); }

    /// Returns VkBuffer for copy/bind. Warns if any Mapped is still outstanding (mappedCount > 0).
    vk::Buffer handle() const {
        if (mBacking->mappedCount.load(std::memory_order_relaxed) != 0) GN_UNLIKELY {
                GN_WARN(GN::getLogger("GN.rdg"))("TransientBufferVulkan::handle: mappedCount is {} (should be 0 before GPU use)",
                                                 mBacking->mappedCount.load(std::memory_order_relaxed));
            }
        return mBacking->buffer ? mBacking->buffer->handle() : vk::Buffer {};
    }

    rapid_vulkan::Ref<rapid_vulkan::Buffer> rapid() const {
        if (mBacking->mappedCount.load(std::memory_order_relaxed) != 0) GN_UNLIKELY {
                GN_WARN(GN::getLogger("GN.rdg"))("TransientBufferVulkan::rapid: mappedCount is {} (should be 0 before GPU use)",
                                                 mBacking->mappedCount.load(std::memory_order_relaxed));
            }
        return mBacking->buffer;
    }

    bool setContent(const void * data, uint64_t size) override;

    AutoRef<Blob> readback() const override;

    auto map() -> TransientBuffer::Mapped override;
    void unmap(const TransientBuffer::Mapped & mapped) override;

    uint64_t offset() const { return mOffset; }
    uint64_t size() const { return mSize; }
};

AutoRef<TransientArena> createVulkanTransientArena(const StrA & name, const TransientArena::CreateParameters & params);

/// Static helpers for Buffer → Vk handle / rapid_vulkan::Buffer. Three overloads per function: const Buffer &, const AutoRef<Buffer> &, const Buffer *.
struct BufferUtils {
    static inline vk::Buffer getHandle(const Buffer & buffer) {
        if (buffer.isKindOf<TransientBuffer>()) { return static_cast<const TransientBufferVulkan &>(buffer).handle(); }
        GN_ASSERT(buffer.isKindOf<PersistentBuffer>());
        return static_cast<const PersistentBufferVulkan &>(buffer).handle();
    }
    static inline vk::Buffer getHandle(const AutoRef<Buffer> & buffer) {
        if (!buffer) return VK_NULL_HANDLE;
        return getHandle(*buffer);
    }
    static inline vk::Buffer getHandle(const Buffer * buffer) {
        if (!buffer) return VK_NULL_HANDLE;
        return getHandle(*buffer);
    }

    static inline uint64_t getSize(const Buffer & buffer) {
        if (buffer.isKindOf<TransientBuffer>()) return static_cast<const TransientBufferVulkan &>(buffer).size();
        GN_ASSERT(buffer.isKindOf<PersistentBuffer>());
        return static_cast<const PersistentBufferVulkan &>(buffer).size();
    }
    static inline uint64_t getSize(const AutoRef<Buffer> & buffer) {
        if (!buffer) return 0;
        return getSize(*buffer);
    }
    static inline uint64_t getSize(const Buffer * buffer) {
        if (!buffer) return 0;
        return getSize(*buffer);
    }

    static inline rapid_vulkan::Ref<rapid_vulkan::Buffer> toRapid(const Buffer & buffer) {
        if (buffer.isKindOf<TransientBuffer>()) { return static_cast<const TransientBufferVulkan &>(buffer).rapid(); }
        GN_ASSERT(buffer.isKindOf<PersistentBuffer>());
        return static_cast<const PersistentBufferVulkan &>(buffer).rapid();
    }
    static inline rapid_vulkan::Ref<rapid_vulkan::Buffer> toRapid(const AutoRef<Buffer> & buffer) {
        if (!buffer) return {};
        return toRapid(*buffer);
    }
    static inline rapid_vulkan::Ref<rapid_vulkan::Buffer> toRapid(const Buffer * buffer) {
        if (!buffer) return {};
        return toRapid(*buffer);
    }
};

} // namespace GN::rdg
