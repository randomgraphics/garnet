#pragma once
#include "submission.h"
#include "vk-gpu-context.h"
#include <garnet/base/array.h>

namespace GN::rdg {

class CommandBufferManagerVulkan : public SubmissionImpl::Context {
public:
    inline static constexpr Guid TYPE = {0x6ad8b59d, 0xe672, 0x4b5e, {0x8e, 0xec, 0xf7, 0xac, 0xd4, 0xf1, 0x99, 0xdd}};

    struct ConstructParameters {
        AutoRef<GpuContext> gpu;
    };

    enum CommandBufferType {
        GRAPHICS,
        COMPUTE,
        TRANSFER,
    };

    struct CommandBuffer {
        rapid_vulkan::Ref<rapid_vulkan::CommandQueue> queue;
        rapid_vulkan::CommandBuffer *                 commandBuffer {};
        bool                                          submit {};
    };

    CommandBufferManagerVulkan(const ConstructParameters & params);

    ~CommandBufferManagerVulkan() override;

    /// Called by task in prepare pass to ask for certain type of command buffer.
    /// \return Returns a unique identifier for this request. 0 for failure.
    uint64_t prepare(CommandBufferType type);

    /// Called by task in execution pass to retrieve the requested command buffer.
    /// If the submit flag is set, the caller must submit the command buffer to the queue, or drop it on failure.
    CommandBuffer execute(uint64_t commandBufferId);

private:
    struct Entry {
        CommandBufferType                             type {};
        bool                                          submit {};
        rapid_vulkan::Ref<rapid_vulkan::CommandQueue> queue;
        rapid_vulkan::CommandBuffer                   commandBuffer;
    };

    rapid_vulkan::Ref<rapid_vulkan::CommandQueue> getQueueForType(CommandBufferType type) const;

    AutoRef<GpuContextVulkan>                     mGpu;
    rapid_vulkan::Ref<rapid_vulkan::CommandQueue> mGraphicsQueue;
    rapid_vulkan::Ref<rapid_vulkan::CommandQueue> mComputeQueue;
    rapid_vulkan::Ref<rapid_vulkan::CommandQueue> mTransferQueue;
    DynaArray<Entry>                              mEntries;
};

} // namespace GN::rdg