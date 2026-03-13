#include "pch.h"
#include <garnet/rdg/rtti.h>

#include <atomic>
#include <mutex>
#include <string>
#include <unordered_map>

namespace GN::rdg {

GN_API uint64_t RuntimeType::getNextUniqueTypeId() {
    static std::atomic<uint64_t> nextId = 1;
    return nextId.fetch_add(1, std::memory_order_relaxed);
}

} // namespace GN::rdg
