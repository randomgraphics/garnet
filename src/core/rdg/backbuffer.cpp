#include "vk-backbuffer.h"

static GN::Logger * sLogger = GN::getLogger("GN.rdg");

namespace GN::rdg {

// =============================================================================
// Backbuffer::create() - API-neutral dispatch by context's backend
// Context from GpuContext::create() is always GpuContextCommon; cast and query api().
// =============================================================================

AutoRef<Backbuffer> Backbuffer::create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params) {
    if (!params.context) {
        GN_ERROR(sLogger)("Backbuffer::create: context is null, name='{}'", name);
        return {};
    }

    auto * common = static_cast<GpuContextCommon *>(params.context.get());
    switch (common->api()) {
    case GpuContextCommon::Api::Vulkan:
        return createVulkanBackbuffer(db, name, params);
    case GpuContextCommon::Api::D3D12:
        GN_ERROR(sLogger)("Backbuffer::create: D3D12 backend not implemented yet");
        return {};
    case GpuContextCommon::Api::Metal:
        GN_ERROR(sLogger)("Backbuffer::create: Metal backend not implemented yet");
        return {};
    default:
        GN_ERROR(sLogger)("Backbuffer::create: unknown API, name='{}'", name);
        return {};
    }
}

} // namespace GN::rdg
