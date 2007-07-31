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
            makeDelegate( this, &ResourceThread::load ),
            "Loading thread" ) )
        return failure();

    if( !mDecompressor.init(
            makeDelegate( this, &ResourceThread::decompress ),
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
UInt32 GN::engine::RenderEngine::ResourceThread::load( void * param )
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

        GN_ASSERT( GROP_LOAD == cmd->op );
        GN_ASSERT( cmd->loader );

        cmd->noerr = cmd->loader->load( cmd->resource->desc, cmd->tmpbuf );

        // load done. push it to decompress thread
        cmd->op = GROP_DECOMPRESS;
        submitResourceCommand( cmd );

        commands->consumeEnd();
    }

    return 0;
}

//
//
// -----------------------------------------------------------------------------
UInt32 GN::engine::RenderEngine::ResourceThread::decompress( void * param )
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

        if( cmd->noerr )
        {
            GN_ASSERT( GROP_DECOMPRESS == cmd->op );
            GN_ASSERT( cmd->loader );

            DynaArray<UInt8> decompressed;

            cmd->noerr = cmd->loader->decompress( cmd->resource->desc, decompressed, cmd->tmpbuf );

            // swap data decomparessed data into command's temp buffer
            cmd->tmpbuf.swap( decompressed );
        }

        // push it to draw thread for copy
        // TODO: What happens, if there's multiple decompress threads, which means that
        // TODO: resource commands submitted in later in render engine may submit to
        // TODO: draw thread earlier.
        cmd->op = GROP_COPY;
        mEngine.drawThread().submitResourceCommand( cmd );

        commands->consumeEnd();
    }

    return 0;
}
