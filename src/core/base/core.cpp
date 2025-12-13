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
// a helper function to dump memory content for debug purpose.
// -----------------------------------------------------------------------------
[[maybe_unused]] static void dumpMemoryContent(const char * name, uintptr_t addr, size_t size) {
    printf("Memory content of %s at address 0x%p, size %zu bytes:\n", name, (void *) addr, size);
    for (size_t i = 0; i < size; ++i) {
        printf("%02X ", ((unsigned char *) addr)[i]);
        // insert \n every 16 bytes.
        if ((i + 1) % 16 == 0) { printf("\n"); }
    }
    printf("\n");
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

    // dumpMemoryContent("before realloc", oldPtr, header->size);
    // printf("before realloc: offset = %zu, total size = %zu, alignment: %zu\n", oldOffset, header->size, alignment);

    // validate input parameter range.
    if (0 == sizeInBytes) GN_UNLIKELY sizeInBytes = alignment;

    // reallocate the memory block.
    auto newTotalSize = sizeof(MemoryHeader) + alignment - 1 + sizeInBytes;
    auto newPtr       = (uintptr_t)::realloc((void *) oldPtr, newTotalSize);
    if (!newPtr) {
        GN_ERROR(sHeapLogger())("out of memory!");
        return nullptr;
    }

    // calculate the new memory properties.
    auto newAlignedAddress = math::nextMultiple(newPtr + sizeof(MemoryHeader), alignment);
    GN_ASSERT(0 == (newAlignedAddress % alignment));                       // double check the alignment.
    GN_ASSERT(newAlignedAddress >= (newPtr + sizeof(MemoryHeader)));       // double check we have enough space for the header.
    GN_ASSERT((newAlignedAddress - newPtr + sizeInBytes) <= newTotalSize); // double check we have enough space for the user visible memory.
    auto newOffset = newAlignedAddress - newPtr;

    // dumpMemoryContent("after realloc", newPtr, newTotalSize);
    // printf("after realloc: offset = %zu, total size = %zu, alignment: %zu\n", newOffset, newTotalSize,
    //        alignment);

    // if new offset is different from the old offset, we need to move the user visible memory to the new address.
    if (newOffset != oldOffset) {
        auto dst        = (void *) newAlignedAddress;
        auto src        = (void *) (newPtr + oldOffset);
        auto sizeToMove = std::min(oldSize, sizeInBytes);
        memmove(dst, src, sizeToMove); // have to use memmove to handle the overlap.
    }

    // Update the header info. Must do this after we move the memory content to avoid accidental overwrite the user data.
    auto newHeader       = (MemoryHeader *) newAlignedAddress - 1;
    newHeader->alignment = alignment;
    newHeader->offset    = newOffset;
    newHeader->size      = newTotalSize;

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
