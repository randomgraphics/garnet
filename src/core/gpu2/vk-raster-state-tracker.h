#pragma once

#include <garnet/GNgpu2.h>
#include "vk-buffer.h"
#include "vk-buffer-state.h"
#include "vk-gpu-image-state.h"
#include "vk-texture.h"

#include <unordered_map>
#include <vector>

namespace GN::gpu2 {

/// Tracks and batches Vulkan image state transitions for a single raster pass.
///
/// Usage:
///   1. Call addRasterTarget() for the pass's render target.
///   2. For each draw, call upgradeForDrawRasterState() so that any attachment registered as
///      read-only is promoted to read-write when the draw's merged state requires it.
///   3. For each draw, call addGpuResourceTable() and addRasterGeometry() to register shader
///      resources and geometry buffers.
///   4. Call emitPrePassBarriers() — emits a single pipelineBarrier that transitions every
///      tracked resource to its required layout before vkCmdBeginRendering.
///   5. From GpuPayload::onSubmitComplete(), call flushStatesToResources() — writes the
///      pass-time layout/access/stages back to each resource's gpuStates so the next pass
///      can read the correct current state when building its own pre-pass barriers.
class RasterStateTrackerVulkan {
public:
    /// Returns false if a hazard was detected; the caller should abort the render pass entirely.
    bool addColorTarget(TextureVulkanBase * tex, const GpuResourceView & view);
    bool addDepthStencilTarget(TextureVulkanBase * tex, const GpuResourceView & view, bool readOnly = false);
    bool addSampledTexture(TextureVulkanBase * tex, const GpuResourceView & view,
                           vk::PipelineStageFlags stages = vk::PipelineStageFlagBits::eVertexShader | vk::PipelineStageFlagBits::eFragmentShader);
    bool addStorageTexture(TextureVulkanBase * tex, const GpuResourceView & view, vk::PipelineStageFlags stages = vk::PipelineStageFlagBits::eFragmentShader);

    bool addUniformBuffer(BufferVulkan *         buf,
                          vk::PipelineStageFlags stages = vk::PipelineStageFlagBits::eVertexShader | vk::PipelineStageFlagBits::eFragmentShader);
    bool addStorageBuffer(BufferVulkan * buf, bool write = false, vk::PipelineStageFlags stages = vk::PipelineStageFlagBits::eFragmentShader);
    bool addVertexBuffer(BufferVulkan * buf);
    bool addIndexBuffer(BufferVulkan * buf);

    /// Returns the IDs of every resource view that was rejected due to a hazard.
    /// An empty vector means all resources were registered successfully.
    std::vector<uint64_t> addGpuResourceTable(const GpuResourceTable & table);

    /// Returns false if any vertex/index buffer had a hazard; hazardous buffers are not registered.
    /// If false the caller should skip the entire draw (geometry cannot be partially bound).
    bool addRasterGeometry(const RasterGeometry & geom);
    /// Returns false if any render target texture has a hazard; the caller should abort the render pass.
    bool addRasterTarget(const RasterTarget & rt);

    /// Scans all tracked depth-stencil attachments that were registered as read-only and promotes
    /// them to read-write when \p drawState requires depth writes or stencil writes. Must be called
    /// once per draw (with its fully-merged state) before emitPrePassBarriers().
    ///
    /// Promotion is permanent for the whole pass: once upgraded, the attachment stays read-write.
    /// This is required because Vulkan prohibits image layout changes inside a dynamic render pass,
    /// so the most permissive layout needed by any draw must be used from the beginning.
    ///
    /// Color attachments are always in eColorAttachmentOptimal (which covers both read and write)
    /// and are never downgraded to read-only, so this method only examines depth-stencil entries.
    void upgradeForDrawRasterState(const RasterState & drawState);

    /// Returns the pass-time image layout for a tracked attachment, or eUndefined if not tracked.
    /// Valid after all add*() and upgradeForDrawRasterState() calls and before emitPrePassBarriers().
    vk::ImageLayout attachmentPassLayout(const TextureVulkanBase * tex) const;

    void emitPrePassBarriers(vk::CommandBuffer cb);
    void flushStatesToResources();

private:
    struct TrackedAttachment {
        TextureVulkanBase *               tex;
        GpuResourceView::SubresourceRange resolvedRange;
        vk::ImageLayout                   passLayout;
        vk::AccessFlags                   passAccess;
        vk::PipelineStageFlags            passStages;
        vk::ImageAspectFlags              aspect;
        bool                              isWrite;   ///< true if this usage writes to the resource
        const char *                      usageName; ///< for hazard error messages
    };

    static GpuResourceView::SubresourceRange resolveRange(const GpuResourceView & view, const Texture::Descriptor & d);
    static bool                              subresourceRangesOverlap(const GpuResourceView::SubresourceRange & a, const GpuResourceView::SubresourceRange & b);

    /// Checks \p incoming against all already-registered attachments for the same texture.
    /// Reports an error and returns false if a read/write or write/write hazard is found.
    bool checkHazard(const TrackedAttachment & incoming) const;

    /// Returns true if the attachment was registered without a hazard, false otherwise.
    bool addAttachment(TrackedAttachment a);

    // Keyed by tex->id; each texture may have multiple entries for distinct subresource ranges.
    std::unordered_map<uint64_t, std::vector<TrackedAttachment>> mAttachments;

    struct TrackedBuffer {
        BufferVulkan *         buf;
        vk::AccessFlags        passAccess;
        vk::PipelineStageFlags passStages;
        bool                   isWrite;
        const char *           usageName;
    };

    bool checkBufferHazard(const TrackedBuffer & incoming) const;
    bool addBuffer(TrackedBuffer b);

    // Keyed by buf->id; at most one entry per buffer (access flags merged on re-registration).
    std::unordered_map<uint64_t, TrackedBuffer> mBuffers;
};

} // namespace GN::gpu2
