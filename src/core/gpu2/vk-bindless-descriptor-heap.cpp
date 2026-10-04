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
    vk::DescriptorBindingFlags bindingFlags = vk::DescriptorBindingFlagBits::eUpdateAfterBind | vk::DescriptorBindingFlagBits::ePartiallyBound;

    vk::DescriptorSetLayoutBindingFlagsCreateInfo flagsInfo;
    flagsInfo.setBindingFlags(bindingFlags);

    // 3. Descriptor set layout binding
    vk::DescriptorSetLayoutBinding binding;
    binding.setBinding(mBindingIndex)
        .setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
        .setDescriptorCount(mCapacity)
        .setStageFlags(vk::ShaderStageFlagBits::eAll);

    vk::DescriptorSetLayoutCreateInfo layoutInfo;
    layoutInfo.setFlags(vk::DescriptorSetLayoutCreateFlagBits::eUpdateAfterBindPool).setBindings(binding).setPNext(&flagsInfo);

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
    imageInfo.setSampler(sampler).setImageView(imgView).setImageLayout(shaderReadOnlyLayout(tex->descriptor().format));

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
    uint32_t slot = bindless::INVALID_DESCRIPTOR_INDEX;
    if (allocate(ArrayView<const GpuResourceView>(&view, 1), ArrayView<uint32_t>(&slot, 1))) { return slot; }
    return bindless::INVALID_DESCRIPTOR_INDEX;
}

bool VkBindlessDescriptorHeap::allocate(ArrayView<const GpuResourceView> views, ArrayView<uint32_t> outIndices) {
    if (views.size() != outIndices.size()) {
        GN_ERROR(sLogger, "VkBindlessDescriptorHeap::allocate batch: views size {} != outIndices size {}", views.size(), outIndices.size());
        return false;
    }
    if (views.empty()) return true;

    std::lock_guard<std::mutex> lock(mMutex);
    const size_t                count     = views.size();
    const size_t                available = mFreeList.size() + (mCapacity - mNextIndex);
    if (count > available) {
        GN_ERROR(sLogger, "VkBindlessDescriptorHeap: insufficient capacity for batch of {} descriptors (available: {})", count, available);
        return false;
    }

    // Phase 1: validate all views upfront before modifying any allocator state
    std::vector<vk::ImageView>   imgViews;
    std::vector<vk::ImageLayout> layouts;
    layouts.reserve(count);
    imgViews.reserve(count);
    for (size_t i = 0; i < count; ++i) {
        auto * tex = RuntimeType::cast<TextureVulkanBase>(views[i].texture().get());
        if (!tex) {
            GN_ERROR(sLogger, "VkBindlessDescriptorHeap::allocate batch: view[{}] is not a valid TextureVulkanBase", i);
            return false;
        }
        vk::ImageView iv = tex->nativeView(views[i].imageView);
        if (!iv) {
            GN_ERROR(sLogger, "VkBindlessDescriptorHeap::allocate batch: view[{}] failed to get nativeView", i);
            return false;
        }
        imgViews.push_back(iv);
        layouts.push_back(shaderReadOnlyLayout(tex->descriptor().format));
    }

    // Phase 2: reserve slots and prepare driver descriptor writes
    vk::Sampler                          sampler = mDefaultSampler ? mDefaultSampler->handle() : vk::Sampler {};
    std::vector<uint32_t>                slots;
    std::vector<vk::DescriptorImageInfo> imageInfos;
    std::vector<vk::WriteDescriptorSet>  writes;
    slots.reserve(count);
    imageInfos.reserve(count);
    writes.reserve(count);

    for (size_t i = 0; i < count; ++i) {
        uint32_t slot = bindless::INVALID_DESCRIPTOR_INDEX;
        if (!mFreeList.empty()) {
            slot = mFreeList.back();
            mFreeList.pop_back();
        } else {
            slot = mNextIndex++;
        }
        slots.push_back(slot);

        vk::DescriptorImageInfo info;
        info.setSampler(sampler).setImageView(imgViews[i]).setImageLayout(layouts[i]);
        imageInfos.push_back(info);
    }

    for (size_t i = 0; i < count; ++i) {
        vk::WriteDescriptorSet write;
        write.setDstSet(mDescriptorSet)
            .setDstBinding(mBindingIndex)
            .setDstArrayElement(slots[i])
            .setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
            .setDescriptorCount(1)
            .setImageInfo(imageInfos[i]);
        writes.push_back(write);
    }

    // Phase 3: batched driver update
    mGpu->vulkanDevice().handle().updateDescriptorSets(static_cast<uint32_t>(writes.size()), writes.data(), 0, nullptr);

    // Phase 4: commit allocations
    for (size_t i = 0; i < count; ++i) {
        uint32_t s        = slots[i];
        mResources[s]     = views[i].resource;
        mSlotAllocated[s] = true;
        mActiveCount++;
        outIndices[i] = s;
    }

    return true;
}

bool VkBindlessDescriptorHeap::update(uint32_t slot, const GpuResourceView & view) {
    return update(ArrayView<const uint32_t>(&slot, 1), ArrayView<const GpuResourceView>(&view, 1)) == 1;
}

uint32_t VkBindlessDescriptorHeap::update(ArrayView<const uint32_t> slots, ArrayView<const GpuResourceView> views) {
    if (slots.size() != views.size()) {
        GN_ERROR(sLogger, "VkBindlessDescriptorHeap::update batch: slots size {} != views size {}", slots.size(), views.size());
        return 0;
    }
    if (slots.empty()) return 0;

    std::lock_guard<std::mutex> lock(mMutex);
    const size_t                count = slots.size();

    vk::Sampler                          sampler = mDefaultSampler ? mDefaultSampler->handle() : vk::Sampler {};
    std::vector<vk::DescriptorImageInfo> imageInfos;
    std::vector<vk::WriteDescriptorSet>  writes;
    std::vector<size_t>                  validIndices;
    imageInfos.reserve(count);
    writes.reserve(count);
    validIndices.reserve(count);

    // Permissive policy: inspect each slot/view pair independently, skipping invalid ones
    for (size_t i = 0; i < count; ++i) {
        uint32_t s = slots[i];
        if (s >= mCapacity || !mSlotAllocated[s]) {
            GN_VERBOSE(sLogger, "VkBindlessDescriptorHeap::update batch: slot {} at index {} is not active; skipping", s, i);
            continue;
        }

        auto * tex = RuntimeType::cast<TextureVulkanBase>(views[i].texture().get());
        if (!tex) {
            GN_VERBOSE(sLogger, "VkBindlessDescriptorHeap::update batch: view[{}] is not a valid TextureVulkanBase; skipping", i);
            continue;
        }
        vk::ImageView iv = tex->nativeView(views[i].imageView);
        if (!iv) {
            GN_VERBOSE(sLogger, "VkBindlessDescriptorHeap::update batch: view[{}] failed to get nativeView; skipping", i);
            continue;
        }

        vk::DescriptorImageInfo info;
        info.setSampler(sampler).setImageView(iv).setImageLayout(shaderReadOnlyLayout(tex->descriptor().format));
        imageInfos.push_back(info);

        vk::WriteDescriptorSet write;
        write.setDstSet(mDescriptorSet)
            .setDstBinding(mBindingIndex)
            .setDstArrayElement(s)
            .setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
            .setDescriptorCount(1)
            .setImageInfo(imageInfos.back());
        writes.push_back(write);
        validIndices.push_back(i);
    }

    if (writes.empty()) return 0;

    // Batched driver update for all valid slots
    mGpu->vulkanDevice().handle().updateDescriptorSets(static_cast<uint32_t>(writes.size()), writes.data(), 0, nullptr);

    // Commit new resource references for updated slots
    for (size_t vi = 0; vi < validIndices.size(); ++vi) {
        size_t origIdx             = validIndices[vi];
        mResources[slots[origIdx]] = views[origIdx].resource;
    }

    return static_cast<uint32_t>(writes.size());
}

void VkBindlessDescriptorHeap::free(uint32_t slot) { free(ArrayView<const uint32_t>(&slot, 1)); }

void VkBindlessDescriptorHeap::free(ArrayView<const uint32_t> slots) {
    if (slots.empty()) return;

    std::lock_guard<std::mutex> lock(mMutex);
    for (uint32_t s : slots) {
        if (s >= mCapacity || !mSlotAllocated[s]) {
            GN_WARN(sLogger, "VkBindlessDescriptorHeap::free: slot {} was not allocated or double-freed", s);
            continue;
        }
        mResources[s].clear();
        mSlotAllocated[s] = false;
        mFreeList.push_back(s);
        if (mActiveCount > 0) mActiveCount--;
    }
}

uint32_t VkBindlessDescriptorHeap::size() const {
    std::lock_guard<std::mutex> lock(mMutex);
    return mActiveCount;
}

AutoRef<bindless::DescriptorHeap> createVkBindlessDescriptorHeap(const StrA & name, const bindless::DescriptorHeap::CreateParameters & cp) {
    auto heap = AutoRef<VkBindlessDescriptorHeap>(new VkBindlessDescriptorHeap(name, cp));
    if (!heap->nativeDescriptorSet()) return {};
    return heap;
}

} // namespace GN::gpu2
