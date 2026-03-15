#pragma once
/**
 * GpuUploadSlot — Vulkan-backend-internal ring-buffer slot lifecycle helper.
 *
 * NOT a public header. Never include from src/inc/.
 *
 * Each upload action (GpuBufferUploadVulkan, future UploadPoolVulkan,
 * UploadTextureVulkan) owns N GpuUploadSlot instances (one per ring slot).
 * The slot tracks:
 *   - which GPU submission is currently reading from this slot (submissionId),
 *   - which resources must stay alive until that submission completes (keepAlive).
 *
 * Lifecycle per slot per frame:
 *   waitUntilReady()      — wait for GPU to finish reading previous frame's data.
 *   recycle()             — drop keepAlive refs (releases staging buffers, blobs, …).
 *   [write data / record GPU commands]
 *   notifyCompletion(id)  — record the new submission ID so we can wait next time.
 */

#include "vk-gpu-context.h"
#include <garnet/base/array.h>

namespace GN::rdg {

struct GpuUploadSlot {
    rapid_vulkan::CommandQueue::SubmissionID submissionId = {}; ///< GPU submission currently using this slot.
    DynaArray<AutoRef<RefCounter>>           keepAlive;         ///< Resources to hold until submissionId is done.

    /// True when the slot is not associated with any in-flight GPU work.
    /// A fresh or already-recycled slot returns true.
    bool isReady() const { return submissionId.empty(); }

    /// Block until the GPU has finished using this slot, then recycle it.
    /// Logs a warning if a wait is actually needed (CPU-GPU sync point).
    void waitUntilReady(const char * slotDebugName = nullptr) {
        if (submissionId.empty()) return;
        static GN::Logger * sLogger = GN::getLogger("GN.rdg.upload");
        GN_WARN(sLogger)("GpuUploadSlot{}: CPU-GPU sync — waiting for slot to become ready. "
                         "This stall is expected when all ring slots are in flight simultaneously.",
                         slotDebugName ? slotDebugName : "?");
        submissionId.wait();
        recycle();
    }

    /// Release all keepAlive references and reset the submission token.
    /// Call only after the GPU submission is confirmed complete.
    void recycle() {
        keepAlive.clear();
        submissionId = {};
    }

    /// Add a resource reference that must outlive the current submission.
    void addKeepAlive(AutoRef<RefCounter> ref) { keepAlive.append(std::move(ref)); }

    /// Record the submission ID for the GPU work that now reads from this slot.
    /// Call immediately after submitting the command buffer that uses this slot.
    void notifyCompletion(rapid_vulkan::CommandQueue::SubmissionID id) { submissionId = id; }
};

} // namespace GN::rdg
