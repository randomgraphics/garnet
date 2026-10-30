#include "pch.h"
#include "vk-bindless-compute-pso-cache.h"

static GN::Logger * sLogger = GN::getLogger("GN.gpu2.vk.bindless.compute.pso");

namespace GN::gpu2 {

VkBindlessComputePsoCache::VkBindlessComputePsoCache(GpuContextVulkan2 & gpu): mGpu(gpu) {}

VkBindlessComputePsoCache::~VkBindlessComputePsoCache() {
    auto vkDev = mGpu.vulkanDevice().handle();
    for (auto & [k, pipe] : mCache) {
        if (pipe) { vkDev.destroyPipeline(pipe); }
    }
    mCache.clear();
}

size_t VkBindlessComputePsoCache::cacheSize() const { return mCache.size(); }

vk::Pipeline VkBindlessComputePsoCache::getOrCreate(vk::PipelineLayout layout, const GpuShaderVulkan * cs) {
    if (!layout || !cs || !cs->rvShader() || !cs->rvShader()->handle()) { return vk::Pipeline {}; }

    BindlessComputePsoKey key;
    key.pipelineLayout = layout;
    key.shaderHash     = static_cast<uint64_t>(cs->id);

    std::lock_guard<std::mutex> lock(mMutex);
    auto                        it = mCache.find(key);
    if (it != mCache.end()) { return it->second; }

    vk::Pipeline p = buildPipeline(layout, cs);
    if (p) { mCache.emplace(key, p); }
    return p;
}

vk::Pipeline VkBindlessComputePsoCache::buildPipeline(vk::PipelineLayout layout, const GpuShaderVulkan * cs) {
    auto vkDev = mGpu.vulkanDevice().handle();

    vk::ComputePipelineCreateInfo info;
    info.setLayout(layout);
    info.setStage(vk::PipelineShaderStageCreateInfo {{}, vk::ShaderStageFlagBits::eCompute, cs->rvShader()->handle(), "main"});

    try {
        auto res = vkDev.createComputePipeline(vk::PipelineCache {}, info);
        return res.value;
    } catch (const std::exception & e) {
        GN_ERROR(sLogger, "VkBindlessComputePsoCache: createComputePipeline failed: {}", e.what());
        return vk::Pipeline {};
    }
}

} // namespace GN::gpu2
