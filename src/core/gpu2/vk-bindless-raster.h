#pragma once

#include "vk-gpu-context.h"
#include "vk-bindless-payload.h"
#include "vk-bindless-descriptor-heap.h"
#include "vk-bindless-pipeline-layout.h"
#include <garnet/GNgpu2.h>

namespace GN::gpu2 {

/// Vulkan implementation of bindless::Raster pass recorder.
class VkBindlessRaster final : public bindless::Raster {
public:
    GN_REGISTER_RUNTIME_TYPE(bindless::Raster);

    VkBindlessRaster(const StrA & name, AutoRef<GpuContextVulkan2> gpu, RasterTarget target, AutoRef<bindless::DescriptorHeap> heap, uint32_t heapSetIndex,
                     vk::PipelineLayout pipelineLayout, vk::DescriptorPool passPool, std::vector<vk::DescriptorSet> passSets);

    ~VkBindlessRaster() override;

    void                recordDraw(const DrawParameters & params) override;
    void                retainCleanup(std::function<void()> cleanup) override;
    AutoRef<GpuPayload> seal() override;

private:
    AutoRef<GpuContextVulkan2>        mGpu;
    RasterTarget                      mRenderTarget;
    AutoRef<bindless::DescriptorHeap> mHeap;
    uint32_t                          mHeapSetIndex = 0;
    vk::PipelineLayout                mPipelineLayout {};
    vk::DescriptorPool                mPassDescriptorPool {};
    std::vector<vk::DescriptorSet>    mPassDescriptorSets;

    std::vector<StoredBindlessDraw>    mDraws;
    std::vector<uint8_t>               mImmediateData;
    std::vector<std::function<void()>> mRetainedCleanups;
    bool                               mSealed = false;
};

AutoRef<bindless::Raster> createVkBindlessRaster(const StrA & name, const bindless::Raster::CreateParameters & cp);

} // namespace GN::gpu2
