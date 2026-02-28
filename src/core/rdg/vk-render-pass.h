#pragma once
#include "vk-gpu-context.h"
#include "submission.h"

namespace GN::rdg {

class ResourceTrackerVulkan;

class RenderPassManagerVulkan {
public:
    inline static const uint64_t TYPE = getNextUniqueTypeId();

    struct ConstructParameters {
        AutoRef<GpuContextVulkan> gpu;
    };

    struct RenderPass {
        uint64_t              firstTaskIndex = 0;
        uint64_t              lastTaskIndex  = 0;
        AutoRef<RenderTarget> renderTarget; // could be empty, if this is no render target is set between the first and last task.
    };

    RenderPassManagerVulkan(const ConstructParameters & params): mGpu(params.gpu) {}

    ~RenderPassManagerVulkan() = default;

    /// Called by tasks in prepare pass to collect render target information.
    bool collectRenderTargetUsage(TaskInfo & taskInfo, AutoRef<RenderTarget> renderTarget);

    /// Called by presnet action to end rendering to backbuffer.
    /// If current render target is not this backbufer, then do nothing.
    void clearActiveRenderTargetIfBackbuffer(TaskInfo & taskInfo, AutoRef<Backbuffer> backbuffer);

    /// Called by task in execution pass to begin render pass.
    const RenderPass * execute(TaskInfo & taskInfo, vk::CommandBuffer commandBuffer);

private:
    AutoRef<GpuContextVulkan> mGpu;
    std::vector<RenderPass>   mRenderPasses;

    bool beginRenderPass(const RenderTarget & renderTarget, vk::CommandBuffer commandBuffer);
};

} // namespace GN::rdg