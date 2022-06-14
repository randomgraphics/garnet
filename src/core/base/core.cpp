#include "pch.h"
#include "garnet/GNinput.h"
#include <stdlib.h>

// GN::input::Input singletons
GN_API GN::input::Input * GN::input::Input::msInstancePtr = 0;

namespace GN {
static Logger * sHeapLogger = getLogger("GN.core.heapAllocation");

//
//
// -----------------------------------------------------------------------------
GN_API ProfilerManager & ProfilerManager::sGetGlobalInstance() {
    static ProfilerManager sInstance;
    return sInstance;
}

//
//
// -----------------------------------------------------------------------------
GN_API void * HeapMemory::alloc(size_t sz) { return HeapMemory::alignedAlloc(sz, 0); }

//
//
// -----------------------------------------------------------------------------
GN_API void * HeapMemory::realloc(void * ptr, size_t sz) { return HeapMemory::alignedRealloc(ptr, sz, 0); }

//
//
// -----------------------------------------------------------------------------
GN_API void * HeapMemory::alignedAlloc(size_t sizeInBytes, size_t alignment) {
    if (0 == alignment) alignment = sizeof(size_t);
#if GN_DARWIN
    void * ptr;
    if (posix_memalign(&ptr, sizeInBytes, alignment)) ptr = nullptr;
#elif GN_POSIX
    void * ptr;
    if (1 == alignment)
        ptr = malloc(sizeInBytes);
    else
        ptr = aligned_alloc(alignment, sizeInBytes);
#else
    void * ptr = _aligned_malloc(sizeInBytes, alignment);
#endif
    if (0 == ptr) { GN_ERROR(sHeapLogger)("out of memory!"); }
    return ptr;
}

//
//
// -----------------------------------------------------------------------------
GN_API void * HeapMemory::alignedRealloc(void * ptr, size_t sizeInBytes, size_t alignment) {
    if (0 == alignment) alignment = sizeof(size_t);
#if GN_POSIX
    ptr = realloc(ptr, sizeInBytes);
#else
    ptr = _aligned_realloc(ptr, sizeInBytes, alignment);
    if (0 == ptr) { GN_ERROR(sHeapLogger)("out of memory!"); }
#endif
    return ptr;
}

//
//
// -----------------------------------------------------------------------------
GN_API void HeapMemory::dealloc(void * ptr) {
#if GN_POSIX
    return ::free(ptr);
#else
    return _aligned_free(ptr);
#endif
}
} // namespace GN
