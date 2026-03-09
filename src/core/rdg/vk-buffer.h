#pragma once

#include "vk-gpu-context.h"

namespace GN::rdg {

class BufferVulkan : public Buffer {
    AutoRef<GpuContextVulkan>               mGpu;
    rapid_vulkan::Ref<rapid_vulkan::Buffer> mVkBuffer;
    uint64_t                                mSize = 0;

public:
    BufferVulkan(ArtifactDatabase & db, const StrA & name, AutoRef<GpuContextVulkan> gpu): Buffer(db, TYPE_ID, TYPE_NAME, name), mGpu(std::move(gpu)) {}

    GpuContext & gpu() const override { return *mGpu; }

    bool setContent(const void * data, uint64_t size) override;

    vk::Buffer vkHandle() const { return mVkBuffer ? mVkBuffer->desc().handle : vk::Buffer {}; }

    /// For use with rapid_vulkan::Drawable::v() (BufferView). Returns null ref if buffer not yet created.
    rapid_vulkan::Ref<rapid_vulkan::Buffer> refVkBuffer() const { return mVkBuffer; }
};

AutoRef<Buffer> createVulkanBuffer(ArtifactDatabase & db, const StrA & name, const Buffer::CreateParameters & params);

} // namespace GN::rdg
