#include "pch.h"
#include "vk-texture.h"
#include "vk-gpu-cnc.h"
#include "vk-cnc-common.h"
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
        GN_ERROR(sLogger, "validateDesc: invalid descriptor (zero dimension), width={} height={} depth={} faces={} samples={}", result.width, result.height,
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
    // Storage usage is needed for compute dispatch reads/writes; add it whenever the format supports it.
    if (optimalFeatures & vk::FormatFeatureFlagBits::eStorageImage) cp.info.usage |= vk::ImageUsageFlagBits::eStorage;

    // The 2D/cube helpers overwrite imageType and depth; preserve the explicit 3D description above for volumes.
    if (descriptor.depth == 1) {
        if (descriptor.faces == 6)
            cp.setCube(descriptor.width).setLevels(descriptor.levels).setFormat(cp.info.format);
        else
            cp.set2D(descriptor.width, descriptor.height, descriptor.faces).setLevels(descriptor.levels).setFormat(cp.info.format);
    }
    return rv::Ref<rv::Image>(new rv::Image(cp));
}

} // namespace

// -----------------------------------------------------------------------------
// TextureVulkanBase
// -----------------------------------------------------------------------------

TextureVulkanBase::TextureVulkanBase(const GN::RuntimeType::TypeInfo & leafType, const StrA & entityName): Texture(leafType, entityName) {}

TextureVulkanBase::~TextureVulkanBase() = default;

gfx::img::Image TextureVulkanBase::readback() const {
    if (!mGpu || !mGpu->ready() || !mImage || mDescriptor.samples != 1 || !singleCopyAspect(mDescriptor)) return {};
    const auto &       d = mDescriptor;
    gfx::img::Extent3D extent;
    extent.set(d.width, d.height, d.depth);
    const auto                planeDesc = gfx::img::PlaneDesc::make(d.format, extent);
    gfx::img::Image           image(gfx::img::ImageDesc::make(planeDesc, 1, d.faces, d.levels));
    DynaArray<GpuCnC::Region> regions;
    for (uint32_t face = 0; face < d.faces; ++face) {
        for (uint32_t level = 0; level < d.levels; ++level) {
            GpuCnC::Region region;
            region.face        = face;
            region.mip         = level;
            region.imageExtent = {std::max(1u, d.width >> level), std::max(1u, d.height >> level), std::max(1u, d.depth >> level)};
            regions.append(region);
        }
    }
    auto cnc = GpuCnC::create({.gpu = mGpu});
    if (!cnc) return {};
    auto future  = cnc->recordDownloadImage(AutoRef<Texture>(const_cast<TextureVulkanBase *>(this)), regions);
    auto payload = cnc->seal();
    if (!payload) return {};
    // Use the tracked transfer path so all downloaded subresources return to shader-readable state.
    mGpu->submit(GpuContext::SubmitParameters(name + "/readback").appendWork(payload));
    mGpu->waitForIdle();
    auto content = future.get();
    if (!content.blob || content.regions.size() != regions.size()) return {};
    const auto     layout = d.format.layoutDesc();
    const uint64_t bb = d.format.bytesPerBlock(), bw = std::max(1u, (uint32_t) layout.blockWidth), bh = std::max(1u, (uint32_t) layout.blockHeight);
    for (const auto & region : content.regions) {
        const gfx::img::PlaneCoord coord {0, region.face, region.mip};
        const auto &               plane    = image.plane(coord);
        const uint64_t             rows     = (region.imageExtent.y + bh - 1) / bh;
        const uint64_t             rowBytes = ((region.imageExtent.x + bw - 1) / bw) * bb;
        const auto *               src      = (const uint8_t *) content.blob->data() + region.dataOffset;
        auto *                     dst      = (uint8_t *) image.at(coord);
        for (uint32_t z = 0; z < region.imageExtent.z; ++z)
            for (uint64_t y = 0; y < rows; ++y) memcpy(dst + z * plane.slice + y * plane.pitch, src + (z * rows + y) * rowBytes, (size_t) rowBytes);
    }
    return image;
}

bool TextureVulkanBase::setContent(const gfx::img::Image & image) {
    if (!mGpu || !mGpu->ready() || !mImage || image.empty() || mDescriptor.samples != 1 || !singleCopyAspect(mDescriptor)) return false;
    if (image.format() != mDescriptor.format || image.width() != mDescriptor.width || image.height() != mDescriptor.height ||
        image.depth() != mDescriptor.depth || image.desc().faces != mDescriptor.faces || image.desc().levels != mDescriptor.levels) {
        GN_ERROR(sLogger, "TextureVulkanBase::setContent: incompatible image, name='{}'", name);
        return false;
    }
    auto payload = createCncImageUploadPayload(mGpu, AutoRef<Texture>(this), image);
    if (!payload) return false;
    // The CNC payload restores the tracked shader-readable invariant before signaling completion.
    mGpu->submit(GpuContext::SubmitParameters(name + "/set-content").appendWork(payload));
    mGpu->waitForIdle();
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
            GN_ERROR(sLogger, "OwnedTextureVulkan::initOwned: context is null, name='{}'", name);
            return false;
        }
        mGpu        = params.context.staticCastTo<GpuContextVulkan2>();
        mDescriptor = validateDesc(params.descriptor);
        if (0 == mDescriptor.width) return false;
        const rv::Device & dev = mGpu->vulkanDevice();
        if (!dev.gi()) {
            GN_ERROR(sLogger, "OwnedTextureVulkan::initOwned: invalid Vulkan device, name='{}'", name);
            return false;
        }
        mOwnedImage = createVkImage(mDescriptor, *dev.gi());
        if (!mOwnedImage || !mOwnedImage->handle()) return false;
        rv::setVkHandleName(dev.gi()->device, mOwnedImage->handle(), name.c_str());

        setVulkanHandles(mOwnedImage.get());
        return true;
    }

    bool initFromLoad(const LoadParameters & params) {
        reset();
        if (!params.context) {
            GN_ERROR(sLogger, "OwnedTextureVulkan::initFromLoad: context is null");
            return false;
        }
        mGpu = params.context.staticCastTo<GpuContextVulkan2>();
        if (!mGpu || !mGpu->ready()) return false;

        StrA path = params.filename;
        if (path.empty()) {
            GN_ERROR(sLogger, "OwnedTextureVulkan::initFromLoad: filename is empty");
            return false;
        }
        if (!GN::fs::isAbsPath(path)) path = GN::fs::resolvePath(GN::fs::getCurrentDir(), path);
        auto fp = GN::fs::openFile(path, std::ios::in | std::ios::binary);
        if (!fp) {
            GN_ERROR(sLogger, "OwnedTextureVulkan::initFromLoad: cannot open '{}'", path);
            return false;
        }
        gfx::img::Image image = gfx::img::Image::load(fp->input(), path.c_str());
        if (image.empty()) {
            GN_ERROR(sLogger, "OwnedTextureVulkan::initFromLoad: failed to load '{}'", path);
            return false;
        }
        mDescriptor = validateDesc(descriptorFromImageDesc(image.desc()));
        if (0 == mDescriptor.width) return false;

        const rv::Device & dev = mGpu->vulkanDevice();
        if (!dev.gi()) return false;
        mOwnedImage = createVkImage(mDescriptor, *dev.gi());
        if (!mOwnedImage || !mOwnedImage->handle()) return false;
        mOwnedImage->setName(path.c_str());

        setVulkanHandles(mOwnedImage.get());
        rv::CommandQueue * gq = dev.graphics();
        if (!gq) {
            GN_ERROR(sLogger, "OwnedTextureVulkan::initFromLoad: no graphics queue");
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
                const size_t                    planeSize = (size_t) plane.slice * (plane.extent.d ? (size_t) plane.extent.d : 1u);
                rv::Image::SetContentParameters sc;
                sc.setQueue(*gq);
                sc.mipLevel   = l;
                sc.arrayLayer = f;
                sc.pitch      = (size_t) plane.pitch;
                sc.setPixels(planeSize, image.at(pc));
                mOwnedImage->setContent(sc);
            }

        GN_INFO(sLogger, "Loaded texture '{}'", path);
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
