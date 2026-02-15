#include "vk-texture.h"
#include "gpu-context.h"

static GN::Logger * sLogger = GN::getLogger("GN.rdg");

namespace GN::rdg {

// =============================================================================
// Texture::create() - API-neutral dispatch by context's backend
// =============================================================================

GN_API AutoRef<Texture> Texture::create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params) {
    if (!params.context) {
        GN_ERROR(sLogger)("Texture::create: context is null, name='{}'", name);
        return {};
    }

    auto * common = static_cast<GpuContextCommon *>(params.context.get());
    switch (common->api()) {
    case GpuContextCommon::Api::Vulkan:
        return createVulkanTexture(db, name, params);
    case GpuContextCommon::Api::D3D12:
        GN_ERROR(sLogger)("Texture::create: D3D12 backend not implemented yet");
        return {};
    case GpuContextCommon::Api::Metal:
        GN_ERROR(sLogger)("Texture::create: Metal backend not implemented yet");
        return {};
    default:
        GN_ERROR(sLogger)("Texture::create: unknown API, name='{}'", name);
        return {};
    }
}

// =============================================================================
// Texture::load() - API-neutral dispatch by context's backend
// =============================================================================

GN_API AutoRef<Texture> Texture::load(ArtifactDatabase & db, const LoadParameters & params) {
    if (!params.context) {
        GN_ERROR(sLogger)("Texture::load: context is null, filename='{}'", params.filename);
        return {};
    }

    auto * common = static_cast<GpuContextCommon *>(params.context.get());
    switch (common->api()) {
    case GpuContextCommon::Api::Vulkan:
        return loadVulkanTexture(db, params);
    case GpuContextCommon::Api::D3D12:
        GN_ERROR(sLogger)("Texture::load: D3D12 backend not implemented yet");
        return {};
    case GpuContextCommon::Api::Metal:
        GN_ERROR(sLogger)("Texture::load: Metal backend not implemented yet");
        return {};
    default:
        GN_ERROR(sLogger)("Texture::load: unknown API, filename='{}'", params.filename);
        return {};
    }
}

} // namespace GN::rdg
