#include "pch.h"
#define RAPID_VULKAN_IMPLEMENTATION
#include "vk-gpu-context.h"

static GN::Logger * sLogger = GN::getLogger("GN.rdg.vk");

namespace GN::rdg {

// =============================================================================
// GpuContextVulkan implementation (Instance/Device via std::optional, not Ref - they do not inherit Root)
// =============================================================================

GpuContextVulkan::GpuContextVulkan(ArtifactDatabase & db, const StrA & name, const CreateParameters & /* params */)
    : GpuContextCommon(db, name, GpuContextCommon::Api::Vulkan) {
    if (0 == sequence) {
        GN_ERROR(sLogger)("GpuContextVulkan::GpuContextVulkan: duplicate type+name, name='{}'", name);
        return;
    }
    rapid_vulkan::Instance::ConstructParameters ip;
    ip.apiVersion = VK_API_VERSION_1_3; // requires at least Vulkan 1.3+ to use dynamic rendering feature.
    mInstance.emplace(ip);
    if (!mInstance->handle()) {
        GN_ERROR(sLogger)("GpuContextVulkan::GpuContextVulkan: failed to create Vulkan instance, name='{}'", name);
        return;
    }
    rapid_vulkan::Device::ConstructParameters dp;
    dp.setInstance(mInstance->handle());
    mDevice.emplace(dp);
}

GpuContextVulkan::~GpuContextVulkan() { GN_INFO(sLogger)("Destorying Vulkan GPU context, name='{}'", name); }

// =============================================================================
// createVulkanGpuContext - API-specific factory
// =============================================================================

AutoRef<GpuContext> createVulkanGpuContext(ArtifactDatabase & db, const StrA & name, const GpuContext::CreateParameters & params) {
    auto p = std::make_unique<GpuContextVulkan>(db, name, params);
    if (p->sequence == 0 || !p->instance().handle() || !p->device().handle()) return {};
    return AutoRef<GpuContext>(p.release());
}

} // namespace GN::rdg
