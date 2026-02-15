#include "vk-texture.h"
#include "gpu-context.h"
#include <garnet/GNrdg.h>

static GN::Logger * sLogger = GN::getLogger("GN.rdg");

namespace GN::rdg {

// =============================================================================
// TextureReadback::create() - API-neutral dispatch by context's backend
// =============================================================================

GN_API AutoRef<TextureReadback> TextureReadback::create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params) {
    if (!params.gpu) {
        GN_ERROR(sLogger)("TextureReadback::create: gpu is null, name='{}'", name);
        return {};
    }
    auto * common = static_cast<GpuContextCommon *>(params.gpu.get());
    switch (common->api()) {
    case GpuContextCommon::Api::Vulkan:
        return createVulkanTextureReadback(db, name, params);
    case GpuContextCommon::Api::D3D12:
        GN_ERROR(sLogger)("TextureReadback::create: D3D12 backend not implemented yet");
        return {};
    case GpuContextCommon::Api::Metal:
        GN_ERROR(sLogger)("TextureReadback::create: Metal backend not implemented yet");
        return {};
    default:
        GN_ERROR(sLogger)("TextureReadback::create: unknown API, name='{}'", name);
        return {};
    }
}

} // namespace GN::rdg
