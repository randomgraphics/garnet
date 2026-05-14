#pragma once

#include "vk-gpu-context.h"

#include <variant>

namespace GN::gpu2 {

/// Vulkan-specific GPU payload.
struct GpuPayloadVulkan : GpuPayload {
    GN_REGISTER_RUNTIME_TYPE(GpuPayload);

    explicit GpuPayloadVulkan(const StrA & name): GpuPayload(TYPE_INFO(), name) {}

    /// The semaphore that will be triggered by GPU when the payload is processed.
    /// This is non-empty if and only if the payload has been submitted to GPU.
    /// Used internally to prevent a payload to be submitted multiple times.
    vk::Semaphore semaphore() const {
        if (auto pooled = std::get_if<PooledSemaphoreVulkan>(mSemaphore)) {
            return pooled.get();
        } else if (auto native = std::get_if<vk::Semaphore>(mSempahore)) {
            return native;
        } else return nullptr;
    }

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

    void setSemaphore(vk::Semaphore s) {
        GN_ASSERT(!semaphore());
        mSemaphore = s;
    }

    void setSemaphore(PooledSemaphoreVulkan s) {
        GN_ASSERT(!semaphore);
        mSemaphore = s;
    }

private:
    std::variant<PooledSemaphoreVulkan, vk::Semaphore> mSemaphore;
};

} // namespace GN::gpu2
