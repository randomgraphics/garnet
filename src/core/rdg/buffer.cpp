#include "pch.h"
#include "vk-transient-buffer.h"
#include "gpu-context.h"

static GN::Logger * sLogger = GN::getLogger("GN.rdg");

namespace GN::rdg {

GN_API AutoRef<PersistentBuffer> PersistentBuffer::create(const StrA & name, const CreateParameters & params) {
    if (!params.context) GN_UNLIKELY {
            GN_ERROR(sLogger)("PersistentBuffer::create: context is null, name='{}'", name);
            return {};
        }
    auto * common = static_cast<GpuContextCommon *>(params.context.get());
    switch (common->api()) {
    case GpuContextCommon::Api::Vulkan:
        return createVulkanPersistentBuffer(name, params);
    case GpuContextCommon::Api::D3D12:
        GN_ERROR(sLogger)("PersistentBuffer::create: D3D12 backend not implemented yet");
        return {};
    case GpuContextCommon::Api::Metal:
        GN_ERROR(sLogger)("PersistentBuffer::create: Metal backend not implemented yet");
        return {};
    default:
        GN_ERROR(sLogger)("PersistentBuffer::create: unknown API, name='{}'", name);
        return {};
    }
}

GN_API AutoRef<TransientArena> TransientArena::create(const StrA & name, const CreateParameters & params) {
    if (!params.context) GN_UNLIKELY {
            GN_ERROR(sLogger)("TransientBufferArena::create: context is null, name='{}'", name);
            return {};
        }
    auto * common = static_cast<GpuContextCommon *>(params.context.get());
    switch (common->api()) {
    case GpuContextCommon::Api::Vulkan:
        return createVulkanTransientArena(name, params);
    default:
        GN_ERROR(sLogger)("TransientBufferArena::create: unknown API, name='{}'", name);
        return {};
    }
}

} // namespace GN::rdg
