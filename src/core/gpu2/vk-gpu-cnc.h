#pragma once

#include <garnet/GNgpu2.h>

namespace GN::gpu2 {

AutoRef<GpuCnC> createGpuCncVulkan2(const GpuCnC::CreateParameters & params);

} // namespace GN::gpu2
