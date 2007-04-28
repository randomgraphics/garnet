#include "pch.h"
#include "drawThread.h"

static GN::Logger * sLogger = GN::getLogger("GN.engine.RenderEngine.DrawThread");

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
    mDoSomething = createSyncEvent( false, true ); // initial unsignaled, auto reset
    mDrawBufferEmpty = createSyncEvent( true, false ); // initial signaled, manual reset
    mDrawBufferNotFull  = createSemaphore( DRAW_BUFFER_COUNT, DRAW_BUFFER_COUNT, "DrawThread.DrawBufferNotFull" );
    if( !mDoSomething || !mDrawBufferEmpty || !mDrawBufferNotFull ) return failure();

    // initial other data
    mQuitDrawThread = false;
    mResetRenderer = false;
    mReadingIndex = 0;
    mWritingIndex = 0;
    mDrawFence = 0;

    // create thread
    mDrawThread = createThread(
        makeDelegate(this,&DrawThread::threadProc),
        0, // no parameter
        TP_NORMAL,
        false, // not suspended
        "RenderEngine.DrawThread" );
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

    // delete thread
    if( mDrawThread )
    {
        // tell draw thread to exit
        mQuitDrawThread = true;
        mDoSomething->signal();

        // wait for end of draw thread
        mDrawThread->waitForTermination();

        safeDelete( mDrawThread );
    }

    // delete sync objects
    safeDelete( mDoSomething );
    safeDelete( mDrawBufferEmpty );
    safeDelete( mDrawBufferNotFull );

    // delete draw buffers
    for( int i = 0; i < DRAW_BUFFER_COUNT; ++i )
    {
        mDrawBuffers[i].quit();
    }

    // delete all pending resource commands
    if( !mResourceCommands.empty() )
    {
        GN_WARN(sLogger)( "DrawThread shut down: drop %d unhandled resource commands.", mResourceCommands.size() );

        ResourceCommandItem * i1 = mResourceCommands.head(), * i2;
        while( i1 )
        {
            i2 = i1->next;

            mResourceCommands.remove( i1 );

            delete i1;

            i1 = i2;
        }

        GN_ASSERT( mResourceCommands.empty() );
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
    mDrawBufferEmpty->unsignal(); // block any thread that waits for idle.
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
    while( !mQuitDrawThread )
    {
        // wait for something to do
        mDoSomething->wait();

        if( mQuitDrawThread )
        {
            break;
        }

        if( mResetRenderer )
        {
            // TODO: reset rendering device
            GN_UNIMPL();
        }

        handleResourceCommands();

        while( !drawBufferEmpty() )
        {
            handleDrawCommands();

            // Note that this is the only place that updates reading pointer
            mDrawBufferMutex.lock();
    		mReadingIndex = ( mReadingIndex + 1 ) & (DRAW_BUFFER_COUNT-1);
            if( mReadingIndex == mWritingIndex ) mDrawBufferEmpty->signal(); // wake up thread waiting for idle.
            mDrawBufferMutex.unlock();
            mDrawBufferNotFull->wake(); // wake up threads that are waiting for draw command submission.
        }
    }

    // quit thread
    mDrawBufferNotFull->wake(); // wake threads that are waiting for draw command submission
    mDrawBufferEmpty->signal(); // wake up threads that are waiting for idle.
    return 0;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::DrawThread::handleDrawCommands()
{
    // make sure reading and writing operate on differnent buffer
    GN_ASSERT( !drawBufferEmpty() );

    gfx::Renderer & r = gRenderer;

    if( r.drawBegin() )
    {
        DrawBuffer & db = mDrawBuffers[mReadingIndex];

        const DrawCommand * command = (const DrawCommand*)db.buffer;

        const DrawCommand * last = (const DrawCommand*)db.next;

        while( command < last && !mQuitDrawThread )
        {
            // resource command has priority
            handleResourceCommands();

            if( 0 == command->getPendingResourceCount() )
            {
                // all resources are ready. do it!
                doDraw( *command );

                // update draw fence
                mDrawFence = command->fence;

                // next command
                ++command;
            }
            else
            {
                // sleep for a while, then repeat current command
                sleepCurrentThread( 0 );
            }
        }

        r.drawEnd();
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::DrawThread::handleResourceCommands()
{
    mResourceMutex.lock();
    ResourceCommandItem * item = mResourceCommands.head();
    mResourceMutex.unlock();

    ResourceCommandItem * removedItem;
    bool remove;

    while( item && !mQuitDrawThread )
    {
        remove = false;
        removedItem = NULL;

        if( GROP_DISPOSE == item->command.op )
        {
            remove = true;
        }
        else
        {
            // check draw fence
            if( item->command.waitForDrawFence < mDrawFence )
            {
                remove = true;
                switch( item->command.op )
                {
                    case GROP_LOAD :
                        GN_UNIMPL();
                        break;

                    case GROP_LOCK :
                        GN_UNIMPL();
                        break;

                    case GROP_UNLOCK :
                        GN_UNIMPL();
                        break;

                    case GROP_DISPOSE :
                        GN_UNIMPL();
                        break;

                    default:
                        GN_UNEXPECTED();
                        break;
                }
            }
        }

        // remove handled resource out of command buffer
        mResourceMutex.lock();
        if( remove )
        {
            removedItem = item;
            item = item->next;
            mResourceCommands.remove( removedItem );
        }
        else
        {
            item = item->next;
        }
        mResourceMutex.unlock();

        // delete removed command item
        // TODO: use memory pool
        delete removedItem;
    }
}


//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::DrawThread::doDraw( const DrawCommand & )
{
}
