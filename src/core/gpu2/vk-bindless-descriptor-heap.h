#pragma once

#include "vk-gpu-context.h"
#include <garnet/GNgpu2.h>
#include <map>
#include <mutex>
#include <vector>

namespace GN::gpu2 {

class VkBindlessDescriptorHeap final : public bindless::DescriptorHeap {
public:
    GN_REGISTER_RUNTIME_TYPE(bindless::DescriptorHeap);
    VkBindlessDescriptorHeap(const StrA &, const CreateParameters &);
    ~VkBindlessDescriptorHeap() override;
    DescriptorIndex         allocate(DescriptorType, const GpuResourceView &) override;
    bool                    update(DescriptorIndex, const GpuResourceView &) override;
    size_t                  update(ArrayView<const DescriptorIndex>, ArrayView<const GpuResourceView>) override;
    void                    free(DescriptorIndex) override;
    void                    free(ArrayView<const DescriptorIndex>) override;
    MaterialToken           allocateMaterial(uint64_t size, uint64_t alignment) override;
    GpuResourceView         materialView(MaterialToken token) const override;
    void                    freeMaterial(MaterialToken token) override;
    AutoRef<Buffer>         materialBuffer() const { return mMaterialBuffer; }
    uint32_t                capacity() const override { return mCapacity; }
    uint32_t                size() const override;
    uint32_t                bindingIndex() const override { return mBindingIndex; }
    AutoRef<GpuContext>     gpu() const override { return mGpu; }
    vk::DescriptorSet       nativeDescriptorSet() const { return mDescriptorSet; }
    vk::DescriptorSetLayout nativeDescriptorSetLayout() const { return mDescriptorSetLayout; }

private:
    struct Slot {
        GpuResourceView view;
        DescriptorType  type = SAMPLED_TEXTURE;
    };
    struct MaterialRange {
        uint64_t offset = 0, size = 0;
    };

    struct Write {
        vk::DescriptorImageInfo  image;
        vk::DescriptorBufferInfo buffer;
        vk::WriteDescriptorSet   descriptor;
    };
    bool                                   prepareWrite(DescriptorIndex, const GpuResourceView &, Write &) const;
    bool                                   allocated(DescriptorIndex) const;
    AutoRef<GpuContextVulkan2>             mGpu;
    AutoRef<Buffer>                        mMaterialBuffer;
    std::map<uint64_t, uint64_t>           mMaterialFreeRanges;
    std::map<MaterialToken, MaterialRange> mMaterials;
    uint32_t                               mCapacity = 0, mBindingIndex = 0;
    vk::DescriptorSetLayout                mDescriptorSetLayout {};
    vk::DescriptorPool                     mDescriptorPool {};
    vk::DescriptorSet                      mDescriptorSet {};
    mutable std::mutex                     mMutex;
    uint32_t                               mNextIndex = 0, mActiveCount = 0;
    std::vector<uint32_t>                  mFreeList;
    std::vector<Slot>                      mSlots;
};

AutoRef<bindless::DescriptorHeap> createVkBindlessDescriptorHeap(const StrA &, const bindless::DescriptorHeap::CreateParameters &);

} // namespace GN::gpu2
