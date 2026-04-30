#pragma once
// DISABLED: GpuCommandBuffer / GpuCommandBufferVulkan2 are superseded by GpuPayloadVulkan::recordForVulkanSubmit.
// Kept for reference during the transition; delete when GpuCommandBuffer itself is removed.
#if 0

    #include "gpu-command-buffer.h"
    #include "vk-gpu-context.h"

    #include <vector>

namespace GN::gfx::gpu2 {

// Vulkan backend completes the opaque nested types from \c gpu.h (single definition for all rdg2 Vulkan TUs).
struct GpuCommandBuffer::Payload {
    virtual ~Payload()                   = default;
    Payload()                            = default;
    Payload(const Payload &)             = delete;
    Payload & operator=(const Payload &) = delete;

    virtual void recordForVulkanSubmit(void * vulkanRecordContext) = 0;

    /// After submitted GPU work has finished (\c waitIdle), write each resource's tracked latest state from this payload.
    virtual void flushResourceStatesAfterSubmit() = 0;
};

struct GpuCommandBuffer::Fence {
    virtual ~Fence()                 = default;
    Fence()                          = default;
    Fence(const Fence &)             = delete;
    Fence & operator=(const Fence &) = delete;
};

/// Passed to \c GpuCommandBuffer::Payload::recordForVulkanSubmit from the Vulkan submit path.
struct VulkanSubmitRecordContext {
    const rv::Device * dev   = nullptr;
    rv::CommandQueue * queue = nullptr;
    rv::CommandBuffer  cmd;
};

class GpuCommandBufferVulkan2 final : public GpuCommandBufferCommon {
public:
    GN_REGISTER_RUNTIME_TYPE(GpuCommandBufferCommon);

    explicit GpuCommandBufferVulkan2(const StrA & entityName, AutoRef<GpuContextVulkan2> gpu);
    ~GpuCommandBufferVulkan2() override;

    GpuCommandBuffer::Fence * submit(const SubmitParameters & sp) override;
    GpuCommandBuffer::Fence * submitVk(const SubmitParameters & sp, const std::vector<vk::Semaphore> & waitSemaphores);
    void adoptPayloadFromSeal(GpuCommandBuffer::Payload * p);

private:
    AutoRef<GpuContextVulkan2> vkGpu() const;
    ArrayContainer<GpuCommandBuffer::Payload *> mPendingSealedPayloads;
};

AutoRef<GpuCommandBuffer> createGpuCommandBufferVulkan2(const GpuCommandBuffer::CreateParameters & params);

} // namespace GN::gfx::gpu2

#endif // 0
