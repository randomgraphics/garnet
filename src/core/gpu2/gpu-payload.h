#pragma once

#include <garnet/GNgpu2.h>

namespace GN::gfx::gpu2 {

/// API-neutral base class for a GPU payload. This is the internal definition of the
/// forward-declared \c GpuPayload in gpu2.h; used by both GpuContext and Swapchain
/// implementations without circular includes.
///
/// A payload is a self-contained unit of recorded GPU work (draws, dispatches, copies, etc.).
/// It carries no command buffer reference; the association with a specific command buffer and
/// any required pipeline barriers are resolved at submit time inside GpuContext::submit().
struct GpuPayload {
    // Empty base class; all actual data is in API-specific subclasses.
};

} // namespace GN::gfx::gpu2
