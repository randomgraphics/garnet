#pragma once

#include <garnet/GNgpu2.h>
#include "vk-gpu-context.h"

namespace GN::gpu2 {

/// Tracks the latest known Vulkan image state per subresource (mip × array layer).
struct TextureGpuImageState {
    struct ImageState {
        vk::ImageLayout        layout = vk::ImageLayout::eUndefined;
        vk::AccessFlags        access = vk::AccessFlagBits::eNone;
        vk::PipelineStageFlags stages = vk::PipelineStageFlagBits::eBottomOfPipe;

        bool operator==(const ImageState & o) const { return layout == o.layout && access == o.access && stages == o.stages; }
        bool operator!=(const ImageState & o) const { return !(*this == o); }

        static constexpr ImageState UNDEFINED() { return {vk::ImageLayout::eUndefined, vk::AccessFlagBits::eNone, vk::PipelineStageFlagBits::eBottomOfPipe}; }

        static constexpr ImageState TRANSFER_DST() {
            return {vk::ImageLayout::eTransferDstOptimal, vk::AccessFlagBits::eTransferWrite, vk::PipelineStageFlagBits::eTransfer};
        }
    };

    TextureGpuImageState() = default;

    /// Reallocates and seeds each subresource with \p initialState. Clears when either dimension is zero.
    void reset(uint32_t mipLevels, uint32_t arrayLayers, const ImageState & initialState = ImageState::UNDEFINED());

    /// Returns the current state of the given subresource, or null if out of range.
    const ImageState * get(uint32_t mip, uint32_t arrayLayer) const;

    /// Set the state for all subresources in \p range. Returns true if any subresource changed.
    bool set(const GpuResourceView::SubresourceRange & range, const ImageState & newState, const StrA & resourceName = {});

    uint32_t mipLevels() const { return mNumMips; }
    uint32_t arrayLayers() const { return mNumLayers; }

private:
    size_t subResourceIndex(uint32_t mip, uint32_t arrayLayer) const { return (size_t) mip * mNumLayers + arrayLayer; }

    uint32_t              mNumMips   = 0;
    uint32_t              mNumLayers = 0;
    DynaArray<ImageState> mSubresourceStates;
};

} // namespace GN::gpu2
