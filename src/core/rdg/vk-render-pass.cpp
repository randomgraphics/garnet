#include "vk-render-pass.h"

namespace GN::rdg {

static GN::Logger * sLogger = GN::getLogger("GN.rdg");

RenderPassManagerVulkan::RenderPassManagerVulkan(const ConstructParameters &): SubmissionImpl::Context(TYPE) {
    // to be implemented
}

RenderPassManagerVulkan::~RenderPassManagerVulkan() {}

uint64_t RenderPassManagerVulkan::prepare(const RenderTarget & renderTarget) {
    // Similar as the command buffer manager, this function returns an 1-based ID for each draw action.
    // The ID is used to retrieve the render pass information in the execute() function.
    // If the render target is different from the previous one, then we need to create a new render pass.
    // If the render target is the same as the previous one, then we can reuse the existing render pass.
    GN_ERROR(sLogger)("RenderPassManagerVulkan::prepare: not implemented");
    (void) renderTarget;
    return 0;
}

// Retrieve the render pass information for a given ID (which is 1-based index into the internal render pass entry array)
// Mark the last entry as need-to-end.
RenderPassManagerVulkan::RenderPass RenderPassManagerVulkan::execute(uint64_t renderPassId, vk::CommandBuffer commandBuffer) {
    GN_ERROR(sLogger)("RenderPassManagerVulkan::execute: not implemented");
    (void) renderPassId;
    (void) commandBuffer;
    return RenderPass();
}

} // namespace GN::rdg