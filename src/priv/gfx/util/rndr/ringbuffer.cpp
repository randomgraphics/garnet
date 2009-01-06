#include "pch.h"
#include "ringbuffer.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.util.rndr.RingBuffer");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::RingBuffer::init( size_t ringBufferSize )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::RingBuffer, () );

    mSize  = ringBufferSize;
    mBegin = (UInt8*)heapAlloc( mSize );
    if( NULL == mBegin ) { GN_ERROR(sLogger)( "fail to allocate ring buffer." ); return failure(); }
    mEnd = mBegin + mSize;
    mReadPtr = mWritePtr = mBegin;
    mNotFull = createSyncEventGroup( 2, false, true, NULL );  // initial unsignaled, auto-reset
    mNotEmpty = createSyncEventGroup( 2, false, true, NULL ); // initial unsignaled, auto-reset
    if( NULL == mNotFull || NULL == mNotEmpty )
    {
        GN_ERROR(sLogger)( "fail to create ring buffer sync events." );
        return failure();
    }

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::RingBuffer::quit()
{
    GN_GUARD;

    delete mNotFull;
    delete mNotEmpty;
    heapFree( mBegin );

    // standard quit procedure
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
    if( mNotFull ) mNotFull->signal( 1 );
    if( mNotEmpty ) mNotEmpty->signal( 1 );
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
        int freeSize = mReadPtr - mWritePtr;
        if( freeSize <= 0 ) freeSize += mSize;
        GN_ASSERT( freeSize > 0 );
        freeSize -= 1;

        if( (size_t)freeSize < size )
        {
            int idx = mNotFull->waitAny();
            if( 1 == idx )
            {
                // received quit message
                return NULL;
            }
            GN_ASSERT( 0 == idx );
        }
        else
        {
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
    mNotEmpty->signal( 0 );
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
        int unconsumedSize = mWritePtr - mReadPtr;
        if( unconsumedSize < 0 ) unconsumedSize += mSize;
        GN_ASSERT( unconsumedSize >= 0 );

        if( (size_t)unconsumedSize < size )
        {
            int idx = mNotEmpty->waitAny();
            if( 1 == idx )
            {
                // received quit message
                return NULL;
            }
            GN_ASSERT( 0 == idx );
        }
        else
        {
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
    mNotFull->signal( 0 );
}
