#pragma once

#include "gpu-payload.h"
#include "vk-buffer-state.h"
#include "vk-gpu-image-state.h"
#include "vk-texture.h"

#include <optional>
#include <unordered_map>

namespace GN::gfx::gpu2 {

/// Vulkan-specific GPU payload.
struct GpuPayloadVulkan : GpuPayload {

    /// One texture referenced by the payload.
    /// Carries the full per-subresource image state at the start of the payload's work,
    /// plus an ordered list of state transitions applied during execution.
    struct TextureRef {
        AutoRef<TextureVulkanBase>          texture;
        TextureGpuImageState                stateBefore; ///< full per-subresource state snapshot before the payload runs
        std::optional<TextureGpuImageState> stateAfter;  ///< if empty, after state equals before state
    };

    /// One buffer referenced by the payload.
    struct BufferRef {
        AutoRef<Buffer>                  buffer;
        BufferStateVulkan                stateBefore;
        std::optional<BufferStateVulkan> stateAfter; ///< if empty, after state equals before state
    };

    vk::Semaphore                                       semaphore = {};
    vk::Fence                                           fence     = {};
    std::unordered_map<TextureVulkanBase *, TextureRef> textures;
    std::unordered_map<Buffer *, BufferRef>             buffers;

    struct RecordContext {
        const rv::Device * dev   = nullptr;
        rv::CommandQueue * queue = nullptr;
        rv::CommandBuffer  cmd;
    };

    virtual void recordForVulkanSubmit(const RecordContext &) {}
    virtual void onSubmitComplete() {}
};

} // namespace GN::gfx::gpu2
