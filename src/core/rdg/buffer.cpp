#include "pch.h"
#include "vk-buffer.h"
#include "gpu-context.h"

static GN::Logger * sLogger = GN::getLogger("GN.rdg");

namespace GN::rdg {

GN_API AutoRef<Buffer> Buffer::create(ArtifactDatabase & db, const StrA & name, const CreateParameters & params) {
    if (!params.context) GN_UNLIKELY {
            GN_ERROR(sLogger)("Buffer::create: context is null, name='{}'", name);
            return {};
        }
    auto * common = static_cast<GpuContextCommon *>(params.context.get());
    switch (common->api()) {
    case GpuContextCommon::Api::Vulkan:
        return createVulkanBuffer(db, name, params);
    case GpuContextCommon::Api::D3D12:
        GN_ERROR(sLogger)("Buffer::create: D3D12 backend not implemented yet");
        return {};
    case GpuContextCommon::Api::Metal:
        GN_ERROR(sLogger)("Buffer::create: Metal backend not implemented yet");
        return {};
    default:
        GN_ERROR(sLogger)("Buffer::create: unknown API, name='{}'", name);
        return {};
    }
}

} // namespace GN::rdg
