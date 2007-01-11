#ifndef __GN_GFX_BUFFER_H__
#define __GN_GFX_BUFFER_H__
// *****************************************************************************
/// \file    buffer.h
/// \brief   Vertex and index buffer interface
/// \author  chenlee (2005.9.30)
// *****************************************************************************

namespace GN { namespace gfx
{
    struct VtxBuf;
    struct IdxBuf;

    ///
    /// Vertex buffer content loader
    ///
    typedef Delegate1<bool,VtxBuf&> VtxBufLoader;

    ///
    /// Index buffer content loader
    ///
    typedef Delegate1<bool,IdxBuf&> IdxBufLoader;

    ///
    /// Vertex buffer interface.
    ///
    struct VtxBuf : public RefCounter
    {
        ///
        /// Get buffer size in bytes
        ///
        size_t getSizeInBytes() const { return mSize; }

        ///
        /// Get buffer usage
        ///
        bool isDynamic() const { return mDynamic; }

        ///
        /// Set content loader
        ///
        void setLoader( const VtxBufLoader & loader ) { mLoader = loader; }

        ///
        /// Lock vertex buffer
        ///
        /// \param offset
        ///     offset in bytes of lock range.
        /// \param bytes
        ///     bytes of lock range. '0' means to the end of the buffer.
        /// \param flag
        ///     Locking flags, see LockFlag.
        ///     Note that LOCK_RW and LOCK_RO can only be used for buffers with system copy
        /// \return
        ///     Return locked buffer pointer. NULL means failed.
        ///
        virtual void * lock( size_t offset, size_t bytes, LockFlag flag ) = 0;

        ///
        /// Unlock specific stream
        ///
        virtual void unlock() = 0;

    protected:

        ///
        /// Set buffer properties
        ///
        void setProperties( size_t bytes, bool dynamic )
        {
            mSize = bytes;
            mDynamic = dynamic;
        }

        ///
        /// Get content loader
        ///
        const VtxBufLoader & getLoader() const { return mLoader; }

    private:

        size_t       mSize;    ///< Buffer size in bytes
        bool         mDynamic; ///< Buffer usage
        VtxBufLoader mLoader;  ///< Content loader
    };

    ///
    /// Index Buffer
    ///
    struct IdxBuf : public RefCounter
    {
        ///
        /// Get index count of the buffer
        ///
        size_t getNumIdx() const { return mNumIdx; }

        ///
        /// Get buffer usage
        ///
        bool isDynamic() const { return mDynamic; }

        ///
        /// Set content loader
        ///
        void setLoader( const IdxBufLoader & loader ) { mLoader = loader; }

        ///
        /// lock the buffer
        ///
        /// \param startIdx
        ///     first index of this locking
        /// \param numIdx
        ///     index count of this locking, '0' means to the end of the buffer.
        /// \param flag
        ///     Locking flags, see LockFlag.
        ///     Note that LOCK_RW and LOCK_RO can only be used for buffers with system copy
        /// \return
        ///     Return locked buffer pointer. NULL means failed.
        ///
        virtual UInt16 * lock( size_t startIdx, size_t numIdx, LockFlag flag ) = 0;

        ///
        /// unlock the buffer
        ///
        virtual void unlock() = 0;

    protected:

        ///
        /// Set buffer properties
        ///
        void setProperties( size_t numIdx, bool dynamic )
        {
            mNumIdx = numIdx;
            mDynamic = dynamic;
        }

        ///
        /// Get content loader
        ///
        const IdxBufLoader & getLoader() const { return mLoader; }

    private:

        size_t       mNumIdx;  ///< Index count
        bool         mDynamic; ///< Buffer usage
        IdxBufLoader mLoader;  ///< Content loader
    };

    ///
    /// Unlock the buffer automatically, before going out of life scope.
    ///
    template<class BUFFER>
    class AutoBufferUnlocker
    {
        BUFFER * mBuf;

    public:

        ///
        /// Ctor
        ///
        AutoBufferUnlocker( BUFFER * buf ) : mBuf(buf) {}

        ///
        /// Dtor
        ///
        ~AutoBufferUnlocker() { if(mBuf) mBuf->unlock(); }
    };
}}

// *****************************************************************************
//                           End of buffer.h
// *****************************************************************************
#endif // __GN_GFX_BUFFER_H__
