#pragma once

#include <garnet/GNgpu2.h>
#include "vk-gpu-command-buffer.h"

namespace GN::gpu2 {

AutoRef<GpuRaster> createGpuRasterVulkan2(const GpuRaster::CreateParameters & params);

} // namespace GN::gpu2
