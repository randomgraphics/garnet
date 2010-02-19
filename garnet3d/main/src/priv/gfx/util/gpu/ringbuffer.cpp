#include "pch.h"
#include "ringbuffer.h"

static GN::Logger * sLogger = GN::GetLogger("GN.gfx.util.gpu.RingBuffer");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::RingBuffer::Init( size_t ringBufferSize )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::RingBuffer, () );

    mSize  = ringBufferSize;
    mBegin = (UInt8*)HeapAlloc( mSize * 2 ); // Note: allocate doulbe sized buffer, to handle rewind issue
    if( NULL == mBegin ) { GN_ERROR(sLogger)( "fail to allocate ring buffer." ); return Failure(); }
    mEnd = mBegin + mSize;
    mReadPtr = mWritePtr = mBegin;
    mNotFull = NewSyncEvent( false, true, NULL );  // initial unsignaled, auto-reset
    mNotEmpty = NewSyncEvent( false, true, NULL ); // initial unsignaled, auto-reset
    if( NULL == mNotFull || NULL == mNotEmpty )
    {
        GN_ERROR(sLogger)( "fail to create ring buffer sync events." );
        return Failure();
    }
    mQuit = false;

    // success
    return Success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::RingBuffer::Quit()
{
    GN_GUARD;

    delete mNotFull;
    delete mNotEmpty;
    HeapFree( mBegin );

    // standard Quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// Public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::RingBuffer::postQuitMessage()
{
    mQuit = true;
    if( mNotFull ) mNotFull->Signal();
    if( mNotEmpty ) mNotEmpty->Signal();
}

//
//
// -----------------------------------------------------------------------------
void *
GN::gfx::RingBuffer::beginProduce( size_t size )
{
    if( size > mSize )
    {
        GN_THROW( "size is too large to fit into ring buffer." );
    }

    while( true )
    {
        intptr_t freeSize = mReadPtr - mWritePtr;
        if( freeSize <= 0 ) freeSize += mSize;
        GN_ASSERT( freeSize > 0 );
        freeSize -= 1;

        if( (size_t)freeSize < size )
        {
            mNotFull->Wait();
            if( mQuit )
            {
                // received Quit message
                return NULL;
            }
        }
        else
        {
            GN_ASSERT( mBegin <= mWritePtr && (mWritePtr+size) <= mEnd+mSize );
            mPendingWriteSize = size;
            return mWritePtr;
        }
    }
}

//
//
// -----------------------------------------------------------------------------
void
GN::gfx::RingBuffer::endProduce()
{
    UInt8 * newWrite = mWritePtr + mPendingWriteSize;
    if( newWrite >= mEnd ) newWrite -= mSize;
    mWritePtr = newWrite;
    GN_ASSERT( mBegin <= mWritePtr && mWritePtr < mEnd );
    mNotEmpty->Signal();
}

//
//
// -----------------------------------------------------------------------------
void *
GN::gfx::RingBuffer::beginConsume( size_t size )
{
    if( size > mSize )
    {
        GN_THROW( "size is too large to fit into ring buffer." );
    }

    while( true )
    {
        intptr_t unconsumedSize = mWritePtr - mReadPtr;
        if( unconsumedSize < 0 ) unconsumedSize += mSize;
        GN_ASSERT( unconsumedSize >= 0 );

        if( (size_t)unconsumedSize < size )
        {
            mNotEmpty->Wait();
            if( mQuit )
            {
                // received Quit message
                return NULL;
            }
        }
        else
        {
            GN_ASSERT( mBegin <= mReadPtr && (mReadPtr+size) <= mEnd+mSize );
            mPendingReadSize = size;
            return mReadPtr;
        }
    }
}

//
//
// -----------------------------------------------------------------------------
void
GN::gfx::RingBuffer::endConsume()
{
    UInt8 * newRead = mReadPtr + mPendingReadSize;
    if( newRead >= mEnd ) newRead -= mSize;
    mReadPtr = newRead;
    GN_ASSERT( mBegin <= mReadPtr && mReadPtr < mEnd );
    mNotFull->Signal();
}
