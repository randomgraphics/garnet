#pragma once

#include <garnet/GNgpu2.h>
#include <mutex>

namespace GN::gpu2 {

/// Common base for API-specific \c GpuContext implementations (Vulkan, D3D12, Metal).
class GpuContextCommon2 : public GpuContext {
public:
    GN_API GN_REGISTER_RUNTIME_TYPE(GpuContext);

    enum class Api {
        VULKAN,
        D3D12,
        METAL,
    };

    Api api() const { return mApi; }

protected:
    Api mApi;
    GpuContextCommon2(const RuntimeType::TypeInfo & typeInfo, const StrA & name, Api api): GpuContext(typeInfo, name), mApi(api) {}
};

} // namespace GN::gpu2
