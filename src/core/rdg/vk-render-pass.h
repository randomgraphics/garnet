#pragma once
#include "vk-gpu-context.h"
#include "submission.h"

namespace GN::rdg {

class ResourceTrackerVulkan;

class RenderPassManagerVulkan : public SubmissionImpl::Context {
public:
    inline static constexpr Guid TYPE = {0x6ad8b59d, 0xe672, 0x4b5e, {0x8e, 0xec, 0xf7, 0xac, 0xd4, 0xf1, 0x99, 0xdd}};

    RenderPassManagerVulkan(): SubmissionImpl::Context(TYPE) {}

    struct ConstructParameters {
        AutoRef<GpuContextVulkan> gpu;
    };

    struct RenderPass {
        uint64_t firstTaskIndex = 0;
        uint64_t lastTaskIndex  = 0;
    };

    RenderPassManagerVulkan(const ConstructParameters & params);

    ~RenderPassManagerVulkan() override;

    // /// Called by task in prepare pass to request to render to a render target.
    bool prepare(TaskInfo & taskInfo, const RenderTarget & renderTarget);

    /// Called by task in execution pass to retrieve the render target requested in prepare() pass.
    /// \param renderTarget The render target to render to.
    /// \param taskIndex The task index of the render pass.
    /// \param rt The resource tracker to get the image layout of the render target before the render pass.
    /// \param commandBuffer The command buffer to record the render pass into.
    /// \return Returns the render pass information that a draw action can act on. Or empty for failure.
    const RenderPass * execute(TaskInfo & taskInfo, const RenderTarget & renderTarget, const ResourceTrackerVulkan & rt, vk::CommandBuffer commandBuffer);

private:
    AutoRef<GpuContextVulkan> mGpu;
    const RenderTarget *      mPrevRenderTarget = nullptr;
    std::vector<RenderPass>   mRenderPasses;
};

} // namespace GN::rdg