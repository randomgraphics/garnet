#include "pch.h"
#include "drawThread.h"
#include "garnet/GNinput.h"
#include "garnet/GNwin.h"

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
    mResetRendererComplete = createSyncEvent( false, true ); // initial unsignaled, auto reset
    mDrawBufferEmpty = createSyncEvent( true, false ); // initial signaled, manual reset
    mDrawBufferNotFull  = createSemaphore( DRAW_BUFFER_COUNT-1, DRAW_BUFFER_COUNT-1, "DrawThread.DrawBufferNotFull" );
    if( !mDoSomething || !mDrawBufferEmpty || !mDrawBufferNotFull ) return failure();

    // initial other data
    mActionQuit = false;
    mActionReset = false;
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
        mActionQuit = true;
        mDoSomething->signal();

        // wait for end of draw thread
        mDrawThread->waitForTermination();

        safeDelete( mDrawThread );
    }

    // delete sync objects
    safeDelete( mDoSomething );
    safeDelete( mResetRendererComplete );
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
        GN_WARN(sLogger)( "DrawThread shut down: drop unhandled resource commands." );

        ResourceCommandItem * i1 = mResourceCommands.head(), * i2;
        while( i1 )
        {
            i2 = i1->next;

            mResourceCommands.remove( i1 );

            ResourceCommandItem::free( i1 );

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
bool GN::engine::RenderEngine::DrawThread::resetRenderer(
     gfx::RendererAPI api,
     const gfx::RendererOptions & ro )
{
    mActionReset = true;
    mRendererApi = api;
    mRendererOptions = ro;
    mDoSomething->signal();

    if( !mResetRendererComplete->wait() ) return false;

    return mResetSuccess;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::DrawThread::frameBegin()
{
    // do nothing
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
    while( !mActionQuit )
    {
        // wait for something to do
        mDoSomething->wait();

        if( mActionQuit )
        {
            mActionQuit = false;
            break;
        }

        if( mActionReset )
        {
            mActionReset = false;
            mResetSuccess = doDeviceReset();
            mResetRendererComplete->signal();
        }

        handleResourceCommands();

        mDrawBufferMutex.lock();
        bool empty = mReadingIndex == mWritingIndex;
        mDrawBufferMutex.unlock();

        while( !empty )
        {
            handleDrawCommands();

            // Note that this is the only place that updates reading pointer
            mDrawBufferMutex.lock();
    		mReadingIndex = ( mReadingIndex + 1 ) & (DRAW_BUFFER_COUNT-1);
            empty = mReadingIndex == mWritingIndex;
            if( empty ) mDrawBufferEmpty->signal(); // wake up thread waiting for idle.
            mDrawBufferMutex.unlock();
            mDrawBufferNotFull->wake(); // wake up threads that are waiting for draw command submission.
        }
    }

    // dispose all resources
    mEngine.resourceCache().deleteAllDeviceData();

    // delete Renderer
    GN::gfx::deleteRenderer();

    // quit thread
    return 0;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::DrawThread::handleDrawCommands()
{
    gfx::Renderer & r = gRenderer;

    // process windows messages
    GN::win::processWindowMessages( r.getDispDesc().windowHandle, true );

    if( r.drawBegin() )
    {
        DrawBuffer & db = mDrawBuffers[mReadingIndex];

        DrawCommand * command = (DrawCommand*)db.buffer;

        DrawCommand * last = (DrawCommand*)db.next;

        while( command < last && !mActionQuit )
        {
            // resource command has priority
            handleResourceCommands();

            // update command's resource waiting list
            int count = (int)command->resourceWaitingCount;
            GN_ASSERT( count >= 0 );
            for( int i = count - 1; i >= 0; --i )
            {
                DrawCommand::ResourceWaitingItem & wi = command->resourceWaitingList[i];
                GraphicsResourceItem * res = mEngine.resourceCache().id2ptr( wi.id );
                if( res->lastCompletedFence >= wi.waitForUpdate )
                {
                    // remove from waiting list
                    if( (i+1) < count )
                    {
                        memcpy(
                            &command->resourceWaitingList[i],
                            &command->resourceWaitingList[i+1],
                            count - ( i + 1 ) );
                    }
                    GN_ASSERT( count > 0 );
                    --count;
                }
            }
            GN_ASSERT( count >= 0 );
            command->resourceWaitingCount = (UInt32)count;

            if( 0 == command->resourceWaitingCount )
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

    ResourceCommandItem * prev;

    while( item && !mActionQuit )
    {
        // process the resource command
        if( item->command.mustAfterThisFence <= mDrawFence )
        {
            // remove it from resource command buffer
            mResourceMutex.lock();
            prev = item;
            item = item->next;
            mResourceCommands.remove( prev );
            mResourceMutex.unlock();

            GraphicsResourceItem * res = mEngine.resourceCache().id2ptr( prev->command.resourceId );

            // update resource's complete fence
            res->lastCompletedFence = prev->command.submittedAtThisFence;

            if( prev->noerr )
            {
                switch( prev->command.op )
                {
                    case GROP_COPY :
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

            // the resource command is done. Free it.
            prev->command.loader->freebuf( prev->data, prev->bytes );
            ResourceCommandItem::free( prev );
        }
        else
        {
            // leave it in buffer, continue search.
            mResourceMutex.lock();
            item = item->next;
            mResourceMutex.unlock();
        }
    }
}

//
//
// -----------------------------------------------------------------------------
bool GN::engine::RenderEngine::DrawThread::doDeviceReset()
{
    GN_GUARD;

    // delete all device data in resource cache
    mEngine.resourceCache().deleteAllDeviceData();

    // (re)create renderer
    GN::gfx::Renderer * r = gfx::createRenderer( mRendererApi );
    if( NULL == r ) return false;
    if( !r->changeOptions( mRendererOptions ) ) return false;

    mDispDesc = r->getDispDesc();

    // reattach input window
    if( gInputPtr && !gInput.attachToWindow( mDispDesc.displayHandle, mDispDesc.windowHandle ) )
    {
        return false;
    }

    // success
    return true;

    GN_UNGUARD;
    
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::DrawThread::doDraw( const DrawCommand & cmd )
{
    gfx::Renderer & r = gRenderer;

    switch( cmd.type )
    {
        case DCT_DRAW_INDEXED :
            break;

        case DCT_SET_CONTEXT :
            break;

        case DCT_DRAW :
            break;

        case DCT_CLEAR:
            r.clearScreen( cmd.clear.color(), cmd.clear.z, cmd.clear.s, cmd.clear.flags );
            break;

        default :
            GN_UNEXPECTED();
    }
}
