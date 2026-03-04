#pragma once

#include "vk-submission-context.h"

namespace GN::rdg {

/// Create Vulkan-backed ClearRenderTarget. Called from ClearRenderTarget::create() when context is Vulkan.
/// Returns null on failure or duplicate type+name.
AutoRef<ClearRenderTarget> createVulkanClearRenderTarget(ArtifactDatabase & db, const StrA & name, const ClearRenderTarget::CreateParameters & params);

/// Create Vulkan-backed GpuDraw. Called from GpuDraw::create() when context is Vulkan.
AutoRef<GpuDraw> createVulkanGpuDraw(ArtifactDatabase & db, const StrA & name, const GpuDraw::CreateParameters & params);

} // namespace GN::rdg
