#pragma once

#include "vk-gpu-context.h"
#include "vk-gpu-shader.h"
#include <garnet/GNgpu2.h>
#include <mutex>
#include <unordered_map>

namespace GN::gpu2 {

struct BindlessComputePsoKey {
    vk::PipelineLayout pipelineLayout {};
    uint64_t           shaderHash = 0;

    bool operator==(const BindlessComputePsoKey & o) const noexcept { return pipelineLayout == o.pipelineLayout && shaderHash == o.shaderHash; }
};

struct BindlessComputePsoKeyHash {
    size_t operator()(const BindlessComputePsoKey & k) const noexcept {
        size_t h = std::hash<uint64_t>()((uint64_t) (VkPipelineLayout) k.pipelineLayout);
        h ^= k.shaderHash + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
};

/// Get-or-create cache for native Vulkan compute pipelines used in bindless CnC passes.
/// Pipelines are cached by (VkPipelineLayout, shaderModule) and destroyed when device shuts down.
class VkBindlessComputePsoCache {
public:
    explicit VkBindlessComputePsoCache(GpuContextVulkan2 & gpu);
    ~VkBindlessComputePsoCache();

    VkBindlessComputePsoCache(const VkBindlessComputePsoCache &)             = delete;
    VkBindlessComputePsoCache & operator=(const VkBindlessComputePsoCache &) = delete;

    /// Returns a cached pipeline or creates a new one.
    vk::Pipeline getOrCreate(vk::PipelineLayout layout, const GpuShaderVulkan * cs);

    size_t cacheSize() const;

private:
    vk::Pipeline buildPipeline(vk::PipelineLayout layout, const GpuShaderVulkan * cs);

    GpuContextVulkan2 &                                                                mGpu;
    std::mutex                                                                         mMutex;
    std::unordered_map<BindlessComputePsoKey, vk::Pipeline, BindlessComputePsoKeyHash> mCache;
};

} // namespace GN::gpu2
