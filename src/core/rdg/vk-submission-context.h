#pragma once
#include "submission.h"
#include <garnet/rdg/rtti.h>
#include "vk-gpu-context.h"
#include "vk-render-pass.h"
#include "vk-command-buffer.h"
#include "vk-resource-tracker.h"
#include <garnet/base/array.h>

namespace GN::rdg {

/// @brief The aggregation of all Vulkan specific submission contexts.
/// The life time of this context is within one submission. It is created
/// on demand when being asked for by a vulkan specific action, and
/// destroyed when the submission is finished.
struct SubmissionContextVulkan : public SubmissionImpl::Context, public SlotBase {
    GN_RDG_REGISTER_RUNTIME_TYPE();

    SubmissionContextVulkan(SubmissionImpl & submission_, AutoRef<GpuContextVulkan> gpu_)
        : SubmissionImpl::Context(TYPE_INFO()), submission(submission_), gpu(gpu_), renderPassManager({gpu_}), commandBufferManager({gpu_, submission_}),
          resourceTracker({submission_}) {}

    SubmissionImpl &           submission;
    AutoRef<GpuContextVulkan>  gpu;
    RenderPassManagerVulkan    renderPassManager;
    CommandBufferManagerVulkan commandBufferManager;
    ResourceTrackerVulkan      resourceTracker;

    // /// Upload actions that wrote data during this submission.
    // /// CommandBufferManagerVulkan::submit() walks this list to distribute GPU completion tokens.
    // DynaArray<GpuBufferUploadVulkan *> activeUploads;

    // // -------------------------------------------------------------------------
    // // Frame Execution Context tracked on each backbuffer artifact.
    // struct FrameState {
    //     const rapid_vulkan::Swapchain::Frame * frame = nullptr;
    // };
    // // mapping from backbuffer artifact to frame pointer. Key is the sequence number of the backbuffer artifact.
    // std::unordered_map<uint64_t, FrameState> bb2frame;
};

} // namespace GN::rdg