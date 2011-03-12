#ifndef __GN_BASE_BLOB_H__
#define __GN_BASE_BLOB_H__
// *****************************************************************************
/// \file
/// \brief   non-typed binary data chunk class
/// \author  chenli@@REDMOND (2009.9.24)
// *****************************************************************************

namespace GN
{
    ///
    /// Represents a chunk of non-typed binary data
    ///
    struct Blob : public RefCounter
    {
        ///
        /// get binary pointer
        ///
        virtual void * data() const = 0;

        ///
        /// get binary size in bytes
        ///
        virtual size_t size() const = 0;
    };

    ///
    /// A simple fixed size blob class
    ///
    class SimpleBlob : public Blob
    {
        void * mBuffer;
        size_t  mSize;

    public:

        /// ctor
        explicit SimpleBlob( size_t sz )
        {
            mBuffer = HeapMemory::alignedAlloc( sz, 16 );
            mSize = (NULL != mBuffer) ? sz : 0;
        }

        /// dtor
        virtual ~SimpleBlob()
        {
           HeapMemory::dealloc( mBuffer );
           mBuffer = 0;
           mSize = 0;
        }

        //@{
        virtual void * data() const { return mBuffer; }
        virtual size_t size() const { return mSize; }
        //@}
    };

    ///
    /// A blob class that used DynaArray as backend.
    ///
    template<typename T>
    class DynaArrayBlob : public Blob
    {
        DynaArray<T> mBuffer;

    public:

        /// ctor
        DynaArrayBlob() {}

        //@{
        virtual void * data() const { return (void*)mBuffer.cptr(); }
        virtual size_t size() const { return sizeof(T) * mBuffer.size(); }
        //@}

        //@{
        DynaArray<T>       & array()       { return mBuffer; }
        const DynaArray<T> & array() const { return mBuffer; }
        //@}
    };
};

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_BLOB_H__
