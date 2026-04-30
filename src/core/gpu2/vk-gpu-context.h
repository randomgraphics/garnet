#pragma once

#include "gpu-context.h"

#include <memory>
#include <optional>

#ifndef RAPID_VULKAN_H_
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
    #define RAPID_VULKAN_NAMESPACE          GN::gfx::gpu2::rv
    #define RAPID_VULKAN_ENABLE_DEBUG_BUILD GN_BUILD_DEBUG_ENABLED
    #define RAPID_VULKAN_LOG(severity, prefix, message)                                                                                          \
        do {                                                                                                                                     \
            GN::Logger::LogLevel logLevel = GN::Logger::LogLevel::INFO;                                                                          \
            if (severity == RAPID_VULKAN_NAMESPACE::LogSeverity::FATAL) {                                                                        \
                logLevel = GN::Logger::LogLevel::FATAL;                                                                                          \
            } else if (severity == RAPID_VULKAN_NAMESPACE::LogSeverity::ERROR_) {                                                                \
                logLevel = GN::Logger::LogLevel::ERROR_;                                                                                         \
            } else if (severity == RAPID_VULKAN_NAMESPACE::LogSeverity::WARNING) {                                                               \
                logLevel = GN::Logger::LogLevel::WARN;                                                                                           \
            } else if (severity == RAPID_VULKAN_NAMESPACE::LogSeverity::INFO) {                                                                  \
                logLevel = GN::Logger::LogLevel::INFO;                                                                                           \
            } else if (severity == RAPID_VULKAN_NAMESPACE::LogSeverity::VERBOSE) {                                                               \
                logLevel = GN::Logger::LogLevel::VERBOSE;                                                                                        \
            } else if (severity == RAPID_VULKAN_NAMESPACE::LogSeverity::DEBUG) {                                                                 \
                logLevel = GN::Logger::LogLevel::INFO;                                                                                           \
            }                                                                                                                                    \
            GN::Logger::LogHelper(GN::getLogger("GN.gfx.gpu2.vk"), logLevel, __FUNCTION__, __FILE__, __LINE__).format("{} {}", prefix, message); \
        } while (false)
    #include <rapid-vulkan/rapid-vulkan.h>
#else
    #error \
        "vk-gpu-context.h included after another TU already included rapid-vulkan; this may cause ODR violations. Please include vk-gpu-context.h before any other TU that may include rapid-vulkan."
#endif // !RAPID_VULKAN_H_

namespace GN::gfx::gpu2 {

/// Vulkan-backed \c GpuContext (rapid-vulkan Instance + Device). Mirrors v1 \c GpuContextVulkan.
class GpuContextVulkan2 : public GpuContextCommon2 {
public:
    GN_API GN_REGISTER_RUNTIME_TYPE(GpuContextCommon2);

    struct Impl;
    std::unique_ptr<Impl> mImpl;

    GpuContextVulkan2(const StrA & name, const CreateParameters & params);
    ~GpuContextVulkan2() override;

    /// True when Instance and Device are both created successfully (for factory; mirrors v1 success checks).
    bool ready() const;

    /// Vulkan device for rdg2 factories (swapchain, etc.). Null when not ready.
    const rv::Device * vulkanDevice() const;

    GpuContext::Caps caps() const override;
    intptr_t         getVulkanInstanceHandle() const override;
    void             submit(const SubmitParameters &) override;
    void             pump() override;
};

/// Create a Vulkan-backed rdg2 \c GpuContext. Used from \c GpuContext::create when API is "vulkan".
AutoRef<GpuContext> createVulkanGpuContext2(const StrA & name, const GpuContext::CreateParameters & params);

} // namespace GN::gfx::gpu2
