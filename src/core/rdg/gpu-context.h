#pragma once

#include <garnet/GNrdg.h>

namespace GN::rdg {

// GpuContextCommon is the "common impl" base for the GpuContext artifact: it extends the
// public artifact (GpuContext) with shared logic and a custom interface (Api enum, api())
// that backend-specific implementations (GpuContextVulkan, etc.) inherit. The public API
// stays on GpuContext; code that needs the extended interface casts to GpuContextCommon.

/// Common base for API-specific GpuContext implementations (Vulkan, D3D12, Metal).
/// Adds custom interface: Api enum and api() (not on the public GpuContext).
class GpuContextCommon : public GpuContext {
public:
    /// Graphics API of a created context (concrete backend only).
    enum class Api {
        Vulkan,
        D3D12,
        Metal,
    };

    Api api() const { return mApi; }

protected:
    Api mApi;

    GpuContextCommon(ArtifactDatabase & db, const StrA & name, Api api)
        : GpuContext(db, TYPE, name), mApi(api) {}
};

} // namespace GN::rdg
