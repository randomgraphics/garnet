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
        //! Get vertex count.
        //!
        size_t getNumVtx() const { return mVtxCount; }

        //!
        //! Get vertex stride
        //!
        size_t getStride() const { return mStride; }

        //!
        //! Get buffer usage
        //!
        ResourceUsage getUsage() const { return mUsage; }

        //!
        //! Lock specific stream
        //!
        //! \param startVtx
        //!     first vertex of this locking
        //! \param numVtx
        //!     vertex count of this locking, '0' means to the end of the buffer.
        //! \param flag
        //!     Locking flags, see LockFlag.
        //!     Note that LOCK_RO can be used for buffer that has system copy
        //! \return
        //!     Return locked buffer pointer. NULL means failed.
        //!
        virtual void * lock( size_t   startVtx,
                             size_t   numVtx,
                             uint32_t flag ) = 0;

        //!
        //! Unlock specific stream
        //!
        virtual void unlock() = 0;

    protected:

        //!
        //! Set buffer properties
        //!
        void setProperties( size_t vtxCount, size_t stride, ResourceUsage usage )
        {
            mVtxCount = vtxCount;
            mStride = stride;
            mUsage = usage;
        }

    private:

        size_t mVtxCount; //!< Vertex count.
        size_t mStride; //!< Vertex stride.
        ResourceUsage mUsage;  //!< Buffer usage
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
        ResourceUsage getUsage() const { return mUsage; }

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
        virtual uint16_t * lock( size_t   startIdx,
                                 size_t   numIdx,
                                 uint32_t flag ) = 0;

        //!
        //! unlock the buffer
        //!
        virtual void unlock() = 0;

    protected:

        //!
        //! Set buffer properties
        //!
        void setProperties( size_t numIdx, ResourceUsage usage )
        {
            mNumIdx = numIdx;
            mUsage = usage;
        }

    private:

        size_t        mNumIdx; //!< index count
        ResourceUsage mUsage;    //!< Buffer usage
    };
}}

// *****************************************************************************
//                           End of buffer.h
// *****************************************************************************
#endif // __GN_GFX_BUFFER_H__
