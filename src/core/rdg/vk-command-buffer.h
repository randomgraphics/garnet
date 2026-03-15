#pragma once
#include "submission.h"
#include "vk-gpu-context.h"
#include <garnet/base/array.h>

namespace GN::rdg {

class CommandBufferManagerVulkan : public SlotBase {
public:
    struct ConstructParameters {
        AutoRef<GpuContextVulkan> gpu;
        SubmissionImpl &          submission;
    };

    enum CommandBufferType {
        GRAPHICS,
        COMPUTE,
        TRANSFER,
    };

    /// Command buffer and queue from execute(). Use commandBuffer.render(drawPack) to record a DrawPack.
    struct CommandBuffer : public NoCopy {
        CommandBuffer() { GN_ASSERT(Action::ExecutionResult::FAILED == *this); }
        CommandBuffer(CommandBufferManagerVulkan & manager, const TaskInfo & taskInfo, rapid_vulkan::CommandBuffer && commandBuffer,
                      rapid_vulkan::CommandQueue * queue)
            : mManager(&manager), mTaskInfo(&taskInfo), mCommandBuffer(std::move(commandBuffer)), mQueue(queue) {}

        CommandBuffer(CommandBuffer && o): mManager(o.mManager), mTaskInfo(o.mTaskInfo), mCommandBuffer(std::move(o.mCommandBuffer)), mQueue(o.mQueue) {
            GN_ASSERT(!o.mCommandBuffer);
            o.mManager  = nullptr;
            o.mTaskInfo = nullptr;
            o.mQueue    = nullptr;
        }

        ~CommandBuffer() {
            if (mManager && mCommandBuffer && mQueue) { mManager->submit(*this); }
        }

        Action::ExecutionResult result() const { return mManager && mCommandBuffer ? Action::ExecutionResult::PASSED : Action::ExecutionResult::FAILED; }

        rapid_vulkan::CommandBuffer & commandBuffer() { return mCommandBuffer; }

        CommandBuffer & operator=(CommandBuffer && o) {
            if (this == &o) return *this;
            mManager       = o.mManager;
            mTaskInfo      = o.mTaskInfo;
            mCommandBuffer = std::move(o.mCommandBuffer);
            mQueue         = o.mQueue;
            o.mManager     = nullptr;
            o.mTaskInfo    = nullptr;
            o.mQueue       = nullptr;
            GN_ASSERT(!o.mCommandBuffer);
            return *this;
        }

        operator Action::ExecutionResult() const { return result(); }

    private:
        CommandBufferManagerVulkan * mManager       = nullptr;
        const TaskInfo *             mTaskInfo      = nullptr;
        rapid_vulkan::CommandBuffer  mCommandBuffer = {};
        rapid_vulkan::CommandQueue * mQueue         = nullptr;
        friend class CommandBufferManagerVulkan;
    };

    CommandBufferManagerVulkan(const ConstructParameters & params);

    ~CommandBufferManagerVulkan();

    /// Called by task in prepare pass to ask for certain type of command buffer.
    Action::ExecutionResult prepare(TaskInfo & taskInfo, CommandBufferType type);

    /// Called by task in execution pass to retrieve the requested command buffer.
    /// \return The command buffer and a flag indicating if the caller needs to submit the command buffer to the queue.
    ///         If the submit flag is set, the caller must submit the command buffer to the queue.
    CommandBuffer execute(TaskInfo & taskInfo);

    /// Called by CommandBuffer's destructor to submit itself to the queue. Do not call this function anywhere else.
    void submit(CommandBuffer & commandBuffer);

private:
    struct Entry {
        CommandBufferType                             type {};
        uint64_t                                      taskIndex {};
        rapid_vulkan::Ref<rapid_vulkan::CommandQueue> queue {};
        rapid_vulkan::CommandBuffer                   commandBuffer {};
    };

    typedef std::map<rapid_vulkan::CommandQueue *, DynaArray<rapid_vulkan::CommandQueue::SubmissionID>> SubmissionIDMap;

    rapid_vulkan::Ref<rapid_vulkan::CommandQueue> getQueueForType(CommandBufferType type) const;

    AutoRef<GpuContextVulkan>                     mGpu;
    rapid_vulkan::Ref<rapid_vulkan::CommandQueue> mGraphicsQueue;
    rapid_vulkan::Ref<rapid_vulkan::CommandQueue> mComputeQueue;
    rapid_vulkan::Ref<rapid_vulkan::CommandQueue> mTransferQueue;
    std::map<uint64_t, Entry>                     mEntries; // key is task index.
    SubmissionIDMap                               mSubmissionIDs;

    void waitForIdle(SubmissionImpl &) {
        for (auto & [queue, submissionIDs] : mSubmissionIDs) {
            vk::ArrayProxy<const rapid_vulkan::CommandQueue::SubmissionID> ids((uint32_t) submissionIDs.size(), submissionIDs.data());
            queue->wait(ids);
        }
        mSubmissionIDs.clear();
    }
};

} // namespace GN::rdg