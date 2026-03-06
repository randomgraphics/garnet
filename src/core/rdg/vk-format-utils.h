#pragma once
#include "vk-gpu-context.h"

namespace GN::rdg {

vk::Format pixelFormatToVkFormat(gfx::img::PixelFormat pf);

gfx::img::PixelFormat vkFormatToPixelFormat(vk::Format vkFmt);

} // namespace GN::rdg