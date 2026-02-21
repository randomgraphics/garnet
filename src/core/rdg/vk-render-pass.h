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
        uint64_t firstTaskIndex = 0;
        uint64_t lastTaskIndex  = 0;
    };

    RenderPassManagerVulkan(const ConstructParameters & params): mGpu(params.gpu) {}

    ~RenderPassManagerVulkan() = default;

    // /// Called by task in prepare pass to request to render to a render target.
    bool prepare(TaskInfo & taskInfo, const RenderTarget & renderTarget);

    struct RenderPassArguments {
        // The render target to render to.
        const RenderTarget & renderTarget;

        // The command buffer that record the vulkan commands into. Can't be null.
        vk::CommandBuffer commandBuffer;

        // Optional. If set, we need to clear the render target to the specified values.
        std::optional<ClearRenderTarget::A::ClearValues> clearValues;
    };

    /// Called by task in execution pass to retrieve the render target requested in prepare() pass.
    /// \return Returns the render pass information that a draw action can act on. Or empty for failure.
    const RenderPass * execute(TaskInfo & taskInfo, const RenderPassArguments & arguments);

private:
    AutoRef<GpuContextVulkan> mGpu;
    const RenderTarget *      mPrevRenderTarget = nullptr;
    std::vector<RenderPass>   mRenderPasses;

    bool beginRenderPass(const RenderPassArguments & arguments);
};

} // namespace GN::rdg