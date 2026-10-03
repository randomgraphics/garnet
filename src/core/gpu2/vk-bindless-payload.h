#pragma once

#include "vk-gpu-context.h"
#include "vk-gpu-payload.h"
#include "vk-bindless-descriptor-heap.h"
#include "vk-bindless-pso-cache.h"
#include <garnet/GNgpu2.h>
#include <vector>

namespace GN::gpu2 {

struct StoredBindlessDraw {
    AutoRef<GpuShader> vs, hs, ds, gs, ps;
    RasterState        mergedState;
    RasterGeometry     geometry;
    uint32_t           instanceCount   = 1;
    uint32_t           immediateOffset = 0;
    uint32_t           immediateSize   = 0;
};

/// Vulkan GPU payload for recorded bindless raster passes.
/// Seals drawing commands, automatically transitions attachments to optimal layout on pass start,
/// binds the global descriptor heap once, and auto-restores attachments to SHADER_READ_ONLY_OPTIMAL.
class VkBindlessPayload final : public GpuPayloadVulkan {
public:
    GN_REGISTER_RUNTIME_TYPE(GpuPayloadVulkan);

    struct ConstructParameters {
        AutoRef<GpuContextVulkan2>         gpu;
        RasterTarget                       target;
        AutoRef<bindless::DescriptorHeap>  heap;
        uint32_t                           heapSetIndex = 0;
        vk::PipelineLayout                 pipelineLayout {};
        std::vector<StoredBindlessDraw>    draws;
        std::vector<uint8_t>               immediateData;
        std::vector<std::function<void()>> retainedCleanups;
        vk::DescriptorPool                 passDescriptorPool {};
        std::vector<vk::DescriptorSet>     passDescriptorSets;
    };

    explicit VkBindlessPayload(const StrA & name, ConstructParameters params);
    ~VkBindlessPayload() override;

    void recordForVulkanSubmit(const RecordContext & ctx) override;
    void onGpuComplete() override;

private:
    AutoRef<GpuContextVulkan2>         mGpu;
    RasterTarget                       mRenderTarget;
    AutoRef<bindless::DescriptorHeap>  mHeap;
    uint32_t                           mHeapSetIndex = 0;
    vk::PipelineLayout                 mPipelineLayout {};
    std::vector<StoredBindlessDraw>    mDraws;
    std::vector<uint8_t>               mImmediateData;
    std::vector<std::function<void()>> mRetainedCleanups;
    vk::DescriptorPool                 mPassDescriptorPool {};
    std::vector<vk::DescriptorSet>     mPassDescriptorSets;
};

} // namespace GN::gpu2
