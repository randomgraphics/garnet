#include "pch.h"
#include <atomic>

namespace GN {

GN_API uint64_t RuntimeType::getNextUniqueTypeId() {
    static std::atomic<uint64_t> nextId = 1;
    return nextId.fetch_add(1, std::memory_order_relaxed);
}

} // namespace GN
