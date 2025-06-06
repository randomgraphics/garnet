#include "pch.h"
#include <atomic>

namespace GN {
namespace detail {

#if GN_BUILD_DEBUG_ENABLED
    std::atomic<size_t> sPayloadInstanceCount {0};
#endif

// class free list of payload connected in single list via the payloed's next pointer.
struct PayLoadFreeList {
    std::atomic<AutoPtrPayload*> mHead {nullptr};

    ~PayLoadFreeList() {
        AutoPtrPayload* p = mHead.exchange(nullptr);
        while (p) {
            AutoPtrPayload* next = static_cast<AutoPtrPayload*>(p->next);
            delete p;
            p = next;
        }
#if GN_BUILD_DEBUG_ENABLED
        sPayloadInstanceCount.store(0);
#endif
    }

    AutoPtrPayload* allocate() {
        // Try to get from free list first
        AutoPtrPayload* current = mHead.load(std::memory_order_acquire);
        while (current) {
            AutoPtrPayload* next = static_cast<AutoPtrPayload*>(current->next);
            if (mHead.compare_exchange_weak(current, next, std::memory_order_acq_rel)) {
                // Successfully got a node from free list
                current->next = nullptr;
                current->ptr = nullptr;
                current->counter = 1;
                return current;
            }
        }
        // No free nodes available, create new one
#if GN_BUILD_DEBUG_ENABLED
        sPayloadInstanceCount.fetch_add(1, std::memory_order_relaxed);
#endif
        return new AutoPtrPayload();
    }

    void recycle(AutoPtrPayload* p) {
        if (!p) return;
        
        AutoPtrPayload* current = mHead.load(std::memory_order_acquire);
        do {
            p->next = current;
        } while (!mHead.compare_exchange_weak(current, p, std::memory_order_acq_rel));
    }
};

static PayLoadFreeList& getFreeList() {
    static PayLoadFreeList sFreeList;
    return sFreeList;
}

AutoPtrPayload * AutoPtrPayload::allocate() { return getFreeList().allocate(); }

void AutoPtrPayload::free(AutoPtrPayload * p) { return getFreeList().recycle(p); }

} // namespace detail
} // namespace GN 