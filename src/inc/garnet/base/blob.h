#ifndef __GN_BASE_BLOB_H__
#define __GN_BASE_BLOB_H__
// *****************************************************************************
/// \file
/// \brief   non-typed binary data chunk class
/// \author  chenli@@REDMOND (2009.9.24)
// *****************************************************************************

namespace GN {
///
/// Represents a chunk of non-typed binary data
///
struct Blob : public RefCounter {
    ///
    /// get binary pointer
    ///
    virtual void * data() const = 0;

    ///
    /// get binary size in bytes
    ///
    virtual uint32 size() const = 0;
};

///
/// A simple fixed size blob class
///
class SimpleBlob : public Blob {
    void * mBuffer;
    uint32 mSize;

public:
    /// ctor
    explicit SimpleBlob(uint32 sz) {
        mBuffer = HeapMemory::alignedAlloc(sz, 16);
        mSize   = (NULL != mBuffer) ? sz : 0;
    }

    /// dtor
    virtual ~SimpleBlob() {
        HeapMemory::dealloc(mBuffer);
        mBuffer = 0;
        mSize   = 0;
    }

    //@{
    virtual void * data() const { return mBuffer; }
    virtual uint32 size() const { return mSize; }
    //@}
};

///
/// A blob class that used DynaArray as backend.
///
template<typename T, typename SIZE_TYPE = size_t>
class DynaArrayBlob : public Blob {
    DynaArray<T, SIZE_TYPE> mBuffer;

public:
    /// ctor
    DynaArrayBlob() {}

    //@{
    virtual void * data() const { return (void *) mBuffer.rawptr(); }
    virtual uint32 size() const { return (uint32) (sizeof(T) * mBuffer.size()); }
    //@}

    //@{
    DynaArray<T, SIZE_TYPE> &       array() { return mBuffer; }
    const DynaArray<T, SIZE_TYPE> & array() const { return mBuffer; }
    //@}
};
} // namespace GN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_BLOB_H__
