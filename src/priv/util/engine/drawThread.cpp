#include "pch.h"
#include "drawThread.h"

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::engine::RenderEngine::DrawThread::init( UInt32 maxDrawCommandBufferBytes )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( DrawThread, () );

    // initialize draw buffers
    for( int i = 0; i < DRAW_BUFFER_COUNT; ++i )
    {
        mDrawBuffers[i].init( maxDrawCommandBufferBytes );
    }

    // create events and semaphores
    mIdle = createEvent( true, false ); // initial signaled, manual reset
    mDoSomething = createEvent( false, true ); // initial unsignaled, auto reset
    mDrawBufferNotFull  = createSemaphore( DRAW_BUFFER_COUNT, DRAW_BUFFER_COUNT, "DrawThread.DrawBufferNotFull" );
    if( !mIdle || !mDoSomething || !mDrawBufferNotFull ) return failure();

    // initial other data
    mQuitThread = 0;
    mResetRenderer = 0;
    mDoSomething = 0;

    // create thread
    mDrawThread = createThread( makeDelegate(this,&threadProc), 0 );
    if( 0 == mDrawThread ) return failure();

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::DrawThread::quit()
{
    GN_GUARD;

    if( mDrawThread )
    {
        // tell draw thread to exit
        mQuitThread = 1;
        mDoSomething->signal();

        // wait for end of draw thread
        mDrawThread->waitForTermination();

        safeDelete( mDrawThread );
    }

    safeDelete( mDrawBufferNotFull );
    safeDelete( mDoSomething );
    safeDelete( mIdle );

    for( int i = 0; i < DRAW_BUFFER_COUNT; ++i )
    {
        mDrawBuffers[i].quit();
    }

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::DrawThread::frameBegin()
{
    // currently do nothing
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::DrawThread::frameEnd()
{
    submitDrawBuffer();
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::DrawThread::submitDrawBuffer()
{
    // must not be called in draw thread
    GN_ASSERT( !mDrawThread->isCurrentThread() );

    // switch to next command buffer
    // note: this is the only place to modify write pointer
    mDrawBufferNotFull->wait();
    mDrawBufferMutex.lock();
    GN_ASSERT( ( ( mWritingIndex + 1 ) & (DRAW_BUFFER_COUNT-1) ) != mReadingIndex ); // make sure command buffer pool is not full.
    mWritingIndex = ( mWritingIndex + 1 ) & (DRAW_BUFFER_COUNT-1);
    mDrawBufferMutex.unlock();

    // wake up draw thread
    mDoSomething->signal();

    // reset the new write buffer
    mDrawBuffers[mWritingIndex].reset();
}

//
//
// -----------------------------------------------------------------------------
UInt32 GN::engine::RenderEngine::DrawThread::threadProc( void * )
{
    while( 1 )
    {
        // wait for something to do
        mDoSomething->wait();

        if( atomCmpXchg32( &mQuitThread, 1, 0 ) )
        {
            // quit thread
            mDrawBufferNotFull->wake(); // wake threads that are waiting for draw command submission
            return 0;
        }

        if( atomCmpXchg32( &mResetRenderer, 1, 0 ) )
        {
            // TODO: reset rendering device
            GN_UNIMPL;
        }

        // TODO: handle resource commands

        while( mReadingIndex != mWritingIndex )
        {
            // TODO: handle all resource and draw commands

            // Note that this is the only place that updates reading pointer
            mDrawBufferMutex.lock();
    		mReadingIndex = ( mReadingIndex + 1 ) & (DRAW_BUFFER_COUNT-1);
            mDrawBufferMutex.unlock();
            mDrawBufferNotFull->wake(); // wake up threads that are waiting for draw command submission.
        }
    }
}
