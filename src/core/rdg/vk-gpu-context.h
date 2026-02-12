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
    const rapid_vulkan::GlobalInfo * globalInfo() const {
        if (!mDevice.has_value() || !mDevice.value().gi()) GN_UNLIKELY {
                GN_ERROR(getLogger("GN.rdg.vk"))("Failed to retrieve Vulkan GlobalInfo: The GPU context is not property initialized, name='{}'", name);
                return nullptr;
            }
        return mDevice.value().gi();
    }
    rapid_vulkan::Device *   device() { return mDevice ? &*mDevice : nullptr; }
    rapid_vulkan::Instance * instance() { return mInstance ? &*mInstance : nullptr; }
};

/// Create a Vulkan-backed GpuContext. Called from GpuContext::create() when api is "vulkan".
/// Returns null on failure or duplicate type+name.
AutoRef<GpuContext> createVulkanGpuContext(ArtifactDatabase & db, const StrA & name, const GpuContext::CreateParameters & params);

} // namespace GN::rdg
