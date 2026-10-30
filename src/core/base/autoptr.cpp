#include "pch.h"
#include <atomic>
#include <mutex>

namespace GN {
namespace detail {

#if GN_BUILD_DEBUG_ENABLED
std::atomic<size_t> sPayloadInstanceCount {0};
#endif

// class free list of payload connected in single list via the payloed's next pointer.
struct PayLoadFreeList {
    std::mutex       mMutex;
    AutoPtrPayload * mHead {nullptr};

    ~PayLoadFreeList() {
        // All payload users must stop before this function-local static is destroyed.
        AutoPtrPayload * p;
        {
            std::lock_guard<std::mutex> lock(mMutex);
            p     = mHead;
            mHead = nullptr;
        }
        while (p) {
            AutoPtrPayload * next = static_cast<AutoPtrPayload *>(p->next);
            delete p;
            p = next;
        }
#if GN_BUILD_DEBUG_ENABLED
        sPayloadInstanceCount.store(0);
#endif
    }

    AutoPtrPayload * allocate() {
        AutoPtrPayload * current;
        {
            // Protect both links and node reuse; a pointer-only CAS permits ABA.
            std::lock_guard<std::mutex> lock(mMutex);
            current = mHead;
            if (current) mHead = static_cast<AutoPtrPayload *>(current->next);
        }
        if (current) {
            current->next    = nullptr;
            current->ptr     = nullptr;
            current->counter = 1;
            return current;
        }
        // No free nodes available, create new one
#if GN_BUILD_DEBUG_ENABLED
        sPayloadInstanceCount.fetch_add(1, std::memory_order_relaxed);
#endif
        return new AutoPtrPayload();
    }

    void recycle(AutoPtrPayload * p) {
        if (!p) return;

        std::lock_guard<std::mutex> lock(mMutex);
        p->next = mHead;
        mHead   = p;
    }
};

static PayLoadFreeList & getFreeList() {
    static PayLoadFreeList sFreeList;
    return sFreeList;
}

AutoPtrPayload * AutoPtrPayload::allocate() { return getFreeList().allocate(); }

void AutoPtrPayload::free(AutoPtrPayload * p) { return getFreeList().recycle(p); }

} // namespace detail
} // namespace GN