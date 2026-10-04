#pragma once

#include <garnet/GNgpu2.h>
#include "vk-buffer.h"
#include "vk-buffer-state.h"
#include "vk-texture.h"

#include <unordered_map>
#include <vector>

namespace GN::gpu2 {

/// Access and layout state of a single texture plane (aspect of a mip/face subresource).
struct TexturePlaneStateVulkan {
    vk::ImageLayout        layout = vk::ImageLayout::eUndefined;
    vk::AccessFlags        access = {};
    vk::PipelineStageFlags stages = vk::PipelineStageFlagBits::eTopOfPipe;
    const char *           usage  = nullptr;

    bool isWrite() const {
        return bool(access & (vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite | vk::AccessFlagBits::eShaderWrite |
                              vk::AccessFlagBits::eTransferWrite | vk::AccessFlagBits::eHostWrite));
    }

    bool operator==(const TexturePlaneStateVulkan & o) const { return layout == o.layout && access == o.access && stages == o.stages; }
    bool operator!=(const TexturePlaneStateVulkan & o) const { return !(*this == o); }

    static inline TexturePlaneStateVulkan SHADER_READ_ONLY() {
        return {
            vk::ImageLayout::eShaderReadOnlyOptimal,
            vk::AccessFlagBits::eShaderRead,
            vk::PipelineStageFlagBits::eFragmentShader | vk::PipelineStageFlagBits::eComputeShader | vk::PipelineStageFlagBits::eVertexShader,
            "shader read-only",
        };
    }

    static inline TexturePlaneStateVulkan UNDEFINED() {
        return {
            vk::ImageLayout::eUndefined,
            {},
            vk::PipelineStageFlagBits::eTopOfPipe,
            "undefined",
        };
    }
};

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
class GpuResourceStateTrackerVulkan {
public:
    static inline uint64_t packPlaneKey(uint32_t mip, uint32_t face, vk::ImageAspectFlagBits aspect) {
        return (uint64_t(mip) << 48) | (uint64_t(face) << 16) | uint64_t(uint32_t(aspect));
    }

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

    std::vector<int64_t> addGpuResourceTable(const GpuResourceTable & table);
    bool                 addRasterGeometry(const RasterGeometry & geom);
    bool                 addRasterTarget(const RasterTarget & rt);

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

    /// Advance a texture's tracked incoming state and emit a barrier restoring it to SHADER_READ_ONLY_OPTIMAL.
    /// Used by bindless passes to satisfy the "writer restores to read-ready" invariant.
    void restoreAttachmentToShaderReadOnly(TextureVulkanBase * tex, const GpuResourceView & view, vk::CommandBuffer cb);

    /// Advance a buffer's tracked state and emit a barrier restoring it to read-ready access
    /// (shader read, uniform read, vertex/index attribute read, indirect read, transfer read).
    /// Used by passes to satisfy the universal "writer restores to read-ready" invariant.
    void restoreBufferToReadReady(BufferVulkan * buf, vk::CommandBuffer cb);

    /// Batched variant: restores multiple buffers to read-ready state in a single pipeline barrier.
    void restoreBuffersToReadReady(ArrayView<BufferVulkan * const> bufs, vk::CommandBuffer cb);

    bool addTexture(TextureVulkanBase * tex, const GpuResourceView::ImageView & view, const TexturePlaneStateVulkan & state);

private:
    struct TrackedTexture {
        bool                 activeThisPass = false;
        TextureVulkanBase *  tex            = nullptr;
        uint32_t             numMips        = 0;
        uint32_t             numLayers      = 0;
        vk::ImageAspectFlags validAspects   = {};
        /// Running batch baseline per plane key. Updated in-place by emitPrePassBarriers()
        /// and restoreAttachmentToShaderReadOnly().
        std::unordered_map<uint64_t, TexturePlaneStateVulkan> incoming;
        /// Per-pass intended states. Cleared by emitPrePassBarriers() between payloads.
        std::unordered_map<uint64_t, TexturePlaneStateVulkan> registered;
        bool                                                  hasWrite = false;

        const TexturePlaneStateVulkan * getIncoming(uint32_t mip, uint32_t face, vk::ImageAspectFlagBits aspect) const {
            auto it = incoming.find(packPlaneKey(mip, face, aspect));
            return it != incoming.end() ? &it->second : nullptr;
        }
        void setIncoming(uint32_t mip, uint32_t face, vk::ImageAspectFlagBits aspect, const TexturePlaneStateVulkan & s) {
            incoming[packPlaneKey(mip, face, aspect)] = s;
        }
    };
    std::unordered_map<int64_t, TrackedTexture> mTextures;

    struct TrackedBuffer {
        BufferVulkan * buf = nullptr;
        /// Running committed state across the batch. Initialized to BufferStateVulkan::READ_READY()
        /// on first registration; updated by emitPrePassBarriers() and restoreBuffersToReadReady().
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

    std::unordered_map<int64_t, TrackedBuffer> mBuffers;
    // unordered_map rehashing preserves element addresses; these lists only visit registrations for the current pass.
    std::vector<TrackedBuffer *>  mActiveBuffers;
    std::vector<TrackedTexture *> mActiveTextures;
    bool                          mHasReadOnlyDepthStencil = false;
};

} // namespace GN::gpu2
