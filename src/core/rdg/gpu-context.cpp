#include "vk-gpu-context.h"

static GN::Logger * sLogger = GN::getLogger("GN.rdg");

namespace GN::rdg {

// =============================================================================
// API resolution: "auto" -> platform default (Windows: d3d12, Mac: metal, else: vulkan)
// =============================================================================

static StrA resolveApi(const StrA & api) {
    if (api.empty() || api == "auto") {
        // #if GN_WINPC
        //         return "d3d12";
        // #elif GN_DARWIN
        //         return "metal";
        // #else
        return "vulkan";
        // #endif
    }
    StrA out = api;
    out.toLower();
    return out;
}

// =============================================================================
// GpuContext::create() - API-neutral dispatch to backend-specific factories
// =============================================================================

GN_API AutoRef<GpuContext> GpuContext::create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params) {
    StrA api = resolveApi(params.api);

    if (api == "vulkan") { return createVulkanGpuContext(db, name, params); }

    if (api == "d3d12") {
        GN_ERROR(sLogger)("GpuContext::create: D3D12 backend not implemented yet");
        return {};
    }

    if (api == "metal") {
        GN_ERROR(sLogger)("GpuContext::create: Metal backend not implemented yet");
        return {};
    }

    GN_ERROR(sLogger)("GpuContext::create: unknown or unsupported API '{}'", api);
    return {};
}

} // namespace GN::rdg
