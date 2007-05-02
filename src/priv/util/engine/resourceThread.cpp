#include "pch.h"
#include "resourceThread.h"
#include "drawThread.h"

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

    // Do custom init here

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

    ResourceCommandItem * item;

    while( NULL != ( item = commands->get() ) )
    {
        GN_ASSERT( GROP_LOAD == item->command.op );
        GN_ASSERT( item->command.loader );

        item->noerr = item->command.loader->load( item->data, item->bytes, item->command.targetLod );

        // load done. push it to decompress thread
        item->command.op = GROP_DECOMPRESS;
        submitResourceCommand( item );
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

    ResourceCommandItem * item;

    while( NULL != ( item = commands->get() ) )
    {
        GN_ASSERT( GROP_DECOMPRESS == item->command.op );
        GN_ASSERT( item->command.loader );
        void * olddata = item->data;
        size_t oldbytes = item->bytes;

        item->noerr = item->command.loader->decompress( item->data, item->bytes, olddata, oldbytes, item->command.targetLod );

        // decompress done, delete loaded data,
        item->command.loader->freebuf( olddata, oldbytes );

        // push it to draw thread for copy
        // TODO: What happens, if there's multiple decompress threads, which means that 
        // TODO: resource commands submitted in later in render engine may submit to
        // TODO: draw thread earlier.
        item->command.op = GROP_COPY;
        mEngine.drawThread().submitResourceCommand( item );
    }

    return 0;
}
