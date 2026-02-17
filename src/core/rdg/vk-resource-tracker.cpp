#include "vk-resource-tracker.h"
#include "vk-texture.h"

namespace GN::rdg {

static GN::Logger * sLogger = GN::getLogger("GN.rdg");

/// Helper class to iterate over subresources of a texture, initialized from a Texture::Descriptor,
/// with an optional subresource index and range.
///
/// The class will first validate the subresource index and range, issue warning if the index and range is invalid.
/// then it'll clamp the index and range into valid values, then remember it.
///
/// The class provides a forEach function that accepts a functor and will run the functor on each and every of
/// the subsource in the valid range. If the range is empty, the functor will not be called.
/// It can iterate through the subresources in two different order: mip major and layer major.
class SubresourceIterator {
public:
    enum class Order { MipMajor, LayerMajor };

    SubresourceIterator(const Texture::Descriptor & desc, const Texture::SubresourceIndex & index = {}, const Texture::SubresourceRange & range = {})
        : mDesc(desc) {
        const uint32_t maxMips   = desc.levels > 0 ? desc.levels : 1u;
        const uint32_t maxLayers = desc.faces > 0 ? desc.faces : 1u;

        if (index.mip >= maxMips || index.face >= maxLayers) {
            GN_VERBOSE(sLogger)("SubresourceIterator: invalid subresource index (mip={}, face={}). There's nothing to iterate over.", index.mip, index.face);
            mBaseMip   = 0;
            mBaseFace  = 0;
            mNumMips   = 0;
            mNumLayers = 0;
            return;
        }

        mBaseMip  = index.mip >= maxMips ? maxMips - 1u : index.mip;
        mBaseFace = index.face >= maxLayers ? maxLayers - 1u : index.face;

        uint32_t numMips   = (range.numMipLevels == (uint32_t) -1) ? (maxMips - mBaseMip) : range.numMipLevels;
        uint32_t numLayers = (range.numArrayLayers == (uint32_t) -1) ? (maxLayers - mBaseFace) : range.numArrayLayers;

        if (mBaseMip + numMips > maxMips || mBaseFace + numLayers > maxLayers) {
            GN_VERBOSE(sLogger)("SubresourceIterator: invalid subresource range, clamping");
        }
        mNumMips   = (mBaseMip + numMips > maxMips) ? (maxMips - mBaseMip) : numMips;
        mNumLayers = (mBaseFace + numLayers > maxLayers) ? (maxLayers - mBaseFace) : numLayers;
    }

    template<typename F>
    void forEach(F && fn, Order order = Order::MipMajor) const {
        if (mNumMips == 0 || mNumLayers == 0) return;
        if (order == Order::MipMajor) {
            for (uint32_t mip = mBaseMip; mip < mBaseMip + mNumMips; ++mip)
                for (uint32_t face = mBaseFace; face < mBaseFace + mNumLayers; ++face)
                    if (!fn(mip, face)) break;
        } else {
            for (uint32_t face = mBaseFace; face < mBaseFace + mNumLayers; ++face)
                for (uint32_t mip = mBaseMip; mip < mBaseMip + mNumMips; ++mip)
                    if (!fn(mip, face)) break;
        }
    }

private:
    Texture::Descriptor mDesc;
    uint32_t            mBaseMip   = 0;
    uint32_t            mBaseFace  = 0;
    uint32_t            mNumMips   = 0;
    uint32_t            mNumLayers = 0;
};

// ---------------------------------------------------------------------------------------------------------------------
// Layout/access/stage mapping from ResourceUsage (implementation detail)
// ---------------------------------------------------------------------------------------------------------------------

static void getImageLayoutAccessStage(ResourceTrackerVulkan::ResourceUsage usage, vk::ImageLayout & layout, vk::AccessFlags & access,
                                      vk::PipelineStageFlags & stage) {
    using R = ResourceTrackerVulkan::ResourceUsage;
    switch (usage) {
    case R::Loading:
    case R::TransferDst:
        layout = vk::ImageLayout::eTransferDstOptimal;
        access = vk::AccessFlagBits::eTransferWrite;
        stage  = vk::PipelineStageFlagBits::eTransfer;
        break;
    case R::Readback:
    case R::TransferSrc:
        layout = vk::ImageLayout::eTransferSrcOptimal;
        access = vk::AccessFlagBits::eTransferRead;
        stage  = vk::PipelineStageFlagBits::eTransfer;
        break;
    case R::ColorAttachment:
        layout = vk::ImageLayout::eColorAttachmentOptimal;
        access = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;
        stage  = vk::PipelineStageFlagBits::eColorAttachmentOutput;
        break;
    case R::DepthStencilAttachment:
        layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
        access = vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite;
        stage  = vk::PipelineStageFlagBits::eEarlyFragmentTests | vk::PipelineStageFlagBits::eLateFragmentTests;
        break;
    case R::ShaderRead:
        layout = vk::ImageLayout::eShaderReadOnlyOptimal;
        access = vk::AccessFlagBits::eShaderRead;
        stage  = vk::PipelineStageFlagBits::eFragmentShader | vk::PipelineStageFlagBits::eComputeShader;
        break;
    case R::ComputeStorage:
        layout = vk::ImageLayout::eGeneral;
        access = vk::AccessFlagBits::eShaderRead | vk::AccessFlagBits::eShaderWrite;
        stage  = vk::PipelineStageFlagBits::eComputeShader;
        break;
    case R::Present:
        layout = vk::ImageLayout::ePresentSrcKHR;
        access = vk::AccessFlagBits::eMemoryRead;
        stage  = vk::PipelineStageFlagBits::eBottomOfPipe;
        break;
    default:
        layout = vk::ImageLayout::eShaderReadOnlyOptimal;
        access = vk::AccessFlagBits::eShaderRead;
        stage  = vk::PipelineStageFlagBits::eFragmentShader;
        break;
    }
}

static vk::ImageAspectFlags imageAspectFromUsage(ResourceTrackerVulkan::ResourceUsage usage) {
    using R = ResourceTrackerVulkan::ResourceUsage;
    if (usage == R::DepthStencilAttachment) { return vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil; }
    return vk::ImageAspectFlagBits::eColor;
}

// ---------------------------------------------------------------------------------------------------------------------
// ResourceTrackerVulkan
// ---------------------------------------------------------------------------------------------------------------------

ResourceTrackerVulkan::ResourceTrackerVulkan(const ConstructParameters & params): SubmissionImpl::Context(TYPE), mGpu(params.gpu) {}

ResourceTrackerVulkan::~ResourceTrackerVulkan() = default;

bool ResourceTrackerVulkan::execute(const ActionParameters & action, vk::CommandBuffer commandBuffer) {
    if (!commandBuffer) GN_UNLIKELY {
            GN_ERROR(sLogger)("ResourceTrackerVulkan::execute: null command buffer");
            return false;
        }

    rapid_vulkan::Barrier  barrier;
    vk::PipelineStageFlags combinedSrcStage {};
    vk::PipelineStageFlags combinedDstStage {};

    for (const TextureResourceUse & use : action.textures) {
        if (!use.texture) GN_UNLIKELY continue;

        auto * texVk = dynamic_cast<TextureVulkan *>(use.texture.get());
        if (!texVk) GN_UNLIKELY continue;

        vk::Image image = texVk->handle();
        if (!image) GN_UNLIKELY continue;

        vk::ImageLayout        newLayout;
        vk::AccessFlags        newAccess;
        vk::PipelineStageFlags newStage;
        getImageLayoutAccessStage(use.usage, newLayout, newAccess, newStage);

        const vk::ImageAspectFlags aspect = imageAspectFromUsage(use.usage);

        SubresourceIterator subIt(use.texture->descriptor(), use.subresourceIndex, use.subresourceRange);
        subIt.forEach(
            [&](uint32_t mip, uint32_t face) {
                const ResourceTrackerVulkan::ImageKey key = {image, mip, face};
                auto                                  it  = mImageState.find(key);
                ImageState                            cur;
                if (it != mImageState.end()) { cur = it->second; }

                if (cur.layout != newLayout || (cur.layout == vk::ImageLayout::eUndefined)) {
                    vk::AccessFlags srcAccess = cur.access;
                    if (cur.layout == vk::ImageLayout::eUndefined) { srcAccess = vk::AccessFlagBits::eNone; }
                    combinedSrcStage |= cur.stage;
                    combinedDstStage |= newStage;
                    const vk::ImageSubresourceRange range(aspect, mip, 1, face, 1);
                    barrier.i(image, srcAccess, newAccess, cur.layout, newLayout, range);
                    cur.layout       = newLayout;
                    cur.access       = newAccess;
                    cur.stage        = newStage;
                    mImageState[key] = cur;
                }
                return true;
            },
            SubresourceIterator::Order::MipMajor);
    }

    for (const BufferResourceUse & use : action.buffers) {
        if (!use.buffer) GN_UNLIKELY continue;
        // Buffer barriers require vk::Buffer from a Vulkan buffer implementation (e.g. BufferVulkan::handle()).
        // Skip until that exists; no other files changed.
        (void) use;
    }

    if (!barrier.memories.empty() || !barrier.buffers.empty() || !barrier.images.empty()) {
        barrier.s(combinedSrcStage, combinedDstStage);
        barrier.cmdWrite(commandBuffer);
    }
    return true;
}

} // namespace GN::rdg
