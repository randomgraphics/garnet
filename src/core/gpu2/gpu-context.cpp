#include "pch.h"
#include "gpu-context.h"
#include "gpu-payload-impl.h"
#include "vk-gpu-context.h"
#include <mutex>
#include <utility>

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

static bool groupContains(GpuPayloadGroup & group, const GpuPayloadImpl & payload) {
    for (auto child = group.firstChild(); child; child = child->nextPayloadInGroup()) {
        if (child == &payload) return true;
    }
    return false;
}

AutoRef<GpuPayload> combineGpuPayloads(const StrA & name, const ArrayProxy<AutoRef<GpuPayload>> & payloads) {
    AutoRef<GpuPayloadImpl>  single;
    AutoRef<GpuPayloadGroup> group;

    for (size_t i = 0; i < payloads.size(); ++i) {
        auto payload = payloads[i];
        if (!payload) continue;

        auto impl = RuntimeType::cast<GpuPayloadImpl>((GpuPayload *) payload);
        if (!impl) GN_UNLIKELY {
                GN_ERROR(sLogger)("GpuPayload::combine('{}'): payload '{}'({}) is not an internal gpu2 payload implementation", name, payload->name,
                                  payload->id);
                return {};
            }

        if (impl->isPayloadInGroup()) GN_UNLIKELY {
                GN_ERROR(sLogger)("GpuPayload::combine('{}'): payload '{}'({}) already belongs to a group", name, payload->name, payload->id);
                return {};
            }

        if (!single) {
            single = impl;
            continue;
        }

        if (impl == (GpuPayloadImpl *) single) GN_UNLIKELY {
                GN_ERROR(sLogger)("GpuPayload::combine('{}'): duplicate payload '{}'({})", name, payload->name, payload->id);
                return {};
            }

        if (!group) {
            group = AutoRef<GpuPayloadGroup>::make(name);
            group->appendChild(single);
        } else if (groupContains(*group, *impl))
            GN_UNLIKELY {
                GN_ERROR(sLogger)("GpuPayload::combine('{}'): duplicate payload '{}'({})", name, payload->name, payload->id);
                return {};
            }

        group->appendChild(AutoRef<GpuPayloadImpl>(impl));
    }

    if (group) return group;
    return single;
}

GN_API AutoRef<GpuPayload> GpuPayload::combine(const StrA & name, const ArrayProxy<AutoRef<GpuPayload>> & payloads) {
    return combineGpuPayloads(name, payloads);
}

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
