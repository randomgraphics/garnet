#include "pch.h"
#include "drawThread.h"
#include "garnet/GNinput.h"

static GN::Logger * sLogger = GN::getLogger("GN.engine2.RenderEngine.DrawThread");

using namespace GN::engine2;
using namespace GN::gfx;

// *****************************************************************************
// draw thread dumper
// *****************************************************************************

#define DUMP_DRAW_THREAD_COMMANDS 0

class DrawThreadDumper
{
    GN::File * mFile;

    static const char * sDrawCommandType2String( int cmd )
    {
        static const char * table[] =
        {
            "DRAW",
            "PRESENT",
        };

        if( 0 <= cmd && cmd < GN::engine2::NUM_DRAW_COMMAND_TYPES )
            return table[cmd];
        else
            return "INVALID_DRAW_COMMAND";
    }

public:

    DrawThreadDumper()
    {
        if( DUMP_DRAW_THREAD_COMMANDS )
        {
            mFile = GN::core::openFile( "drawthread.xml", "wt" );
        }
        else
        {
            mFile = 0;
        }

        if( mFile )
        {
            mFile->print(
                "<?xml version=\"1.0\" standalone=\"yes\"?>\n"
                "<DrawThreadDump>\n" );
        }
    }

    ~DrawThreadDumper()
    {
        if( mFile )
        {
            mFile->print( "</DrawThreadDump>" );

            delete mFile;
        }
    }

    void dumpString( const char * fmt, ... )
    {
        GN::StrA s;

        va_list arglist;
        va_start( arglist, fmt );
        s.formatv( fmt, arglist );
        va_end( arglist );

        mFile->print( s );
        mFile->print( "\n" );
    }

    void dumpCommandOpenTag( GN::engine2::DrawCommandHeader & cmd )
    {
        using namespace GN;

        if( 0 == mFile ) return;

        mFile->print( strFormat( "\t<%s fence=\"%d\">\n",
            sDrawCommandType2String( cmd.type ),
            cmd.fence ) );

        mFile->print( strFormat( "\t\t<resource_waiting_list count=\"%u\">\n", cmd.resourceWaitingCount ) );

        for( size_t i = 0; i < cmd.resourceWaitingCount; ++i )
        {
            const GN::engine2::DrawCommandHeader::ResourceWaitingItem & rwi = cmd.resourceWaitingList[i];

            mFile->print( strFormat( "\t\t\t<rwi fence=\"%d\">%s</rwi>\n",
                rwi.waitForUpdate,
                GraphicsResource::sToString(rwi.resource).cptr() ) );
        }

        mFile->print( "\t\t</resource_waiting_list>\n" );
    }

    void dumpCommandEndTag( GN::engine2::DrawCommandHeader & cmd )
    {
        if( 0 == mFile ) return;

        mFile->print( GN::strFormat( "\t</%s>\n", sDrawCommandType2String( cmd.type ) ) );
    }
};

static DrawThreadDumper sDumper;

// *****************************************************************************
// local functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static bool sCreateDeviceData( GraphicsSystem & gs, GN::engine2::GraphicsResource & res )
{
    const GraphicsResourceDesc & desc = res.desc;

    switch( desc.type )
    {
        case GRT_SURFACE :
        {
            GN_ASSERT( 0 == res.surface );

            res.surface = gs.createSurface( desc.surface.creation );
            if( 0 == res.surface ) return false;

            break;
        }

        case GRT_PARAMETER_SET :
        {
            GN_ASSERT( 0 == res.paramset );

            Kernel * kernel = gs.getKernel( desc.paramset.kernel );
            if( 0 == kernel ) return false;

            res.paramset = kernel->createParameterSet();
            if( 0 == res.paramset ) return false;

            break;
        }

        case GRT_PORT_BINDING :
        {
            GN_ASSERT( 0 == res.binding );

            Kernel * kernel = gs.getKernel( desc.binding.kernel );
            if( 0 == kernel ) return false;

            KernelPortBindingDesc pbd;

            std::map<GN::StrA,SurfaceResourceView>::const_iterator iter = desc.binding.views.begin();
            for( ; iter != desc.binding.views.end(); ++iter )
            {
                SurfaceView & view = pbd.views[iter->first];
                view.surf       = iter->second.surf ? iter->second.surf->surface : 0,
                view.firstLevel = iter->second.firstLevel;
                view.numLevels  = iter->second.numLevels;
                view.firstFace  = iter->second.firstFace;
                view.numFaces   = iter->second.numFaces;
            }

            res.binding = kernel->createPortBinding( pbd );
            if( 0 == res.binding ) return false;

            break;
        }

        case GRT_KERNEL :
        {
            GN_ASSERT( 0 == res.kernel );

            res.kernel = gs.getKernel( desc.kernel.kernel );
            if( 0 == res.kernel ) return false;

            break;
        }

        default:
            GN_UNEXPECTED();
            return false;
    }

    return true;
}
// *****************************************************************************
// draw command functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static void DRAWFUNC_DRAW( RenderEngine &, const void * param, size_t )
{
    GN_GUARD_SLOW;

    struct Param
    {
        GraphicsResource * kernel;
        GraphicsResource * paramset;
        GraphicsResource * binding;
    };

    const Param * p = (const Param *)param;

    GN_ASSERT( p );
    GN_ASSERT( p->kernel && GRT_KERNEL == p->kernel->desc.type && p->kernel->kernel );
    GN_ASSERT( p->paramset && GRT_PARAMETER_SET == p->paramset->desc.type && p->paramset->paramset );

    p->kernel->kernel->render( *p->paramset->paramset, p->binding ? p->binding->binding : 0 );

    if( DUMP_DRAW_THREAD_COMMANDS )
    {
        sDumper.dumpString( "kernel=\"%s\"", p->kernel->desc.kernel.kernel.cptr() );
    }

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
static void DRAWFUNC_PRESENT( RenderEngine & re, const void *, size_t )
{
    GN_GUARD_SLOW;

    re.drawThread().getGraphicsSystem()->present();

#if GN_PROFILE_ENABLED
    re.drawThread().profileFrameTime();
#endif

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// resource command functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void RESFUNC_COPY( RenderEngine & engine, ResourceCommand & cmd )
{
    GN_GUARD;

    GN_ASSERT( engine.resourceCache().checkResource( cmd.resource ) );

    if( 0 == cmd.resource->data )
    {
        if( !sCreateDeviceData( *engine.drawThread().getGraphicsSystem(), *cmd.resource ) )
        {
            cmd.noerr = false;

            // free data loader
            cmd.loader.clear();
            return;
        }
    }

    GN_ASSERT( cmd.loader );
    cmd.noerr = cmd.loader->copy( *cmd.resource, cmd.tmpbuf );

    cmd.loader.clear();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void RESFUNC_DISPOSE( RenderEngine & engine, ResourceCommand & cmd )
{
    GN_GUARD;

    GN_UNUSED_PARAM( engine );
    GN_ASSERT( engine.resourceCache().checkResource( cmd.resource ) );

    switch( cmd.resource->desc.type )
    {
        case GRT_SURFACE :
            safeDelete( cmd.resource->surface );
            break;

        case GRT_PARAMETER_SET :
            safeDelete( cmd.resource->paramset );
            break;

        case GRT_PORT_BINDING :
            safeDelete( cmd.resource->binding );
            break;

        case GRT_KERNEL :
            // no need to delete kernel
            cmd.resource->kernel = 0;
            break;

        default:
            GN_UNEXPECTED();
    }

    GN_UNGUARD;
}

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::engine2::RenderEngine::DrawThread::init( size_t maxDrawCommandBufferBytes )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( DrawThread, () );

    // initialize draw function pointers
    memset( &mDrawFunctions, 0, sizeof(mDrawFunctions) );
    mDrawFunctions[DCT_DRAW]            = &DRAWFUNC_DRAW;
    mDrawFunctions[DCT_PRESENT]         = &DRAWFUNC_PRESENT;
    if( GN_ASSERT_ENABLED )
    {
        for( int i = 0; i < NUM_DRAW_COMMAND_TYPES; ++i )
        {
            GN_ASSERT( mDrawFunctions[i] );
        }
    }

    // initialize draw buffers
    for( int i = 0; i < DRAW_BUFFER_COUNT; ++i )
    {
        mDrawBuffers[i].init( maxDrawCommandBufferBytes );
    }

    // create events and semaphores
    mAction = createSyncEventGroup( NUM_ACTIONS, false, true ); // initial unsignaled, auto reset
    mResetRendererComplete = createSyncEvent( false, true ); // initial unsignaled, auto reset
    mDrawBufferEmpty = createSyncEvent( true, false ); // initial signaled, manual reset
    mDrawBufferNotFull  = createSemaphore( DRAW_BUFFER_COUNT-1, DRAW_BUFFER_COUNT-1, "DrawThread.DrawBufferNotFull" );
    if( !mAction || !mDrawBufferEmpty || !mDrawBufferNotFull ) return failure();

    // initial other data
    mReadingIndex = 0;
    mWritingIndex = 0;
    mCompletedResourceFence = 0;
    mCompletedDrawFence = 0;

    // create thread
    mDrawThread = createThread(
        makeDelegate(this,&DrawThread::threadProc),
        0, // no parameter
        TP_NORMAL,
        false, // not suspended
        "RenderEngine.DrawThread" );
    if( 0 == mDrawThread ) return failure();

#if GN_XENON
    // set thread affinity on xenon platform
    mDrawThread->setAffinity( 2 );
#endif

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine2::RenderEngine::DrawThread::quit()
{
    GN_GUARD;

    // delete thread
    if( mDrawThread )
    {
        // tell draw thread to exit
        mAction->signal( QUIT_ACTION );

        // wait for end of draw thread
        mDrawThread->waitForTermination();

        safeDelete( mDrawThread );
    }

    // delete sync objects
    safeDelete( mAction );
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

        ResourceCommand * i1 = mResourceCommands.head(), * i2;
        while( i1 )
        {
            i2 = i1->next;

            mResourceCommands.remove( i1 );

            ResourceCommand::free( i1 );

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
bool GN::engine2::RenderEngine::DrawThread::resetGraphicsSystem( const gfx::GraphicsSystemCreationParameter & gscp )
{
    mNewGraphicsSystemCreationParameter = gscp;
    mAction->signal( RESET_ACTION );
    if( !mResetRendererComplete->wait() ) return false;
    return mResetSuccess;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine2::RenderEngine::DrawThread::waitForIdle( float time )
{
    // flush pending draw commands
    flushDrawBuffer();

    if(mDrawBufferEmpty) mDrawBufferEmpty->wait( time );

    while( !mResourceCommandEmpty )
    {
        sleepCurrentThread( 0 );
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::engine2::RenderEngine::DrawThread::waitForResource( GraphicsResourceItem * item )
{
    flushDrawBuffer();

    while( item->lastCompletedFence < item->lastSubmissionFence )
    {
        sleepCurrentThread( 0 );
    }
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::engine2::RenderEngine::DrawThread::flushDrawBuffer()
{
    // must not be called in draw thread
    GN_ASSERT( !mDrawThread->isCurrentThread() );

    // switch to next command buffer
    // note: this is the only place to modify write pointer
    {
        GN_SCOPE_PROFILER( RenderEngine_frame_idle );
        mDrawBufferNotFull->wait();
        mDrawBufferMutex.lock();
    }
    GN_ASSERT( ( ( mWritingIndex + 1 ) & (DRAW_BUFFER_COUNT-1) ) != mReadingIndex ); // make sure command buffer pool is not full.
    mWritingIndex = ( mWritingIndex + 1 ) & (DRAW_BUFFER_COUNT-1);
    mDrawBufferEmpty->unsignal(); // block any thread that waits for idle.
    mDrawBufferMutex.unlock();

    // wake up draw thread
    mAction->signal( DRAW_ACTION );

    // reset the new write buffer
    mDrawBuffers[mWritingIndex].reset();
}

//
//
// -----------------------------------------------------------------------------
UInt32 GN::engine2::RenderEngine::DrawThread::threadProc( void * )
{
    GN_SCOPE_PROFILER( RenderEngine_DrawThread_all );

    for(;;)
    {
        // wait for something to do
        int action;
        {
            GN_SCOPE_PROFILER( RenderEngine_DrawThread_idle );
            action = mAction->waitAny();
        }

        if( QUIT_ACTION == action )
        {
            break;
        }

        else if( RESET_ACTION == action )
        {
            mResetSuccess = doGraphicsSystemReset();
            mResetRendererComplete->signal();
        }

        handleResourceCommands();

        mDrawBufferMutex.lock();
        bool empty = mReadingIndex == mWritingIndex;
        mDrawBufferMutex.unlock();

        while( !empty )
        {
            handleDrawCommands();

            // handle resource command again
            handleResourceCommands();

            // Note that this is the only place that updates reading pointer
            mDrawBufferMutex.lock();
    		mReadingIndex = ( mReadingIndex + 1 ) & (DRAW_BUFFER_COUNT-1);
            empty = mReadingIndex == mWritingIndex;
            if( empty ) mDrawBufferEmpty->signal(); // wake up thread waiting for idle.
            mDrawBufferMutex.unlock();
            mDrawBufferNotFull->wake(); // wake up threads that are waiting for draw command submission.
        }
    }

    // destroy graphics system
    mGraphicsSystemCreater.destroy();

    // quit thread
    return 0;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine2::RenderEngine::DrawThread::handleDrawCommands()
{
    DrawBuffer & db = mDrawBuffers[mReadingIndex];

    DrawCommandHeader * command = (DrawCommandHeader*)db.buffer;

    DrawCommandHeader * end = (DrawCommandHeader*)db.next;

    while( command < end )
    {
        // resource command has priority
        handleResourceCommands();

        // update command's resource waiting list
        int count = (int)command->resourceWaitingCount;
        GN_ASSERT( count >= 0 );
        for( int i = count - 1; i >= 0; --i )
        {
            DrawCommandHeader::ResourceWaitingItem & wi = command->resourceWaitingList[i];
            GN_ASSERT( mEngine.resourceCache().checkResource( wi.resource ) );
            if( wi.resource->lastCompletedFence >= wi.waitForUpdate )
            {
                // remove from waiting list
                if( (i+1) < count )
                {
                    memcpy(
                        &command->resourceWaitingList[i],
                        &command->resourceWaitingList[i+1],
                        sizeof(wi) * (count - ( i + 1 )) );
                }
                GN_ASSERT( count > 0 );
                --count;
            }
        }
        GN_ASSERT( count >= 0 );
        command->resourceWaitingCount = (UInt32)count;

        if( 0 == command->resourceWaitingCount )
        {
            if( DUMP_DRAW_THREAD_COMMANDS )
            {
                sDumper.dumpCommandOpenTag( *command );
            }

            // all resources are ready. do it!
            GN_ASSERT( mDrawFunctions[command->type] );
            mDrawFunctions[command->type]( mEngine, command->param(), command->bytes - sizeof(DrawCommandHeader) );

            if( DUMP_DRAW_THREAD_COMMANDS )
            {
                sDumper.dumpCommandEndTag( *command );
            }

            // update draw fence
            mCompletedDrawFence = command->fence;

            // next command
            command = command->next();
            GN_ASSERT( command <= end );
        }
        else
        {
            GN_SCOPE_PROFILER( RenderEngine_DrawThread_wait_for_resources );

            // sleep for a while, then repeat current command
            sleepCurrentThread( 0 );
        }
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::engine2::RenderEngine::DrawThread::handleResourceCommands()
{
    bool loopAgain;
    do
    {
        loopAgain = false;

        mResourceMutex.lock();
        ResourceCommand * cmd = mResourceCommands.head();
        mResourceMutex.unlock();

        ResourceCommand * prev;

        bool postponed = false;

        while( cmd )
        {
            GN_ASSERT( mEngine.resourceCache().checkResource( cmd->resource ) );

            // process the resource command
            if( cmd->mustAfterThisDrawFence <= mCompletedDrawFence &&
                cmd->mustAfterThisResourceFence <= cmd->resource->lastCompletedFence )
            {
                // remove it from resource command buffer
                mResourceMutex.lock();
                prev = cmd;
                cmd = cmd->next;
                mResourceCommands.remove( prev );
                mResourceMutex.unlock();

                if( prev->noerr )
                {
                    switch( prev->op )
                    {
                        case GROP_COPY :
                            RESFUNC_COPY( mEngine, *prev );
                            break;

                        case GROP_DISPOSE :
                            RESFUNC_DISPOSE( mEngine, *prev );
                            break;

                        default:
                            GN_UNEXPECTED();
                            break;
                    }
                }

                // update resource's complete fence
                prev->resource->lastCompletedFence = prev->submittedAtThisFence;

                // update resource fence
                mCompletedResourceFence = prev->submittedAtThisFence;

                // the resource command is done. Free it.
                ResourceCommand::free( prev );

                if( mResourceCommands.empty() ) mResourceCommandEmpty = true;

                loopAgain = true;
            }
            else
            {
                GN_SCOPE_PROFILER( RenderEngine_DrawThread_resource_postponed );

                postponed = true;

                mResourceMutex.lock();
                cmd = cmd->next;
                mResourceMutex.unlock();
            }
        }
    }while( loopAgain );
}

//
//
// -----------------------------------------------------------------------------
bool GN::engine2::RenderEngine::DrawThread::doGraphicsSystemReset()
{
    GN_GUARD;

    if( 0 == mGraphicsSystemCreater.get() ||
        mNewGraphicsSystemCreationParameter != mGraphicsSystemCreationParameter )
    {
        if( !mGraphicsSystemCreater.create( mNewGraphicsSystemCreationParameter ) ) return false;

        mGraphicsSystemCreationParameter = mNewGraphicsSystemCreationParameter;
        mGraphicsSystemDesc              = mGraphicsSystemCreater.get()->getDesc();

        if( gInputPtr )
        {
            gInput.attachToWindow( mGraphicsSystemDesc.display, mGraphicsSystemDesc.window );
        }
    }

    return true;

    GN_UNGUARD;
}
