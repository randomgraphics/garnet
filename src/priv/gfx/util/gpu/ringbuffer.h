#ifndef __GN_GFX_UTIL_GPU_RINGBUFFER_H__
#define __GN_GFX_UTIL_GPU_RINGBUFFER_H__
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
        RingBuffer()          { Clear(); }
        virtual ~RingBuffer() { Quit(); }
        //@}

        // ********************************
        // from StdClass
        // ********************************

        //@{
    public:
        bool Init( size_t ringBufferSize );
        void Quit();
    private:
        void Clear()
        {
            mBegin    = NULL;
            mNotFull  = NULL;
            mNotEmpty = NULL;
        }
        //@}

        // ********************************
        // public functions
        // ********************************
    public:

        /// get ring buffer size
        size_t size() const { return mSize; }

        // post Quit message to ring buffer, unblock any blocked threads.
        void   postQuitMessage();

        // Block calling thread util ring buffer is not full, or the ring buffer is about to Quit.
        // Return NULL, if the ring buffer is about to Quit.
        // Caller has to make sure that the size is no larger then the buffer size.
        void * beginProduce( size_t size );

        // Call this function to notify the ring buffer that production is competed, and to wake any
        // blocking consumer thread.
        void   endProduce();

        // Block calling thread until there's not empty or the ring buffer is about to Quit.
        // Return NULL, if the ring buffer is about to Quit.
        // Caller has to make sure that the size is no larger then the buffer size.
        void * beginConsume( size_t size );

        void   endConsume();

        // ********************************
        // private variables
        // ********************************
    private:

        // empty: readptr == writeptr
        // full:  readptr == (writeptr+1)%buffersize
        size_t           mSize;      // the buffer size
        UInt8          * mBegin;     // Points to the first byte of the ring buffer.
        UInt8          * mEnd;       // Points to the byte that is one byte over the last byte of the ring buffer
        UInt8 * volatile mReadPtr;   // Points to next unconsumed command
        UInt8 * volatile mWritePtr;  // Points to next empty space.
        size_t           mPendingReadSize;
        size_t           mPendingWriteSize; // size of pending write
        SyncEvent      * mNotFull;
        SyncEvent      * mNotEmpty;
        volatile bool    mQuit;

        // ********************************
        // private functions
        // ********************************
    private:
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_GFX_UTIL_GPU_RINGBUFFER_H__
