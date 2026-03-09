#pragma once
#include "vk-gpu-context.h"
#include <garnet/GNrdg.h>

namespace GN::rdg {

vk::Format pixelFormatToVkFormat(gfx::img::PixelFormat pf);

gfx::img::PixelFormat vkFormatToPixelFormat(vk::Format vkFmt);

vk::Format vertexAttributeFormatToVk(GpuGeometry::AttributeFormat f);

} // namespace GN::rdg