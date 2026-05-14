#pragma once

#include "vk-gpu-context.h"

#include <variant>

namespace GN::gpu2 {

/// Extends \c rv::CommandQueue::SyncPoint with an explicit type tag so callers don't have to
/// infer binary vs. timeline from the \c progress value (which the user may set arbitrarily).
struct GpuSyncPoint : rv::CommandQueue::SyncPoint {
    enum class Type { BINARY, TIMELINE };
    Type type = Type::BINARY; // default; only meaningful when semaphore != null

    /// Returns non-null when this is a submitted timeline sync point.
    const rv::CommandQueue::SyncPoint * asTimelinePoint() const {
        return (semaphore && type == Type::TIMELINE) ? static_cast<const rv::CommandQueue::SyncPoint *>(this) : nullptr;
    }

    /// Returns the binary semaphore handle when this is a submitted binary sync point.
    const vk::Semaphore * asBinarySemaphore() const { return (semaphore && type == Type::BINARY) ? &semaphore : nullptr; }

    bool empty() const { return !semaphore; }
         operator bool() const { return !!semaphore; }
};

/// Vulkan-specific GPU payload.
struct GpuPayloadVulkan : GpuPayload {
    GN_REGISTER_RUNTIME_TYPE(GpuPayload);

    explicit GpuPayloadVulkan(const StrA & name): GpuPayload(TYPE_INFO(), name) {}

    /// The sync point that will be triggered by GPU when the payload is processed.
    /// This is non-empty if and only if the payload has been submitted to GPU.
    /// Used internally to prevent a payload to be submitted multiple times.
    const GpuSyncPoint & syncpoint() const { return mSyncPoint; }

    /// Returns the underlying vk::Semaphore handle (timeline or binary); non-null iff submitted.
    vk::Semaphore semaphore() const { return mSyncPoint.semaphore; }

    /// Context handed to recordForVulkanSubmit(). Owned by the caller (GpuContextVulkan2);
    /// the payload borrows it for the duration of the record call and must not retain
    /// pointers beyond that.
    struct RecordContext {
        const rv::Device * dev   = nullptr; ///< rapid-vulkan device — gives access to device handle, queues, gi(), etc.
        rv::CommandQueue * queue = nullptr; ///< target queue for this submission; informs queue-family ownership decisions
        rv::CommandBuffer  cmd;             ///< open command buffer that the payload encodes its work into
    };

    /// Encode this payload's GPU work into ctx.cmd. Invoked once at submit time, in the
    /// order payloads were listed in SubmitParameters::work. Subclasses override; base
    /// is a no-op so an empty payload (e.g. swapchain "ready" sentinel) is valid.
    virtual void recordForVulkanSubmit(const RecordContext &) {}

    /// Host-side hook fired right after the submission's vkQueueSubmit returns
    /// (i.e. work is in flight, not yet completed on the GPU). Subclasses use this to
    /// flush tracked GPU state back to the source resources so that subsequent payloads
    /// see the post-submit state immediately.
    virtual void onSubmitComplete() {};

protected:
    friend class GpuContextVulkan2; // allow the main context to update the semaphore

    // Called by the swapchain to inject an imageAvailable handle as the binary sync point.
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
