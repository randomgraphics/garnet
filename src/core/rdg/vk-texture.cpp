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
    StrA name = params.filename;
    auto * p  = new TextureVulkan(db, name);
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

// =============================================================================
// TextureReadbackVulkan
// =============================================================================

TextureReadbackVulkan::TextureReadbackVulkan(ArtifactDatabase & db, const StrA & name): TextureReadback(db, TYPE, name) {}

std::pair<Action::ExecutionResult, Action::ExecutionContext *> TextureReadbackVulkan::prepare(Submission &, Arguments &) {
    // TODO: allocate staging buffer / record copy texture-to-buffer
    return std::make_pair(Action::PASSED, nullptr);
}

Action::ExecutionResult TextureReadbackVulkan::execute(Submission &, Arguments & arguments, ExecutionContext *) {
    auto * a = arguments.castTo<TextureReadback::A>();
    if (!a) {
        GN_ERROR(sLogger)("TextureReadbackVulkan::execute: invalid arguments");
        return FAILED;
    }
    auto * src = a->source.get();
    auto * dst = a->destination.get();
    if (!src || !dst) {
        GN_ERROR(sLogger)("TextureReadbackVulkan::execute: source or destination not set");
        return FAILED;
    }
    // TODO: copy from texture/backbuffer to staging, then to gfx::img::Image
    (void) src;
    (void) dst;
    return Action::PASSED;
}

AutoRef<TextureReadback> createVulkanTextureReadback(ArtifactDatabase & db, const StrA & name, const TextureReadback::CreateParameters &) {
    auto * p = new TextureReadbackVulkan(db, name);
    if (p->sequence == 0) {
        GN_ERROR(sLogger)("createVulkanTextureReadback: duplicate type+name, name='{}'", name);
        delete p;
        return {};
    }
    return AutoRef<TextureReadback>(p);
}

} // namespace GN::rdg
