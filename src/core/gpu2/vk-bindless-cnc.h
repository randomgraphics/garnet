#pragma once

#include "vk-gpu-context.h"
#include "vk-bindless-descriptor-heap.h"
#include "vk-bindless-pipeline-layout.h"
#include "vk-cnc-common.h"
#include <garnet/GNgpu2.h>
#include <variant>
#include <vector>

namespace GN::gpu2 {

/// Build descriptor sets for pass resources outside the bindless heap.
bool buildBindlessPassDescriptorSets(GpuContextVulkan2 & gpu, const GpuResourceTable & passResources, uint32_t heapSetIndex, vk::ShaderStageFlags stageFlags,
                                     vk::DescriptorPool & outPool, std::vector<vk::DescriptorSet> & outSets);

struct StoredBindlessCompute {
    AutoRef<GpuShader> cs;
    uint32_t           x = 1, y = 1, z = 1;
    uint32_t           immediateOffset = 0;
    uint32_t           immediateSize   = 0;
};

using StoredBindlessCncOp = std::variant<StoredBindlessCompute, StoredBufferToBuffer, StoredBufferToImage, StoredImageToImage, StoredUploadBuffer,
                                         StoredDownloadBuffer, StoredDownloadImage>;

/// Vulkan implementation of bindless::CnC recorder.
class VkBindlessCnC final : public bindless::CnC {
public:
    GN_REGISTER_RUNTIME_TYPE(bindless::CnC);

    VkBindlessCnC(const StrA & name, AutoRef<GpuContextVulkan2> gpu, AutoRef<bindless::DescriptorHeap> heap, uint32_t heapSetIndex,
                  vk::PipelineLayout pipelineLayout, vk::DescriptorPool passPool, std::vector<vk::DescriptorSet> passSets, GpuResourceTable passResources,
                  size_t opCountHint);

    ~VkBindlessCnC() override;

    void recordCompute(const ComputeParameters & params) override;

    void                             recordCopyBuffer(const BufferToBuffer & p) override;
    void                             recordUploadBuffer(AutoRef<Buffer> dst, uint64_t offset, ArrayView<const uint8_t> content) override;
    std::future<AutoRef<const Blob>> recordDownloadBuffer(AutoRef<Buffer> src, uint64_t offset = 0, uint64_t size = uint64_t(~0)) override;
    void                             recordUploadImage(AutoRef<Texture> dst, ArrayView<const uint8_t> content, ArrayView<const Region> regions) override;
    void                             recordCopyImage(const ImageToImage & p) override;
    std::future<TextureContent>      recordDownloadImage(AutoRef<Texture> src, ArrayView<const Region> regions) override;

    void addCleanupCallback(std::function<void()> cleanup) override;

    AutoRef<GpuPayload> seal() override;

private:
    AutoRef<GpuContextVulkan2>        mGpu;
    AutoRef<bindless::DescriptorHeap> mHeap;
    uint32_t                          mHeapSetIndex = 0;
    vk::PipelineLayout                mPipelineLayout {};
    vk::DescriptorPool                mPassDescriptorPool {};
    std::vector<vk::DescriptorSet>    mPassDescriptorSets;
    GpuResourceTable                  mPassResources;

    std::vector<StoredBindlessCncOp>   mOps;
    std::vector<uint8_t>               mImmediateData;
    std::vector<std::function<void()>> mRetainedCleanups;
    std::unique_ptr<CncUploadStorage>  mUploadStorage = std::make_unique<CncUploadStorage>();
    bool                               mSealed        = false;
};

AutoRef<bindless::CnC> createVkBindlessCnc(const StrA & name, const bindless::CnC::CreateParameters & cp);

} // namespace GN::gpu2
