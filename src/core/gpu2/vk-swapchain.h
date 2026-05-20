#pragma once

#include "vk-gpu-context.h"
#include <garnet/GNgpu2.h>

namespace GN::gpu2 {

AutoRef<Swapchain> createSwapchainVulkan2(const Swapchain::CreateDesc & desc);

} // namespace GN::gpu2
