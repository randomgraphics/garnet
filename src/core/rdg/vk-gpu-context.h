#pragma once

#include "gpu-context.h"

#ifdef RAPID_VULKAN_H_
    #error "rapid-vulkan.h is already included"
#endif
#if GN_LINUX
    #if HAS_X11
        #define VK_USE_PLATFORM_XLIB_KHR 1
    #else
        #define VK_USE_PLATFORM_WAYLAND_KHR 1
    #endif
#endif
#if GN_MSWIN
    #define VK_USE_PLATFORM_WIN32_KHR 1
    #define NOMINMAX
#endif
#define RAPID_VULKAN_ENABLE_DEBUG_BUILD GN_BUILD_DEBUG_ENABLED
#define RAPID_VULKAN_LOG(severity, prefix, message)                                                                                     \
    do {                                                                                                                                \
        GN::Logger::LogLevel logLevel = GN::Logger::LogLevel::INFO;                                                                     \
        if (severity == RAPID_VULKAN_NAMESPACE::LogSeverity::FATAL) {                                                                   \
            logLevel = GN::Logger::LogLevel::FATAL;                                                                                     \
        } else if (severity == RAPID_VULKAN_NAMESPACE::LogSeverity::ERROR_) {                                                           \
            logLevel = GN::Logger::LogLevel::ERROR_;                                                                                    \
        } else if (severity == RAPID_VULKAN_NAMESPACE::LogSeverity::WARNING) {                                                          \
            logLevel = GN::Logger::LogLevel::WARN;                                                                                      \
        } else if (severity == RAPID_VULKAN_NAMESPACE::LogSeverity::INFO) {                                                             \
            logLevel = GN::Logger::LogLevel::INFO;                                                                                      \
        } else if (severity == RAPID_VULKAN_NAMESPACE::LogSeverity::VERBOSE) {                                                          \
            logLevel = GN::Logger::LogLevel::VERBOSE;                                                                                   \
        } else if (severity == RAPID_VULKAN_NAMESPACE::LogSeverity::DEBUG) {                                                            \
            logLevel = GN::Logger::LogLevel::INFO;                                                                                      \
        }                                                                                                                               \
        GN::Logger::LogHelper(GN::getLogger("GN.rdg.vk"), logLevel, __FUNCTION__, __FILE__, __LINE__).format("{} {}", prefix, message); \
    } while (false)
#include <rapid-vulkan/rapid-vulkan.h>
#include <memory>
#include <optional>

namespace GN::rdg {

class PsoFactoryVulkan;

/// Vulkan-backed GpuContext (initialized via rapid-vulkan Instance + Device).
class GpuContextVulkan : public GpuContextCommon {
    std::optional<rapid_vulkan::Instance> mInstance;
    std::optional<rapid_vulkan::Device>   mDevice;
    std::unique_ptr<PsoFactoryVulkan>     mPsoFactory;

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

    intptr_t getVulkanInstanceHandle() const override { return (intptr_t) (void *) instance().handle(); }

    const rapid_vulkan::GlobalInfo & globalInfo() const {
        GN_ASSERT(mDevice.has_value() && mDevice.value().gi());
        return *mDevice.value().gi();
    }

    PsoFactoryVulkan & psoFactory() const {
        GN_ASSERT(mPsoFactory);
        return *mPsoFactory;
    }
};

/// Create a Vulkan-backed GpuContext. Called from GpuContext::create() when api is "vulkan".
/// Returns null on failure or duplicate type+name.
AutoRef<GpuContext> createVulkanGpuContext(ArtifactDatabase & db, const StrA & name, const GpuContext::CreateParameters & params);

} // namespace GN::rdg
