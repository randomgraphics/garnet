#include "pch.h"
#include "vk-bindless-pipeline-layout.h"

static GN::Logger * sLogger = GN::getLogger("GN.gpu2.vk.bindless");

namespace GN::gpu2 {

static size_t hashPassResources(const GpuResourceTable & table) {
    if (table.empty()) return 0;
    size_t h = table.size();
    for (size_t s = 0; s < table.size(); ++s) {
        h = h ^ (table[s].size() + 0x9e3779b9 + (h << 6) + (h >> 2));
        for (size_t b = 0; b < table[s].size(); ++b) {
            h = h ^ (table[s][b].size() + 0x9e3779b9 + (h << 6) + (h >> 2));
            for (const auto & v : table[s][b]) {
                h = h ^ (std::hash<uint8_t>()((uint8_t) v.isTexture()) + 0x9e3779b9 + (h << 6) + (h >> 2));
                h = h ^ (std::hash<uint8_t>()((uint8_t) v.isBuffer()) + 0x9e3779b9 + (h << 6) + (h >> 2));
                if (v.isTexture()) {
                    h = h ^ (std::hash<uint8_t>()((uint8_t) v.imageView.type) + 0x9e3779b9 + (h << 6) + (h >> 2));
                } else if (v.isBuffer()) {
                    h = h ^ (std::hash<uint8_t>()((uint8_t) v.bufferView.type) + 0x9e3779b9 + (h << 6) + (h >> 2));
                }
            }
        }
    }
    return h;
}

VkBindlessPipelineLayoutCache::VkBindlessPipelineLayoutCache(GpuContextVulkan2 & gpu): mGpu(gpu) {}

VkBindlessPipelineLayoutCache::~VkBindlessPipelineLayoutCache() {
    auto vkDev = mGpu.vulkanDevice().handle();
    for (auto & [k, layout] : mCache) {
        if (layout) { vkDev.destroyPipelineLayout(layout); }
    }
    mCache.clear();

    for (auto & l : mPassLayoutsToDestroy) {
        if (l) { vkDev.destroyDescriptorSetLayout(l); }
    }
    mPassLayoutsToDestroy.clear();

    if (mEmptyLayout) {
        vkDev.destroyDescriptorSetLayout(mEmptyLayout);
        mEmptyLayout = vk::DescriptorSetLayout {};
    }
}

vk::PipelineLayout VkBindlessPipelineLayoutCache::getOrCreate(const bindless::Raster::CreateParameters & cp, VkBindlessDescriptorHeap * vkHeap) {
    return getOrCreateInternal(cp.heapSetIndex, cp.maxImmediateSize, cp.passResources, vkHeap, vk::ShaderStageFlagBits::eAllGraphics);
}

vk::PipelineLayout VkBindlessPipelineLayoutCache::getOrCreateCompute(const bindless::CnC::CreateParameters & cp, VkBindlessDescriptorHeap * vkHeap) {
    return getOrCreateInternal(cp.heapSetIndex, cp.maxImmediateSize, cp.passResources, vkHeap, vk::ShaderStageFlagBits::eCompute);
}

vk::PipelineLayout VkBindlessPipelineLayoutCache::getOrCreateInternal(uint32_t heapSetIndex, uint32_t maxImmediateSize, const GpuResourceTable & passResources,
                                                                      VkBindlessDescriptorHeap * vkHeap, vk::ShaderStageFlags stageFlags) {
    std::lock_guard<std::mutex> lock(mMutex);

    LayoutKey key;
    key.heapLayout        = vkHeap ? vkHeap->nativeDescriptorSetLayout() : vk::DescriptorSetLayout {};
    key.heapSetIndex      = heapSetIndex;
    key.maxImmediateSize  = maxImmediateSize;
    key.passResourcesHash = hashPassResources(passResources);
    key.stageFlags        = stageFlags;

    auto it = mCache.find(key);
    if (it != mCache.end()) { return it->second; }

    auto vkDev = mGpu.vulkanDevice().handle();
    if (!mEmptyLayout) {
        vk::DescriptorSetLayoutCreateInfo emptyInfo;
        mEmptyLayout = vkDev.createDescriptorSetLayout(emptyInfo);
    }

    // Determine the highest set index needed
    uint32_t maxSetIndex = heapSetIndex;
    if (!passResources.empty()) { maxSetIndex = std::max(maxSetIndex, static_cast<uint32_t>(passResources.size() - 1)); }

    std::vector<vk::DescriptorSetLayout> setLayouts(maxSetIndex + 1, mEmptyLayout);

    // Place the heap layout at heapSetIndex
    if (key.heapLayout) { setLayouts[heapSetIndex] = key.heapLayout; }

    // Build layouts for passResources sets (excluding heapSetIndex)
    for (size_t s = 0; s < passResources.size(); ++s) {
        if (s == heapSetIndex || passResources[s].empty()) continue;

        std::vector<vk::DescriptorSetLayoutBinding> bindings;
        for (size_t b = 0; b < passResources[s].size(); ++b) {
            const auto & slot = passResources[s][b];
            if (slot.empty()) continue;

            vk::DescriptorSetLayoutBinding bind;
            bind.setBinding(static_cast<uint32_t>(b)).setDescriptorCount(static_cast<uint32_t>(slot.size())).setStageFlags(stageFlags);

            if (slot[0].isTexture()) {
                if (slot[0].imageView.type == GpuResourceView::ImageView::STORAGE) {
                    bind.setDescriptorType(vk::DescriptorType::eStorageImage);
                } else {
                    bind.setDescriptorType(vk::DescriptorType::eCombinedImageSampler);
                }
            } else if (slot[0].isBuffer()) {
                if (slot[0].bufferView.type == GpuResourceView::BufferView::STORAGE) {
                    bind.setDescriptorType(vk::DescriptorType::eStorageBuffer);
                } else {
                    bind.setDescriptorType(vk::DescriptorType::eUniformBuffer);
                }
            } else {
                continue;
            }
            bindings.push_back(bind);
        }

        if (!bindings.empty()) {
            vk::DescriptorSetLayoutCreateInfo dslInfo;
            dslInfo.setBindings(bindings);
            auto customLayout = vkDev.createDescriptorSetLayout(dslInfo);
            mPassLayoutsToDestroy.push_back(customLayout);
            setLayouts[s] = customLayout;
        }
    }

    // Push constant range
    std::vector<vk::PushConstantRange> pushRanges;
    if (maxImmediateSize > 0) {
        vk::PushConstantRange pcr;
        pcr.setStageFlags(stageFlags).setOffset(0).setSize(maxImmediateSize);
        pushRanges.push_back(pcr);
    }

    vk::PipelineLayoutCreateInfo plInfo;
    plInfo.setSetLayouts(setLayouts);
    if (!pushRanges.empty()) { plInfo.setPushConstantRanges(pushRanges); }

    vk::PipelineLayout pipelineLayout;
    try {
        pipelineLayout = vkDev.createPipelineLayout(plInfo);
    } catch (const std::exception & e) {
        GN_ERROR(sLogger, "VkBindlessPipelineLayoutCache: createPipelineLayout failed: {}", e.what());
        return vk::PipelineLayout {};
    }

    mCache.emplace(key, pipelineLayout);
    return pipelineLayout;
}

} // namespace GN::gpu2
