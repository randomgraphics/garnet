#include "pch.h"
#include "vk-texture.h"

static GN::Logger * sLogger = GN::getLogger("GN.rdg.vk");

namespace GN::rdg {

// =============================================================================
// TextureVulkan
// =============================================================================

/// Validate the texture descriptor. If failed, returns an descriptor with width == 0.
static Texture::Descriptor validateDesc(const Texture::Descriptor & desc) {
    Texture::Descriptor result = desc;
    if (0 == result.width || 0 == result.height || 0 == result.depth || 0 == result.faces || 0 == result.samples) {
        GN_ERROR(sLogger)
        ("validateDesc: invalid descriptor: zero dimension: (width={}, height={}, depth={}, faces={}, samples={}).", result.width, result.height, result.depth,
         result.faces, result.samples);
        result.width   = 0;
        result.height  = 0;
        result.depth   = 0;
        result.faces   = 0;
        result.samples = 0;
        return result;
    }

    // calculate the number of mip levels based on the width, height and depth.
    if (0 == result.levels) result.levels = rapid_vulkan::calculateMaxMips(result.width, result.height, result.depth);

    // done
    return result;
}

static rapid_vulkan::Ref<rapid_vulkan::Image> createVkImage(const Texture::Descriptor & descriptor) {
    GN_UNIMPL();
    (void) descriptor;
    return {};
}

TextureVulkan::TextureVulkan(ArtifactDatabase & db, const StrA & name): TextureCommon(db, name) {
    if (sequence == 0) { GN_ERROR(sLogger)("TextureVulkan::TextureVulkan: duplicate type+name, name='{}'", name); }
}

bool TextureVulkan::init(const Texture::CreateParameters & params) {
    if (sequence == 0) return false;
    if (!params.context) {
        GN_ERROR(sLogger)("TextureVulkan::init: context is null");
        return false;
    }
    mGpuContext.set(params.context.get());
    mDescriptor = validateDesc(params.descriptor);
    if (0 == mDescriptor.width) return false;
    mImage = createVkImage(mDescriptor);
    if (!mImage || !mImage->handle()) return false;

    // initialize the subresource image state array.
    mSubresourceStates.resize(mDescriptor.levels * mDescriptor.faces);
    return true;
}

gfx::img::Image TextureVulkan::readback() const {
    // TODO: read back the texture content into an image
    GN_UNIMPL();
    return gfx::img::Image();
}

AutoRef<Texture> createVulkanTexture(ArtifactDatabase & db, const StrA & name, const Texture::CreateParameters & params) {
    auto * p = new TextureVulkan(db, name);
    if (p->sequence == 0) {
        GN_ERROR(sLogger)("createVulkanTexture: duplicate type+name, name='{}'", name);
        delete p;
        return {};
    }
    if (!p->init(params)) {
        delete p;
        return {};
    }
    return AutoRef<Texture>(p);
}

bool TextureVulkan::initFromLoad(const Texture::LoadParameters & params) {
    GN_UNIMPL();
    if (sequence == 0) return false;
    if (!params.context) {
        GN_ERROR(sLogger)("TextureVulkan::initFromLoad: context is null");
        return false;
    }
    mGpuContext.set(params.context.get());
    mDescriptor = Texture::Descriptor {}; // placeholder until load fills it
    // TODO: load image from file, create VkImage, upload pixels
    (void) params;
    return false;
}

AutoRef<Texture> loadVulkanTexture(ArtifactDatabase & db, const Texture::LoadParameters & params) {
    StrA   name = params.filename;
    auto * p    = new TextureVulkan(db, name);
    if (p->sequence == 0) {
        GN_ERROR(sLogger)("loadVulkanTexture: duplicate type+name, name='{}'", name);
        delete p;
        return {};
    }
    if (!p->initFromLoad(params)) {
        delete p;
        return {};
    }
    return AutoRef<Texture>(p);
}

vk::Extent3D TextureVulkan::dimensions(uint32_t mip) const {
    if (mip >= mDescriptor.levels) {
        GN_ERROR(sLogger)
        ("TextureVulkan::dimension(): invalid subresource index: (mip={}).", mip);
        return {0, 0, 0};
    }
    vk::Extent3D dim(mDescriptor.width, mDescriptor.height, mDescriptor.depth);
    dim.width  = std::max(1u, dim.width >> mip);
    dim.height = std::max(1u, dim.height >> mip);
    dim.depth  = std::max(1u, dim.depth >> mip);
    return dim;
}

const TextureVulkan::ImageStateTransition * TextureVulkan::getImageState(uint32_t mip, uint32_t arrayLayer) const {
    if (mip >= mDescriptor.levels || arrayLayer >= mDescriptor.faces) return nullptr;
    size_t index = subResourceIndex(mip, arrayLayer);
    GN_ASSERT(index < mSubresourceStates.size());
    return &mSubresourceStates[index];
}

bool TextureVulkan::trackImageState(uint32_t mip, uint32_t levels, uint32_t arrayLayer, uint32_t layers, const ImageState & newState,
                                    ImageStateTransitionFlags flags) {
    if (mip >= mDescriptor.levels || arrayLayer >= mDescriptor.faces) {
        GN_ERROR(sLogger)
        ("TextureVulkan::trackImageState: invalid subresource index and/or range: (mip={}, levels={}, face={}, layers={}).", mip, levels, arrayLayer, layers);
        return false;
    }
    auto mipEnd        = mip + levels;
    auto arrayLayerEnd = arrayLayer + layers;
    if (mipEnd > mDescriptor.levels || arrayLayerEnd > mDescriptor.faces) {
        GN_ERROR(sLogger)
        ("TextureVulkan::trackImageState: invalid subresource index and/or range: (mip={}, levels={}, face={}, layers={}).", mip, levels, arrayLayer, layers);
        return false;
    }
    bool anyChange = false;
    for (uint32_t i = mip; i < mipEnd; i++) {
        for (uint32_t j = arrayLayer; j < arrayLayerEnd; j++) {
            size_t index = subResourceIndex(i, j);
            GN_ASSERT(index < mSubresourceStates.size());
            if (mSubresourceStates[index].curr != newState) {
                anyChange = true;
                mSubresourceStates[index].transitTo(newState, flags);
            }
        }
    }
    return anyChange;
}

// SubresourceIterator(const Texture::Descriptor & desc, const Texture::SubresourceIndex & index, const Texture::SubresourceRange & range)
// : mDesc(desc) {
//     if (0 == desc.levels) mDesc.levels = calculateMaxMips(desc.width, desc.height, desc.depth);

//     const uint32_t maxMips   = mDesc.levels;
//     const uint32_t maxLayers = mDesc.faces;

//     if (index.mip >= maxMips || index.face >= maxLayers) {
//         GN_VERBOSE(sLogger)("SubresourceIterator: invalid subresource index (mip={}, face={}). There's nothing to iterate over.", index.mip, index.face);
//         mBaseMip   = 0;
//         mBaseFace  = 0;
//         mNumMips   = 0;
//         mNumLayers = 0;
//         return;
//     }

//     mBaseMip  = index.mip >= maxMips ? maxMips - 1u : index.mip;
//     mBaseFace = index.face >= maxLayers ? maxLayers - 1u : index.face;

//     uint32_t numMips   = (range.numMipLevels == (uint32_t) -1) ? (maxMips - mBaseMip) : range.numMipLevels;
//     uint32_t numLayers = (range.numArrayLayers == (uint32_t) -1) ? (maxLayers - mBaseFace) : range.numArrayLayers;

//     if (mBaseMip + numMips > maxMips || mBaseFace + numLayers > maxLayers) {
//         GN_VERBOSE(sLogger)("SubresourceIterator: invalid subresource range, clamping");
//     }
//     mNumMips   = (mBaseMip + numMips > maxMips) ? (maxMips - mBaseMip) : numMips;
//     mNumLayers = (mBaseFace + numLayers > maxLayers) ? (maxLayers - mBaseFace) : numLayers;
// }

} // namespace GN::rdg
