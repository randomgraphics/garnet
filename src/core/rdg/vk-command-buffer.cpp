#include "vk-command-buffer.h"

namespace GN::rdg {

static GN::Logger * sLogger = GN::getLogger("GN.rdg");

CommandBufferManagerVulkan::CommandBufferManagerVulkan(const ConstructParameters & params) : SubmissionImpl::Context(TYPE) {
    mGpu.set(static_cast<GpuContextVulkan *>(params.gpu.get()));
    const rapid_vulkan::Device & dev = mGpu->device();
    const rapid_vulkan::GlobalInfo * gi = dev.gi();
    rapid_vulkan::CommandQueue * gq = dev.graphics();
    if (!gq) GN_UNLIKELY { return; }
    mGraphicsQueue.reset(new rapid_vulkan::CommandQueue(
        rapid_vulkan::CommandQueue::ConstructParameters {{"rdg-graphics"}, gi, gq->family(), gq->index()}));
    rapid_vulkan::CommandQueue * cq = dev.compute();
    if (cq && cq != gq) {
        mComputeQueue.reset(new rapid_vulkan::CommandQueue(
            rapid_vulkan::CommandQueue::ConstructParameters {{"rdg-compute"}, gi, cq->family(), cq->index()}));
    } else {
        mComputeQueue = mGraphicsQueue;
    }
    rapid_vulkan::CommandQueue * tq = dev.transfer();
    if (tq && tq != gq) {
        mTransferQueue.reset(new rapid_vulkan::CommandQueue(
            rapid_vulkan::CommandQueue::ConstructParameters {{"rdg-transfer"}, gi, tq->family(), tq->index()}));
    } else {
        mTransferQueue = mGraphicsQueue;
    }
}

CommandBufferManagerVulkan::~CommandBufferManagerVulkan() {}

rapid_vulkan::Ref<rapid_vulkan::CommandQueue> CommandBufferManagerVulkan::getQueueForType(CommandBufferType type) const {
    switch (type) {
    case GRAPHICS: return mGraphicsQueue;
    case COMPUTE: return mComputeQueue.empty() ? mGraphicsQueue : mComputeQueue;
    case TRANSFER: return mTransferQueue.empty() ? mGraphicsQueue : mTransferQueue;
    }
    return mGraphicsQueue;
}

uint64_t CommandBufferManagerVulkan::prepare(CommandBufferType type) {
    rapid_vulkan::Ref<rapid_vulkan::CommandQueue> queue = getQueueForType(type);
    if (queue.empty()) GN_UNLIKELY { return 0; }

    bool needNew = mEntries.empty() || type != mEntries.back().type;
    if (needNew) {
        if (!mEntries.empty()) {
            mEntries.back().submit = true;
        }
        rapid_vulkan::CommandBuffer cb = queue->begin("rdg", vk::CommandBufferLevel::ePrimary);
        if (!cb) GN_UNLIKELY { return 0; }
        if (!mEntries.append(Entry {type, false, queue, cb})) GN_UNLIKELY { return 0; }
    } else {
        const Entry & prev = mEntries.back();
        if (!mEntries.append(Entry {type, false, prev.queue, prev.commandBuffer})) GN_UNLIKELY { return 0; }
    }
    return static_cast<uint64_t>(mEntries.size());
}

CommandBufferManagerVulkan::CommandBuffer CommandBufferManagerVulkan::execute(uint64_t commandBufferId) {
    if (commandBufferId == 0) GN_UNLIKELY {
            GN_ERROR(sLogger)("CommandBufferManagerVulkan::execute: invalid commandBufferId 0");
            return {};
        }
    size_t index = static_cast<size_t>(commandBufferId - 1);
    if (index >= mEntries.size()) GN_UNLIKELY {
            GN_ERROR(sLogger)("CommandBufferManagerVulkan::execute: commandBufferId {} out of range", commandBufferId);
            return {};
        }
    Entry & e = mEntries[index];
    if (index == mEntries.size() - 1) {
        e.submit = true;
    }
    return CommandBuffer {e.queue, &e.commandBuffer, e.submit};
}

} // namespace GN::rdg
