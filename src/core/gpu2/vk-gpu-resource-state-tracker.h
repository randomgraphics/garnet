#pragma once

#include <garnet/GNgpu2.h>
#include "vk-buffer.h"
#include "vk-buffer-state.h"
#include "vk-texture.h"

#include <unordered_map>
#include <vector>

namespace GN::gpu2 {

/// Tracks Vulkan image/buffer state transitions across one or more raster passes in a batch.
///
/// Lifetime matches one GpuContextVulkan2::submit() call. Shared by all payloads via RecordContext.
///
/// Per-payload usage:
///   1. add*() — register this pass's render targets, shader resources, and geometry buffers.
///   2. emitPrePassBarriers() — emit a single pipelineBarrier. Advances the per-resource
///      "incoming" state to the post-barrier value and clear out registered state. So subsequent
///      payloads see the correct "from" layout without any extra bookkeeping.
///   3. (record the render pass)
///
/// After all payloads are recorded and vkQueueSubmit succeeds, call flushToResources() once to
/// write the batch's final resource states back to actual texture/buffer objects.
class GpuResourceStateTrackerVulkan {
public:
    /// Returns false if a hazard was detected; the caller should abort the render pass.
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
    bool addTransferSrcBuffer(BufferVulkan * buf);
    bool addTransferDstBuffer(BufferVulkan * buf);
    bool addTransferDstImage(TextureVulkanBase * tex, const GpuResourceView::ImageView & view);
    bool addTransferSrcImage(TextureVulkanBase * tex, const GpuResourceView::ImageView & view);

    std::vector<uint64_t> addGpuResourceTable(const GpuResourceTable & table);
    bool                  addRasterGeometry(const RasterGeometry & geom);
    bool                  addRasterTarget(const RasterTarget & rt);

    void upgradeForDrawRasterState(const RasterState & drawState);

    /// Returns the currently-tracked layout for a single subresource (mip, face) of a tracked
    /// texture by querying the incoming (post-barrier) state. For depth-stencil textures whose
    /// depth and stencil planes have diverged, combines them into the canonical Vulkan split layout.
    /// Returns eUndefined if the texture is not tracked or the subresource has no recorded state.
    /// Must be called after emitPrePassBarriers() — incoming is undefined before the first call.
    vk::ImageLayout texturePassLayout(const TextureVulkanBase * tex, uint32_t mip = 0, uint32_t face = 0) const;

    /// Emit pipeline barriers for all resources registered since the last call to this method.
    /// For each resource whose state changed, the barrier "from" side is the current incoming state
    /// and the "to" side is the registered intended state. After emitting, incoming is advanced to
    /// the post-barrier value and the "registered" states are cleared out — the next payload's
    /// add*() calls will have a fresh start that represents the updated baseline.
    void emitPrePassBarriers(vk::CommandBuffer cb);

    /// Write the batch's final resource states to actual texture/buffer objects.
    /// Call once after vkQueueSubmit succeeds. For images: writes all planes of the updated
    /// incoming state. For buffers: writes the committed access/stage accumulated this batch.
    void flushToResources();

    bool addTexture(TextureVulkanBase * tex, const GpuResourceView::ImageView & view, const rv::Image::State::PlaneState & state);

private:
    struct TrackedTexture {
        TextureVulkanBase * tex = nullptr;
        /// Running batch baseline. Initialized from tex->getState() on first registration.
        /// Updated in-place by emitPrePassBarriers() whenever a barrier is emitted for a plane,
        /// so subsequent payloads always see the correct "from" state without extra bookkeeping.
        rv::Image::State incoming;
        /// Per-pass intended states. Cleared by emitPrePassBarriers() between payloads.
        std::unordered_map<uint64_t, rv::Image::State::PlaneState> registered;
    };
    std::unordered_map<uint64_t, TrackedTexture> mTextures;

    struct TrackedBuffer {
        BufferVulkan * buf = nullptr;
        /// Running committed state (analogous to TrackedTexture::incoming for images).
        /// Initialized from buf->gpuState on first registration; updated by emitPrePassBarriers().
        vk::AccessFlags        committedAccess = {};
        vk::PipelineStageFlags committedStages = vk::PipelineStageFlagBits::eTopOfPipe;
        /// Per-pass intended access. Reset by emitPrePassBarriers() between payloads.
        bool                   activeThisPass = false; ///< true if registered in the current pass
        vk::AccessFlags        passAccess     = {};
        vk::PipelineStageFlags passStages     = vk::PipelineStageFlagBits::eBottomOfPipe;
        bool                   isWrite        = false;
        const char *           usageName      = "<unspecified>";
    };
    bool checkBufferHazard(const TrackedBuffer & incoming) const;
    bool addBuffer(TrackedBuffer b);

    std::unordered_map<uint64_t, TrackedBuffer> mBuffers;
};

} // namespace GN::gpu2
