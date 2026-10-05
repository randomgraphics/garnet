#include "pch.h"
#include "vk-bindless-descriptor-heap.h"
#include "vk-texture.h"
#include "vk-buffer.h"
#include "vk-sampler.h"
#include <array>
#include <algorithm>

namespace GN::gpu2 {
namespace {
constexpr std::array<vk::DescriptorType, 5> TYPES = {vk::DescriptorType::eSampledImage, vk::DescriptorType::eStorageImage, vk::DescriptorType::eUniformBuffer,
                                                     vk::DescriptorType::eStorageBuffer, vk::DescriptorType::eSampler};
}

VkBindlessDescriptorHeap::VkBindlessDescriptorHeap(const StrA & name, const CreateParameters & cp)
    : bindless::DescriptorHeap(TYPE_INFO(), name), mCapacity(cp.capacity), mBindingIndex(cp.bindingIndex) {
    mGpu = RuntimeType::cast<GpuContextVulkan2>(cp.gpu.get());
    if (!mGpu || !mGpu->ready() || !mCapacity || mCapacity > (1u << 28) || mBindingIndex > UINT32_MAX - 4) return;
    auto &         device     = mGpu->vulkanDevice();
    const auto     properties = device.gi()->physical.getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceDescriptorIndexingProperties>();
    const auto &   limits     = properties.get<vk::PhysicalDeviceDescriptorIndexingProperties>();
    const uint32_t limit      = std::min({limits.maxDescriptorSetUpdateAfterBindSampledImages, limits.maxPerStageDescriptorUpdateAfterBindSampledImages,
                                          limits.maxDescriptorSetUpdateAfterBindStorageImages, limits.maxPerStageDescriptorUpdateAfterBindStorageImages,
                                          limits.maxDescriptorSetUpdateAfterBindUniformBuffers, limits.maxPerStageDescriptorUpdateAfterBindUniformBuffers,
                                          limits.maxDescriptorSetUpdateAfterBindStorageBuffers, limits.maxPerStageDescriptorUpdateAfterBindStorageBuffers,
                                          limits.maxDescriptorSetUpdateAfterBindSamplers, limits.maxPerStageDescriptorUpdateAfterBindSamplers});
    if (mCapacity > limit || uint64_t(mCapacity) * TYPES.size() > limits.maxPerStageUpdateAfterBindResources ||
        uint64_t(mCapacity) * TYPES.size() > limits.maxUpdateAfterBindDescriptorsInAllPools)
        return;
    std::array<vk::DescriptorSetLayoutBinding, TYPES.size()> bindings;
    std::array<vk::DescriptorBindingFlags, TYPES.size()>     flags;
    std::array<vk::DescriptorPoolSize, TYPES.size()>         sizes;
    for (uint32_t i = 0; i < TYPES.size(); ++i) {
        bindings[i] = vk::DescriptorSetLayoutBinding(mBindingIndex + i, TYPES[i], mCapacity, vk::ShaderStageFlagBits::eAll);
        flags[i]    = vk::DescriptorBindingFlagBits::eUpdateAfterBind | vk::DescriptorBindingFlagBits::ePartiallyBound;
        sizes[i]    = vk::DescriptorPoolSize(TYPES[i], mCapacity);
    }
    try {
        vk::DescriptorSetLayoutBindingFlagsCreateInfo flagInfo;
        flagInfo.setBindingFlags(flags);
        vk::DescriptorSetLayoutCreateInfo layout;
        layout.setFlags(vk::DescriptorSetLayoutCreateFlagBits::eUpdateAfterBindPool).setBindings(bindings).setPNext(&flagInfo);
        mDescriptorSetLayout = device.handle().createDescriptorSetLayout(layout);
        vk::DescriptorPoolCreateInfo pool;
        pool.setFlags(vk::DescriptorPoolCreateFlagBits::eUpdateAfterBind).setMaxSets(1).setPoolSizes(sizes);
        mDescriptorPool = device.handle().createDescriptorPool(pool);
        vk::DescriptorSetAllocateInfo allocation;
        allocation.setDescriptorPool(mDescriptorPool).setSetLayouts(mDescriptorSetLayout);
        mDescriptorSet = device.handle().allocateDescriptorSets(allocation).front();
        mSlots.resize(mCapacity);
        mFreeList.reserve(mCapacity);
    } catch (const std::exception & e) {
        GN_ERROR(getLogger("GN.gpu2.vk.bindless"), "Descriptor heap creation failed: {}", e.what());
        mDescriptorSet = vk::DescriptorSet {};
    }
}

VkBindlessDescriptorHeap::~VkBindlessDescriptorHeap() {
    if (!mGpu || !mGpu->ready()) return;
    if (mDescriptorPool) mGpu->vulkanDevice().handle().destroyDescriptorPool(mDescriptorPool);
    if (mDescriptorSetLayout) mGpu->vulkanDevice().handle().destroyDescriptorSetLayout(mDescriptorSetLayout);
}

bool VkBindlessDescriptorHeap::prepareWrite(DescriptorIndex index, const GpuResourceView & view, Write & write) const {
    if (index.type >= TYPES.size() || view.empty() || view.combinedTextureSampler) return false;
    auto & descriptor = write.descriptor;
    descriptor.setDstSet(mDescriptorSet)
        .setDstBinding(mBindingIndex + index.type)
        .setDstArrayElement(index.slot)
        .setDescriptorType(TYPES[index.type])
        .setDescriptorCount(1);
    if (index.type == SAMPLED_TEXTURE || index.type == STORAGE_TEXTURE) {
        auto *     texture  = RuntimeType::cast<TextureVulkanBase>(view.texture().get());
        const auto expected = index.type == SAMPLED_TEXTURE ? GpuResourceView::ImageView::SAMPLED : GpuResourceView::ImageView::STORAGE;
        if (!texture || texture->context() != mGpu.get() || view.imageView.type != expected) return false;
        auto image = texture->nativeView(view.imageView);
        if (!image) return false;
        write.image.setImageView(image).setImageLayout(index.type == SAMPLED_TEXTURE ? shaderReadOnlyLayout(texture->descriptor().format)
                                                                                     : vk::ImageLayout::eGeneral);
        descriptor.setImageInfo(write.image);
    } else if (index.type == UNIFORM_BUFFER || index.type == STORAGE_BUFFER) {
        auto *     buffer   = RuntimeType::cast<BufferVulkan>(view.buffer().get());
        const auto expected = index.type == UNIFORM_BUFFER ? GpuResourceView::BufferView::UNIFORM : GpuResourceView::BufferView::STORAGE;
        if (!buffer || buffer->context() != mGpu.get() || view.bufferView.type != expected || view.bufferView.offset >= buffer->bufferSize()) return false;
        const auto &   limits    = mGpu->vulkanDevice().gi()->physical.getProperties().limits;
        const uint64_t size      = view.bufferView.size ? view.bufferView.size : buffer->bufferSize() - view.bufferView.offset;
        const uint64_t alignment = index.type == UNIFORM_BUFFER ? limits.minUniformBufferOffsetAlignment : limits.minStorageBufferOffsetAlignment;
        const uint64_t maxSize   = index.type == UNIFORM_BUFFER ? limits.maxUniformBufferRange : limits.maxStorageBufferRange;
        if (size > buffer->bufferSize() - view.bufferView.offset || size > maxSize || view.bufferView.offset % alignment) return false;
        write.buffer.setBuffer(buffer->nativeBuffer()).setOffset(view.bufferView.offset).setRange(size);
        descriptor.setBufferInfo(write.buffer);
    } else {
        auto * sampler = RuntimeType::cast<SamplerVulkan>(view.sampler().get());
        if (!sampler || sampler->context() != mGpu.get()) return false;
        write.image.setSampler(sampler->nativeSampler());
        descriptor.setImageInfo(write.image);
    }
    return true;
}

bool VkBindlessDescriptorHeap::allocated(DescriptorIndex index) const {
    return index.type < TYPES.size() && index.slot < mSlots.size() && !mSlots[index.slot].view.empty() && uint32_t(mSlots[index.slot].type) == index.type;
}

VkBindlessDescriptorHeap::DescriptorIndex VkBindlessDescriptorHeap::allocate(DescriptorType type, const GpuResourceView & view) {
    std::lock_guard<std::mutex> lock(mMutex);
    if (!mDescriptorSet || (mFreeList.empty() && mNextIndex == mCapacity)) return INVALID_DESCRIPTOR_INDEX;
    const uint32_t  slot = mFreeList.empty() ? mNextIndex : mFreeList.back();
    DescriptorIndex index {(uint32_t(type) << 28) | slot};
    Write           write;
    if (uint32_t(type) >= TYPES.size() || !prepareWrite(index, view, write)) return INVALID_DESCRIPTOR_INDEX;
    mGpu->vulkanDevice().handle().updateDescriptorSets(1, &write.descriptor, 0, nullptr);
    if (mFreeList.empty())
        ++mNextIndex;
    else
        mFreeList.pop_back();
    mSlots[slot] = {view, type};
    ++mActiveCount;
    return index;
}

bool   VkBindlessDescriptorHeap::update(DescriptorIndex index, const GpuResourceView & view) { return update({&index, 1}, {&view, 1}) == 1; }
size_t VkBindlessDescriptorHeap::update(ArrayView<const DescriptorIndex> indices, ArrayView<const GpuResourceView> views) {
    if (indices.size() != views.size()) return 0;
    std::lock_guard<std::mutex>         lock(mMutex);
    std::vector<Write>                  storage(indices.size());
    std::vector<vk::WriteDescriptorSet> writes;
    std::vector<size_t>                 valid;
    writes.reserve(indices.size());
    valid.reserve(indices.size());
    for (size_t i = 0; i < indices.size(); ++i) {
        if (!allocated(indices[i]) || !prepareWrite(indices[i], views[i], storage[i])) continue;
        writes.push_back(storage[i].descriptor);
        valid.push_back(i);
    }
    if (writes.empty()) return 0;
    mGpu->vulkanDevice().handle().updateDescriptorSets(writes, {});
    for (auto i : valid) mSlots[indices[i].slot].view = views[i];
    return writes.size();
}

void VkBindlessDescriptorHeap::free(DescriptorIndex index) { free({&index, 1}); }
void VkBindlessDescriptorHeap::free(ArrayView<const DescriptorIndex> indices) {
    std::lock_guard<std::mutex> lock(mMutex);
    for (auto index : indices) {
        if (!allocated(index)) continue;
        mSlots[index.slot].view = {};
        mFreeList.push_back(index.slot);
        --mActiveCount;
    }
}
uint32_t VkBindlessDescriptorHeap::size() const {
    std::lock_guard<std::mutex> lock(mMutex);
    return mActiveCount;
}

AutoRef<bindless::DescriptorHeap> createVkBindlessDescriptorHeap(const StrA & name, const bindless::DescriptorHeap::CreateParameters & cp) {
    auto heap = referenceTo(new VkBindlessDescriptorHeap(name, cp));
    return heap->nativeDescriptorSet() ? AutoRef<bindless::DescriptorHeap>(heap) : AutoRef<bindless::DescriptorHeap>();
}

} // namespace GN::gpu2
