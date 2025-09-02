#ifndef __GN_BASE_MEMORY_H__
#define __GN_BASE_MEMORY_H__
// *****************************************************************************
/// \file
/// \brief   Basic memory management routines
/// \author  chenlee (2005.7.23)
// *****************************************************************************

#include "basicDefines.h"
#include <new>

/// \name macro to exception throw
#define GN_THROW_BADALLOC() // throw(std::bad_alloc)
#define GN_NOTHROW()        throw()

// *****************************************************************************
// Basic memory allocation/deallocation routines
// *****************************************************************************

namespace GN {
namespace HeapMemory {
///
/// Allocate memory from heap. Can cross DLL boundary.
///
GN_API void * alloc(size_t sizeInBytes);

///
/// Re-allocate memory from heap. Can cross DLL boundary.
///
GN_API void * realloc(void * ptr, size_t sizeInBytes);

///
/// Allocate aligned memory from heap. Can cross DLL boundary
///
GN_API void * alignedAlloc(size_t sizeInBytes, size_t alignment);

///
/// Re-allocate aligned memory from heap. Can cross DLL boundary
///
GN_API void * alignedRealloc(void * ptr, size_t sizeInBytes, size_t alignment);

///
/// Free heap-allocated memory (aligned or unaligned). Can cross DLL boundary.
///
GN_API void dealloc(void * ptr);
} // namespace HeapMemory
} // namespace GN

// *****************************************************************************
// Global new/delete operators
// *****************************************************************************

#if GN_ICL
    #pragma warning(push)
    #pragma warning(disable : 522) // inline function is declared after being used.
#endif

// #if GN_CLANG
// /// \name overloaded global new and delete operators
// //@{
// void * operator new(size_t s) GN_THROW_BADALLOC() { return ::GN::HeapMemory::alloc(s); }
// void * operator new[](size_t s) GN_THROW_BADALLOC() { return ::GN::HeapMemory::alloc(s); }
// void   operator delete(void * p) GN_NOTHROW() { ::GN::HeapMemory::dealloc(p); }
// void   operator delete[](void * p) GN_NOTHROW() { ::GN::HeapMemory::dealloc(p); }
// //@}
// #endif

#if GN_ICL
    #pragma warning(pop)
#endif

// *****************************************************************************
// Other common memory management utilities
// *****************************************************************************

namespace GN {
///
/// enable CRT memory leak checking. Currently only work for MSVC compiler
///
/// \param breakOnAllocID       Set allocation ID for "break-on-memory-allocation". Set to 0 to disable it.
GN_API void enableCRTMemoryCheck(long breakOnAllocID = 0);

///
/// free heap memory pointer allocated using GN::HeapMemory functions
// ------------------------------------------------------------------------
template<typename T>
GN_FORCE_INLINE void safeHeapDealloc(T *& ptr) {
    if (ptr) {
        HeapMemory::dealloc(ptr);
        ptr = 0;
    }
}

///
/// delete one object
// ------------------------------------------------------------------------
template<typename T>
GN_FORCE_INLINE void safeDelete(T *& ptr) {
    if (ptr) {
        delete ptr;
        ptr = 0;
    }
}

///
/// delete object array
// ------------------------------------------------------------------------
template<typename T>
GN_FORCE_INLINE void safeDeleteArray(T *& ptr) {
    if (ptr) {
        delete[] ptr;
        ptr = 0;
    }
}

///
/// Safe release COM interface
///
template<typename T>
GN_FORCE_INLINE void safeRelease(T *& ptr) {
    if (ptr) {
        ptr->Release();
        ptr = 0;
    }
}

///
/// Safe release RefCounter class
///
template<typename T>
GN_FORCE_INLINE void safeDecref(T *& ptr) {
    if (ptr) {
        ptr->decref();
        ptr = 0;
    }
}

template<size_t ITEM_SIZE>
struct DefaultMemoryAlignment {
    static const size_t VALUE = ((0 == (ITEM_SIZE % 16)) ? 16 : ((0 == (ITEM_SIZE % 8)) ? 8 : ((0 == (ITEM_SIZE % 4)) ? 4 : ((0 == (ITEM_SIZE % 2)) ? 2 : 1))));
};

///
/// Raw heap memory Allocator, no constructors and destructors involved.
///
struct RawHeapMemoryAllocator {
    /// Allocate raw memory from heap
    static inline void * sAllocate(size_t sizeInBytes, size_t alignmentInBytes) { return HeapMemory::alignedAlloc(sizeInBytes, alignmentInBytes); }

    /// Deallocate raw memory buffer.
    static inline void sDeallocate(void * ptr) { HeapMemory::dealloc(ptr); }
};

///
/// C++ object allocator built on top of a raw memory allocator
///
template<typename T, typename RAW_MEMORY_ALLOCATOR = RawHeapMemoryAllocator>
struct CxxObjectAllocator {
    /// Allocate raw memory from heap. No calling constructors
    static inline T * sAllocate(size_t objectCount, size_t alignmentInBytes = DefaultMemoryAlignment<sizeof(T)>::VALUE) {
        return (T *) RAW_MEMORY_ALLOCATOR::sAllocate(objectCount * sizeof(T), alignmentInBytes);
    }

    /// Deallocate memory buffer. No calling destructors.
    static inline void sDeallocate(void * ptr) { RAW_MEMORY_ALLOCATOR::sDeallocate(ptr); }

    /// Inplace construct a single object
    static inline void sConstruct(T * ptr) {
        if (ptr) GN_LIKELY {
                new (ptr) T;
            }
    }

    /// Inplace copy construct a single object.
    static inline void sConstruct(T * ptr, const T & x) {
        if (ptr) GN_LIKELY {
                new (ptr) T(x);
            }
    }

    /// Inplace move construct a single object.
    static inline void sConstruct(T * ptr, T && x) {
        if (ptr) GN_LIKELY {
                new (ptr) T(std::move(x));
            }
    }

    /// Inplace destruct a single object. No memory freeing.
    static inline void sDestruct(T * ptr) {
        if constexpr (!std::is_pod<T>()) {
            if (ptr) GN_LIKELY ptr->T::~T();
        } else {
            // do nothing to POD type.
            (void) ptr;
        }
    }
};

///
/// Fix-sized raw memory pool, no ctor/dtor involved.
///
template<size_t ITEM_SIZE, size_t ALIGNMENT = DefaultMemoryAlignment<ITEM_SIZE>::VALUE, size_t INITIAL_ITEMS_PER_BLOCK = 32, size_t MAX_ITEMS = 0>
class FixSizedRawMemoryPool : public NoCopy {
    // alignment must be 2^N and can not be zero
    GN_CASSERT((0 == ((ALIGNMENT - 1) & ALIGNMENT)) && (ALIGNMENT > 0));

    template<size_t N, size_t A>
    struct Alignment {
        static const size_t VALUE = (N + (A - 1)) & ~(A - 1);
    };

    static const size_t ALIGNED_ITEM_SIZE = Alignment<Alignment<ITEM_SIZE, sizeof(void *)>::VALUE + sizeof(void *) * 3, ALIGNMENT>::VALUE;

    union Item {
        uint8_t raw[ALIGNED_ITEM_SIZE];
        struct {
            uint8_t data[ITEM_SIZE];
            void *  pool; ///< pointer to the pool
            Item *  prev; ///< points to previous item
            Item *  next; ///< points to next item
        };
    };
    GN_CASSERT(sizeof(Item) == ALIGNED_ITEM_SIZE);

    struct Block {
        size_t  count; ///< number of items in item array
        Item *  items; ///< item array
        Block * next;  ///< points to next block
    };

    Block * mBlocks;
    Item *  mItems;
    Item *  mFreeItems;
    size_t  mItemCount;
    size_t  mNewBlockSize;

public:
    ///
    /// Default ctor.
    ///
    FixSizedRawMemoryPool(): mBlocks(0), mItems(0), mFreeItems(0), mItemCount(0), mNewBlockSize(INITIAL_ITEMS_PER_BLOCK) {}

    ///
    /// Default ctor.
    ///
    ~FixSizedRawMemoryPool() { freeAll(); }

    ///
    /// make sure a valid pointer belongs to this pool
    ///
    bool check(const void * p) const {
        if (0 == p) return false;

        Item * i = (Item *) p;

        return i->pool == this;
    }

    ///
    /// Allocate raw memory for one item
    ///
    void * alloc() {
        if (MAX_ITEMS > 0 && mItemCount == MAX_ITEMS) {
            GN_ERROR(getLogger("FixSizedRawMemoryPool"))("out of pool memory!");
            return 0;
        }

        if (0 == mFreeItems) {
            // no free items. create new block
            Block * b = (Block *) HeapMemory::alloc(sizeof(Block));
            if (0 == b) {
                GN_ERROR(getLogger("FixSizedRawMemoryPool"))("out of heap memory!");
                return 0;
            }
            b->items = (Item *) HeapMemory::alignedAlloc(sizeof(Item) * mNewBlockSize, ALIGNMENT);
            if (0 == b->items) {
                GN_ERROR(getLogger("FixSizedRawMemoryPool"))("out of heap memory!");
                HeapMemory::dealloc(b);
                return 0;
            }
            b->count = mNewBlockSize;
            mNewBlockSize *= 2; // size of next block is doubled.

            // build free list
            for (size_t i = 0; i < b->count; ++i) {
                b->items[i].pool = this;
                b->items[i].next = mFreeItems;
                mFreeItems       = &b->items[i];
            }

            // add to block list
            b->next = mBlocks;
            mBlocks = b;
        }

        // get one from free list.
        Item * p   = mFreeItems;
        mFreeItems = mFreeItems->next;

        // insert to item list
        p->prev = 0;
        p->next = mItems;
        if (mItems) mItems->prev = p;
        mItems = p;

        ++mItemCount;

        return p;
    }

    ///
    /// Deallocate
    ///
    void dealloc(void * p) {
        if (0 == p) return;

        Item * i = (Item *) p;
        if (!check(p)) {
            GN_ERROR(getLogger("FixSizedRawMemoryPool"))("invalid pointer!");
            return;
        }

        if (0 == mItemCount) {
            GN_ERROR(getLogger("FixSizedRawMemoryPool"))("input pointer is not belong to this pool!");
            return;
        }

        --mItemCount;

        // remove from item list
        if (i->prev) i->prev->next = i->next;
        if (i->next) i->next->prev = i->prev;
        if (i == mItems) mItems = i->next;

        // add to free list
        i->next    = mFreeItems;
        mFreeItems = i;
    }

    ///
    /// free all items
    ///
    void freeAll() {
        Block * p;
        while (mBlocks) {
            p       = mBlocks;
            mBlocks = mBlocks->next;
            HeapMemory::dealloc(p->items);
            HeapMemory::dealloc(p);
        }
        mBlocks       = 0;
        mItems        = 0;
        mFreeItems    = 0;
        mItemCount    = 0;
        mNewBlockSize = INITIAL_ITEMS_PER_BLOCK;
    }

    ///
    /// get first item in allocator
    ///
    void * getFirst() const { return mItems; }

    ///
    /// get next item in allocator
    ///
    void * getNext(const void * p) const {
        GN_ASSERT(p);
        return ((Item *) p)->next;
    }
};

///
/// object pool
///
template<class T, class RAW_MEMORY_POOL = FixSizedRawMemoryPool<sizeof(T)>>
class ObjectPool {
    RAW_MEMORY_POOL mRawMem;

    T * doAlloc() {
        T * p = (T *) mRawMem.alloc();
        if (0 == p) return 0;

        // construct the object, using defualt constructor
        new (p) T;

        // success
        return p;
    }

    void doDealloc(T * p) {
        if (0 == p) return;

        // destruct the object
        p->T::~T();

        // free p
        mRawMem.dealloc(p);
    }

    void doFreeAll() {
        // destruct all objects
        for (T * p = (T *) mRawMem.getFirst(); 0 != p; p = (T *) mRawMem.getNext(p)) { p->T::~T(); }

        // free memory
        mRawMem.freeAll();
    }

public:
    //@{

    ObjectPool() {}

    ~ObjectPool() { deconstructAndFreeAll(); }

    //@}

    //@{
    T *  allocConstructed() { return doAlloc(); }
    T *  allocUnconstructed() { return (T *) mRawMem.alloc(); }
    void deconstructAndFree(void * p) { doDealloc((T *) p); }
    void freeWithoutDeconstruct(void * p) { mRawMem.dealloc(p); }
    void deconstructAndFreeAll() { doFreeAll(); }
    bool check(const T * p) const { return mRawMem.check(p); }
    T *  getFirst() const { return (T *) mRawMem.getFirst(); }
    T *  getNext(const T * p) const { return (T *) mRawMem.getNext(p); }
    //@}
};
} // namespace GN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_MEMORY_H__
