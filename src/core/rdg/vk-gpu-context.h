#pragma once

#include "gpu-context.h"

#ifdef RAPID_VULKAN_H_
    #error "rapid-vulkan.h is already included"
#endif
#if GN_LINUX
    #define VK_USE_PLATFORM_WAYLAND_KHR 1
#endif
#define RAPID_VULKAN_ENABLE_DEBUG_BUILD GN_BUILD_DEBUG_ENABLED
// TODO: hook rapid_vulkan log with GN::Logger
#include <rapid-vulkan/rapid-vulkan.h>
#include <optional>

namespace GN::rdg {

/// Vulkan-backed GpuContext (initialized via rapid-vulkan Instance + Device).
class GpuContextVulkan : public GpuContextCommon {
    std::optional<rapid_vulkan::Instance> mInstance;
    std::optional<rapid_vulkan::Device>   mDevice;

public:
    GpuContextVulkan(ArtifactDatabase & db, const StrA & name, const CreateParameters & params);

    ~GpuContextVulkan() override;

    const rapid_vulkan::Instance & instance() const {
        GN_ASSERT(mInstance.has_value() && mInstance.value().handle());
        return mInstance.value();
    }

    const rapid_vulkan::Device & device() const {
        GN_ASSERT(mDevice.has_value() && mDevice.value().handle());
        return mDevice.value();
    }

    const rapid_vulkan::GlobalInfo & globalInfo() const {
        GN_ASSERT(mDevice.has_value() && mDevice.value().gi());
        return *mDevice.value().gi();
    }
};

/// Create a Vulkan-backed GpuContext. Called from GpuContext::create() when api is "vulkan".
/// Returns null on failure or duplicate type+name.
AutoRef<GpuContext> createVulkanGpuContext(ArtifactDatabase & db, const StrA & name, const GpuContext::CreateParameters & params);

} // namespace GN::rdg
