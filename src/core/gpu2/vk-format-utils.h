#pragma once
#include "vk-gpu-context.h"
#include <garnet/GNrdg.h>

namespace GN::gfx::gpu2 {

vk::Format pixelFormatToVkFormat(gfx::img::PixelFormat pf);

gfx::img::PixelFormat vkFormatToPixelFormat(vk::Format vkFmt);

vk::Format vertexAttributeFormatToVk(RenderGeometry::AttributeFormat f);

} // namespace GN::gfx::gpu2