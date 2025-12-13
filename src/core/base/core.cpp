#include "pch.h"
#include "garnet/GNinput.h"
#include <stdlib.h>

// GN::input::Input singletons
GN_API GN::input::Input * GN::input::Input::msInstancePtr = 0;

namespace GN {

static Logger * sHeapLogger() { return getLogger("GN.core.heapAllocation"); }

//
//
// -----------------------------------------------------------------------------
GN_API ProfilerManager & ProfilerManager::sGetGlobalInstance() {
    static ProfilerManager sInstance;
    return sInstance;
}

struct MemoryHeader {
    size_t alignment;
    size_t offset; // offset from the user visible memory to the actual memory allocated.
    size_t size;   // size of the actual memory allocated.
};

/// platform independant aligned allocation by allocating extra bytes in front of the the returned pointer address to
/// store extra informaton like the real heap address and alignment size and offsets.
// -----------------------------------------------------------------------------
GN_API void * HeapMemory::alignedAlloc(size_t sizeInBytes, size_t alignment) {
    if (0 == alignment) GN_UNLIKELY alignment = sizeof(size_t);

    if (0 == sizeInBytes) GN_UNLIKELY sizeInBytes = alignment;

    // Need to allocate at least (sizeof(header) + a - 1) bytes to make sure we can align the address returned to user
    // while still having enough space in front of user visible memory to store the header.
    size_t additional = sizeof(MemoryHeader) + alignment - 1;
    size_t totalSize  = additional + sizeInBytes;
    auto   p          = (uintptr_t)::malloc(totalSize);
    if (!p) {
        GN_ERROR(sHeapLogger())("out of memory!");
        return nullptr;
    }

    auto alignedAddress = math::nextMultiple(p + sizeof(MemoryHeader), alignment);
    GN_ASSERT(0 == (alignedAddress % alignment));               // double check the alignment.
    GN_ASSERT(alignedAddress >= (p + sizeof(MemoryHeader)));    // double check we have enough space for the header.
    GN_ASSERT((alignedAddress - p + sizeInBytes) <= totalSize); // double check we have enough space for the user visible memory.

    auto header       = (MemoryHeader *) alignedAddress - 1;
    header->alignment = alignment;
    header->offset    = alignedAddress - p;
    header->size      = totalSize;

    // done
    return (void *) alignedAddress;
}

//
//
// -----------------------------------------------------------------------------
GN_API void * HeapMemory::realloc(void * ptr, size_t sizeInBytes) {
    if (!ptr) return alignedAlloc(sizeInBytes, sizeof(size_t));

    // retrieve the original memory header.
    auto header    = (MemoryHeader *) ptr - 1;
    auto oldPtr    = (uintptr_t) ptr - header->offset;
    auto oldOffset = header->offset;
    auto oldSize   = header->size - oldOffset; // old user visible memory size.
    auto alignment = header->alignment;

    // validate input parameter range.
    if (0 == sizeInBytes) GN_UNLIKELY sizeInBytes = alignment;

    auto newTotalSize = sizeof(MemoryHeader) + alignment - 1 + sizeInBytes;
    auto newPtr       = (uintptr_t)::realloc((void *) oldPtr, newTotalSize);
    if (!newPtr) {
        GN_ERROR(sHeapLogger())("out of memory!");
        return nullptr;
    }

    auto newAlignedAddress = math::nextMultiple(newPtr + sizeof(MemoryHeader), alignment);
    GN_ASSERT(0 == (newAlignedAddress % alignment));                       // double check the alignment.
    GN_ASSERT(newAlignedAddress >= (newPtr + sizeof(MemoryHeader)));       // double check we have enough space for the header.
    GN_ASSERT((newAlignedAddress - newPtr + sizeInBytes) <= newTotalSize); // double check we have enough space for the user visible memory.

    auto newHeader       = (MemoryHeader *) newAlignedAddress - 1;
    newHeader->alignment = alignment;
    newHeader->offset    = newAlignedAddress - newPtr;
    newHeader->size      = newTotalSize;

    // if new offset is different from the old offset, we need to move the user visible memory to the new address.
    if (newHeader->offset != oldOffset) {
        auto dst        = (void *) newAlignedAddress;
        auto src        = (void *) (newPtr + oldOffset);
        auto sizeToMove = std::min(oldSize, sizeInBytes);
        memmove(dst, src, sizeToMove); // have to use memmove to handle the overlap.
    }

    // done
    return (void *) newAlignedAddress;
}

//
//
// -----------------------------------------------------------------------------
GN_API void HeapMemory::dealloc(void * ptr) {
    if (!ptr) GN_UNLIKELY return;
    auto header  = (MemoryHeader *) ptr - 1;
    auto realPtr = (uintptr_t) ptr - header->offset;
    ::free((void *) realPtr);
}

//
//
// -----------------------------------------------------------------------------
#if GN_MSVC
    #include <crtdbg.h>
GN_API void enableCRTMemoryCheck(long breakOnAllocID) {
    int tmpDbgFlag;
    tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
    _CrtSetDbgFlag(tmpDbgFlag);

    if (0 != breakOnAllocID) { _CrtSetBreakAlloc(breakOnAllocID); }
}
#else
GN_API void enableCRTMemoryCheck(long) {}
#endif

} // namespace GN
