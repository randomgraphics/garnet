#pragma once

#include "vk-gpu-context.h"

namespace GN::gpu2 {

/// Tracks the Vulkan access state of an entire buffer. No offset/size: the whole buffer
/// is assumed to be in a uniform access state at all times. Queue family ownership is
/// not tracked here; it is resolved at submit time when the target queue is known.
struct BufferStateVulkan {
    vk::AccessFlags        access = vk::AccessFlagBits::eNone;
    vk::PipelineStageFlags stages = vk::PipelineStageFlagBits::eTopOfPipe;

    bool operator==(const BufferStateVulkan & o) const { return access == o.access && stages == o.stages; }
    bool operator!=(const BufferStateVulkan & o) const { return !(*this == o); }

    static constexpr BufferStateVulkan UNDEFINED() { return {vk::AccessFlagBits::eNone, vk::PipelineStageFlagBits::eTopOfPipe}; }
};

} // namespace GN::gpu2
