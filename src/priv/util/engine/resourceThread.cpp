#include "pch.h"
#include "resourceThread.h"
#include "drawThread.h"

static GN::Logger * sLogger = GN::getLogger("GN.engine.RenderEngine.ResourceThread");

// *****************************************************************************
// SubThread class
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::engine::RenderEngine::ResourceThread::SubThread::init( const ThreadProcedure & proc, const char * name )
{
    commands.clear();

    GN_ASSERT( 0 == thread );
    thread = createThread( proc, &commands, TP_NORMAL, false, name );
    if( 0 == thread ) return false;

    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::ResourceThread::SubThread::quit()
{
    commands.clear();
    if( thread )
    {
        commands.postQuitEvent();
        thread->waitForTermination();
        safeDelete( thread );
    }
}

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::engine::RenderEngine::ResourceThread::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( ResourceThread, () );

    if( !mLoader.init(
            makeDelegate( this, &ResourceThread::loadstore ),
            "Loading thread" ) )
        return failure();

    if( !mDecompressor.init(
            makeDelegate( this, &ResourceThread::process ),
            "Decompressing thread" ) )
        return failure();

#if GN_XENON
    // set thread affinity on xenon platform
    mLoader.thread->setAffinity( 4 );
    mDecompressor.thread->setAffinity( 5 );
#endif

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::ResourceThread::quit()
{
    GN_GUARD;

    mLoader.quit();
    mDecompressor.quit();

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
void GN::engine::RenderEngine::ResourceThread::waitForIdle()
{
    while( !mLoader.commands.empty() || !mDecompressor.commands.empty() )
        sleepCurrentThread( 0 );
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
UInt32 GN::engine::RenderEngine::ResourceThread::loadstore( void * param )
{
    GN_SCOPE_PROFILER( RenderEngine_ResourceThread_load );

    GN_ASSERT( param );
    ResourceCommandBuffer * commands = (ResourceCommandBuffer*)param;

    ResourceCommand * cmd;

    for(;;)
    {
        {
            GN_SCOPE_PROFILER( RenderEngine_ResourceThread_load_idle );
            cmd = commands->consumeBegin();
        }

        if( NULL == cmd ) break;

        GN_ASSERT( cmd->loadstore );

        switch( cmd->op )
        {
            case GROP_LOAD:
            {
                if( cmd->noerr )
                {
                    cmd->noerr = cmd->loadstore->load( cmd->resource->desc, cmd->tmpbuf );
                }
                // push to process thread for decompress
                cmd->op = GROP_DECOMPRESS;
                submitResourceCommand( cmd );
                break;
            }

            case GROP_STORE:
            {
                if( cmd->noerr )
                {
                    cmd->loadstore->store( cmd->resource->desc, cmd->tmpbuf );
                }
                // This is the last command, free the loader
                ResourceCommand::free( cmd );
                break;
            }

            default:
                GN_UNEXPECTED();
                break;
        }

        commands->consumeEnd();
    }

    return 0;
}

//
//
// -----------------------------------------------------------------------------
UInt32 GN::engine::RenderEngine::ResourceThread::process( void * param )
{
    GN_SCOPE_PROFILER( RenderEngine_ResourceThread_decompress );

    GN_ASSERT( param );
    ResourceCommandBuffer * commands = (ResourceCommandBuffer*)param;

    ResourceCommand * cmd;

    for(;;)
    {
        {
            GN_SCOPE_PROFILER( RenderEngine_ResourceThread_decompress_idle );
            cmd = commands->consumeBegin();
        }

        if( 0 == cmd ) break;

        GN_ASSERT( cmd->noerr );
        GN_ASSERT( cmd->loadstore );

        DynaArray<UInt8> processed;

        switch( cmd->op )
        {
            case GROP_DECOMPRESS:
            {
                if( cmd->noerr )
                {
                    cmd->noerr = cmd->loadstore->decompress( cmd->resource->desc, processed, cmd->tmpbuf );
                    cmd->tmpbuf.swap( processed );
                }
                // push to draw thread for download
                cmd->op = GROP_DOWNLOAD;
                mEngine.drawThread().submitResourceCommand( cmd );
                break;
            }

            case GROP_COMPRESS:
                if( cmd->noerr )
                {
                    cmd->noerr = cmd->loadstore->decompress( cmd->resource->desc, processed, cmd->tmpbuf );
                    cmd->tmpbuf.swap( processed );
                }
                // push to load thread for store
                cmd->op = GROP_STORE;
                submitResourceCommand( cmd );
                break;

            default:
                GN_UNEXPECTED();
        }

        commands->consumeEnd();
    }

    return 0;
}
