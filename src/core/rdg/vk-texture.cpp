#include "vk-texture.h"

static GN::Logger * sLogger = GN::getLogger("GN.rdg.vk");

namespace GN::rdg {

// =============================================================================
// TextureVulkan
// =============================================================================

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
    mDescriptor = params.descriptor;
    // TODO: create VkImage and allocate memory for the descriptor
    return true;
}

gfx::img::Image TextureVulkan::readback() const {
    // TODO: read back the texture content into an image
    GN_ERROR(sLogger)("TextureVulkan::readback: not implemented");
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
    if (sequence == 0) return false;
    if (!params.context) {
        GN_ERROR(sLogger)("TextureVulkan::initFromLoad: context is null");
        return false;
    }
    mGpuContext.set(params.context.get());
    mDescriptor = Texture::Descriptor {}; // placeholder until load fills it
    // TODO: load image from file, create VkImage, upload pixels
    (void) params;
    return true;
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

} // namespace GN::rdg
