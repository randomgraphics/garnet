#pragma once
#include "vk-gpu-context.h"

namespace GN::gpu2 {

struct PassFormats {
    std::vector<vk::Format> colors;
    vk::Format              depth = vk::Format::eUndefined;
};

vk::Format pixelFormatToVkFormat(gfx::img::PixelFormat pf);

gfx::img::PixelFormat vkFormatToPixelFormat(vk::Format vkFmt);

vk::Format vertexAttributeFormatToVk(RasterGeometry::AttributeFormat f);

vk::CompareOp           compareToVk(RasterState::Compare c);
vk::StencilOp           stencilOpToVk(RasterState::StencilState::Op op);
vk::BlendFactor         blendArgToVk(RasterTarget::BlendState::Arg a);
vk::BlendOp             blendOpToVk(RasterTarget::BlendState::Op o);
vk::ColorComponentFlags writeMaskToVk(uint8_t w);

// Derive Vulkan aspect flags from a requested view format relative to the texture's native format.
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

inline vk::ImageLayout shaderReadOnlyLayout(gfx::img::PixelFormat format) {
    // Descriptors and the ready-state tracker must agree, especially after depth readback or attachment use.
    const auto aspects = aspectFromViewFormat(format, format);
    return (aspects & (vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil)) ? vk::ImageLayout::eDepthStencilReadOnlyOptimal
                                                                                             : vk::ImageLayout::eShaderReadOnlyOptimal;
}

bool attachmentExtent(const GpuResourceView & view, vk::Extent2D & extent);

bool resolveColorAttachment(const GpuResourceView & v, vk::Image * outImage, vk::ImageView * outView, vk::Extent2D * outExt, vk::Format * outVkFormat);

vk::Viewport rsViewportToVk(const RasterState::Viewport & vp, vk::Extent2D ext);

vk::Rect2D rsScissorToVk(const RasterState::ScissorRect & sr, vk::Extent2D ext);

void mergeRenderState(RasterState & dst, const RasterState & src);

} // namespace GN::gpu2
