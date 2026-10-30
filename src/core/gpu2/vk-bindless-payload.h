#pragma once

#include "vk-gpu-context.h"
#include "vk-gpu-payload.h"
#include "vk-bindless-descriptor-heap.h"
#include "vk-bindless-pso-cache.h"
#include <garnet/GNgpu2.h>
#include <memory>
#include <memory_resource>
#include <vector>

namespace GN::gpu2 {

struct StoredBindlessDraw {
    AutoRef<GpuShader> vs, hs, ds, gs, ps;
    RasterState        mergedState;
    RasterGeometry     geometry;
    uint32_t           immediateOffset    = 0;
    uint32_t           immediateSize      = 0;
    uint32_t           boundResourceIndex = ~0u;

    StoredBindlessDraw(const bindless::Raster::DrawParameters & params, const RasterState & state, std::pmr::memory_resource * memRes, uint32_t immOffset,
                       uint32_t immSize, uint32_t boundResIdx = ~0u)
        : vs(params.vs), hs(params.hs), ds(params.ds), gs(params.gs), ps(params.ps), mergedState(state), geometry(params.geometry, memRes),
          immediateOffset(immOffset), immediateSize(immSize), boundResourceIndex(boundResIdx) {}
};

struct BoundResourceEntry {
    vk::PipelineLayout             pipelineLayout {};
    vk::DescriptorPool             descriptorPool {};
    std::vector<vk::DescriptorSet> descriptorSets;
};

/// All per-pass draw memory: the draw array and every geometry array snapshotted into it.
///
/// Heap-allocated once per pass and handed from recorder to payload by pointer. Moving the
/// pmr::vector itself into another container would not work: pmr allocators do not propagate on
/// move-assignment, so the target would reallocate from its own resource and copy every draw.
/// Holding everything in one object also pins destruction order (draws -> pool -> arena -> backing buffer).
struct BindlessDrawStorage {
    const size_t                           backingSize;
    std::unique_ptr<uint8_t[]>             backing;
    std::pmr::monotonic_buffer_resource    arena;
    std::pmr::unsynchronized_pool_resource pool;
    std::pmr::vector<StoredBindlessDraw>   draws;

    /// Preallocates for eight buffers with eight attributes each per draw, plus pool overhead.
    /// The upstream resource handles larger layouts or underestimated draw counts.
    explicit BindlessDrawStorage(size_t drawCountHint, std::pmr::memory_resource * upstream = std::pmr::get_default_resource());

    BindlessDrawStorage(const BindlessDrawStorage &)             = delete;
    BindlessDrawStorage & operator=(const BindlessDrawStorage &) = delete;
};

/// Vulkan GPU payload for recorded bindless raster passes.
/// Seals drawing commands, automatically transitions attachments to optimal layout on pass start,
/// binds the global descriptor heap once, and auto-restores attachments to SHADER_READ_ONLY_OPTIMAL.
class VkBindlessPayload final : public GpuPayloadVulkan {
public:
    GN_REGISTER_RUNTIME_TYPE(GpuPayloadVulkan);

    struct ConstructParameters {
        AutoRef<GpuContextVulkan2>           gpu;
        RasterTarget                         target;
        AutoRef<bindless::DescriptorHeap>    heap;
        uint32_t                             heapSetIndex = 0;
        vk::PipelineLayout                   pipelineLayout {};
        std::unique_ptr<BindlessDrawStorage> storage;
        std::vector<uint8_t>                 immediateData;
        std::vector<std::function<void()>>   retainedCleanups;
        vk::DescriptorPool                   passDescriptorPool {};
        std::vector<vk::DescriptorSet>       passDescriptorSets;
        std::vector<BoundResourceEntry>      boundResources;
        std::vector<GpuResourceTable>        boundResourceTables;
    };

    explicit VkBindlessPayload(const StrA & name, ConstructParameters params);
    ~VkBindlessPayload() override;

    void recordForVulkanSubmit(const RecordContext & ctx) override;
    void onGpuComplete() override;

private:
    AutoRef<GpuContextVulkan2>           mGpu;
    RasterTarget                         mRenderTarget;
    AutoRef<bindless::DescriptorHeap>    mHeap;
    uint32_t                             mHeapSetIndex = 0;
    vk::PipelineLayout                   mPipelineLayout {};
    std::unique_ptr<BindlessDrawStorage> mStorage;
    std::vector<uint8_t>                 mImmediateData;
    std::vector<std::function<void()>>   mRetainedCleanups;
    vk::DescriptorPool                   mPassDescriptorPool {};
    std::vector<vk::DescriptorSet>       mPassDescriptorSets;
    std::vector<BoundResourceEntry>      mBoundResources;
    std::vector<GpuResourceTable>        mBoundResourceTables;
};

} // namespace GN::gpu2
