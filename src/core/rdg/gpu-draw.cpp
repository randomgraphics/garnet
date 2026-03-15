#include "pch.h"
#include "vk-draw-actions.h"
#include "gpu-context.h"

static GN::Logger * sLogger = GN::getLogger("GN.rdg");

namespace GN::rdg {

// =============================================================================
// ClearRenderTarget::create() - API-neutral dispatch by context's backend
// =============================================================================

GN_API AutoRef<ClearRenderTarget> ClearRenderTarget::create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params) {
    if (!params.gpu) GN_UNLIKELY {
            GN_ERROR(sLogger)("ClearRenderTarget::create: context is null, name='{}'", name);
            return {};
        }
    auto common = static_cast<GpuContextCommon *>(params.gpu.get());
    switch (common->api()) {
    case GpuContextCommon::Api::Vulkan:
        return createVulkanClearRenderTarget(db, name, params);
    case GpuContextCommon::Api::D3D12:
        GN_ERROR(sLogger)("ClearRenderTarget::create: D3D12 backend not implemented yet");
        return {};
    case GpuContextCommon::Api::Metal:
        GN_ERROR(sLogger)("ClearRenderTarget::create: Metal backend not implemented yet");
        return {};
    default:
        GN_ERROR(sLogger)("ClearRenderTarget::create: unknown API, name='{}'", name);
        return {};
    }
}

// =============================================================================
// GpuDraw::create() - API-neutral dispatch by context's backend
// =============================================================================

GN_API AutoRef<GpuDraw> GpuDraw::create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params) {
    if (!params.context) GN_UNLIKELY {
            GN_ERROR(sLogger)("GpuDraw::create: context is null, name='{}'", name);
            return {};
        }
    auto common = static_cast<GpuContextCommon *>(params.context.get());
    switch (common->api()) {
    case GpuContextCommon::Api::Vulkan:
        return createVulkanGpuDraw(db, name, params);
    case GpuContextCommon::Api::D3D12:
        GN_ERROR(sLogger)("GpuDraw::create: D3D12 backend not implemented yet");
        return {};
    case GpuContextCommon::Api::Metal:
        GN_ERROR(sLogger)("GpuDraw::create: Metal backend not implemented yet");
        return {};
    default:
        GN_ERROR(sLogger)("GpuDraw::create: unknown API, name='{}'", name);
        return {};
    }
}

} // namespace GN::rdg
