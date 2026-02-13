#pragma once
#include "submission.h"
#include "vk-gpu-context.h"


namespace GN::rdg {

class RenderPassManagerVulkan : public RenderPassManager {
public:
    struct ConstructParameters {
        AutoRef<GpuContext> context;
    };

    struct RenderPass {
        rapid_vulkan::RenderPass renderPass {};
        std::optional<vk::RenderPassBeginInfo> beginInfo {}; // If has value, we must call renderPass.cmdBegin() before drawing to the render target.
        bool next {}; // If true, we must call renderPass.cmdNext() before drawing to the render target.
        bool end {}; // If true, we must call renderPass.cmdEnd() after drawing to the render target.
    };

    RenderPassManagerVulkan(const ConstructParameters & params);
    ~RenderPassManagerVulkan() override;

    /// Add a render target to the render pass manager.
    /// \return Return a unique identifier for the render target. 0 for failure.
    /// - If the same render target is added multiple times, w/o other render targets in between,
    ///   the same identifier will be returned.
    /// - this method is only allowed to be called before the first call to getRenderPass().
    auto prepare(const RenderTarget & renderTarget) -> uint64_t;

    /// Returns the render pass information that a draw action can act on.
    RenderPass execute(uint64_t renderTargetId) const;
};

} // namespace GN::rdg