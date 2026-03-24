#include "pch.h"
#include "vk-texture.h"
#include "vk-gpu-context.h"
#include "vk-format-utils.h"

static GN::Logger * sLogger = GN::getLogger("GN.rdg.vk");

namespace GN::rdg {

// =============================================================================
// Helpers: pixel format mapping, descriptor from image
// =============================================================================

static Texture::Descriptor descriptorFromImageDesc(const gfx::img::ImageDesc & id) {
    gfx::img::PlaneCoord p {};
    Texture::Descriptor  d;
    d.format  = id.format(p);
    d.width   = (uint32_t) id.width(p);
    d.height  = (uint32_t) id.height(p);
    d.depth   = (uint32_t) id.depth(p);
    d.faces   = id.faces;
    d.levels  = id.levels ? (uint32_t) id.levels : (uint32_t) rapid_vulkan::calculateMaxMips(d.width, d.height, d.depth);
    d.samples = 1;
    return d;
}

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

static rapid_vulkan::Ref<rapid_vulkan::Image> createVkImage(const Texture::Descriptor & descriptor, const rapid_vulkan::GlobalInfo & gi) {
    rapid_vulkan::Image::ConstructParameters cp;
    cp.gi               = &gi;
    cp.info.imageType   = (descriptor.depth > 1) ? vk::ImageType::e3D : vk::ImageType::e2D;
    cp.info.format      = pixelFormatToVkFormat(descriptor.format);
    cp.info.extent      = vk::Extent3D(descriptor.width, descriptor.height, descriptor.depth);
    cp.info.mipLevels   = descriptor.levels;
    cp.info.arrayLayers = descriptor.faces;
    cp.info.samples     = (vk::SampleCountFlagBits) descriptor.samples;
    cp.info.tiling      = vk::ImageTiling::eOptimal;
    cp.info.usage       = vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eTransferSrc;

    vk::FormatProperties   props           = gi.physical.getFormatProperties(cp.info.format);
    vk::FormatFeatureFlags optimalFeatures = props.optimalTilingFeatures;
    if (optimalFeatures & vk::FormatFeatureFlagBits::eColorAttachment) cp.info.usage |= vk::ImageUsageFlagBits::eColorAttachment;
    if (optimalFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment) cp.info.usage |= vk::ImageUsageFlagBits::eDepthStencilAttachment;

    if (descriptor.faces == 6 && descriptor.depth == 1)
        cp.setCube(descriptor.width).setLevels(descriptor.levels).setFormat(cp.info.format);
    else
        cp.set2D(descriptor.width, descriptor.height, descriptor.faces).setLevels(descriptor.levels).setFormat(cp.info.format);
    return rapid_vulkan::Ref<rapid_vulkan::Image>(new rapid_vulkan::Image(cp));
}

TextureVulkan::TextureVulkan(const StrA & name): TextureCommon(name) {}

bool TextureVulkan::init(const Texture::CreateParameters & params) {
    if (!params.context) {
        GN_ERROR(sLogger)("TextureVulkan::init: context is null");
        return false;
    }
    mGpuContext.set(params.context.get());
    mDescriptor = validateDesc(params.descriptor);
    if (0 == mDescriptor.width) return false;
    auto * vkCtx = static_cast<GpuContextVulkan *>(params.context.get());
    mImage       = createVkImage(mDescriptor, vkCtx->globalInfo());
    if (!mImage || !mImage->handle()) return false;
    rapid_vulkan::setVkHandleName(vkCtx->globalInfo().device, mImage->handle(), name.c_str());

    mState = TextureState(mDescriptor.levels, mDescriptor.faces);
    return true;
}

bool TextureVulkan::upload1x1Mip(const uint8_t rgba[4]) {
    if (!mImage || !mImage->handle() || mDescriptor.width != 1 || mDescriptor.height != 1) GN_UNLIKELY {
            GN_ERROR(sLogger)("TextureVulkan::upload1x1Mip: invalid state or non-1x1 texture");
            return false;
        }
    auto * vkCtx = static_cast<GpuContextVulkan *>(mGpuContext.get());
    if (!vkCtx) GN_UNLIKELY return false;
    rapid_vulkan::CommandQueue * gq = vkCtx->device().graphics();
    if (!gq) GN_UNLIKELY {
            GN_ERROR(sLogger)("TextureVulkan::upload1x1Mip: no graphics queue");
            return false;
        }
    rapid_vulkan::Image::SetContentParameters sc;
    sc.setQueue(*gq);
    sc.mipLevel = 0;
    sc.area.w   = 1;
    sc.area.h   = 1;
    sc.area.d   = 1;
    sc.pitch    = 4;
    sc.pixels   = rgba;
    for (uint32_t face = 0; face < mDescriptor.faces; ++face) {
        sc.arrayLayer = face;
        mImage->setContent(sc);
    }
    return true;
}

namespace {

gfx::img::Image contentToImage(const rapid_vulkan::Image::Content & content) {
    if (content.subresources.empty() || content.storage.empty()) return gfx::img::Image();
    gfx::img::PixelFormat pf = vkFormatToPixelFormat(content.format);
    if (pf == gfx::img::PixelFormat::UNKNOWN()) return gfx::img::Image();
    const auto & sub = content.subresources[0];
    uint32_t     w   = sub.extent.width;
    uint32_t     h   = sub.extent.height;
    uint32_t     d   = sub.extent.depth;
    if (w == 0 || h == 0) return gfx::img::Image();
    gfx::img::Extent3D extent;
    extent.set(w, h, d);
    gfx::img::PlaneDesc planeDesc = gfx::img::PlaneDesc::make(pf, extent);
    gfx::img::ImageDesc imageDesc = gfx::img::ImageDesc::make(planeDesc, 1, 1, 1);
    return gfx::img::Image(imageDesc, content.storage.data(), content.storage.size());
}

} // namespace

gfx::img::Image TextureVulkan::readback() const {
    if (!mImage || !mImage->handle()) GN_UNLIKELY {
            GN_ERROR(sLogger)("TextureVulkan::readback: no image");
            return gfx::img::Image();
        }
    auto * vkCtx = static_cast<GpuContextVulkan *>(mGpuContext.get());
    if (!vkCtx) GN_UNLIKELY {
            GN_ERROR(sLogger)("TextureVulkan::readback: invalid context");
            return gfx::img::Image();
        }
    rapid_vulkan::CommandQueue * gq = vkCtx->device().graphics();
    if (!gq) GN_UNLIKELY {
            GN_ERROR(sLogger)("TextureVulkan::readback: no graphics queue");
            return gfx::img::Image();
        }
    rapid_vulkan::Image::ReadContentParameters readParams;
    readParams.setQueue(*gq);
    auto content = mImage->readContent(readParams);
    return contentToImage(content);
}

AutoRef<Texture> createVulkanTexture(const StrA & name, const Texture::CreateParameters & params) {
    auto * p = new TextureVulkan(name);
    if (!p->init(params)) {
        delete p;
        return {};
    }
    return AutoRef<Texture>(p);
}

bool TextureVulkan::initFromLoad(const Texture::LoadParameters & params) {
    if (!params.context) {
        GN_ERROR(sLogger)("TextureVulkan::initFromLoad: context is null");
        return false;
    }
    mGpuContext.set(params.context.get());
    StrA path = params.filename;
    if (path.empty()) {
        GN_ERROR(sLogger)("TextureVulkan::initFromLoad: filename is empty");
        return false;
    }
    if (!GN::fs::isAbsPath(path)) path = GN::fs::resolvePath(GN::fs::getCurrentDir(), path);
    auto fp = GN::fs::openFile(path, std::ios::in | std::ios::binary);
    if (!fp) {
        GN_ERROR(sLogger)("TextureVulkan::initFromLoad: cannot open file '{}'", path);
        return false;
    }
    gfx::img::Image image = gfx::img::Image::load(fp->input(), path.c_str());
    if (image.empty()) {
        GN_ERROR(sLogger)("TextureVulkan::initFromLoad: failed to load image '{}'", path);
        return false;
    }
    mDescriptor = validateDesc(descriptorFromImageDesc(image.desc()));
    if (0 == mDescriptor.width) return false;

    auto * vkCtx = static_cast<GpuContextVulkan *>(params.context.get());
    mImage       = createVkImage(mDescriptor, vkCtx->globalInfo());
    if (!mImage || !mImage->handle()) return false;

    mState = TextureState(mDescriptor.levels, mDescriptor.faces);

    rapid_vulkan::CommandQueue * gq = vkCtx->device().graphics();
    if (!gq) {
        GN_ERROR(sLogger)("TextureVulkan::initFromLoad: no graphics queue");
        return false;
    }
    for (uint32_t f = 0; f < mDescriptor.faces; ++f)
        for (uint32_t l = 0; l < mDescriptor.levels; ++l) {
            gfx::img::PlaneCoord pc {};
            pc.face              = (size_t) f;
            pc.level             = (size_t) l;
            const auto &   plane = image.plane(pc);
            const uint32_t w     = (uint32_t) plane.extent.w;
            const uint32_t h     = (uint32_t) plane.extent.h;
            if (w == 0 || h == 0) continue;
            rapid_vulkan::Image::SetContentParameters sc;
            sc.setQueue(*gq).mipLevel = l;
            sc.arrayLayer             = f;
            sc.area.w                 = w;
            sc.area.h                 = h;
            sc.area.d                 = 1;
            sc.pitch                  = (size_t) plane.pitch;
            sc.pixels                 = image.at(pc);
            mImage->setContent(sc);
        }
    return true;
}

AutoRef<Texture> loadVulkanTexture(const Texture::LoadParameters & params) {
    StrA name = params.filename;
    auto p    = AutoRef<TextureVulkan>::make(name);
    if (!p->initFromLoad(params)) {
        p.clear();
        return {};
    }
    return AutoRef<Texture>(p);
}

AutoRef<Texture> createDefault1x1Texture(AutoRef<GpuContext> context, const StrA & name, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    if (!context) GN_UNLIKELY return {};
    Texture::Descriptor desc;
    desc.setDimensions(1, 1).setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM()).setFaces(1).setLevels(1);
    auto tex = Texture::create(name, Texture::CreateParameters {.context = context, .descriptor = desc});
    if (!tex) GN_UNLIKELY return {};
    auto *  tv      = static_cast<TextureVulkan *>(tex.get());
    uint8_t rgba[4] = {r, g, b, a};
    if (!tv->upload1x1Mip(rgba)) { return {}; }
    return tex;
}

AutoRef<Texture> createDefault1x1CubemapTexture(AutoRef<GpuContext> context, const StrA & name, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    if (!context) GN_UNLIKELY return {};
    Texture::Descriptor desc;
    desc.setDimensions(1, 1).setFormat(gfx::img::PixelFormat::RGBA_8_8_8_8_UNORM()).setFaces(6).setLevels(1);
    auto tex = Texture::create(name, Texture::CreateParameters {.context = context, .descriptor = desc});
    if (!tex) GN_UNLIKELY return {};
    auto *  tv      = static_cast<TextureVulkan *>(tex.get());
    uint8_t rgba[4] = {r, g, b, a};
    if (!tv->upload1x1Mip(rgba)) { return {}; }
    return tex;
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

auto TextureState::get(uint32_t mip, uint32_t arrayLayer) const -> const ImageStateTransition * {
    if (mip >= mNumMips || arrayLayer >= mNumArrayLayers) return nullptr;
    size_t index = subResourceIndex(mip, arrayLayer);
    GN_ASSERT(index < mSubresourceStates.size());
    return &mSubresourceStates[index];
}

bool TextureVulkan::trackImageState(uint32_t mip, uint32_t levels, uint32_t arrayLayer, uint32_t layers, const ImageState & newState,
                                    ImageStateTransitionFlags flags) {
    GpuResourceView::SubresourceRange range;
    range.i.mip            = mip;
    range.i.face           = arrayLayer;
    range.e.numMipLevels   = levels;
    range.e.numArrayLayers = layers;
    return mState.set(range, newState, flags);
}

bool TextureState::set(const GpuResourceView::SubresourceRange & range, const ImageState & newState, ImageStateTransitionFlags flags) {
    uint32_t mip        = range.i.mip;
    uint32_t arrayLayer = range.i.face;
    uint32_t levels     = range.e.numMipLevels;
    uint32_t layers     = range.e.numArrayLayers;
    rapid_vulkan::clampRange(mip, levels, mNumMips);
    rapid_vulkan::clampRange(arrayLayer, layers, mNumArrayLayers);
    if (levels == 0 || layers == 0) return false;
    auto mipEnd        = mip + levels;
    auto arrayLayerEnd = arrayLayer + layers;
    bool anyChange     = false;
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

void TextureState::assignFrom(const TextureState & src) {
    const size_t n = std::min(mSubresourceStates.size(), src.mSubresourceStates.size());
    for (size_t i = 0; i < n; ++i) {
        mSubresourceStates[i].prev = src.mSubresourceStates[i].curr;
        mSubresourceStates[i].curr = src.mSubresourceStates[i].curr;
    }
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
