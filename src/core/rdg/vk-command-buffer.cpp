#include "pch.h"
#include "vk-command-buffer.h"
#include "vk-buffer-state.h"
#include "vk-submission-context.h"
#include "vk-texture.h"
#include <garnet/rdg/artifacts.h>

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
Action::ExecutionResult CommandBufferManagerVulkan::prepare(TaskInfo & taskInfo, CommandBufferType type) {
    rapid_vulkan::Ref<rapid_vulkan::CommandQueue> queue = getQueueForType(type);
    if (queue.empty()) GN_UNLIKELY {
            return Action::FAILED;
        }

    GN_ASSERT(mEntries.find(taskInfo.index) == mEntries.end());
    mEntries[taskInfo.index] = Entry {type, taskInfo.index, queue};

    return Action::PASSED;
};

CommandBufferManagerVulkan::CommandBuffer CommandBufferManagerVulkan::execute(TaskInfo & taskInfo) {
    auto iter = mEntries.find(taskInfo.index);
    if (iter == mEntries.end()) GN_UNLIKELY {
            GN_ERROR(sLogger)("CommandBufferManagerVulkan::execute: task index {} not found (mEntries.size()={})", taskInfo.index, mEntries.size());
            return {};
        }
    Entry & e = iter->second;

    if (iter == mEntries.begin()) {
        // this is the first task that needs command buffer.
        e.commandBuffer = e.queue->begin(fmt::format("rdg-command-buffer ({})", taskInfo).c_str(), vk::CommandBufferLevel::ePrimary);
        if (!e.commandBuffer) GN_UNLIKELY {
                GN_ERROR(sLogger)("CommandBufferManagerVulkan::execute: queue->begin() returned empty for task {}", taskInfo.index);
                return {};
            }
    } else {
        // get the previous entry.
        auto    prevIter = std::prev(iter);
        Entry & prev     = prevIter->second;
        if (prev.type != e.type) {
            // the type has changed. need to start a new command buffer.
            e.commandBuffer = e.queue->begin(fmt::format("rdg-command-buffer ({})", taskInfo).c_str(), vk::CommandBufferLevel::ePrimary);
            if (!e.commandBuffer) GN_UNLIKELY {
                    GN_ERROR(sLogger)("CommandBufferManagerVulkan::execute: queue->begin() returned empty for task {} (type change)", taskInfo.index);
                    return {};
                }
        } else {
            // the type is the same. reuse the previous command buffer.
            e.commandBuffer = prev.commandBuffer;
        }
    }

    // check if we need to submit the command buffer.
    bool needToSubmit = (iter == std::prev(mEntries.end())) || (iter->second.type != std::next(iter)->second.type);

    // done
    return CommandBuffer(*this, taskInfo, std::move(e.commandBuffer), needToSubmit ? e.queue.get() : nullptr);
}

void CommandBufferManagerVulkan::submit(CommandBuffer & commandBuffer) {
    GN_ASSERT(commandBuffer.mManager == this && commandBuffer.mTaskInfo && commandBuffer.mCommandBuffer && commandBuffer.mQueue);
    GN_VERBOSE(sLogger)("{} - submitting command buffer to queue", *commandBuffer.mTaskInfo);
    for (auto & [tex, state] : commandBuffer.mTextureStates) {
        if (tex) tex->state().assignFrom(state, tex->name);
    }
    for (auto & [bb, imageMap] : commandBuffer.mBackbufferStates) {
        if (bb)
            for (auto & [image, transition] : imageMap) bb->assignFrom(image, transition);
    }
    for (auto & [buf, transition] : commandBuffer.mBufferStates) {
        if (buf) assignBufferState(buf, transition);
    }
    auto   submissionID = commandBuffer.mQueue->submit(rapid_vulkan::CommandQueue::SubmitParameters {.commandBuffers = {commandBuffer.mCommandBuffer}});
    auto   q            = commandBuffer.mQueue;
    auto & ids          = mSubmissionIDs[q];
    ids.append(submissionID);
    // // Distribute GPU completion token to all upload actions registered this submission.
    // // This lets each upload slot know which GPU submission is currently reading its data,
    // // so waitUntilReady() can block only when the slot is actually in flight.
    // auto sc = commandBuffer.mTaskInfo->submission.getSumissionContext<SubmissionContextVulkan>();
    // if (sc) {
    //     for (size_t i = 0; i < sc->activeUploads.size(); ++i) { sc->activeUploads[i]->notifyCompletion(submissionID); }
    // }
}

bool CommandBufferManagerVulkan::CommandBuffer::transitionTexture(TextureVulkan * tex, const GpuResourceView::SubresourceRange & range,
                                                                  const TextureState::ImageState & newState, TextureState::ImageStateTransitionFlags flags) {
    if (!tex) return false;
    auto it = mTextureStates.find(tex);
    if (it == mTextureStates.end()) { it = mTextureStates.emplace(tex, tex->state()).first; }
    return it->second.set(range, newState, flags, tex->name);
}

const TextureState::ImageStateTransition * CommandBufferManagerVulkan::CommandBuffer::getTextureState(TextureVulkan * tex, uint32_t mip,
                                                                                                      uint32_t arrayLayer) const {
    auto it = mTextureStates.find(tex);
    if (it == mTextureStates.end()) return nullptr;
    return it->second.get(mip, arrayLayer);
}

bool CommandBufferManagerVulkan::CommandBuffer::transitionBackbuffer(BackbufferVulkan * bb, const TextureState::ImageState & newState,
                                                                     TextureState::ImageStateTransitionFlags flags) {
    if (!bb) return false;
    const rapid_vulkan::Image * image = bb->backBufferImage();
    if (!image) return false;
    auto & perBb = mBackbufferStates[bb];
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
    auto it = mBackbufferStates.find(bb);
    if (it == mBackbufferStates.end()) return nullptr;
    auto it2 = it->second.find(image);
    if (it2 == it->second.end()) return nullptr;
    return &it2->second;
}

bool CommandBufferManagerVulkan::CommandBuffer::transitionBuffer(Buffer * buffer, vk::DeviceSize, vk::DeviceSize, vk::AccessFlags newAccess,
                                                                 vk::PipelineStageFlags newStage) {
    if (!buffer) return false;
    BufferStateTransition * resourceState = GN::rdg::getBufferState(buffer);
    if (!resourceState) return false;
    auto it = mBufferStates.find(buffer);
    if (it == mBufferStates.end()) it = mBufferStates.emplace(buffer, *resourceState).first;
    return it->second.transitTo(newAccess, newStage, buffer->name);
}

const BufferStateTransition * CommandBufferManagerVulkan::CommandBuffer::getBufferState(Buffer * buffer) const {
    if (!buffer) return nullptr;
    auto it = mBufferStates.find(buffer);
    if (it == mBufferStates.end()) return nullptr;
    return &it->second;
}

} // namespace GN::rdg
