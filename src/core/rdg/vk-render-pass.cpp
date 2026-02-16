#include "vk-render-pass.h"

namespace GN::rdg {

static GN::Logger * sLogger = GN::getLogger("GN.rdg");

RenderPassManagerVulkan::RenderPassManagerVulkan(const ConstructParameters &): SubmissionImpl::Context(TYPE) {
    // to be implemented
}

RenderPassManagerVulkan::~RenderPassManagerVulkan() {}

uint64_t RenderPassManagerVulkan::prepare(const RenderTarget & renderTarget) {
    GN_ERROR(sLogger)("RenderPassManagerVulkan::prepare: not implemented");
    (void) renderTarget;
    return 0;
}

RenderPassManagerVulkan::RenderPass RenderPassManagerVulkan::execute(uint64_t renderPassId) {
    GN_ERROR(sLogger)("RenderPassManagerVulkan::execute: not implemented");
    (void) renderPassId;
    return RenderPass();
}

} // namespace GN::rdg