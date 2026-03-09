#include "pch.h"
#include "vk-command-buffer.h"

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
}

CommandBufferManagerVulkan::~CommandBufferManagerVulkan() {}

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
            GN_ERROR(sLogger)("CommandBufferManagerVulkan::execute: task index {} not found", taskInfo.index);
            return {};
        }
    Entry & e = iter->second;

    if (iter == mEntries.begin()) {
        // this is the first task that needs command buffer.
        e.commandBuffer = e.queue->begin(fmt::format("rdg-command-buffer ({})", taskInfo).c_str(), vk::CommandBufferLevel::ePrimary);
    } else {
        // get the previous entry.
        auto    prevIter = std::prev(iter);
        Entry & prev     = prevIter->second;
        if (prev.type != e.type) {
            // the type has changed. need to start a new command buffer.
            e.commandBuffer = e.queue->begin(fmt::format("rdg-command-buffer ({})", taskInfo).c_str(), vk::CommandBufferLevel::ePrimary);
        } else {
            // the type is the same. reuse the previous command buffer.
            e.commandBuffer = prev.commandBuffer;
        }
    }

    // check if we need to submit the command buffer.
    bool needToSubmit = (iter == std::prev(mEntries.end())) || (iter->second.type != std::next(iter)->second.type);

    // done
    return CommandBuffer {e.queue, e.commandBuffer, needToSubmit};
}

} // namespace GN::rdg
