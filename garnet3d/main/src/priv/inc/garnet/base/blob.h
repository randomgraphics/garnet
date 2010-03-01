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
        virtual size_t Size() const = 0;
    };

    ///
    /// A simple implementation of blob class
    ///
    class SimpleBlob : public Blob
    {
        DynaArray<UInt8> mBuffer;

    public:

        /// ctor
        explicit SimpleBlob( size_t sz = 0 ) : mBuffer(sz) {}

        //@{
        virtual void * data() const { return (void*)mBuffer.ToRawPtr(); }
        virtual size_t Size() const { return mBuffer.Size(); }
        //@}

        /// resize the buffer
        void Resize( size_t newSize ) { mBuffer.Resize( newSize ); }
    };
};

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_BLOB_H__
