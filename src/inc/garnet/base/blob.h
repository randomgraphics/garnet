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
    // Type definitions
    struct ImplBase {
        virtual ~ImplBase() {};
    };

    // Disable copy semantics
    GN_NO_COPY(Blob);
    GN_NO_MOVE(Blob);

    // Destructor
    virtual ~Blob() {
        mData = nullptr;
        mSize = 0;
        delete mImpl;
        mImpl = 0;
    }

    template<typename T>
    SafeArrayAccessor<T> accessor() const { return SafeArrayAccessor<T>((T*)mData, mSize / sizeof(T)); }

    // Clear the blob. Make it empty.
    void clear() {
        mData = nullptr;
        mSize = 0;
        delete mImpl;
        mImpl = 0;
    }

protected:
    void *     mData = nullptr;
    size_t     mSize = 0;
    ImplBase * mImpl = nullptr;
};

template<typename T class OBJECT_ALLOCATOR = CxxObjectAllocator<T>>
class SimpleBlob : public Blob {
    typedef Blob Base;

    struct Impl : Base::ImplBase {
        size_t mSize = 0;
        T * mData = nullptr;
        Impl(size_t size, T * data) : mSize(size), mData(data) {}
        ~Impl() override {
            if (mData) {
                details::inplaceDestructArray(Base::mSize, mData);
                OBJECT_ALLOCATOR::sDeallocate(mData);
            }
            mData = nullptr;
            mSize = 0;
        }
    };

public:
    explicit SimpleBlob(size_t count) {
        if (count > 0) {
            // allocate raw memory
            Base::mData = OBJECT_ALLOCATOR::sAllocate(count);
            if (!Base::mData) GN_UNLIKELY {
                GN_ERROR(getLogger("GN.base.Blob"))("Failed to allocate memory for blob of %zu bytes", count * sizeof(T));
            } else {
                // default construct the data array.
                details::inplaceDefaultConstructArray(count, (T*)Base::mData);
                Base::mSize = count;
                Base::mImpl = new Impl(count, (T*)Base::mData);
            }
        }
    }

    // // Copy constructor from raw data array
    // SimpleBlob(size_t count, const T * data) {
    //     if (count > 0) {
    //         // allocate raw memory
    //         Base::mData = static_cast<T *>(OBJECT_ALLOCATOR::sAllocate(count));
    //         if (!Base::mData) GN_UNLIKELY {
    //                 GN_ERROR(getLogger("GN.base.Blob"))("Failed to allocate memory for blob of %zu bytes", count * sizeof(T));
    //                 Base::mSize = 0;
    //             }
    //         else if (data)
    //             GN_UNLIKELY {
    //                 // copy construct the data array.
    //                 details::inplaceCopyConstructArray(count, Base::mData, data);
    //             }
    //         else {
    //             // default construct the data array.
    //             details::inplaceDefaultConstructArray(count, Base::mData);
    //         }
    //     }
    // }

    // // Copy constructor from single initial value
    // SimpleBlob(size_t count, const T & value) {
    //     if (count > 0) {
    //         Base::mData = static_cast<T *>(OBJECT_ALLOCATOR::sAllocate(count * sizeof(T)));
    //         if (!Base::mData) GN_UNLIKELY {
    //                 GN_ERROR(getLogger("GN.base.Blob"))("Failed to allocate memory for blob of size %zu", count * sizeof(T));
    //                 Base::mSize = 0;
    //             }
    //         else { details::inplaceCopyConstructArray(count, Base::mData, value); }
    //     }
    // }
};

template<typename T = uint8_t, class OBJECT_ALLOCATOR = CxxObjectAllocator<T>>
class DynaArrayBlob : public Blob {
private:
    struct Impl : Base::ImplBase {
        DynaArray<T, size_t, OBJECT_ALLOCATOR> array;
    };

    typedef Blob Base;

    DynaArray<T, size_t, OBJECT_ALLOCATOR> array() const {
        return static_cast<Impl*>(Base::mImpl)->array;
    }

public:
    DynaArrayBlob() {
        Base::mImpl = new Impl();
    }

    DynaArrayBlob & reserve(size_t count) {
        auto reservedSize = std::max(count, Base::mSize);
        auto & a = array();
        a.reserve(reservedSize);
        Base::mData = a.data(); // in case the array is reallocated
        return *this;
    }

    DynaArrayBlob & resize(size_t count) {
        auto & a = array();
        a.resize(count);
        Base::mData = a.data(); // in case the array is reallocated
        Base::mSize = a.size();
        return *this;
    }

    DynaArrayBlob & append(const T & value) {
        auto & a = array();
        a.push_back(value);
        Base::mData = a.data(); // in case the array is reallocated
        Base::mSize = a.size();
        return *this;
    }
};

}

#endif // _GN_BASE_BLOB_H_