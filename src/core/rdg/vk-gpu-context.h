#pragma once

#include "gpu-context.h"
#if GN_LINUX
    #define VK_USE_PLATFORM_WAYLAND_KHR 1
#endif
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
