#pragma once

#include <garnet/GNgpu2.h>
#include "vk-gpu-image-state.h"
#include "vk-texture.h"

namespace GN::gpu2 {

/// Tracks and batches Vulkan image state transitions for a single raster pass.
///
/// Usage:
///   1. Call addColorTarget() / addDepthStencilTarget() for every attachment.
///   2. Before vkcb.beginRendering(), call emitPrePassBarriers() — transitions every
///      resource to its required layout in one pipelineBarrier call.
///   3. After vkcb.endRendering(), call emitPostPassBarriers() — flushes writes.
///   4. From GpuPayload::onSubmitComplete(), call flushStatesToResources() — writes the
///      final per-subresource state back to each resource's gpuStates.
class RasterStateTrackerVulkan {
public:
    void addColorTarget(TextureVulkanBase * tex, const GpuResourceView & view);
    void addDepthStencilTarget(TextureVulkanBase * tex, const GpuResourceView & view, bool readOnly = false);
    void addSampledTexture(TextureVulkanBase * tex, const GpuResourceView & view,
                           vk::PipelineStageFlags stages = vk::PipelineStageFlagBits::eVertexShader | vk::PipelineStageFlagBits::eFragmentShader);
    void addStorageTexture(TextureVulkanBase * tex, const GpuResourceView & view, vk::PipelineStageFlags stages = vk::PipelineStageFlagBits::eFragmentShader);

    void emitPrePassBarriers(vk::CommandBuffer cb);
    void flushStatesToResources();

private:
    struct TrackedAttachment {
        TextureVulkanBase *               tex;
        GpuResourceView::SubresourceRange resolvedRange;
        // State required during the render pass
        vk::ImageLayout        passLayout;
        vk::AccessFlags        passAccess;
        vk::PipelineStageFlags passStages;
        // State the resource will be in after this pass (persisted back via flushStatesToResources)
        vk::ImageLayout        finalLayout;
        vk::AccessFlags        finalAccess;
        vk::PipelineStageFlags finalStages;
        vk::ImageAspectFlags   aspect;
        bool                   isWrite;   ///< true if this usage writes to the resource
        const char *           usageName; ///< for hazard error messages
    };

    static GpuResourceView::SubresourceRange resolveRange(const GpuResourceView & view, const Texture::Descriptor & d);
    static bool                              subresourceRangesOverlap(const GpuResourceView::SubresourceRange & a, const GpuResourceView::SubresourceRange & b);

    /// Checks \p incoming against all already-registered attachments for the same texture.
    /// Reports an error and returns false if a read/write or write/write hazard is found.
    bool checkHazard(const TrackedAttachment & incoming) const;

    void addAttachment(TrackedAttachment a);

    DynaArray<TrackedAttachment> mAttachments;
};

} // namespace GN::gpu2
