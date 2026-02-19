#pragma once
#include <cstdint>
#include <atomic>

namespace GN::rdg {

inline uint64_t getNextUniqueTypeId() {
    static std::atomic<uint64_t> nextId = 1;
    return nextId.fetch_add(1, std::memory_order_relaxed);
}

} // namespace GN::rdg