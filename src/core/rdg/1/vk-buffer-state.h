#pragma once

#include "vk-gpu-context.h"

namespace GN::rdg {

struct Buffer;

/// Access and stage for a buffer (no layout; buffers don't have image layout).
struct BufferState {
    vk::AccessFlags        access = vk::AccessFlagBits::eNone;
    vk::PipelineStageFlags stage  = vk::PipelineStageFlagBits::eBottomOfPipe;

    bool operator==(const BufferState & other) const { return access == other.access && stage == other.stage; }
    bool operator!=(const BufferState & other) const { return !operator==(other); }

    static constexpr BufferState NONE() { return {vk::AccessFlagBits::eNone, vk::PipelineStageFlagBits::eBottomOfPipe}; }
};

/// Prev/curr state for barrier recording and flushing.
struct BufferStateTransition {
    BufferState prev = BufferState::NONE();
    BufferState curr = BufferState::NONE();

    bool transitTo(vk::AccessFlags newAccess, vk::PipelineStageFlags newStage, const StrA & resourceName = {});
    void assignFrom(const BufferStateTransition & other, const StrA & resourceName = {});
};

/// Get the buffer state transition for a Vulkan buffer; null if \p buffer is not a Vulkan buffer.
BufferStateTransition *       getBufferState(Buffer * buffer);
const BufferStateTransition * getBufferState(const Buffer * buffer);

/// Flush transition to buffer resource (no-op if not a Vulkan buffer).
void assignBufferState(Buffer * buffer, const BufferStateTransition & transition);

} // namespace GN::rdg
