#pragma once
#include "submission.h"
#include "vk-backbuffer.h"
#include "vk-buffer-state.h"
#include "vk-gpu-context.h"
#include "vk-texture.h"
#include <garnet/base/array.h>
#include <unordered_map>

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
        CommandBuffer() = default;
        CommandBuffer(CommandBufferManagerVulkan & manager, const TaskInfo & taskInfo, rapid_vulkan::CommandBuffer && commandBuffer,
                      rapid_vulkan::CommandQueue * queue)
            : mManager(&manager), mTaskInfo(&taskInfo), mCommandBuffer(std::move(commandBuffer)), mQueue(queue) {}

        CommandBuffer(CommandBuffer && o)
            : mManager(o.mManager), mTaskInfo(o.mTaskInfo), mCommandBuffer(std::move(o.mCommandBuffer)), mQueue(o.mQueue),
              mTextureStates(std::move(o.mTextureStates)), mBackbufferStates(std::move(o.mBackbufferStates)), mBufferStates(std::move(o.mBufferStates)) {
            GN_ASSERT(!o.mCommandBuffer);
            o.mManager  = nullptr;
            o.mTaskInfo = nullptr;
            o.mQueue    = nullptr;
        }

        ~CommandBuffer() {
            if (mManager && mCommandBuffer && mQueue) { mManager->submit(*this); }
        }

        bool valid() const { return mManager && mCommandBuffer; }
        // Action::ExecutionResult result() const { return mManager && mCommandBuffer ? Action::ExecutionResult::PASSED : Action::ExecutionResult::FAILED; }

        rapid_vulkan::CommandBuffer & commandBuffer() { return mCommandBuffer; }

        /// Texture layout tracker: use command-buffer state during recording; flush to resources on submit.
        /// If this is the first reference to \p tex in this command buffer, uses the texture's current state as source.
        /// Returns true if any subresource state changed (caller should record barriers from getTextureState).
        bool transitionTexture(TextureVulkan * tex, const GpuResourceView::SubresourceRange & range, const TextureState::ImageState & newState,
                               TextureState::ImageStateTransitionFlags flags = TextureState::ImageStateTransitionFlags::DEFAULT());
        /// Returns the transition for one subresource after transitionTexture; null if out of range or tex not tracked.
        const TextureState::ImageStateTransition * getTextureState(TextureVulkan * tex, uint32_t mip, uint32_t arrayLayer) const;

        /// Backbuffer layout tracker: same as texture (CB state during recording; flush to resource on submit).
        bool transitionBackbuffer(BackbufferVulkan * bb, const TextureState::ImageState & newState,
                                  TextureState::ImageStateTransitionFlags flags = TextureState::ImageStateTransitionFlags::DEFAULT());
        const TextureState::ImageStateTransition * getBackbufferState(BackbufferVulkan * bb) const;

        /// Buffer access/stage tracker: same pattern as texture (CB state during recording; flush on submit).
        /// \p offset and \p size are the range used for barriers; first reference uses buffer's current state.
        /// Returns true if state changed (caller should record buffer barrier from getBufferState).
        bool transitionBuffer(Buffer * buffer, vk::DeviceSize offset, vk::DeviceSize size, vk::AccessFlags newAccess, vk::PipelineStageFlags newStage);
        const BufferStateTransition * getBufferState(Buffer * buffer) const;

        CommandBuffer & operator=(CommandBuffer && o) {
            if (this == &o) return *this;
            mManager          = o.mManager;
            mTaskInfo         = o.mTaskInfo;
            mCommandBuffer    = std::move(o.mCommandBuffer);
            mQueue            = o.mQueue;
            mTextureStates    = std::move(o.mTextureStates);
            mBackbufferStates = std::move(o.mBackbufferStates);
            mBufferStates     = std::move(o.mBufferStates);
            o.mManager        = nullptr;
            o.mTaskInfo       = nullptr;
            o.mQueue          = nullptr;
            GN_ASSERT(!o.mCommandBuffer);
            return *this;
        }

        operator bool() const { return valid(); }

    private:
        typedef std::unordered_map<TextureVulkan *, TextureState>                                                                           TextureStateMap;
        typedef std::unordered_map<BackbufferVulkan *, std::unordered_map<const rapid_vulkan::Image *, TextureState::ImageStateTransition>> BackbufferStateMap;
        typedef std::unordered_map<Buffer *, BufferStateTransition>                                                                         BufferStateMap;

        CommandBufferManagerVulkan * mManager       = nullptr;
        const TaskInfo *             mTaskInfo      = nullptr;
        rapid_vulkan::CommandBuffer  mCommandBuffer = {};
        rapid_vulkan::CommandQueue * mQueue         = nullptr;
        TextureStateMap              mTextureStates;
        BackbufferStateMap           mBackbufferStates;
        BufferStateMap               mBufferStates;
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