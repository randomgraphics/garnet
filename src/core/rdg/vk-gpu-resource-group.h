#pragma once

#include "vk-gpu-context.h"

namespace GN::rdg {

/// Vulkan implementation of GpuResourceGroup: one VkDescriptorSetLayout, one VkDescriptorPool, one VkDescriptorSet.
class GpuResourceGroupVulkan : public GpuResourceGroup {
    AutoRef<GpuContextVulkan>  mGpu;
    DynaArray<SlotDescription> mSlots;

    vk::DescriptorSetLayout mLayout {};
    vk::DescriptorPool      mPool {};
    vk::DescriptorSet       mSet {};

    /// Current views per slot (for addToReadWriteList). Indexed by slot; each slot has count elements.
    DynaArray<DynaArray<GpuResourceView>> mBoundViews;

    void buildLayoutAndPool();
    void addSlotToReadWriteList(size_t slot, std::unordered_set<uint64_t> & readList, std::unordered_set<uint64_t> & writeList) const;

public:
    GpuResourceGroupVulkan(ArtifactDatabase & db, const StrA & name, AutoRef<GpuContextVulkan> gpu, const CreateParameters & params);
    ~GpuResourceGroupVulkan() override;

    GpuContext & gpu() const override { return *mGpu; }

    void setResourceViews(size_t slot, size_t offset, SafeArrayAccessor<const GpuResourceView> views) override;
    void addToReadWriteList(std::unordered_set<uint64_t> & readList, std::unordered_set<uint64_t> & writeList) const override;

    vk::DescriptorSetLayout vkLayout() const { return mLayout; }
    vk::DescriptorSet       vkSet() const { return mSet; }
};

AutoRef<GpuResourceGroup> createVulkanGpuResourceGroup(ArtifactDatabase & db, const StrA & name, const GpuResourceGroup::CreateParameters & params);

} // namespace GN::rdg
