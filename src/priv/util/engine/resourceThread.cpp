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
    GN_ASSERT( param );
    ResourceCommandBuffer * commands = (ResourceCommandBuffer*)param;

    ResourceCommand * cmd;

    while( NULL != ( cmd = commands->get() ) )
    {
        GN_ASSERT( GROP_LOAD == cmd->op );
        GN_ASSERT( cmd->loader );

        GN_INFO(sLogger)( "Load %s", mEngine.resourceCache().id2name(cmd->resourceId).cptr() );

        cmd->noerr = cmd->loader->load( cmd->data, cmd->bytes, cmd->targetLod );

        // load done. push it to decompress thread
        cmd->op = GROP_DECOMPRESS;
        submitResourceCommand( cmd );
    }

    return 0;
}

//
//
// -----------------------------------------------------------------------------
UInt32 GN::engine::RenderEngine::ResourceThread::decompress( void * param )
{
    GN_ASSERT( param );
    ResourceCommandBuffer * commands = (ResourceCommandBuffer*)param;

    ResourceCommand * cmd;

    while( NULL != ( cmd = commands->get() ) )
    {
        GN_ASSERT( GROP_DECOMPRESS == cmd->op );
        GN_ASSERT( cmd->loader );
        void * olddata = cmd->data;
        size_t oldbytes = cmd->bytes;

        GN_INFO(sLogger)( "Decompress %s", mEngine.resourceCache().id2name(cmd->resourceId).cptr() );

        cmd->noerr = cmd->loader->decompress( cmd->data, cmd->bytes, olddata, oldbytes, cmd->targetLod );

        // decompress done, delete loaded data,
        cmd->loader->freebuf( olddata, oldbytes );

        // push it to draw thread for copy
        // TODO: What happens, if there's multiple decompress threads, which means that 
        // TODO: resource commands submitted in later in render engine may submit to
        // TODO: draw thread earlier.
        cmd->op = GROP_COPY;
        mEngine.drawThread().submitResourceCommand( cmd );
    }

    return 0;
}
