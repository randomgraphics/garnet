#include "vk-render-pass.h"

namespace GN::rdg {

static GN::Logger * sLogger = GN::getLogger("GN.rdg");

RenderPassManagerVulkan::RenderPassManagerVulkan(const ConstructParameters &) : SubmissionImpl::Context(TYPE) {
    // to be implemented
}

RenderPassManagerVulkan::~RenderPassManagerVulkan() {}

uint64_t RenderPassManagerVulkan::prepare(const RenderTarget & renderTarget) {
    (void) renderTarget;
    return 0;
}

RenderPassManagerVulkan::RenderPass RenderPassManagerVulkan::execute(uint64_t renderPassId) {
    (void) renderPassId;
    return RenderPass();
}

} // namespace GN::rdg