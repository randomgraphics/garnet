#include "pch.h"
#include "vk-buffer-state.h"
#include "vk-persistent-buffer.h"
#include "vk-transient-buffer.h"

namespace GN::rdg {

static GN::Logger * sBufferStateLogger = GN::getLogger("GN.rdg.vk");

bool BufferStateTransition::transitTo(vk::AccessFlags newAccess, vk::PipelineStageFlags newStage, const StrA & resourceName) {
    if (curr.access == newAccess && curr.stage == newStage) return false;
    if (!resourceName.empty()) {
        GN_VERBOSE(sBufferStateLogger, "buffer '{}': access 0x{:x} -> 0x{:x}, stage 0x{:x} -> 0x{:x}", resourceName.c_str(), static_cast<uint32_t>(curr.access),
                   static_cast<uint32_t>(newAccess), static_cast<uint32_t>(curr.stage), static_cast<uint32_t>(newStage));
    }
    prev = curr;
    curr = {newAccess, newStage};
    return true;
}

void BufferStateTransition::assignFrom(const BufferStateTransition & other, const StrA & resourceName) {
    if (!resourceName.empty() && (curr.access != other.curr.access || curr.stage != other.curr.stage)) {
        GN_VERBOSE(sBufferStateLogger, "buffer '{}': state sync after submit (access 0x{:x} -> 0x{:x}, stage 0x{:x} -> 0x{:x})", resourceName.c_str(),
                   static_cast<uint32_t>(curr.access), static_cast<uint32_t>(other.curr.access), static_cast<uint32_t>(curr.stage),
                   static_cast<uint32_t>(other.curr.stage));
    }
    prev = other.curr;
    curr = other.curr;
}

BufferStateTransition * getBufferState(Buffer * buffer) {
    if (!buffer) return nullptr;
    if (auto * p = dynamic_cast<PersistentBufferVulkan *>(buffer)) return &p->state();
    if (auto * t = dynamic_cast<TransientBufferVulkan *>(buffer)) return &t->state();
    return nullptr;
}

const BufferStateTransition * getBufferState(const Buffer * buffer) {
    if (!buffer) return nullptr;
    if (auto * p = dynamic_cast<const PersistentBufferVulkan *>(buffer)) return &p->state();
    if (auto * t = dynamic_cast<const TransientBufferVulkan *>(buffer)) return &t->state();
    return nullptr;
}

void assignBufferState(Buffer * buffer, const BufferStateTransition & transition) {
    if (!buffer) return;
    if (auto * p = dynamic_cast<PersistentBufferVulkan *>(buffer))
        p->state().assignFrom(transition, buffer->name);
    else if (auto * t = dynamic_cast<TransientBufferVulkan *>(buffer))
        t->state().assignFrom(transition, buffer->name);
}

} // namespace GN::rdg
