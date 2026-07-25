#include "pch.h"
#include <atomic>

namespace GN {

GN_API uint64_t RuntimeType::getNextUniqueTypeId() {
    static std::atomic<uint64_t> nextId = 1;
    return nextId.fetch_add(1, std::memory_order_relaxed);
}

/// Process-wide monotonic id generator for RCRT64. The first id is 1; 0 is never assigned.
static int64_t nextRcrt64Id() {
    static std::atomic<int64_t> nextId = 1;
    return nextId.fetch_add(1, std::memory_order_relaxed);
}

/// Constructs an RCRT64 with an automatically generated id. Defined here (and exported via
/// GN_API on the declaration) so the id counter lives in exactly one place in the process,
/// keeping generated ids unique even across DLL boundaries.
GN_API RCRT64::RCRT64(const RuntimeType::TypeInfo & type, const StrA & name_): RefCountedRuntimeType(type, nextRcrt64Id(), name_) {}

} // namespace GN
