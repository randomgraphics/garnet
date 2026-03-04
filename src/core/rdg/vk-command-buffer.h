#pragma once
#include "submission.h"
#include "vk-gpu-context.h"
#include <garnet/base/array.h>

namespace GN::rdg {

class CommandBufferManagerVulkan {
public:
    struct ConstructParameters {
        AutoRef<GpuContextVulkan> gpu;
    };

    enum CommandBufferType {
        GRAPHICS,
        COMPUTE,
        TRANSFER,
    };

    struct CommandBuffer {
        rapid_vulkan::Ref<rapid_vulkan::CommandQueue> queue;
        rapid_vulkan::CommandBuffer                   commandBuffer {};
        bool                                          submit {};
    };

    CommandBufferManagerVulkan(const ConstructParameters & params);

    ~CommandBufferManagerVulkan();

    /// Called by task in prepare pass to ask for certain type of command buffer.
    Action::ExecutionResult prepare(TaskInfo & taskInfo, CommandBufferType type);

    /// Called by task in execution pass to retrieve the requested command buffer.
    /// \return The command buffer and a flag indicating if the caller needs to submit the command buffer to the queue.
    ///         If the submit flag is set, the caller must submit the command buffer to the queue.
    CommandBuffer execute(TaskInfo & taskInfo);

private:
    struct Entry {
        CommandBufferType                             type {};
        uint64_t                                      taskIndex {};
        rapid_vulkan::Ref<rapid_vulkan::CommandQueue> queue {};
        rapid_vulkan::CommandBuffer                   commandBuffer {};
    };

    rapid_vulkan::Ref<rapid_vulkan::CommandQueue> getQueueForType(CommandBufferType type) const;

    AutoRef<GpuContextVulkan>                     mGpu;
    rapid_vulkan::Ref<rapid_vulkan::CommandQueue> mGraphicsQueue;
    rapid_vulkan::Ref<rapid_vulkan::CommandQueue> mComputeQueue;
    rapid_vulkan::Ref<rapid_vulkan::CommandQueue> mTransferQueue;
    std::map<uint64_t, Entry>                     mEntries; // key is task index.
};

} // namespace GN::rdg