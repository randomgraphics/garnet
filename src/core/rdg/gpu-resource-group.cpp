#include "pch.h"
// GpuResourceGroup commented out: use GpuResourceTable + Drawable/DrawPack instead.
#if 0
    #include "gpu-context.h"
    #include "vk-gpu-resource-group.h"

static GN::Logger * sLogger = GN::getLogger("GN.rdg");

namespace GN::rdg {

GN_API AutoRef<GpuResourceGroup> GpuResourceGroup::create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params) {
    if (!params.context) GN_UNLIKELY {
            GN_ERROR(sLogger)("GpuResourceGroup::create: context is null, name='{}'", name);
            return {};
        }
    auto * common = static_cast<GpuContextCommon *>(params.context.get());
    switch (common->api()) {
    case GpuContextCommon::Api::Vulkan:
        return createVulkanGpuResourceGroup(db, name, params);
    case GpuContextCommon::Api::D3D12:
        GN_ERROR(sLogger)("GpuResourceGroup::create: D3D12 backend not implemented yet");
        return {};
    case GpuContextCommon::Api::Metal:
        GN_ERROR(sLogger)("GpuResourceGroup::create: Metal backend not implemented yet");
        return {};
    default:
        GN_ERROR(sLogger)("GpuResourceGroup::create: unknown API, name='{}'", name);
        return {};
    }
}

} // namespace GN::rdg
#endif
