#pragma once

#include "vk-gpu-context.h"
#include <garnet/GNgpu2.h>
#include <mutex>
#include <vector>

namespace GN::gpu2 {

/// Vulkan implementation of bindless::DescriptorHeap.
///
/// Allocates a long-lived descriptor set with VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT
/// and VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT. Maintains a thread-safe slot allocator
/// with free-list recycling for concurrent resource streaming.
class VkBindlessDescriptorHeap : public bindless::DescriptorHeap {
public:
    GN_REGISTER_RUNTIME_TYPE(bindless::DescriptorHeap);

    VkBindlessDescriptorHeap(const StrA & name, const CreateParameters & cp);
    ~VkBindlessDescriptorHeap() override;

    uint32_t allocate(const GpuResourceView & view) override;
    bool     allocate(ArrayView<const GpuResourceView> views, ArrayView<uint32_t> outIndices) override;
    bool     update(uint32_t slot, const GpuResourceView & view) override;
    uint32_t update(ArrayView<const uint32_t> slots, ArrayView<const GpuResourceView> views) override;
    void     free(uint32_t slot) override;
    void     free(ArrayView<const uint32_t> slots) override;

    uint32_t capacity() const override { return mCapacity; }
    uint32_t size() const override;
    uint32_t bindingIndex() const override { return mBindingIndex; }

    AutoRef<GpuContext> gpu() const override { return mGpu; }

    vk::DescriptorSet       nativeDescriptorSet() const { return mDescriptorSet; }
    vk::DescriptorSetLayout nativeDescriptorSetLayout() const { return mDescriptorSetLayout; }

private:
    bool writeDescriptor(uint32_t slot, const GpuResourceView & view);

    AutoRef<GpuContextVulkan2> mGpu;
    uint32_t                   mCapacity     = 0;
    uint32_t                   mBindingIndex = 0;

    vk::DescriptorSetLayout mDescriptorSetLayout {};
    vk::DescriptorPool      mDescriptorPool {};
    vk::DescriptorSet       mDescriptorSet {};
    rv::Ref<rv::Sampler>    mDefaultSampler;

    mutable std::mutex           mMutex;
    uint32_t                     mNextIndex   = 0;
    uint32_t                     mActiveCount = 0;
    std::vector<uint32_t>        mFreeList;
    std::vector<AutoRef<RCRT64>> mResources;
    std::vector<bool>            mSlotAllocated;
};

AutoRef<bindless::DescriptorHeap> createVkBindlessDescriptorHeap(const StrA & name, const bindless::DescriptorHeap::CreateParameters & cp);

} // namespace GN::gpu2
