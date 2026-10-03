#pragma once

#include "vk-gpu-context.h"
#include "vk-bindless-descriptor-heap.h"
#include <garnet/GNgpu2.h>
#include <mutex>
#include <unordered_map>

namespace GN::gpu2 {

/// Manages and caches native VkPipelineLayout objects for bindless rendering passes.
class VkBindlessPipelineLayoutCache {
public:
    struct LayoutKey {
        vk::DescriptorSetLayout heapLayout {};
        uint32_t                heapSetIndex      = 0;
        uint32_t                pushConstantSize  = 128;
        size_t                  passResourcesHash = 0;

        bool operator==(const LayoutKey & o) const {
            return heapLayout == o.heapLayout && heapSetIndex == o.heapSetIndex && pushConstantSize == o.pushConstantSize &&
                   passResourcesHash == o.passResourcesHash;
        }
    };

    struct KeyHash {
        size_t operator()(const LayoutKey & k) const {
            size_t h = std::hash<uint64_t>()((uint64_t) (VkDescriptorSetLayout) k.heapLayout);
            h ^= std::hash<uint32_t>()(k.heapSetIndex) + 0x9e3779b9 + (h << 6) + (h >> 2);
            h ^= std::hash<uint32_t>()(k.pushConstantSize) + 0x9e3779b9 + (h << 6) + (h >> 2);
            h ^= k.passResourcesHash + 0x9e3779b9 + (h << 6) + (h >> 2);
            return h;
        }
    };

    explicit VkBindlessPipelineLayoutCache(GpuContextVulkan2 & gpu);
    ~VkBindlessPipelineLayoutCache();

    VkBindlessPipelineLayoutCache(const VkBindlessPipelineLayoutCache &)             = delete;
    VkBindlessPipelineLayoutCache & operator=(const VkBindlessPipelineLayoutCache &) = delete;

    /// Get or create a VkPipelineLayout matching the given bindless configuration.
    vk::PipelineLayout getOrCreate(const bindless::Raster::CreateParameters & cp, VkBindlessDescriptorHeap * vkHeap);

private:
    GpuContextVulkan2 &                                        mGpu;
    std::mutex                                                 mMutex;
    std::unordered_map<LayoutKey, vk::PipelineLayout, KeyHash> mCache;
    std::vector<vk::DescriptorSetLayout>                       mPassLayoutsToDestroy;
    vk::DescriptorSetLayout                                    mEmptyLayout {};
};

} // namespace GN::gpu2
