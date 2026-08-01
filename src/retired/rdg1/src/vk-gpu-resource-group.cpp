#include "pch.h"
// GpuResourceGroupVulkan commented out: use GpuResourceTable + Drawable/DrawPack instead.
#if 0
    #include "vk-gpu-resource-group.h"
    #include "vk-transient-buffer.h"
    #include "vk-texture.h"
    #include "vk-backbuffer.h"
    #include "vk-format-utils.h"
    #include "gpu-context.h"

namespace GN::rdg {

static GN::Logger * sLogger = GN::getLogger("GN.rdg.vk");

namespace {

static vk::DescriptorType slotTypeToVk(GpuResourceGroup::SlotDescription::Type t) {
    using Type = GpuResourceGroup::SlotDescription::Type;
    switch (t) {
    case Type::UNIFORM_BUFFER:
        return vk::DescriptorType::eUniformBuffer;
    case Type::STORAGE_BUFFER:
        return vk::DescriptorType::eStorageBuffer;
    case Type::SAMPLED_TEXTURE:
        return vk::DescriptorType::eCombinedImageSampler;
    case Type::STORAGE_TEXTURE:
        return vk::DescriptorType::eStorageImage;
    case Type::SAMPLER:
        return vk::DescriptorType::eSampler;
    default:
        return vk::DescriptorType::eUniformBuffer;
    }
}

static vk::ShaderStageFlags stagesToVk(GpuShaderStageFlags stages) {
    auto                 m = static_cast<uint32_t>(stages);
    vk::ShaderStageFlags f {};
    if (m & static_cast<uint32_t>(GpuShaderStageBits::VERTEX)) f |= vk::ShaderStageFlagBits::eVertex;
    if (m & static_cast<uint32_t>(GpuShaderStageBits::HULL)) f |= vk::ShaderStageFlagBits::eTessellationControl;
    if (m & static_cast<uint32_t>(GpuShaderStageBits::DOMAIN_)) f |= vk::ShaderStageFlagBits::eTessellationEvaluation;
    if (m & static_cast<uint32_t>(GpuShaderStageBits::GEOMETRY)) f |= vk::ShaderStageFlagBits::eGeometry;
    if (m & static_cast<uint32_t>(GpuShaderStageBits::PIXEL)) f |= vk::ShaderStageFlagBits::eFragment;
    if (m & static_cast<uint32_t>(GpuShaderStageBits::COMPUTE)) f |= vk::ShaderStageFlagBits::eCompute;
    return f ? f : (vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment);
}

/// Get VkImageView for a GpuResourceView that is a texture or backbuffer. Returns VK_NULL_HANDLE on failure.
static vk::ImageView getImageView(const GpuResourceView & view, const rapid_vulkan::GlobalInfo &) {
    if (!view.isImage()) return {};
    const auto & iv        = view.imageView;
    uint32_t     mipLevels = iv.range.e.numMipLevels;
    uint32_t     numLayers = iv.range.e.numArrayLayers;
    if (mipLevels == (uint32_t) -1) mipLevels = 1;
    if (numLayers == (uint32_t) -1) numLayers = 1;

    vk::ImageAspectFlags aspect =
        view.isTexture() ? vk::ImageAspectFlagBits::eColor : (view.isBackbuffer() ? vk::ImageAspectFlagBits::eColor : vk::ImageAspectFlagBits::eColor);

    if (view.isTexture()) {
        auto tex = view.texture().staticCastTo<TextureVulkan>();
        if (!tex || !tex->image()) return {};
        vk::Format format =
            (iv.format != gfx::img::PixelFormat::UNKNOWN()) ? pixelFormatToVkFormat(iv.format) : pixelFormatToVkFormat(tex->descriptor().format);
        auto params = rapid_vulkan::Image::GetViewParameters()
                          .setType(vk::ImageViewType::e2D)
                          .setFormat(format)
                          .setRange(vk::ImageSubresourceRange(aspect, iv.range.i.mip, mipLevels, iv.range.i.face, numLayers));
        return tex->image()->getView(params);
    }
    if (view.isBackbuffer()) {
        auto bb = view.backbuffer().staticCastTo<BackbufferVulkan>();
        if (!bb || !bb->backBufferImage()) return {};
        const auto * img    = bb->backBufferImage();
        vk::Format   format = img->desc().format;
        auto         params =
            rapid_vulkan::Image::GetViewParameters().setType(vk::ImageViewType::e2D).setFormat(format).setRange(vk::ImageSubresourceRange(aspect, 0, 1, 0, 1));
        return img->getView(params);
    }
    return {};
}

} // namespace

// =============================================================================
// GpuResourceGroupVulkan
// =============================================================================

void GpuResourceGroupVulkan::buildLayoutAndPool() {
    const auto & gi  = mGpu->globalInfo();
    auto &       dev = gi.device;

    std::vector<vk::DescriptorSetLayoutBinding> bindings;
    std::map<vk::DescriptorType, uint32_t>      poolCounts;

    for (size_t i = 0; i < mSlots.size(); ++i) {
        const auto & s = mSlots[i];
        if (s.count == 0) continue;
        uint32_t           count = static_cast<uint32_t>(s.count);
        vk::DescriptorType dtype = slotTypeToVk(s.type);
        bindings.push_back(vk::DescriptorSetLayoutBinding {}
                               .setBinding(static_cast<uint32_t>(i))
                               .setDescriptorType(dtype)
                               .setDescriptorCount(count)
                               .setStageFlags(stagesToVk(s.stages)));
        poolCounts[dtype] += count;
    }

    if (bindings.empty()) return;

    vk::DescriptorSetLayoutCreateInfo layoutInfo {};
    layoutInfo.setBindings(bindings);
    mLayout = dev.createDescriptorSetLayout(layoutInfo, gi.allocator);
    if (!mLayout) GN_UNLIKELY {
            GN_ERROR(sLogger, "GpuResourceGroupVulkan: failed to create descriptor set layout, name='{}'", name);
            return;
        }

    std::vector<vk::DescriptorPoolSize> poolSizes;
    poolSizes.reserve(poolCounts.size());
    for (const auto & kv : poolCounts) poolSizes.push_back(vk::DescriptorPoolSize(kv.first, kv.second));

    vk::DescriptorPoolCreateInfo poolInfo {};
    poolInfo.setPoolSizes(poolSizes).setMaxSets(1);
    mPool = dev.createDescriptorPool(poolInfo, gi.allocator);
    if (!mPool) GN_UNLIKELY {
            GN_ERROR(sLogger, "GpuResourceGroupVulkan: failed to create descriptor pool, name='{}'", name);
            dev.destroy(mLayout, gi.allocator);
            mLayout = nullptr;
            return;
        }

    vk::DescriptorSetAllocateInfo allocInfo {};
    allocInfo.setDescriptorPool(mPool).setSetLayouts(mLayout);
    auto sets = dev.allocateDescriptorSets(allocInfo);
    if (sets.empty()) GN_UNLIKELY {
            GN_ERROR(sLogger, "GpuResourceGroupVulkan: failed to allocate descriptor set, name='{}'", name);
            dev.destroy(mPool, gi.allocator);
            dev.destroy(mLayout, gi.allocator);
            mPool   = nullptr;
            mLayout = nullptr;
            return;
        }
    mSet = sets[0];
}

GpuResourceGroupVulkan::GpuResourceGroupVulkan(const StrA & name, AutoRef<GpuContextVulkan> gpu,
                                               const GpuResourceGroup::CreateParameters & params)
    : GpuResourceGroup(GpuResourceGroup::TYPE_INFO(), name), mGpu(std::move(gpu)), mSlots(params.slots) {
    mBoundViews.resize(params.slots.size());
    buildLayoutAndPool();
}

GpuResourceGroupVulkan::~GpuResourceGroupVulkan() {
    if (!mGpu) return;
    const auto & gi = mGpu->globalInfo();
    if (mPool) gi.safeDestroy(mPool);
    if (mLayout) gi.safeDestroy(mLayout);
    mSet    = nullptr;
    mPool   = nullptr;
    mLayout = nullptr;
    mGpu    = {};
}

void GpuResourceGroupVulkan::setResourceViews(size_t slot, size_t offset, ArrayView<const GpuResourceView> views) {
    if (slot >= mSlots.size()) GN_UNLIKELY return;
    if (!mSet) return;

    const auto & slotDesc = mSlots[slot];
    const size_t n        = views.size();
    if (n == 0) return;

    // Update stored views for addToReadWriteList
    auto & stored = mBoundViews[slot];
    stored.clear();
    for (size_t i = 0; i < n; ++i) stored.append(views[i]);

    const auto & gi  = mGpu->globalInfo();
    auto &       dev = gi.device;

    std::vector<vk::WriteDescriptorSet>   writes;
    std::vector<vk::DescriptorBufferInfo> bufferInfos;
    std::vector<vk::DescriptorImageInfo>  imageInfos;

    const uint32_t binding      = static_cast<uint32_t>(slot);
    const uint32_t arrayElement = static_cast<uint32_t>(offset);

    using SlotType = GpuResourceGroup::SlotDescription::Type;
    if (slotDesc.type == SlotType::UNIFORM_BUFFER || slotDesc.type == SlotType::STORAGE_BUFFER) {
        bufferInfos.reserve(n);
        for (size_t i = 0; i < n; ++i) {
            const auto & v      = views[i];
            auto         handle = BufferUtils::getHandle(v.buffer());
            if (!handle) GN_UNLIKELY continue;
            uint64_t off = v.bufferView.offset;
            uint64_t sz  = v.bufferView.size;
            if (sz == 0) sz = vk::DeviceSize(-1);
            bufferInfos.push_back(vk::DescriptorBufferInfo {}.setBuffer(handle).setOffset(off).setRange(sz));
        }
        if (!bufferInfos.empty()) {
            writes.push_back(vk::WriteDescriptorSet {}
                                 .setDstSet(mSet)
                                 .setDstBinding(binding)
                                 .setDstArrayElement(arrayElement)
                                 .setDescriptorType(slotTypeToVk(slotDesc.type))
                                 .setBufferInfo(bufferInfos));
        }
    } else if (slotDesc.type == SlotType::SAMPLED_TEXTURE || slotDesc.type == SlotType::STORAGE_TEXTURE) {
        imageInfos.reserve(n);
        vk::ImageLayout layout = (slotDesc.type == SlotType::SAMPLED_TEXTURE) ? vk::ImageLayout::eShaderReadOnlyOptimal : vk::ImageLayout::eGeneral;
        for (size_t i = 0; i < n; ++i) {
            vk::ImageView iv = getImageView(views[i], gi);
            if (!iv) continue;
            imageInfos.push_back(vk::DescriptorImageInfo {}.setImageLayout(layout).setImageView(iv).setSampler(VK_NULL_HANDLE));
        }
        if (!imageInfos.empty()) {
            vk::DescriptorType dtype =
                (slotDesc.type == SlotType::SAMPLED_TEXTURE) ? vk::DescriptorType::eCombinedImageSampler : vk::DescriptorType::eStorageImage;
            writes.push_back(vk::WriteDescriptorSet {}
                                 .setDstSet(mSet)
                                 .setDstBinding(binding)
                                 .setDstArrayElement(arrayElement)
                                 .setDescriptorType(dtype)
                                 .setImageInfo(imageInfos));
        }
    }
    // SAMPLER-only slot: would need SamplerVulkan; skip for now

    if (!writes.empty()) dev.updateDescriptorSets(writes, {});
}

void GpuResourceGroupVulkan::addSlotToReadWriteList(size_t slot, Arguments::ArtifactReadWriteList & list) const {
    if (slot >= mBoundViews.size()) return;
    const auto & slotDesc = mSlots[slot];
    using SlotType        = GpuResourceGroup::SlotDescription::Type;
    bool isRead           = (slotDesc.type == SlotType::UNIFORM_BUFFER || slotDesc.type == SlotType::SAMPLED_TEXTURE || slotDesc.type == SlotType::SAMPLER);
    bool isWrite          = (slotDesc.type == SlotType::STORAGE_BUFFER || slotDesc.type == SlotType::STORAGE_TEXTURE);

    for (const auto & v : mBoundViews[slot]) {
        if (!v.artifact) continue;
        const Artifact * ptr = v.artifact.get();
        if (isRead) list.readList.insert(ptr);
        if (isWrite) list.writeList.insert(ptr);
    }
}

void GpuResourceGroupVulkan::addToReadWriteList(Arguments::ArtifactReadWriteList & list) const {
    for (size_t s = 0; s < mBoundViews.size(); ++s) addSlotToReadWriteList(s, list);
}

// =============================================================================
// createVulkanGpuResourceGroup
// =============================================================================

AutoRef<GpuResourceGroup> createVulkanGpuResourceGroup(const StrA & name, const GpuResourceGroup::CreateParameters & params) {
    if (!params.context) GN_UNLIKELY {
            GN_ERROR(sLogger, "createVulkanGpuResourceGroup: context is null, name='{}'", name);
            return {};
        }
    auto gpu = params.context.staticCastTo<GpuContextVulkan>();
    if (!gpu) GN_UNLIKELY {
            GN_ERROR(sLogger, "createVulkanGpuResourceGroup: context is not Vulkan, name='{}'", name);
            return {};
        }
    return AutoRef<GpuResourceGroup>(new GpuResourceGroupVulkan(name, std::move(gpu), params));
}

} // namespace GN::rdg
#endif
