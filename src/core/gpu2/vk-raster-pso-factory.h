#pragma once

// Must be the first vk-* header in any TU so it can set up RAPID_VULKAN_NAMESPACE
// before rapid-vulkan.h is pulled in.
#include <garnet/GNgpu2.h>
#include "vk-gpu-context.h"
#include "vk-gpu-shader.h"

#include <vector>

namespace GN::gpu2 {

// ── PassFormats ───────────────────────────────────────────────────────────────
// Resolved VkFormats for the current render pass; shared between the factory
// (PSO key) and the raster payload (beginRendering).

struct PassFormats {
    std::vector<vk::Format> colors;
    vk::Format              depth = vk::Format::eUndefined;
};

// ── Gpu2RasterPsoCreateParams ─────────────────────────────────────────────────
// Everything the factory needs to look up or create a pipeline.

struct Gpu2RasterPsoCreateParams {
    const GpuShaderVulkan *                vs = nullptr;
    const GpuShaderVulkan *                ps = nullptr; ///< may be null (vertex-only)
    const RasterState &                    state;
    const RasterGeometry &                 geometry;
    const PassFormats &                    formats;
    const RasterTarget::ColorTargetArray & colorTargets;
};

// ── RasterPsoFactory ──────────────────────────────────────────────────────────
// Get-or-create cache for Vulkan graphics pipelines.
// Owned by GpuContextVulkan2; must not outlive the Vulkan device.

class RasterPsoFactory {
public:
    explicit RasterPsoFactory(GpuContextVulkan2 & gpu);
    ~RasterPsoFactory();

    RasterPsoFactory(const RasterPsoFactory &)             = delete;
    RasterPsoFactory & operator=(const RasterPsoFactory &) = delete;

    /// Returns a cached pipeline or creates a new one. Returns empty ref on failure.
    rv::Ref<const rv::GraphicsPipeline> getOrCreate(const Gpu2RasterPsoCreateParams & params);

    size_t cacheSize() const;

private:
    class Impl;
    Impl * mImpl = nullptr;
};

} // namespace GN::gpu2
