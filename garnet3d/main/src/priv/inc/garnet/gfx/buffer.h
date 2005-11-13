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
        //! Get buffer length in bytes
        //!
        size_t getLength() const { return mLength; }
        
        //!
        //! Lock specific stream
        //!
        //! \param offset
        //!     offset in bytes
        //! \param bytes
        //!     Total locked bytes
        //! \param flag
        //!     Locking flags, see LockFlag
        //! \return
        //!     Return locked buffer pointer. NULL means failed.
        //!
        virtual void * lock( size_t   offset,
                             size_t   bytes,
                             LockFlag flag ) = 0;

        //!
        //! Unlock specific stream
        //!
        virtual void unlock() = 0;

    protected:

        //!
        //! Set buffer length
        //!
        void setLength( size_t newValue ) { mLength = newValue; }

        //!
        //! Set buffer usage
        //!
        void setUsage( ResourceUsage newValue ) { mUsage = newValue; }

    private:

        size_t        mLength; //!< Length in bytes
        ResourceUsage mUsage;  //!< Buffer usage
    };

    //!
    //! Index Buffer
    //!
    struct IdxBuf : public RefCounter
    {
        //!
        //! lock the buffer
        //!
        //! \param startIdx
        //!     first index of this locking
        //! \param numIdx
        //!     index count of this locking, '0' means to the end of the buffer.
        //! \param flag
        //!     Locking flags, see LockFlag
        //! \return
        //!     Return locked buffer pointer. NULL means failed.
        //!
        virtual uint16_t * lock( size_t   startIdx,
                                 size_t   numIdx,
                                 LockFlag flag ) = 0;

        //!
        //! unlock the buffer
        //!
        virtual void unlock() = 0;
    };
}}

// *****************************************************************************
//                           End of buffer.h
// *****************************************************************************
#endif // __GN_GFX_BUFFER_H__
