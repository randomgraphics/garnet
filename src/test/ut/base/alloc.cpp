#include "../testCommon.h"

class ReallocTest : public CxxTest::TestSuite {
public:
    void testReallocNullPointerAllocates() {
        size_t size = 128;
        void * ptr  = GN::HeapMemory::realloc(nullptr, size);
        TS_ASSERT(ptr != nullptr);
        GN::HeapMemory::dealloc(ptr);
    }

    void testReallocSimpleSmoke() {
        size_t size = 128;
        char * ptr  = (char *) GN::HeapMemory::alignedAlloc(size, 32);
        TS_ASSERT(ptr != nullptr);
        if (!ptr) return;

        // fill the first 10 bytes of the memory with some data.
        for (size_t i = 0; i < 10; ++i) ptr[i] = (char) (42 + i);

        char * newPtr = (char *) GN::HeapMemory::realloc(ptr, size * 2);
        TS_ASSERT(newPtr != nullptr);

        // make sure the new pointer is aligned to the original alignment.
        TS_ASSERT_EQUALS(0, (uintptr_t(newPtr) % 32));

        // make sure the old content is preserved.
        for (size_t i = 0; i < 10; ++i) TS_ASSERT_EQUALS(newPtr[i], (char) (42 + i));

        GN::HeapMemory::dealloc(newPtr);
    }

    void testReallocExpandsMemoryAndPreservesContentAndAlignment() {
        size_t origSize = 8;
        char * ptr      = (char *) GN::HeapMemory::alignedAlloc(origSize, 32); // aligned to 32 bytes.
        TS_ASSERT(ptr != nullptr);
        if (!ptr) return;

        // fill the memory with some data.
        for (size_t i = 0; i < origSize; ++i) ptr[i] = (char) (42 + i);

        // keep increasing the size until the memory is reallocated to a new address.
        size_t newSize = origSize;
        char * newPtr  = nullptr;
        for (;;) {
            newSize *= 2;
            newPtr = (char *) GN::HeapMemory::realloc(ptr, newSize);
            TS_ASSERT(newPtr != nullptr);
            if (ptr != newPtr) break;
        }
        TS_ASSERT(newPtr != nullptr);

        if (newPtr) {
            // Old content should be the same
            for (size_t i = 0; i < origSize; ++i) { TS_ASSERT_EQUALS(newPtr[i], (char) (42 + i)); }

            // the new pointer should be aligned to the original alignment.
            TS_ASSERT_EQUALS(0, (uintptr_t(newPtr) % 32));

            // done. free the memory.
            GN::HeapMemory::dealloc(newPtr);
        } else {
            // failed to reallocate the memory. free the original memory.
            GN::HeapMemory::dealloc(ptr);
        }
    }
};
