#ifndef __GN_GFX_UTIL_RNDR_RINGBUFFER_H__
#define __GN_GFX_UTIL_RNDR_RINGBUFFER_H__
// *****************************************************************************
/// \file
/// \brief   ring buffer for multithread renderer
/// \author  chenli@@REDMOND (2009.1.5)
// *****************************************************************************

namespace GN { namespace gfx
{
    ///
    /// ring buffer that supports only single reader and single writer
    ///
    class RingBuffer : public StdClass
    {
        GN_DECLARE_STDCLASS( RingBuffer, StdClass );

        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        RingBuffer()          { clear(); }
        virtual ~RingBuffer() { quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool init( size_t );
        void quit();
    private:
        void clear() {}
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        // Block calling thread util ring buffer is not full, or the ring buffer is about to quit.
        //
        // Return NULL, if the ring buffer is about to quit.
        void * beginProduce( size_t );

        // Call this function to notify the ring buffer that production is competed, and to wake any
        // blocking consumer thread.
        void   endProduce();

        // Block calling thread until there's someting to cosume or the ring buffer
        // is about to quit.
        //
        // Returns NULL, if the ring buffer is going to quit.
        void * consume( size_t );

        // ********************************
        // private variables
        // ********************************
    private:

        // empty: readptr == writeptr
        // full:  readptr == (writeptr+1)%buffersize
        UInt32                  * mRingBufferBegin; // Points to the first byte of the ring buffer.
        UInt32                  * mRingBufferEnd;   // Points to the byte that is one byte over the last byte of the ring buffer
        volatile const UInt32   * mReadPtr; // Points to next unconsumed command
        volatile UInt32         * mWritePtr; // Points to next empty space.
        SyncEvent               * mRingBufferFull;
        SyncEventGroup          * mRingBufferEmpty;

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_UTIL_RNDR_RINGBUFFER_H__
