#include "vk-texture.h"
#include "vk-format-utils.h"

#include <garnet/base/filesys.h>

#include <algorithm>
#include <cstring>

static GN::Logger * sLogger = GN::getLogger("GN.gpu2.vk");

namespace GN::gpu2 {

namespace {

Texture::Descriptor descriptorFromImageDesc(const gfx::img::ImageDesc & id) {
    gfx::img::PlaneCoord p {};
    Texture::Descriptor  d;
    d.format  = id.format(p);
    d.width   = (uint32_t) id.width(p);
    d.height  = (uint32_t) id.height(p);
    d.depth   = (uint32_t) id.depth(p);
    d.faces   = id.faces;
    d.levels  = id.levels ? (uint32_t) id.levels : (uint32_t) rv::calculateMaxMips(d.width, d.height, d.depth);
    d.samples = 1;
    return d;
}

Texture::Descriptor validateDesc(const Texture::Descriptor & desc) {
    Texture::Descriptor result = desc;
    if (0 == result.width || 0 == result.height || 0 == result.depth || 0 == result.faces || 0 == result.samples) {
        GN_ERROR(sLogger)("validateDesc: invalid descriptor (zero dimension), width={} height={} depth={} faces={} samples={}", result.width, result.height,
                          result.depth, result.faces, result.samples);
        result.width   = 0;
        result.height  = 0;
        result.depth   = 0;
        result.faces   = 0;
        result.samples = 0;
        return result;
    }
    if (0 == result.levels) result.levels = rv::calculateMaxMips(result.width, result.height, result.depth);
    return result;
}

rv::Ref<rv::Image> createVkImage(const Texture::Descriptor & descriptor, const rv::GlobalInfo & gi) {
    rv::Image::ConstructParameters cp;
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
    return rv::Ref<rv::Image>(new rv::Image(cp));
}

gfx::img::Image contentToImage(const rv::Image::Content & content, size_t subresourceIndex = 0) {
    if (subresourceIndex >= content.subresources.size() || content.storage.empty()) return gfx::img::Image();
    gfx::img::PixelFormat pf = vkFormatToPixelFormat(content.format);
    if (pf == gfx::img::PixelFormat::UNKNOWN()) return gfx::img::Image();
    const auto & sub = content.subresources[subresourceIndex];
    uint32_t     w   = sub.extent.width;
    uint32_t     h   = sub.extent.height;
    uint32_t     d   = sub.extent.depth;
    if (w == 0 || h == 0) return gfx::img::Image();
    gfx::img::Extent3D extent;
    extent.set(w, h, d);
    gfx::img::PlaneDesc planeDesc = gfx::img::PlaneDesc::make(pf, extent);
    gfx::img::ImageDesc imageDesc = gfx::img::ImageDesc::make(planeDesc, 1, 1, 1);
    const uint8_t *     src       = content.storage.data() + sub.offset;
    size_t              sz        = (size_t) sub.pitch * h * d;
    return gfx::img::Image(imageDesc, src, sz);
}

} // namespace

// -----------------------------------------------------------------------------
// TextureVulkanBase
// -----------------------------------------------------------------------------

TextureVulkanBase::TextureVulkanBase(const GN::RuntimeType::TypeInfo & leafType, const StrA & entityName): Texture(leafType, entityName) {}

TextureVulkanBase::~TextureVulkanBase() = default;

gfx::img::Image TextureVulkanBase::readback() const {
    if (!mRvImage || !mImage) {
        GN_ERROR(sLogger)("TextureVulkanBase::readback: no image, name='{}'", name);
        return gfx::img::Image();
    }
    if (!mGpu || !mGpu->ready()) {
        GN_ERROR(sLogger)("TextureVulkanBase::readback: no GpuContext, name='{}'", name);
        return gfx::img::Image();
    }
    const rv::Device * dev = mGpu->vulkanDevice();
    if (!dev) return gfx::img::Image();
    rv::CommandQueue * gq = dev->graphics();
    if (!gq) {
        GN_ERROR(sLogger)("TextureVulkanBase::readback: no graphics queue, name='{}'", name);
        return gfx::img::Image();
    }
    mGpu->waitIdle(); // Ensure all submitted GPU operations are complete before readback.
    rv::Image::ReadContentParameters readParams;
    readParams.setQueue(*gq);
    auto content = mRvImage->readContent(readParams);
    if (!content) return {};
    return contentToImage(content);
}

bool TextureVulkanBase::setContent(const gfx::img::Image & image) {
    if (!mRvImage || !mImage) {
        GN_ERROR(sLogger)("TextureVulkanBase::setContent: texture not writable or not initialized, name='{}'", name);
        return false;
    }
    if (image.empty()) {
        GN_ERROR(sLogger)("TextureVulkanBase::setContent: input image is empty, name='{}'", name);
        return false;
    }
    if (!mGpu || !mGpu->ready()) return false;
    const rv::Device * dev = mGpu->vulkanDevice();
    if (!dev || !dev->gi()) return false;
    rv::CommandQueue * gq = dev->graphics();
    if (!gq) return false;
    mGpu->waitIdle(); // Ensure all submitted GPU operations are complete before setting new content.
    for (uint32_t f = 0; f < mDescriptor.faces; ++f) {
        for (uint32_t l = 0; l < mDescriptor.levels; ++l) {
            gfx::img::PlaneCoord pc {};
            pc.face              = (size_t) f;
            pc.level             = (size_t) l;
            const auto &   plane = image.plane(pc);
            const uint32_t w     = (uint32_t) plane.extent.w;
            const uint32_t h     = (uint32_t) plane.extent.h;
            if (w == 0 || h == 0) continue;
            rv::Image::SetContentParameters sc;
            sc.setQueue(*gq);
            sc.mipLevel   = l;
            sc.arrayLayer = f;
            sc.area.w     = w;
            sc.area.h     = h;
            sc.area.d     = 1;
            sc.pitch      = (size_t) plane.pitch;
            sc.pixels     = image.at(pc);
            mRvImage->setContent(sc); // auto-updates state to TRANSFER_DST on success
        }
    }
    return true;
}

// -----------------------------------------------------------------------------
// OwnedTextureVulkan — regular device-local texture (create / load)
// -----------------------------------------------------------------------------

class OwnedTextureVulkan final : public TextureVulkanBase {
public:
    GN_REGISTER_RUNTIME_TYPE(TextureVulkanBase);

    explicit OwnedTextureVulkan(const StrA & entityName): TextureVulkanBase(OwnedTextureVulkan::TYPE_INFO(), entityName) {}

    bool initOwned(const CreateParameters & params) {
        reset();
        if (!params.context) {
            GN_ERROR(sLogger)("OwnedTextureVulkan::initOwned: context is null, name='{}'", name);
            return false;
        }
        mGpu        = params.context.staticCastTo<GpuContextVulkan2>();
        mDescriptor = validateDesc(params.descriptor);
        if (0 == mDescriptor.width) return false;
        const rv::Device * dev = mGpu->vulkanDevice();
        if (!dev || !dev->gi()) {
            GN_ERROR(sLogger)("OwnedTextureVulkan::initOwned: invalid Vulkan device, name='{}'", name);
            return false;
        }
        mOwnedImage = createVkImage(mDescriptor, *dev->gi());
        if (!mOwnedImage || !mOwnedImage->handle()) return false;
        rv::setVkHandleName(dev->gi()->device, mOwnedImage->handle(), name.c_str());

        setVulkanHandles(mOwnedImage.get());
        return true;
    }

    bool initFromLoad(const LoadParameters & params) {
        reset();
        if (!params.context) {
            GN_ERROR(sLogger)("OwnedTextureVulkan::initFromLoad: context is null");
            return false;
        }
        mGpu = params.context.staticCastTo<GpuContextVulkan2>();
        if (!mGpu || !mGpu->ready()) return false;

        StrA path = params.filename;
        if (path.empty()) {
            GN_ERROR(sLogger)("OwnedTextureVulkan::initFromLoad: filename is empty");
            return false;
        }
        if (!GN::fs::isAbsPath(path)) path = GN::fs::resolvePath(GN::fs::getCurrentDir(), path);
        auto fp = GN::fs::openFile(path, std::ios::in | std::ios::binary);
        if (!fp) {
            GN_ERROR(sLogger)("OwnedTextureVulkan::initFromLoad: cannot open '{}'", path);
            return false;
        }
        gfx::img::Image image = gfx::img::Image::load(fp->input(), path.c_str());
        if (image.empty()) {
            GN_ERROR(sLogger)("OwnedTextureVulkan::initFromLoad: failed to load '{}'", path);
            return false;
        }
        mDescriptor = validateDesc(descriptorFromImageDesc(image.desc()));
        if (0 == mDescriptor.width) return false;

        const rv::Device * dev = mGpu->vulkanDevice();
        if (!dev || !dev->gi()) return false;
        mOwnedImage = createVkImage(mDescriptor, *dev->gi());
        if (!mOwnedImage || !mOwnedImage->handle()) return false;
        mOwnedImage->setName(path.c_str());

        setVulkanHandles(mOwnedImage.get());
        rv::CommandQueue * gq = dev->graphics();
        if (!gq) {
            GN_ERROR(sLogger)("OwnedTextureVulkan::initFromLoad: no graphics queue");
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
                rv::Image::SetContentParameters sc;
                sc.setQueue(*gq).mipLevel = l;
                sc.arrayLayer             = f;
                sc.area.w                 = w;
                sc.area.h                 = h;
                sc.area.d                 = 1;
                sc.pitch                  = (size_t) plane.pitch;
                sc.pixels                 = image.at(pc);
                mOwnedImage->setContent(sc); // auto-updates state to TRANSFER_DST on success
            }
        return true;
    }

private:
    rv::Ref<rv::Image> mOwnedImage {};

    void reset() {
        mOwnedImage.clear();
        setVulkanHandles(nullptr);
        mDescriptor = {};
        mGpu.clear();
    }
};

AutoRef<Texture> createTextureVulkan2(const StrA & entityName, const Texture::CreateParameters & params) {
    auto p = new OwnedTextureVulkan(entityName);
    if (!p->initOwned(params)) {
        delete p;
        return {};
    }
    return AutoRef<Texture>(p);
}

AutoRef<Texture> loadTextureVulkan2(const Texture::LoadParameters & params) {
    StrA entityName = params.filename.empty() ? StrA("texture") : params.filename;
    auto p          = new OwnedTextureVulkan(entityName);
    if (!p->initFromLoad(params)) {
        delete p;
        return {};
    }
    return AutoRef<Texture>(p);
}

} // namespace GN::gpu2
