#include "pch.h"
#define RAPID_VULKAN_IMPLEMENTATION
#include "vk-gpu-context.h"
#include "vk-pso-factory.h"

static GN::Logger * sLogger = GN::getLogger("GN.rdg.vk");

namespace GN::rdg {

static rapid_vulkan::Instance::Validation getVkInstanceValidation(GpuContext::CreateParameters::DebugMode debugMode) {
    switch (debugMode) {
    case GpuContext::CreateParameters::DebugMode::DISABLED:
        return rapid_vulkan::Instance::Validation::VALIDATION_DISABLED;
    case GpuContext::CreateParameters::DebugMode::ENABLED:
#if GN_BUILD_DEBUG_ENABLED
        return rapid_vulkan::Instance::Validation::LOG_ON_VK_ERROR | rapid_vulkan::Instance::Validation::BREAK_ON_VK_ERROR;
#else
        return rapid_vulkan::Instance::Validation::LOG_ON_VK_ERROR;
#endif
    default:
        return rapid_vulkan::Instance::Validation::VALIDATION_DISABLED;
    }
}

static rapid_vulkan::Device::Verbosity getVkDeviceVerbosity(GpuContext::CreateParameters::Verbosity verbosity) {
    switch (verbosity) {
    case GpuContext::CreateParameters::Verbosity::SILENCE:
        return rapid_vulkan::Device::Verbosity::SILENCE;
    case GpuContext::CreateParameters::Verbosity::BRIEF:
        return rapid_vulkan::Device::Verbosity::BRIEF;
    case GpuContext::CreateParameters::Verbosity::VERBOSE:
        return rapid_vulkan::Device::Verbosity::VERBOSE;
    default:
        return rapid_vulkan::Device::Verbosity::SILENCE;
    }
}

// =============================================================================
// GpuContextVulkan implementation (Instance/Device via std::optional, not Ref - they do not inherit Root)
// =============================================================================

GpuContextVulkan::GpuContextVulkan(const StrA & name, const CreateParameters & params): GpuContextCommon(name, GpuContextCommon::Api::Vulkan) {
    rapid_vulkan::Instance::ConstructParameters ip;
    ip.apiVersion = VK_API_VERSION_1_3; // requires at least Vulkan 1.3+ to use dynamic rendering feature.
    ip.backtrace  = []() -> std::string { return GN::backtrace().c_str(); };
    ip.setValidation(getVkInstanceValidation(params.debug));
    ip.setPrintVkInfo(getVkDeviceVerbosity(params.howToPrintDeviceCaps));
    mInstance.emplace(ip);
    if (!mInstance->handle()) {
        GN_ERROR(sLogger)("GpuContextVulkan::GpuContextVulkan: failed to create Vulkan instance, name='{}'", name);
        return;
    }
    rapid_vulkan::Device::ConstructParameters dp;
    dp.setPrintVkInfo(ip.printVkInfo);
#if GN_DARWIN
    // Vulkan 1.3 spec says dynamic rendering part of the core feature that can be used w/o enable it explicitly.
    // However, MoltenVK requires it to be enabled explicitly.
    dp.addDeviceExtension("VK_KHR_dynamic_rendering");
#endif
    dp.addFeature(vk::PhysicalDeviceVulkan13Features().setDynamicRendering(true));
    dp.setInstance(mInstance->handle());
    mDevice.emplace(dp);
    if (mDevice->handle()) { mPsoFactory = std::make_unique<PsoFactoryVulkan>(*this); }
}

GpuContextVulkan::~GpuContextVulkan() { GN_INFO(sLogger)("Destroying Vulkan GPU context, name='{}'", name); }

// =============================================================================
// createVulkanGpuContext - API-specific factory
// =============================================================================

AutoRef<GpuContext> createVulkanGpuContext(const StrA & name, const GpuContext::CreateParameters & params) {
    auto p = std::make_unique<GpuContextVulkan>(name, params);
    if (!p->instance().handle() || !p->device().handle()) return {};
    return AutoRef<GpuContext>(p.release());
}

} // namespace GN::rdg
