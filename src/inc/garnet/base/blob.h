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
template<typename T = uint8_t, class SIZE_T = size_t>
class Blob : public RefCounted {
protected:
    T *    mData = nullptr;
    SIZE_T mSize = 0;

public:
    // Type definitions
    typedef T         value_type;
    typedef T &       reference;
    typedef const T & const_reference;
    typedef T *       pointer;
    typedef const T * const_pointer;
    typedef T *       iterator;
    typedef const T * const_iterator;
    typedef SIZE_T    size_type;

    // Disable copy semantics
    GN_NO_COPY_NO_MOVE(Blob);

    // Destructor
    ~Blob() {
        if (mData) {
            details::inplaceDestructArray(mSize, mData);
            OBJECT_ALLOCATOR::sDeallocate(mData);
        }
    }

    // Move assignment operator
    Blob & operator=(Blob && other) noexcept {
        if (this != &other) {
            if (mData) {
                // Destroy existing objects
                for (size_type i = 0; i < mSize; ++i) { mData[i].~T(); }
                HeapMemory::dealloc(mData);
            }
            mData       = other.mData;
            mSize       = other.mSize;
            other.mData = nullptr;
            other.mSize = 0;
        }
        return *this;
    }

    // Element access
    reference at(size_type pos) {
        GN_ASSERT(pos < mSize);
        return mData[pos];
    }

    const_reference at(size_type pos) const {
        GN_ASSERT(pos < mSize);
        return mData[pos];
    }

    reference operator[](size_type pos) {
        GN_ASSERT(pos < mSize);
        return mData[pos];
    }

    const_reference operator[](size_type pos) const {
        GN_ASSERT(pos < mSize);
        return mData[pos];
    }

    reference front() {
        GN_ASSERT(!empty());
        return mData[0];
    }

    const_reference front() const {
        GN_ASSERT(!empty());
        return mData[0];
    }

    reference back() {
        GN_ASSERT(!empty());
        return mData[mSize - 1];
    }

    const_reference back() const {
        GN_ASSERT(!empty());
        return mData[mSize - 1];
    }

    // Data access
    pointer       data() { return mData; }
    const_pointer data() const { return mData; }

    // Iterators
    iterator       begin() { return mData; }
    const_iterator begin() const { return mData; }
    const_iterator cbegin() const { return mData; }

    iterator       end() { return mData + mSize; }
    const_iterator end() const { return mData + mSize; }
    const_iterator cend() const { return mData + mSize; }

    // Capacity
    bool      empty() const { return mSize == 0; }
    size_type size() const { return mSize; }

    // // attach to existing data
    // Blob<T> & attachTo(SIZE_T count, T * data) {
    //     clear();
    //     mSize = count;
    //     mData = data;
    //     return *this;
    // }

    // Move to another array with different type
    template<typename T2>
    Blob<T2> moveTo() {
        Blob<T2> result(mSize);
        result.attachTo((SIZE_T) (mSize * sizeof(T) / sizeof(T2)), (T2 *) mData);
        mData = nullptr;
        mSize = 0;
        return result;
    }

    // Clear the blob. Make it empty.
    virtual void clear() = 0;
};

template<typename T, class SIZE_T = size_t, class OBJECT_ALLOCATOR = CxxObjectAllocator<T>>
class SimpleBlob : public Blob<T, SIZE_T> {
public:
    SimpleBlob() {}

    // Constructor new blob of certain size.
    explicit SimpleBlob(size_type count): mData(nullptr), mSize(count) {
        if (count > 0) {
            // allocate raw memory
            mData = static_cast<T *>(OBJECT_ALLOCATOR::sAllocate(count));
            if (!mData) GN_UNLIKELY {
                    GN_ERROR(getLogger("GN.base.Blob"))("Failed to allocate memory for blob of %zu bytes", count * sizeof(T));
                    mSize = 0;
                }
            else {
                // default construct the data array.
                details::inplaceDefaultConstructArray(count, mData);
            }
        }
    }

    // Copy constructor from raw data array
    explicit SimpleBlob(size_type count, const T * data): mData(nullptr), mSize(count) {
        if (count > 0) {
            // allocate raw memory
            mData = static_cast<T *>(OBJECT_ALLOCATOR::sAllocate(count));
            if (!mData) GN_UNLIKELY {
                    GN_ERROR(getLogger("GN.base.Blob"))("Failed to allocate memory for blob of %zu bytes", count * sizeof(T));
                    mSize = 0;
                }
            else if (data)
                GN_UNLIKELY {
                    // copy construct the data array.
                    details::inplaceCopyConstructArray(count, mData, data);
                }
            else {
                // default construct the data array.
                details::inplaceDefaultConstructArray(count, mData);
            }
        }
    }

    // Copy constructor from single initial value
    SimpleBlob(size_type count, const T & value): mData(nullptr), mSize(count) {
        if (count > 0) {
            mData = static_cast<T *>(OBJECT_ALLOCATOR::sAllocate(count * sizeof(T)));
            if (!mData) GN_UNLIKELY {
                    GN_ERROR(getLogger("GN.base.Blob"))("Failed to allocate memory for blob of size %zu", count * sizeof(T));
                    mSize = 0;
                }
            else { details::inplaceCopyConstructArray(count, mData, value); }
        }
    }

    ~SimpleBlob() override { clear(); }

    void clear() override {
        if (mData) {
            details::inplaceDestructArray(mSize, mData);
            OBJECT_ALLOCATOR::sDeallocate(mData);
            mData = nullptr;
            mSize = 0;
        }
    }
};

template<typename T, class SIZE_T = size_t, class OBJECT_ALLOCATOR = CxxObjectAllocator<T>>
class DynaArrayBlob : public Blob<T, SIZE_T> {
private:
    DynaArray<T, SIZE_T, OBJECT_ALLOCATOR> mArray;

public:
    DynaArrayBlob() {}

    ~DynaArrayBlob() override { clear(); }

    DynaArrayBlob & reserve(SIZE_T count) {
        auto reservedSize = std::max(count, mSize);
        mArray.reserve(reservedSize);
        mData = mArray.data(); // in case the array is reallocated
        return *this;
    }

    DynaArrayBlob & resize(SIZE_T count) {
        mArray.resize(count);
        mData = mArray.data(); // in case the array is reallocated
        mSize = mArray.size();
        return *this;
    }

    DynaArrayBlob & append(const T & value) {
        mArray.push_back(value);
        mData = mArray.data(); // in case the array is reallocated
        mSize = mArray.size();
        return *this;
    }

    void clear() override {
        mArray.clear();
        mData = nullptr;
        mSize = 0;
    }
};

}

#endif // _GN_BASE_BLOB_H_
