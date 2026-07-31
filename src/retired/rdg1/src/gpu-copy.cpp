#include "pch.h"
#include "vk-copy.h"

static GN::Logger * sLogger = GN::getLogger("GN.rdg");

namespace GN::rdg {

GN_API AutoRef<GpuCopy> GpuCopy::create(const StrA & name, const CreateParameters & params) {
    if (!params.gpu) GN_UNLIKELY {
            GN_ERROR(sLogger, "GpuCopy::create: gpu is null, name='{}'", name);
            return {};
        }
    auto * common = static_cast<GpuContextCommon *>(params.gpu.get());
    switch (common->api()) {
    case GpuContextCommon::Api::Vulkan:
        return createVulkanGpuCopy(name, params);
    case GpuContextCommon::Api::D3D12:
        GN_ERROR(sLogger, "GpuCopy::create: D3D12 backend not implemented yet");
        return {};
    case GpuContextCommon::Api::Metal:
        GN_ERROR(sLogger, "GpuCopy::create: Metal backend not implemented yet");
        return {};
    default:
        GN_ERROR(sLogger, "GpuCopy::create: unknown API, name='{}'", name);
        return {};
    }
}

} // namespace GN::rdg
