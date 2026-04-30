#include "pch.h"
#include "vk-command-buffer.h"
#include "vk-buffer-state.h"
#include "vk-submission-context.h"
#include "vk-texture.h"

namespace GN::rdg {

static GN::Logger * sLogger = GN::getLogger("GN.rdg");

CommandBufferManagerVulkan::CommandBufferManagerVulkan(const ConstructParameters & params) {
    GN_REQUIRE(params.gpu, "GPU context can't be empty.");
    mGpu                                 = params.gpu;
    const rapid_vulkan::Device &     dev = mGpu->device();
    const rapid_vulkan::GlobalInfo * gi  = dev.gi();
    rapid_vulkan::CommandQueue *     gq  = dev.graphics();
    if (!gq) GN_UNLIKELY {
            return;
        }
    mGraphicsQueue.reset(new rapid_vulkan::CommandQueue(rapid_vulkan::CommandQueue::ConstructParameters {{"rdg-graphics"}, gi, gq->family(), gq->index()}));
    rapid_vulkan::CommandQueue * cq = dev.compute();
    if (cq && cq != gq) {
        mComputeQueue.reset(new rapid_vulkan::CommandQueue(rapid_vulkan::CommandQueue::ConstructParameters {{"rdg-compute"}, gi, cq->family(), cq->index()}));
    } else {
        mComputeQueue = mGraphicsQueue;
    }
    rapid_vulkan::CommandQueue * tq = dev.transfer();
    if (tq && tq != gq) {
        mTransferQueue.reset(new rapid_vulkan::CommandQueue(rapid_vulkan::CommandQueue::ConstructParameters {{"rdg-transfer"}, gi, tq->family(), tq->index()}));
    } else {
        mTransferQueue = mGraphicsQueue;
    }

    // drain all submitted command buffers at the end of execution.
    connectToSignal<&CommandBufferManagerVulkan::waitForIdle>(params.submission.endOfSubmission);
}

CommandBufferManagerVulkan::~CommandBufferManagerVulkan() {
    // by the time the destructor is called, all submissions should have been completed.
    GN_ASSERT(mSubmissionIDs.empty());
}

rapid_vulkan::Ref<rapid_vulkan::CommandQueue> CommandBufferManagerVulkan::getQueueForType(CommandBufferType type) const {
    switch (type) {
    case GRAPHICS:
        return mGraphicsQueue;
    case COMPUTE:
        return mComputeQueue.empty() ? mGraphicsQueue : mComputeQueue;
    case TRANSFER:
        return mTransferQueue.empty() ? mGraphicsQueue : mTransferQueue;
    }
    return mGraphicsQueue;
}

// Collecting all tasks that needs a command buffer, create an entry for each of them.
Action::ExecutionResult CommandBufferManagerVulkan::prepare(TaskInfo & taskInfo) {
    mCommandEntry.theVeryLastTaskIndexWeHaveSeen = taskInfo.index;
    return Action::PASSED;
};

CommandBufferManagerVulkan::CommandProxy CommandBufferManagerVulkan::execute(TaskInfo & taskInfo, CommandBufferType bufferType) {
    // See if we need a new command buffer.
    if (!mCommandEntry.prevCommandBuffer || mCommandEntry.prevCommandBuffer->type != bufferType) {
        // submit the previous command buffer before creating a new one.
        if (mCommandEntry.prevCommandBuffer) { submit(taskInfo, *mCommandEntry.prevCommandBuffer); }

        // create a new command buffer.
        auto q = getQueueForType(bufferType);
        auto b = std::make_shared<CommandBuffer>(bufferType, q,
                                                 q->begin(fmt::format("rdg-command-buffer ({})", taskInfo).c_str(), vk::CommandBufferLevel::ePrimary));
        if (!b) GN_UNLIKELY {
                GN_ERROR(sLogger)("CommandBufferManagerVulkan::execute: queue->begin() returned empty for task {}", taskInfo.index);
                return {};
            }

        mCommandEntry.prevCommandBuffer = std::move(b);
    }

    // Mark the command buffer as needing submission if we are processing the very last task.
    bool needToSubmit = (mCommandEntry.theVeryLastTaskIndexWeHaveSeen == taskInfo.index);

    // done
    GN_ASSERT(mCommandEntry.prevCommandBuffer);
    return CommandProxy(*this, taskInfo, mCommandEntry.prevCommandBuffer.get(), needToSubmit);
}

void CommandBufferManagerVulkan::submit(const TaskInfo & taskInfo, const CommandBuffer & cb) {
    GN_VERBOSE(sLogger)("{} - submitting command buffer to queue", taskInfo);
    for (auto & [tex, state] : cb.textureStates) {
        if (tex) tex->state().assignFrom(state, tex->name);
    }
    for (auto & [bb, imageMap] : cb.backbufferStates) {
        if (bb)
            for (auto & [image, transition] : imageMap) bb->assignFrom(image, transition);
    }
    for (auto & [buf, transition] : cb.bufferStates) {
        if (buf) assignBufferState(buf, transition);
    }
    auto   submissionID = cb.queue->submit(rapid_vulkan::CommandQueue::SubmitParameters {.commandBuffers = {cb.commandBuffer}});
    auto & ids          = mSubmissionIDs[cb.queue.get()];
    ids.append(submissionID);
}

bool CommandBufferManagerVulkan::CommandBuffer::transitionTexture(TextureVulkan * tex, const GpuResourceView::SubresourceRange & range,
                                                                  const TextureState::ImageState & newState, TextureState::ImageStateTransitionFlags flags) {
    if (!tex) return false;
    auto it = textureStates.find(tex);
    if (it == textureStates.end()) {
        auto insertIt = textureStates.emplace(tex, tex->state());
        it            = insertIt.first;
    }
    return it->second.set(range, newState, flags, tex->name);
}

const TextureState::ImageStateTransition * CommandBufferManagerVulkan::CommandBuffer::getTextureState(TextureVulkan * tex, uint32_t mip,
                                                                                                      uint32_t arrayLayer) const {
    auto it = textureStates.find(tex);
    if (it == textureStates.end()) return nullptr;
    return it->second.get(mip, arrayLayer);
}

bool CommandBufferManagerVulkan::CommandBuffer::transitionBackbuffer(BackbufferVulkan * bb, const TextureState::ImageState & newState,
                                                                     TextureState::ImageStateTransitionFlags flags) {
    if (!bb) return false;
    const rapid_vulkan::Image * image = bb->backBufferImage();
    if (!image) return false;
    auto & perBb = backbufferStates[bb];
    auto   it    = perBb.find(image);
    if (it == perBb.end()) it = perBb.emplace(image, bb->getImageState()).first;
    bool changed = (it->second.curr != newState);
    it->second.transitTo(newState, flags);
    return changed;
}

const TextureState::ImageStateTransition * CommandBufferManagerVulkan::CommandBuffer::getBackbufferState(BackbufferVulkan * bb) const {
    if (!bb) return nullptr;
    const rapid_vulkan::Image * image = bb->backBufferImage();
    if (!image) return nullptr;
    auto it = backbufferStates.find(bb);
    if (it == backbufferStates.end()) return nullptr;
    auto it2 = it->second.find(image);
    if (it2 == it->second.end()) return nullptr;
    return &it2->second;
}

bool CommandBufferManagerVulkan::CommandBuffer::transitionBuffer(Buffer * buffer, vk::DeviceSize, vk::DeviceSize, vk::AccessFlags newAccess,
                                                                 vk::PipelineStageFlags newStage) {
    if (!buffer) return false;
    BufferStateTransition * resourceState = GN::rdg::getBufferState(buffer);
    if (!resourceState) return false;
    auto it = bufferStates.find(buffer);
    if (it == bufferStates.end()) it = bufferStates.emplace(buffer, *resourceState).first;
    return it->second.transitTo(newAccess, newStage, buffer->name);
}

const BufferStateTransition * CommandBufferManagerVulkan::CommandBuffer::getBufferState(Buffer * buffer) const {
    if (!buffer) return nullptr;
    auto it = bufferStates.find(buffer);
    if (it == bufferStates.end()) return nullptr;
    return &it->second;
}

} // namespace GN::rdg
