#ifndef __GN_GFX_BUFFER_H__
#define __GN_GFX_BUFFER_H__
// *****************************************************************************
//! \file    buffer.h
//! \brief   Vertex and index buffer interface
//! \author  chenlee (2005.9.30)
// *****************************************************************************

namespace GN { namespace gfx
{
    //!
    //! Vertex buffer interface.
    //!
    struct VtxBuf : public RefCounter
    {
        //!
        //! Get buffer size in bytes
        //!
        size_t getSizeInBytes() const { return mSize; }

        //!
        //! Get buffer usage
        //!
        bool isDynamic() const { return mDynamic; }

        //!
        //! Lock specific stream
        //!
        //! \param offset
        //!     offset in bytes of lock range.
        //! \param bytes
        //!     bytes of lock range. '0' means to the end of the buffer.
        //! \param flag
        //!     Locking flags, see LockFlag.
        //!     Note that LOCK_RO can be used for buffer that has system copy
        //! \return
        //!     Return locked buffer pointer. NULL means failed.
        //!
        virtual void * lock( size_t offset, size_t bytes, uint32_t flag ) = 0;

        //!
        //! Unlock specific stream
        //!
        virtual void unlock() = 0;

    protected:

        //!
        //! Set buffer properties
        //!
        void setProperties( size_t bytes, bool dynamic )
        {
            mSize = bytes;
            mDynamic = dynamic;
        }

    private:

        size_t mSize;    //!< Buffer size in bytes
        bool   mDynamic; //!< Buffer usage
    };

    //!
    //! Index Buffer
    //!
    struct IdxBuf : public RefCounter
    {
        //!
        //! Get index count of the buffer
        //!
        size_t getNumIdx() const { return mNumIdx; }

        //!
        //! Get buffer usage
        //!
        bool isDynamic() const { return mDynamic; }

        //!
        //! lock the buffer
        //!
        //! \param startIdx
        //!     first index of this locking
        //! \param numIdx
        //!     index count of this locking, '0' means to the end of the buffer.
        //! \param flag
        //!     Locking flags, see LockFlag.
        //!     Note that LOCK_RO can be used for buffer that has system copy
        //! \return
        //!     Return locked buffer pointer. NULL means failed.
        //!
        virtual uint16_t * lock( size_t startIdx, size_t numIdx, uint32_t flag ) = 0;

        //!
        //! unlock the buffer
        //!
        virtual void unlock() = 0;

    protected:

        //!
        //! Set buffer properties
        //!
        void setProperties( size_t numIdx, bool dynamic )
        {
            mNumIdx = numIdx;
            mDynamic = dynamic;
        }

    private:

        size_t mNumIdx;  //!< Index count
        bool   mDynamic; //!< Buffer usage
    };
}}

// *****************************************************************************
//                           End of buffer.h
// *****************************************************************************
#endif // __GN_GFX_BUFFER_H__
