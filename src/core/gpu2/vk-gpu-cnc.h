#pragma once

#include <garnet/GNgpu2.h>

namespace GN::gpu2 {

AutoRef<GpuCnC> createGpuCncVulkan2(const GpuCnC::CreateParameters & params);

// Synchronous convenience methods need a failure result even though the public recording methods return void.
AutoRef<GpuPayload> createCncBufferUploadPayload(AutoRef<GpuContext> gpu, AutoRef<Buffer> dst, ArrayView<const uint8_t> content, uint64_t offset);
AutoRef<GpuPayload> createCncImageUploadPayload(AutoRef<GpuContext> gpu, AutoRef<Texture> dst, const gfx::img::Image & content);

} // namespace GN::gpu2
