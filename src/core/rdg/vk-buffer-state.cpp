#include "pch.h"
#include "vk-buffer-state.h"
#include "vk-persistent-buffer.h"
#include "vk-transient-buffer.h"
#include <garnet/rdg/artifacts.h>

namespace GN::rdg {

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
        p->assignFrom(transition);
    else if (auto * t = dynamic_cast<TransientBufferVulkan *>(buffer))
        t->assignFrom(transition);
}

} // namespace GN::rdg
