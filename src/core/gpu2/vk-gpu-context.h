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
    #define RAPID_VULKAN_NAMESPACE          GN::gpu2::rv
    #define RAPID_VULKAN_ENABLE_DEBUG_BUILD GN_BUILD_DEBUG_ENABLED
    #define RAPID_VULKAN_LOG(severity, prefix, message)                                                                   \
        do {                                                                                                              \
            GN::Logger::LogLevel logLevel = GN::Logger::LogLevel::INFO;                                                   \
            if (severity == RAPID_VULKAN_NAMESPACE::LogSeverity::FATAL) {                                                 \
                logLevel = GN::Logger::LogLevel::FATAL;                                                                   \
            } else if (severity == RAPID_VULKAN_NAMESPACE::LogSeverity::ERROR_) {                                         \
                logLevel = GN::Logger::LogLevel::ERROR_;                                                                  \
            } else if (severity == RAPID_VULKAN_NAMESPACE::LogSeverity::WARNING) {                                        \
                logLevel = GN::Logger::LogLevel::WARN;                                                                    \
            } else if (severity == RAPID_VULKAN_NAMESPACE::LogSeverity::INFO) {                                           \
                logLevel = GN::Logger::LogLevel::INFO;                                                                    \
            } else if (severity == RAPID_VULKAN_NAMESPACE::LogSeverity::VERBOSE) {                                        \
                logLevel = GN::Logger::LogLevel::VERBOSE;                                                                 \
            } else if (severity == RAPID_VULKAN_NAMESPACE::LogSeverity::BABBLE) {                                         \
                logLevel = GN::Logger::LogLevel::BABBLE;                                                                  \
            }                                                                                                             \
            GN_LOG_EX(GN::getLogger("GN.gpu2.vk"), logLevel, __FUNCTION__, __FILE__, __LINE__)("{} {}", prefix, message); \
        } while (false)
    #include <rapid-vulkan/rapid-vulkan.h>
#else
    #error \
        "vk-gpu-context.h included after another TU already included rapid-vulkan; this may cause ODR violations. Please include vk-gpu-context.h before any other TU that may include rapid-vulkan."
#endif // !RAPID_VULKAN_H_

#include "vk-resource-pool.h"

namespace GN::gpu2 {

class RasterPsoFactory; // defined in vk-raster-pso-factory.h

typedef ResourcePoolVulkan<vk::Fence>   FencePoolVulkan;
typedef AutoRef<FencePoolVulkan::Entry> PooledFenceVulkan;

/// Vulkan-backed \c GpuContext (rapid-vulkan Instance + Device). Mirrors v1 \c GpuContextVulkan.
class GpuContextVulkan2 : public GpuContextCommon2 {
public:
    GN_API GN_REGISTER_RUNTIME_TYPE(GpuContextCommon2);

    GpuContextVulkan2(const StrA & name, const CreateParameters & params);
    ~GpuContextVulkan2() override;

    bool ready() const { return mReady; }

    /// Vulkan device for rdg2 factories (swapchain, etc.). Only valid when ready().
    const rv::Device & vulkanDevice() const {
        GN_ASSERT(ready());
        return mDevice.value();
    }

    /// PSO cache for raster pipelines. Only valid when ready().
    RasterPsoFactory & psoFactory() const {
        GN_ASSERT(mPsoFactory);
        return *mPsoFactory;
    }

    // Fence pool
    ResourcePoolVulkan<vk::Fence> & fencePool() const {
        GN_ASSERT(mFencePool.has_value());
        return mFencePool.value();
    }

    GpuContext::Caps caps() const override { return mCaps; }

    intptr_t getVulkanInstanceHandle() const override {
        if (!mInstance.has_value() || !mInstance->handle()) return 0;
        return (intptr_t) (void *) mInstance->handle();
    }

    void submit(const SubmitParameters &) override;
    void pump() override { pumpInternal(false); }
    void waitForIdle() override { pumpInternal(true); }

private:
    void pumpInternal(bool waitForIdle);

    // Frequently queried items live directly on the class so their accessors above can be inlined.
    bool                        mReady = false;
    std::optional<rv::Instance> mInstance;
    std::optional<rv::Device>   mDevice;
    GpuContext::Caps            mCaps;

    // mutable factories and pools: may change even on logically const paths.
    mutable std::unique_ptr<RasterPsoFactory> mPsoFactory; // incomplete type; can't use std::optional
    mutable std::optional<FencePoolVulkan>    mFencePool;

    // Other internal impl details that we'd like to hide from public context header.
    struct Impl;
    std::unique_ptr<Impl> mImpl;
};

/// Create a Vulkan-backed rdg2 \c GpuContext. Used from \c GpuContext::create when API is "vulkan".
AutoRef<GpuContext> createVulkanGpuContext2(const StrA & name, const GpuContext::CreateParameters & params);

} // namespace GN::gpu2
