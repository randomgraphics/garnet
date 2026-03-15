#pragma once
/**
 * GpuBufferUploadVulkan — Vulkan backend implementation of GpuBufferUpload.
 * Backend-internal header. Do NOT include from src/inc/.
 */

#include "vk-gpu-context.h"

namespace GN::rdg {

// class GpuCopyVulkan : public GpuCopy {
// public:
//     // /// Each ring slot owns a pre-allocated BufferVulkan artifact and slot lifecycle state.
//     // /// The Buffer artifact is what gets referenced by GpuResourceView::artifact so that
//     // /// GpuResourceGroupVulkan's descriptor set binding recognizes it as a buffer.
//     // struct Slot : public GpuUploadSlot {
//     //     AutoRef<Buffer> bufferArtifact; ///< BufferVulkan holding a pre-allocated VkBuffer.
//     // };

//     // GpuBufferUploadVulkan(ArtifactDatabase & db, const StrA & name, AutoRef<GpuContextVulkan> gpu, const CreateParameters & params);

//     // ~GpuBufferUploadVulkan() override = default;

//     // /// True if construction succeeded (slots allocated for HOST_MAP).
//     // bool isValid() const { return mMechanism != Mechanism::HOST_MAP || !mSlots.empty(); }

//     // ExecutionResult prepare(TaskInfo & taskInfo, Arguments & arguments) override;
//     // ExecutionResult execute(TaskInfo & taskInfo, Arguments & arguments) override;

//     // GpuResourceView nextBufferView() const override;
//     // GpuResourceView currentBufferView() const override;

//     // /// Called by CommandBufferManagerVulkan::submit() after the draw command buffer that
//     // /// reads from this upload's current slot is submitted to the GPU queue.
//     // /// Records the submission token so the next execute() can wait before reusing the slot.
//     // void notifyCompletion(rapid_vulkan::CommandQueue::SubmissionID id) {
//     //     if (!mSlots.empty()) mSlots[mCurrentSlot].notifyCompletion(id);
//     // }

//     // // Internal state — accessible to backend tests (this is a backend-internal class, not public API).
//     // ArtifactDatabase &        mDb;
//     // AutoRef<GpuContextVulkan> mGpu;
//     // uint64_t                  mSize;
//     // Mechanism                 mMechanism;
//     // DynaArray<Slot>           mSlots;
//     // uint32_t                  mCurrentSlot = 0;

// private:
//     // bool initSlots(uint32_t ringSlots);
// };

AutoRef<GpuCopy> createVulkanGpuCopy(ArtifactDatabase & db, const StrA & name, const GpuCopy::CreateParameters & params);

} // namespace GN::rdg
