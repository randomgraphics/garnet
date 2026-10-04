#pragma once

#include "vk-gpu-context.h"
#include "vk-gpu-payload.h"
#include "vk-bindless-descriptor-heap.h"
#include "vk-bindless-compute-pso-cache.h"
#include "vk-bindless-cnc.h"
#include "vk-cnc-common.h"
#include <garnet/GNgpu2.h>
#include <vector>

namespace GN::gpu2 {

/// Vulkan GPU payload for recorded bindless copy and compute (CnC) passes.
class VkBindlessCncPayload final : public GpuPayloadVulkan {
public:
    GN_REGISTER_RUNTIME_TYPE(GpuPayloadVulkan);

    struct ConstructParameters {
        AutoRef<GpuContextVulkan2>         gpu;
        AutoRef<bindless::DescriptorHeap>  heap;
        uint32_t                           heapSetIndex = 0;
        vk::PipelineLayout                 pipelineLayout {};
        std::vector<StoredBindlessCncOp>   ops;
        std::vector<uint8_t>               immediateData;
        std::vector<std::function<void()>> retainedCleanups;
        vk::DescriptorPool                 passDescriptorPool {};
        std::vector<vk::DescriptorSet>     passDescriptorSets;
        GpuResourceTable                   passResources;
    };

    explicit VkBindlessCncPayload(const StrA & name, ConstructParameters params);
    ~VkBindlessCncPayload() override;

    void recordForVulkanSubmit(const RecordContext & ctx) override;
    void onGpuComplete() override;

private:
    AutoRef<GpuContextVulkan2>         mGpu;
    AutoRef<bindless::DescriptorHeap>  mHeap;
    uint32_t                           mHeapSetIndex = 0;
    vk::PipelineLayout                 mPipelineLayout {};
    std::vector<StoredBindlessCncOp>   mOps;
    std::vector<uint8_t>               mImmediateData;
    std::vector<std::function<void()>> mRetainedCleanups;
    vk::DescriptorPool                 mPassDescriptorPool {};
    std::vector<vk::DescriptorSet>     mPassDescriptorSets;
    GpuResourceTable                   mPassResources;
};

} // namespace GN::gpu2
