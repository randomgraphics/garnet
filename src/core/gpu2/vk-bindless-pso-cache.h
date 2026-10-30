#pragma once

#include "vk-gpu-context.h"
#include "vk-gpu-shader.h"
#include "vk-format-utils.h"
#include <garnet/GNgpu2.h>
#include <mutex>
#include <unordered_map>

namespace GN::gpu2 {

/// Compact key for bindless Vulkan graphics pipelines.
struct BindlessPsoKey {
    vk::PipelineLayout pipelineLayout {};
    uint64_t           shaderHash = 0; ///< FNV-multiply hash of vs and ps shader IDs

    union {
        uint64_t geomWord = 0;
        struct {
            uint64_t noInput     : 1; ///< 1 = fullscreen triangle, no VBOs
            uint64_t numBindings : 3;
            uint64_t stride0     : 12; ///< binding 0 stride in bytes (max 4095)
            uint64_t stride1     : 12;
            uint64_t stride2     : 12;
            uint64_t numAttribs  : 5;
            uint64_t attrHash    : 16; ///< hash of vertex attributes
        };
    };

    union {
        uint64_t stateWord = 0;
        struct {
            uint64_t fillMode       : 2;
            uint64_t cullMode       : 2;
            uint64_t frontFace      : 1;
            uint64_t depthFunc      : 3;
            uint64_t depthWrite     : 1;
            uint64_t stencilEnable  : 1;
            uint64_t stencilCompare : 3;
            uint64_t stencilPass    : 3;
            uint64_t stencilFail    : 3;
            uint64_t stencilZFail   : 3;
            uint64_t stencilRef     : 8;
            uint64_t stencilRdMask  : 8;
            uint64_t stencilWrMask  : 8;
            uint64_t _statePad      : 18;
        };
    };

    uint16_t colorFmts[8] = {};
    uint16_t depthFmt     = 0;
    uint8_t  colorCount   = 0;
    uint8_t  _rtPad       = 0;
    uint64_t blendHash    = 0;

    bool operator==(const BindlessPsoKey & o) const noexcept;

    static BindlessPsoKey make(vk::PipelineLayout layout, const GpuShaderVulkan & vs, const GpuShaderVulkan * hs, const GpuShaderVulkan * ds,
                               const GpuShaderVulkan * gs, const GpuShaderVulkan * ps, const RasterState & state, const RasterGeometry & geom,
                               const PassFormats & formats, const StackArray<RasterTarget::ColorTarget, 8> & colorTargets);
};

struct BindlessPsoKeyHash {
    size_t operator()(const BindlessPsoKey & k) const noexcept;
};

/// Get-or-create cache for native Vulkan graphics pipelines used in bindless raster passes.
/// Owned by GpuContextVulkan2; pipelines are destroyed when the device shuts down.
class VkBindlessPsoCache {
public:
    explicit VkBindlessPsoCache(GpuContextVulkan2 & gpu);
    ~VkBindlessPsoCache();

    VkBindlessPsoCache(const VkBindlessPsoCache &)             = delete;
    VkBindlessPsoCache & operator=(const VkBindlessPsoCache &) = delete;

    /// Returns a cached pipeline or creates a new one using Vulkan 1.3 dynamic rendering.
    vk::Pipeline getOrCreate(vk::PipelineLayout layout, const GpuShaderVulkan * vs, const GpuShaderVulkan * hs, const GpuShaderVulkan * ds,
                             const GpuShaderVulkan * gs, const GpuShaderVulkan * ps, const RasterState & state, const RasterGeometry & geom,
                             const PassFormats & formats, const StackArray<RasterTarget::ColorTarget, 8> & colorTargets);

    size_t cacheSize() const;

private:
    vk::Pipeline buildPipeline(vk::PipelineLayout layout, const GpuShaderVulkan * vs, const GpuShaderVulkan * hs, const GpuShaderVulkan * ds,
                               const GpuShaderVulkan * gs, const GpuShaderVulkan * ps, const RasterState & state, const RasterGeometry & geom,
                               const PassFormats & formats, const StackArray<RasterTarget::ColorTarget, 8> & colorTargets);

    GpuContextVulkan2 &                                                  mGpu;
    std::mutex                                                           mMutex;
    std::unordered_map<BindlessPsoKey, vk::Pipeline, BindlessPsoKeyHash> mCache;
};

} // namespace GN::gpu2
