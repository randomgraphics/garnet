#pragma once

#include "vk-gpu-context.h"
#include "vk-gpu-resource-state-tracker.h"

#include <variant>

namespace GN::gpu2 {

/// Extends \c rv::CommandQueue::SyncPoint with an explicit type tag so callers don't have to
/// infer binary vs. timeline from the \c progress value (which the user may set arbitrarily).
struct GpuSyncPoint : rv::CommandQueue::SyncPoint {
    enum class Type { BINARY, TIMELINE };
    Type type = Type::BINARY; // default; only meaningful when semaphore != null

    const rv::CommandQueue::SyncPoint * asTimelinePoint() const {
        return (semaphore && type == Type::TIMELINE) ? static_cast<const rv::CommandQueue::SyncPoint *>(this) : nullptr;
    }
    const vk::Semaphore * asBinarySemaphore() const { return (semaphore && type == Type::BINARY) ? &semaphore : nullptr; }

    bool empty() const { return !semaphore; }
         operator bool() const { return !!semaphore; }
};

/// Vulkan-specific GPU payload.
struct GpuPayloadVulkan : GpuPayload {
    GN_REGISTER_RUNTIME_TYPE(GpuPayload);

    explicit GpuPayloadVulkan(const StrA & name): GpuPayload(TYPE_INFO(), name) {}

    const GpuSyncPoint & syncpoint() const { return mSyncPoint; }
    vk::Semaphore        semaphore() const { return mSyncPoint.semaphore; }

    /// Context handed to recordForVulkanSubmit(). Owned by GpuContextVulkan2; payloads must
    /// not retain any pointers from it beyond the duration of the record call.
    struct RecordContext {
        const rv::Device *              dev   = nullptr;        ///< rapid-vulkan device
        rv::CommandQueue *              queue = nullptr;        ///< target queue for this submission
        rv::CommandBuffer               cmd;                    ///< open command buffer
        GpuResourceStateTrackerVulkan * batchTracker = nullptr; ///< the shared resource state tracker for the entire batch.
    };

    /// Encode this payload's GPU work into ctx.cmd. Invoked once at submit time, in order.
    virtual void recordForVulkanSubmit(const RecordContext &) {}

    /// Hook fired right after vkQueueSubmit returns. Available for any post-submit bookkeeping;
    /// resource state flushing is handled by submit() via batchTracker->flushToResources().
    virtual void onSubmitComplete() {}

    /// Hook fired once from pump()/waitForIdle() after this payload's submission fence has signaled,
    /// i.e. its GPU work is guaranteed complete. Use it for post-completion CPU work such as
    /// reading back data from staging buffers and resolving download futures. It is NOT called if the
    /// payload is dropped without ever being submitted — in that case the payload's destructor runs
    /// instead, which is where un-submitted work must release its resources / break its promises.
    virtual void onGpuComplete() {}

protected:
    friend class GpuContextVulkan2;

    void setSemaphore(vk::Semaphore s) {
        mSyncPoint.semaphore = s;
        mSyncPoint.stages    = vk::PipelineStageFlagBits::eAllCommands;
        mSyncPoint.progress  = 0;
        mSyncPoint.type      = GpuSyncPoint::Type::BINARY;
    }

    void setTimelinePoint(const rv::CommandQueue::SyncPoint & sp) {
        static_cast<rv::CommandQueue::SyncPoint &>(mSyncPoint) = sp;
        mSyncPoint.type                                        = GpuSyncPoint::Type::TIMELINE;
    }

private:
    GpuSyncPoint mSyncPoint;
};

} // namespace GN::gpu2
