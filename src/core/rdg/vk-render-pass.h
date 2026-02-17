#pragma once
#include "submission.h"
#include "vk-gpu-context.h"

namespace GN::rdg {

class RenderPassManagerVulkan : public SubmissionImpl::Context {
public:
    inline static constexpr Guid TYPE = {0x6ad8b59d, 0xe672, 0x4b5e, {0x8e, 0xec, 0xf7, 0xac, 0xd4, 0xf1, 0x99, 0xdd}};

    RenderPassManagerVulkan(): SubmissionImpl::Context(TYPE) {}

    struct ConstructParameters {
        AutoRef<GpuContextVulkan> gpu;
    };

    struct RenderPass {
        rapid_vulkan::Ref<rapid_vulkan::RenderPass> renderPass;
        std::optional<vk::RenderPassBeginInfo>      beginInfo {}; // If has value, we must call renderPass.cmdBegin() before drawing to the render target.
        bool                                        next {};      // If true, we must call renderPass.cmdNext() before drawing to the render target.
        bool                                        end {};       // If true, we must call renderPass.cmdEnd() after drawing to the render target.
    };

    RenderPassManagerVulkan(const ConstructParameters & params);

    ~RenderPassManagerVulkan() override;

    /// Called by task in prepare pass to request to render to a render target.
    /// \return Return a unique identifier to represent this request. 0 for failure.
    uint64_t prepare(const RenderTarget & renderTarget);

    /// Called by task in execution pass to retrieve the render target requested in prepare() pass.
    /// \return Returns the render pass information that a draw action can act on.
    /// - If the beginInfo has value, the caller must call renderPass.cmdBegin() before issue any draw commands.
    /// - If the next flag is true, the caller must call renderPass.cmdNext() before issue any draw commands.
    /// - If the end flag is true, the caller must call renderPass.cmdEnd() after issue all draw commands.
    RenderPass execute(uint64_t renderTargetId);
};

} // namespace GN::rdg