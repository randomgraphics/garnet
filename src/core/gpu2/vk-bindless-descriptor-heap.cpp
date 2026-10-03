#include "pch.h"
#include "vk-bindless-descriptor-heap.h"
#include "vk-texture.h"

static GN::Logger * sLogger = GN::getLogger("GN.gpu2.vk.bindless");

namespace GN::gpu2 {

VkBindlessDescriptorHeap::VkBindlessDescriptorHeap(const StrA & name, const CreateParameters & cp)
    : bindless::DescriptorHeap(TYPE_INFO(), name), mCapacity(cp.capacity), mBindingIndex(cp.bindingIndex) {
    if (!cp.gpu) {
        GN_ERROR(sLogger, "VkBindlessDescriptorHeap: null GpuContext");
        return;
    }
    mGpu = RuntimeType::cast<GpuContextVulkan2>(cp.gpu.get());
    if (!mGpu || !mGpu->ready()) {
        GN_ERROR(sLogger, "VkBindlessDescriptorHeap: GpuContext is not a ready GpuContextVulkan2");
        return;
    }

    const auto & dev   = mGpu->vulkanDevice();
    auto         vkDev = dev.handle();

    // 1. Create default linear sampler for textures
    rv::Sampler::ConstructParameters scp;
    scp.gi = dev.gi();
    scp.setLinear();
    scp.info.maxLod = VK_LOD_CLAMP_NONE;
    mDefaultSampler = rv::Ref<rv::Sampler>::make(scp);

    // 2. Descriptor set layout binding flags (Vulkan 1.2 core)
    vk::DescriptorBindingFlags bindingFlags =
        vk::DescriptorBindingFlagBits::eUpdateAfterBind | vk::DescriptorBindingFlagBits::ePartiallyBound;

    vk::DescriptorSetLayoutBindingFlagsCreateInfo flagsInfo;
    flagsInfo.setBindingFlags(bindingFlags);

    // 3. Descriptor set layout binding
    vk::DescriptorSetLayoutBinding binding;
    binding.setBinding(mBindingIndex)
        .setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
        .setDescriptorCount(mCapacity)
        .setStageFlags(vk::ShaderStageFlagBits::eAll);

    vk::DescriptorSetLayoutCreateInfo layoutInfo;
    layoutInfo.setFlags(vk::DescriptorSetLayoutCreateFlagBits::eUpdateAfterBindPool)
        .setBindings(binding)
        .setPNext(&flagsInfo);

    try {
        mDescriptorSetLayout = vkDev.createDescriptorSetLayout(layoutInfo);
    } catch (const std::exception & e) {
        GN_ERROR(sLogger, "VkBindlessDescriptorHeap: createDescriptorSetLayout failed: {}", e.what());
        return;
    }

    // 4. Descriptor pool with update-after-bind flag
    vk::DescriptorPoolSize poolSize;
    poolSize.setType(vk::DescriptorType::eCombinedImageSampler).setDescriptorCount(mCapacity);

    vk::DescriptorPoolCreateInfo poolInfo;
    poolInfo.setFlags(vk::DescriptorPoolCreateFlagBits::eUpdateAfterBind).setMaxSets(1).setPoolSizes(poolSize);

    try {
        mDescriptorPool = vkDev.createDescriptorPool(poolInfo);
    } catch (const std::exception & e) {
        GN_ERROR(sLogger, "VkBindlessDescriptorHeap: createDescriptorPool failed: {}", e.what());
        vkDev.destroyDescriptorSetLayout(mDescriptorSetLayout);
        mDescriptorSetLayout = vk::DescriptorSetLayout {};
        return;
    }

    // 5. Allocate descriptor set
    vk::DescriptorSetAllocateInfo allocInfo;
    allocInfo.setDescriptorPool(mDescriptorPool).setSetLayouts(mDescriptorSetLayout);

    try {
        auto sets = vkDev.allocateDescriptorSets(allocInfo);
        if (!sets.empty()) { mDescriptorSet = sets[0]; }
    } catch (const std::exception & e) {
        GN_ERROR(sLogger, "VkBindlessDescriptorHeap: allocateDescriptorSets failed: {}", e.what());
        vkDev.destroyDescriptorPool(mDescriptorPool);
        vkDev.destroyDescriptorSetLayout(mDescriptorSetLayout);
        mDescriptorPool      = vk::DescriptorPool {};
        mDescriptorSetLayout = vk::DescriptorSetLayout {};
        return;
    }

    mResources.resize(mCapacity);
    mSlotAllocated.resize(mCapacity, false);
}

VkBindlessDescriptorHeap::~VkBindlessDescriptorHeap() {
    if (mGpu && mGpu->ready()) {
        auto vkDev = mGpu->vulkanDevice().handle();
        if (mDescriptorPool) {
            vkDev.destroyDescriptorPool(mDescriptorPool);
            mDescriptorPool = vk::DescriptorPool {};
        }
        if (mDescriptorSetLayout) {
            vkDev.destroyDescriptorSetLayout(mDescriptorSetLayout);
            mDescriptorSetLayout = vk::DescriptorSetLayout {};
        }
    }
}

bool VkBindlessDescriptorHeap::writeDescriptor(uint32_t slot, const GpuResourceView & view) {
    if (!mGpu || !mDescriptorSet) return false;
    auto * tex = RuntimeType::cast<TextureVulkanBase>(view.texture().get());
    if (!tex) {
        GN_ERROR(sLogger, "VkBindlessDescriptorHeap::writeDescriptor: resource is not a valid TextureVulkanBase");
        return false;
    }
    vk::ImageView imgView = tex->nativeView(view.imageView);
    if (!imgView) {
        GN_ERROR(sLogger, "VkBindlessDescriptorHeap::writeDescriptor: failed to get nativeView for slot {}", slot);
        return false;
    }

    vk::Sampler sampler = mDefaultSampler ? mDefaultSampler->handle() : vk::Sampler {};

    vk::DescriptorImageInfo imageInfo;
    imageInfo.setSampler(sampler).setImageView(imgView).setImageLayout(vk::ImageLayout::eShaderReadOnlyOptimal);

    vk::WriteDescriptorSet write;
    write.setDstSet(mDescriptorSet)
        .setDstBinding(mBindingIndex)
        .setDstArrayElement(slot)
        .setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
        .setDescriptorCount(1)
        .setImageInfo(imageInfo);

    mGpu->vulkanDevice().handle().updateDescriptorSets(1, &write, 0, nullptr);
    return true;
}

uint32_t VkBindlessDescriptorHeap::allocate(const GpuResourceView & view) {
    std::lock_guard<std::mutex> lock(mMutex);
    uint32_t                    slot = bindless::INVALID_DESCRIPTOR_INDEX;
    if (!mFreeList.empty()) {
        slot = mFreeList.back();
        mFreeList.pop_back();
    } else if (mNextIndex < mCapacity) {
        slot = mNextIndex++;
    } else {
        GN_ERROR(sLogger, "VkBindlessDescriptorHeap: capacity {} reached", mCapacity);
        return bindless::INVALID_DESCRIPTOR_INDEX;
    }

    if (!writeDescriptor(slot, view)) {
        mFreeList.push_back(slot);
        return bindless::INVALID_DESCRIPTOR_INDEX;
    }

    mResources[slot]     = view.resource;
    mSlotAllocated[slot] = true;
    mActiveCount++;
    return slot;
}

bool VkBindlessDescriptorHeap::update(uint32_t slot, const GpuResourceView & view) {
    std::lock_guard<std::mutex> lock(mMutex);
    if (slot >= mCapacity || !mSlotAllocated[slot]) {
        GN_ERROR(sLogger, "VkBindlessDescriptorHeap::update: slot {} is not active", slot);
        return false;
    }
    if (!writeDescriptor(slot, view)) { return false; }
    mResources[slot] = view.resource;
    return true;
}

void VkBindlessDescriptorHeap::free(uint32_t slot) {
    std::lock_guard<std::mutex> lock(mMutex);
    if (slot >= mCapacity || !mSlotAllocated[slot]) {
        GN_WARN(sLogger, "VkBindlessDescriptorHeap::free: slot {} was not allocated or double-freed", slot);
        return;
    }
    mResources[slot].clear();
    mSlotAllocated[slot] = false;
    mFreeList.push_back(slot);
    if (mActiveCount > 0) mActiveCount--;
}

uint32_t VkBindlessDescriptorHeap::size() const {
    std::lock_guard<std::mutex> lock(mMutex);
    return mActiveCount;
}

AutoRef<bindless::DescriptorHeap> createVkBindlessDescriptorHeap(const StrA & name,
                                                                 const bindless::DescriptorHeap::CreateParameters & cp) {
    auto heap = AutoRef<VkBindlessDescriptorHeap>(new VkBindlessDescriptorHeap(name, cp));
    if (!heap->nativeDescriptorSet()) return {};
    return heap;
}

} // namespace GN::gpu2
