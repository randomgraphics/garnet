#pragma once

#include "vk-gpu-context.h"

namespace GN::rdg {

/// Vulkan buffer usage conversion. Used by both PersistentBuffer and TransientArena backends.
vk::BufferUsageFlags bufferUsageToVk(BufferUsageFlags usage);

class PersistentBufferVulkan : public PersistentBuffer {
    AutoRef<GpuContextVulkan>               mGpu;
    uint64_t                                mAllocatedSize = 0; ///< Capacity of mVkBuffer (fixed at construction).
    rapid_vulkan::Ref<rapid_vulkan::Buffer> mVkBuffer;
    vk::Buffer                              mHandle = VK_NULL_HANDLE;

public:
    PersistentBufferVulkan(ArtifactDatabase & db, const StrA & name, AutoRef<GpuContextVulkan> gpu)
        : PersistentBuffer(db, TYPE_INFO(), name), mGpu(std::move(gpu)) {}

    /// Pre-allocate the VkBuffer from CreateParameters. Called once by the factory.
    bool allocate(const CreateParameters & params);

    bool setContent(const void * data, uint64_t size) override;

    AutoRef<Blob> readback() const override;

    GpuContext & gpu() const override { return *mGpu; }

    vk::Buffer                              handle() const { return mHandle; }
    rapid_vulkan::Ref<rapid_vulkan::Buffer> rapid() const { return mVkBuffer; }
    uint64_t                                size() const { return mAllocatedSize; }

private:
};

AutoRef<PersistentBuffer> createVulkanPersistentBuffer(ArtifactDatabase & db, const StrA & name, const PersistentBuffer::CreateParameters & params);

} // namespace GN::rdg
