#define RAPID_VULKAN_IMPLEMENTATION
#include "vk-gpu-context.h"

static GN::Logger * sLogger = GN::getLogger("GN.rdg.vk");

namespace GN::rdg {

// =============================================================================
// GpuContextVulkan implementation (Instance/Device via std::optional, not Ref - they do not inherit Root)
// =============================================================================

GpuContextVulkan::GpuContextVulkan(ArtifactDatabase & db, const StrA & name, const CreateParameters & /* params */)
    : GpuContextCommon(db, name, GpuContextCommon::Api::Vulkan) {
    if (0 == sequence) return;
    rapid_vulkan::Instance::ConstructParameters ip;
    mInstance.emplace(ip);
    rapid_vulkan::Device::ConstructParameters dp;
    dp.setInstance(mInstance->handle());
    mDevice.emplace(dp);
}

// =============================================================================
// createVulkanGpuContext - API-specific factory
// =============================================================================

AutoRef<GpuContext> createVulkanGpuContext(ArtifactDatabase & db, const StrA & name, const GpuContext::CreateParameters & params) {
    auto * p = new GpuContextVulkan(db, name, params);
    if (p->sequence == 0) {
        GN_ERROR(sLogger)("createVulkanGpuContext: duplicate type+name, name='{}'", name);
        delete p;
        return {};
    }
    return AutoRef<GpuContext>(p);
}

} // namespace GN::rdg
