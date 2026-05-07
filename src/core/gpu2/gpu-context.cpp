#include "gpu-context.h"
#include "vk-gpu-context.h"
#include <mutex>

static GN::Logger * sLogger = GN::getLogger("GN.gpu2");

namespace GN::gpu2 {

// =============================================================================
// RootEntity constructor
// =============================================================================

static uint64_t nextRootEntityId() {
    static std::mutex m;
    static uint64_t   counter = 0;
    std::lock_guard   lk(m);
    return counter++;
}

GN_API RootEntity::RootEntity(const RuntimeType::TypeInfo & type, const StrA & name_): RefCounter(), RuntimeType(type), id(nextRootEntityId()), name(name_) {}

// =============================================================================
// API resolution
// =============================================================================

static StrA resolveApi(const StrA & api) {
    if (api.empty() || api == "auto") { return "vulkan"; }
    StrA out = api;
    out.toLower();
    return out;
}

// =============================================================================
// GpuContext::create
// =============================================================================

GN_API AutoRef<GpuContext> GpuContext::create(const StrA & name, const CreateParameters & params) {
    StrA apiStr = resolveApi(params.api);

    if ("vulkan" == apiStr) {
        return createVulkanGpuContext2(name, params);
    } else if ("d3d12" == apiStr) {
        GN_ERROR(sLogger)("GpuContext::create: D3D12 backend not implemented yet");
        return {};
    } else if ("metal" == apiStr) {
        GN_ERROR(sLogger)("GpuContext::create: Metal backend not implemented yet");
        return {};
    } else {
        GN_ERROR(sLogger)("GpuContext::create: unknown or unsupported API '{}'", apiStr);
        return {};
    }
}

} // namespace GN::gpu2
