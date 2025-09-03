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
class Blob : public RefCounter {
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
    GN_NO_COPY(Blob);
    GN_NO_MOVE(Blob);

    // Destructor
    virtual ~Blob() {
        mData = nullptr;
        mSize = 0;
    }

    // Element access
    reference at(SIZE_T pos) {
        GN_ASSERT(pos < mSize);
        return mData[pos];
    }

    const_reference at(SIZE_T pos) const {
        GN_ASSERT(pos < mSize);
        return mData[pos];
    }

    reference operator[](SIZE_T pos) {
        GN_ASSERT(pos < mSize);
        return mData[pos];
    }

    const_reference operator[](SIZE_T pos) const {
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
    SIZE_T size() const { return mSize; }

    // // attach to existing data
    // Blob<T> & attachTo(SIZE_T count, T * data) {
    //     clear();
    //     mSize = count;
    //     mData = data;
    //     return *this;
    // }

    // // Move to another array with different type
    // template<typename T2>
    // Blob<T2> moveTo() {
    //     Blob<T2> result(mSize);
    //     result.attachTo((SIZE_T) (mSize * sizeof(T) / sizeof(T2)), (T2 *) mData);
    //     mData = nullptr;
    //     mSize = 0;
    //     return result;
    // }

    // Clear the blob. Make it empty.
    virtual void clear() = 0;
};

template<typename T = uint8_t, class SIZE_T = size_t, class OBJECT_ALLOCATOR = CxxObjectAllocator<T>>
class SimpleBlob : public Blob<T, SIZE_T> {
    typedef Blob<T, SIZE_T> Base;
public:
    SimpleBlob() {}

    // Constructor new blob of certain size.
    explicit SimpleBlob(SIZE_T count) {
        if (count > 0) {
            // allocate raw memory
            Base::mData = static_cast<T *>(OBJECT_ALLOCATOR::sAllocate(count));
            if (!Base::mData) GN_UNLIKELY {
                GN_ERROR(getLogger("GN.base.Blob"))("Failed to allocate memory for blob of %zu bytes", count * sizeof(T));
                Base::mSize = 0;
            } else {
                // default construct the data array.
                details::inplaceDefaultConstructArray(count, Base::mData);
                Base::mSize = count;
            }
        }
    }

    // Copy constructor from raw data array
    SimpleBlob(SIZE_T count, const T * data) {
        if (count > 0) {
            // allocate raw memory
            Base::mData = static_cast<T *>(OBJECT_ALLOCATOR::sAllocate(count));
            if (!Base::mData) GN_UNLIKELY {
                    GN_ERROR(getLogger("GN.base.Blob"))("Failed to allocate memory for blob of %zu bytes", count * sizeof(T));
                    Base::mSize = 0;
                }
            else if (data)
                GN_UNLIKELY {
                    // copy construct the data array.
                    details::inplaceCopyConstructArray(count, Base::mData, data);
                }
            else {
                // default construct the data array.
                details::inplaceDefaultConstructArray(count, Base::mData);
            }
        }
    }

    // Copy constructor from single initial value
    SimpleBlob(SIZE_T count, const T & value) {
        if (count > 0) {
            Base::mData = static_cast<T *>(OBJECT_ALLOCATOR::sAllocate(count * sizeof(T)));
            if (!Base::mData) GN_UNLIKELY {
                    GN_ERROR(getLogger("GN.base.Blob"))("Failed to allocate memory for blob of size %zu", count * sizeof(T));
                    Base::mSize = 0;
                }
            else { details::inplaceCopyConstructArray(count, Base::mData, value); }
        }
    }

    ~SimpleBlob() override { clear(); }

    void clear() override {
        if (Base::mData) {
            details::inplaceDestructArray(Base::mSize, Base::mData);
            OBJECT_ALLOCATOR::sDeallocate(Base::mData);
            Base::mData = nullptr;
            Base::mSize = 0;
        }
    }
};

template<typename T = uint8_t, class SIZE_T = size_t, class OBJECT_ALLOCATOR = CxxObjectAllocator<T>>
class DynaArrayBlob : public Blob<T, SIZE_T> {
private:
    typedef Blob<T, SIZE_T> Base;
    DynaArray<T, SIZE_T, OBJECT_ALLOCATOR> mArray;

public:
    DynaArrayBlob() {}

    ~DynaArrayBlob() override { clear(); }

    DynaArrayBlob & reserve(SIZE_T count) {
        auto reservedSize = std::max(count, Base::mSize);
        mArray.reserve(reservedSize);
        Base::mData = mArray.data(); // in case the array is reallocated
        return *this;
    }

    DynaArrayBlob & resize(SIZE_T count) {
        mArray.resize(count);
        Base::mData = mArray.data(); // in case the array is reallocated
        Base::mSize = mArray.size();
        return *this;
    }

    DynaArrayBlob & append(const T & value) {
        mArray.push_back(value);
        Base::mData = mArray.data(); // in case the array is reallocated
        Base::mSize = mArray.size();
        return *this;
    }

    void clear() override {
        mArray.clear();
        Base::mData = nullptr;
        Base::mSize = 0;
    }
};

}

#endif // _GN_BASE_BLOB_H_