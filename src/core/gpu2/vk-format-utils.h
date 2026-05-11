#pragma once
#include "vk-gpu-context.h"
#include <garnet/GNrdg.h>

namespace GN::gpu2 {

vk::Format pixelFormatToVkFormat(gfx::img::PixelFormat pf);

gfx::img::PixelFormat vkFormatToPixelFormat(vk::Format vkFmt);

vk::Format vertexAttributeFormatToVk(RasterGeometry::AttributeFormat f);

/// Derive Vulkan aspect flags from a requested view format relative to the texture's native format.
///
/// Depth-stencil textures carry no "aspect" concept in the gpu2 public API; the caller instead
/// requests a view format that encodes which channel(s) to access:
///   - D24S8 (RG_24_UNORM_8_UINT / DS_24_UNORM_8_UINT):
///       RX_24_8_UNORM or DX_24_8_UNORM → depth;  R_8_UINT or S_8_UNORM → stencil;  combined → both
///   - D32S8 (RGX_32_FLOAT_8_UINT_24 / DSX_32_FLOAT_8_UINT_24):
///       R_32_FLOAT or D_32_FLOAT → depth;  R_8_UINT or S_8_UNORM → stencil;  combined → both
///   - Depth-only (D16, DX_24, D32): always depth
///   - Everything else: color
///
/// Returns {} (no bits set) when \p viewFmt is incompatible with a depth-stencil \p textureFmt.
vk::ImageAspectFlags aspectFromViewFormat(gfx::img::PixelFormat viewFmt, gfx::img::PixelFormat textureFmt);

} // namespace GN::gpu2