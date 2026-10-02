#ifndef _GN_BASE_BLOB_H_
#define _GN_BASE_BLOB_H_
// *****************************************************************************
/// \file
/// \brief   Blob utilites
/// \author  chenlee (2025.9)
// *****************************************************************************

namespace GN {

///
/// Fix sized heap memory with regular data accessor and boundary check in debug build.
/// It is movable but not copyable. This is means to pass around non-resizeable binary data block.
///
class Blob : public RefCounter {
public:
    // Disable copy semantics
    GN_NO_COPY(Blob);
    GN_NO_MOVE(Blob);

    // Destructor
    virtual ~Blob() {
        mData = nullptr;
        mSize = 0;
    }

    template<typename T>
    ArrayView<T> accessor() const {
        return ArrayView<T>((T *) mData, mSize / sizeof(T));
    }

    bool empty() const { return mSize == 0; }

    /// Get the size of the blob in bytes.
    size_t size() const { return mSize; }

    /// Get the pointer to the blob buffer.
    void * data() const { return mData; }

    // Clear the blob. Make it empty.
    virtual void clear() {
        mData = nullptr;
        mSize = 0;
    }

protected:
    void * mData = nullptr;
    size_t mSize = 0; ///< size in bytes

    Blob() {}
};

template<typename T, class OBJECT_ALLOCATOR = CxxObjectAllocator<T>>
class SimpleBlob : public Blob {
public:
    // Copy constructor from raw data array
    explicit SimpleBlob(size_t count = 0, const T * data = nullptr) {
        if (count > 0) {
            // allocate raw memory
            mData = static_cast<T *>(OBJECT_ALLOCATOR::sAllocate(count));
            if (!mData) {
                GN_ERROR(getLogger("GN.base.Blob"), "Failed to allocate memory for blob of {} bytes", count * sizeof(T));
            } else if (data) {
                // copy construct the data array.
                details::inplaceCopyConstructArray(count, static_cast<T *>(mData), data);
                mSize = count * sizeof(T);
            } else {
                // default construct the data array.
                details::inplaceDefaultConstructArray(count, static_cast<T *>(mData));
                mSize = count * sizeof(T);
            }
        }
    }

    ~SimpleBlob() override { destroy(); }

    void clear() override { destroy(); }

private:
    void destroy() {
        if (mData) {
            details::inplaceDestructArray(mSize / sizeof(T), static_cast<T *>(mData));
            OBJECT_ALLOCATOR::sDeallocate(mData);
            mData = nullptr;
            mSize = 0;
        }
    }
};

template<typename T, class OBJECT_ALLOCATOR = CxxObjectAllocator<T>>
class DynaArrayBlob : public Blob {
private:
    DynaArray<T, size_t, OBJECT_ALLOCATOR> mArray;

    DynaArray<T, size_t, OBJECT_ALLOCATOR> &       array() { return mArray; }
    const DynaArray<T, size_t, OBJECT_ALLOCATOR> & array() const { return mArray; }

public:
    DynaArrayBlob() = default;

    ~DynaArrayBlob() override {
        mData = nullptr;
        mSize = 0;
    }

    void clear() override {
        mArray.clear();
        mData = nullptr;
        mSize = 0;
    }

    size_t count() const { return mArray.size(); }

    DynaArrayBlob & reserve(size_t count) {
        auto reservedSize = std::max(count, mSize / sizeof(T));
        mArray.reserve(reservedSize);
        mData = mArray.data(); // in case the array is reallocated
        return *this;
    }

    DynaArrayBlob & resize(size_t count) {
        mArray.resize(count);
        mData = mArray.data();
        mSize = mArray.size() * sizeof(T);
        return *this;
    }

    DynaArrayBlob & append(const T & value) {
        mArray.append(value);
        mData = mArray.data();
        mSize = mArray.size() * sizeof(T);
        return *this;
    }
};

template<typename T, class OBJECT_ALLOCATOR = CxxObjectAllocator<T>>
class FixedBlob {
public:
    explicit FixedBlob(size_t count = 1): mBlobSizeInT(count) {
        if (0 == count) mBlobSizeInT = 1;
    }

    ~FixedBlob() {
        // Detach all active blobs and free their data buffers
        for (auto * b = mActiveHead; b != nullptr;) {
            auto * next = b->mNext;
            b->mOwner   = nullptr;
            T * ptr     = static_cast<T *>(b->data());
            if (ptr) {
                for (size_t i = 0; i < mBlobSizeInT; ++i) { OBJECT_ALLOCATOR::sDestruct(ptr + i); }
                OBJECT_ALLOCATOR::sDeallocate(ptr);
                b->clear();
            }
            b = next;
        }
        mActiveHead = nullptr;

        // Free all idle buffers in the free list
        for (T * ptr : mFreeList) { OBJECT_ALLOCATOR::sDeallocate(ptr); }
        mFreeList.clear();
        mTotalBuffers = 0;
    }

    GN_NO_COPY(FixedBlob);
    GN_NO_MOVE(FixedBlob);

    AutoRef<Blob> allocate(const T * data = nullptr) {
        T * ptr = nullptr;
        if (!mFreeList.empty()) {
            ptr = mFreeList.back();
            mFreeList.popBack();
        } else {
            ptr = OBJECT_ALLOCATOR::sAllocate(mBlobSizeInT);
            if (!ptr) GN_UNLIKELY {
                    GN_ERROR(getLogger("GN.base.Blob"), "Failed to allocate memory for fixed blob of {} bytes", mBlobSizeInT * sizeof(T));
                    return {};
                }
            ++mTotalBuffers;
        }

        // Call ctor or copy ctor for each item in the blob
        if (data) {
            details::inplaceCopyConstructArray(mBlobSizeInT, ptr, data);
        } else {
            details::inplaceDefaultConstructArray(mBlobSizeInT, ptr);
        }

        return AutoRef<Blob>(new PooledBlob(mBlobSizeInT * sizeof(T), ptr, this));
    }

    /// Number of elements of type T in each blob
    size_t count() const { return mBlobSizeInT; }

    /// Size of each blob in bytes
    size_t blobSize() const { return mBlobSizeInT * sizeof(T); }

    /// Total number of pooled buffers allocated
    size_t pooledCount() const { return mTotalBuffers; }

    /// Number of free buffers currently in the pool
    size_t freeCount() const { return mFreeList.size(); }

    /// Number of active buffers currently in use
    size_t activeCount() const { return mTotalBuffers - mFreeList.size(); }

private:
    struct PooledBlob : Blob {
        FixedBlob *  mOwner = nullptr;
        PooledBlob * mPrev  = nullptr;
        PooledBlob * mNext  = nullptr;

        PooledBlob(size_t sz, void * ptr, FixedBlob * owner): mOwner(owner) {
            mSize = sz;
            mData = ptr;
            if (mOwner) {
                mNext = mOwner->mActiveHead;
                if (mNext) mNext->mPrev = this;
                mOwner->mActiveHead = this;
            }
        }

        ~PooledBlob() override { releaseToPool(); }

        void clear() override { releaseToPool(); }

    private:
        void releaseToPool() {
            if (mOwner) {
                // Unlink from active list
                if (mPrev)
                    mPrev->mNext = mNext;
                else
                    mOwner->mActiveHead = mNext;
                if (mNext) mNext->mPrev = mPrev;
                mPrev = nullptr;
                mNext = nullptr;

                if (mData) {
                    T * ptr = static_cast<T *>(mData);
                    for (size_t i = 0; i < mOwner->mBlobSizeInT; ++i) { OBJECT_ALLOCATOR::sDestruct(ptr + i); }
                    mOwner->mFreeList.append(ptr);
                }
            }
            mData = nullptr;
            mSize = 0;
        }
    };

    size_t         mBlobSizeInT;
    size_t         mTotalBuffers = 0;
    DynaArray<T *> mFreeList;
    PooledBlob *   mActiveHead = nullptr;
};

template<typename T, class OBJECT_ALLOCATOR = CxxObjectAllocator<T>>
using FixedBlobAllocator = FixedBlob<T, OBJECT_ALLOCATOR>;

} // namespace GN

#endif // _GN_BASE_BLOB_H_