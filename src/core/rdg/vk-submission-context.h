#pragma once
#include "submission.h"
#include "vk-gpu-context.h"
#include "vk-render-pass.h"
#include "vk-command-buffer.h"

namespace GN::rdg {

/// @brief The aggregation of all Vulkan specific submission contexts.
/// The life time of this context is within one submission. It is created
/// on demand when being asked for by a vulkan specific action, and
/// destroyed when the submission is finished.
struct SubmissionContextVulkan : public SubmissionImpl::Context {
    inline static const uint64_t         TYPE_ID   = getNextUniqueTypeId();
    inline static constexpr const char * TYPE_NAME = "SubmissionContextVulkan";

    static SubmissionContextVulkan & get(SubmissionImpl & submission, AutoRef<GpuContextVulkan> gpu) {
        return submission.ensureSubmissionContext<SubmissionContextVulkan>(submission, gpu);
    }

    SubmissionContextVulkan(SubmissionImpl & submission_, AutoRef<GpuContextVulkan> gpu_)
        : SubmissionImpl::Context(TYPE_ID, TYPE_NAME), submission(submission_), gpu(gpu_), renderPassManager({gpu_}), commandBufferManager({gpu_}) {}

    SubmissionImpl &           submission;
    AutoRef<GpuContextVulkan>  gpu;
    RenderPassManagerVulkan    renderPassManager;
    CommandBufferManagerVulkan commandBufferManager;

    // // -------------------------------------------------------------------------
    // // Frame Execution Context tracked on each backbuffer artifact.
    // struct FrameState {
    //     const rapid_vulkan::Swapchain::Frame * frame = nullptr;
    // };
    // // mapping from backbuffer artifact to frame pointer. Key is the sequence number of the backbuffer artifact.
    // std::unordered_map<uint64_t, FrameState> bb2frame;
};

} // namespace GN::rdg