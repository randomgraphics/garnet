#include "pch.h"
#include "vk-bindless-descriptor-heap.h"
#include "vk-texture.h"
#include "vk-buffer.h"
#include "vk-sampler.h"

#include <atomic>
#include <array>
#include <algorithm>

namespace GN::gpu2 {
namespace {
// Tokens are process-wide so a stale or foreign token cannot name a new allocation.
std::atomic<uint64_t>                       NEXT_MATERIAL_TOKEN {1};
constexpr std::array<vk::DescriptorType, 5> TYPES = {vk::DescriptorType::eSampledImage, vk::DescriptorType::eStorageImage, vk::DescriptorType::eUniformBuffer,
                                                     vk::DescriptorType::eStorageBuffer, vk::DescriptorType::eSampler};
} // namespace

VkBindlessDescriptorHeap::VkBindlessDescriptorHeap(const StrA & name, const CreateParameters & cp)
    : bindless::DescriptorHeap(TYPE_INFO(), name), mCapacity(cp.capacity), mBindingIndex(cp.bindingIndex) {
    mGpu = RuntimeType::cast<GpuContextVulkan2>(cp.gpu.get());
    if (!mGpu || !mGpu->ready() || !mCapacity || mCapacity > (1u << 28) || mBindingIndex > UINT32_MAX - 5) return;
    auto &         device       = mGpu->vulkanDevice();
    const auto     properties   = device.gi()->physical.getProperties2<vk::PhysicalDeviceProperties2, vk::PhysicalDeviceDescriptorIndexingProperties>();
    const auto &   limits       = properties.get<vk::PhysicalDeviceDescriptorIndexingProperties>();
    const uint32_t limit        = std::min({limits.maxDescriptorSetUpdateAfterBindSampledImages, limits.maxPerStageDescriptorUpdateAfterBindSampledImages,
                                            limits.maxDescriptorSetUpdateAfterBindStorageImages, limits.maxPerStageDescriptorUpdateAfterBindStorageImages,
                                            limits.maxDescriptorSetUpdateAfterBindUniformBuffers, limits.maxPerStageDescriptorUpdateAfterBindUniformBuffers,
                                            limits.maxDescriptorSetUpdateAfterBindStorageBuffers, limits.maxPerStageDescriptorUpdateAfterBindStorageBuffers,
                                            limits.maxDescriptorSetUpdateAfterBindSamplers, limits.maxPerStageDescriptorUpdateAfterBindSamplers});
    const auto     bufferLimits = device.gi()->physical.getProperties().limits;
    if (!cp.materialCapacity || cp.materialCapacity > bufferLimits.maxStorageBufferRange ||
        uint64_t(mCapacity) + 1 > limits.maxDescriptorSetUpdateAfterBindStorageBuffers ||
        uint64_t(mCapacity) + 1 > limits.maxPerStageDescriptorUpdateAfterBindStorageBuffers)
        return;
    if (mCapacity > limit || (uint64_t(mCapacity) * TYPES.size() + 1) > limits.maxPerStageUpdateAfterBindResources ||
        (uint64_t(mCapacity) * TYPES.size() + 1) > limits.maxUpdateAfterBindDescriptorsInAllPools)
        return;
    std::array<vk::DescriptorSetLayoutBinding, TYPES.size() + 1> bindings;
    std::array<vk::DescriptorBindingFlags, TYPES.size() + 1>     flags {};
    std::array<vk::DescriptorPoolSize, TYPES.size()>             sizes;
    bindings[0] = vk::DescriptorSetLayoutBinding(mBindingIndex, vk::DescriptorType::eStorageBuffer, 1, vk::ShaderStageFlagBits::eAll);
    for (uint32_t i = 0; i < TYPES.size(); ++i) {
        bindings[i + 1] = vk::DescriptorSetLayoutBinding(mBindingIndex + 1 + i, TYPES[i], mCapacity, vk::ShaderStageFlagBits::eAll);
        flags[i + 1]    = vk::DescriptorBindingFlagBits::eUpdateAfterBind | vk::DescriptorBindingFlagBits::ePartiallyBound;
        sizes[i]        = vk::DescriptorPoolSize(TYPES[i], mCapacity);
    }
    ++sizes[3].descriptorCount; // The material SSBO shares the storage-buffer pool budget.
    mMaterialBuffer = Buffer::create(name + "/materials", {.context = mGpu, .size = cp.materialCapacity});
    if (!mMaterialBuffer) return;

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
        mDescriptorSet                  = device.handle().allocateDescriptorSets(allocation).front();
        const auto *             buffer = RuntimeType::cast<BufferVulkan>(mMaterialBuffer.get());
        vk::DescriptorBufferInfo info(buffer->nativeBuffer(), 0, cp.materialCapacity);
        vk::WriteDescriptorSet   materialWrite(mDescriptorSet, mBindingIndex, 0, 1, vk::DescriptorType::eStorageBuffer, nullptr, &info);
        device.handle().updateDescriptorSets(1, &materialWrite, 0, nullptr);
        mMaterialFreeRanges.emplace(0, cp.materialCapacity);
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
    if (!index.tag || index.type >= TYPES.size() || view.empty() || view.combinedTextureSampler) return false;
    auto & descriptor = write.descriptor;
    descriptor.setDstSet(mDescriptorSet)
        .setDstBinding(mBindingIndex + 1 + index.type)
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
    return index.tag && index.type < TYPES.size() && index.slot < mSlots.size() && !mSlots[index.slot].view.empty() &&
           uint32_t(mSlots[index.slot].type) == index.type;
}

VkBindlessDescriptorHeap::DescriptorIndex VkBindlessDescriptorHeap::allocate(DescriptorType type, const GpuResourceView & view) {
    std::lock_guard<std::mutex> lock(mMutex);
    if (!mDescriptorSet || (mFreeList.empty() && mNextIndex == mCapacity)) return INVALID_DESCRIPTOR_INDEX;
    const uint32_t  slot = mFreeList.empty() ? mNextIndex : mFreeList.back();
    DescriptorIndex index {0x80000000u | (uint32_t(type) << 28) | slot};
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

VkBindlessDescriptorHeap::MaterialToken VkBindlessDescriptorHeap::allocateMaterial(uint64_t size, uint64_t alignment) {
    std::lock_guard<std::mutex> lock(mMutex);
    if (!mDescriptorSet || !size || !alignment || (alignment % 4 != 0)) GN_UNLIKELY return INVALID_MATERIAL_TOKEN;
    for (auto it = mMaterialFreeRanges.begin(); it != mMaterialFreeRanges.end(); ++it) {
        const uint64_t start = it->first, available = it->second;
        const uint64_t remainder = start % alignment;
        const uint64_t padding   = remainder ? (alignment - remainder) : 0;
        if (padding > available || size > available - padding) continue;
        const uint64_t offset = start + padding;
        const auto     token  = NEXT_MATERIAL_TOKEN.fetch_add(1, std::memory_order_relaxed);
        if (!token) GN_UNLIKELY return INVALID_MATERIAL_TOKEN;
        mMaterials.emplace(token, MaterialRange {offset, size});
        mMaterialFreeRanges.erase(it);
        if (padding) mMaterialFreeRanges.emplace(start, padding);
        if (available > padding + size) mMaterialFreeRanges.emplace(offset + size, available - padding - size);
        return token;
    }
    return INVALID_MATERIAL_TOKEN;
}

GpuResourceView VkBindlessDescriptorHeap::materialView(MaterialToken token) const {
    std::lock_guard<std::mutex> lock(mMutex);
    const auto                  it = mMaterials.find(token);
    if (it == mMaterials.end()) GN_UNLIKELY return {};
    return GpuResourceView {mMaterialBuffer}
        .setBufferViewType(GpuResourceView::BufferView::STORAGE)
        .setBufferViewOffset(it->second.offset)
        .setBufferViewSize(it->second.size);
}

void VkBindlessDescriptorHeap::freeMaterial(MaterialToken token) {
    std::lock_guard<std::mutex> lock(mMutex);
    const auto                  allocation = mMaterials.find(token);
    if (allocation == mMaterials.end()) GN_UNLIKELY return;
    auto range = allocation->second;
    mMaterials.erase(allocation);
    auto next = mMaterialFreeRanges.lower_bound(range.offset);
    if (next != mMaterialFreeRanges.begin()) {
        auto previous = std::prev(next);
        if (previous->first + previous->second == range.offset) {
            range.offset = previous->first;
            range.size += previous->second;
            mMaterialFreeRanges.erase(previous);
        }
    }
    if (next != mMaterialFreeRanges.end() && range.offset + range.size == next->first) {
        range.size += next->second;
        mMaterialFreeRanges.erase(next);
    }
    mMaterialFreeRanges.emplace(range.offset, range.size);
}

AutoRef<bindless::DescriptorHeap> createVkBindlessDescriptorHeap(const StrA & name, const bindless::DescriptorHeap::CreateParameters & cp) {
    auto heap = referenceTo(new VkBindlessDescriptorHeap(name, cp));
    return heap->nativeDescriptorSet() ? AutoRef<bindless::DescriptorHeap>(heap) : AutoRef<bindless::DescriptorHeap>();
}

} // namespace GN::gpu2
