//
//
// -----------------------------------------------------------------------------
inline GN::engine::DrawCommand &
GN::engine::RenderEngine::DrawThread::newDrawCommand()
{
    DrawBuffer & db = mDrawBuffers[mWritingIndex];

    if( db.rooms() < sizeof(DrawCommand) )
    {
        static Logger * sLogger = getLogger("GN.engine.RenderEngine.DrawThread");
        GN_ERROR(sLogger)( "No enough room in draw buffer to hold more drawing commands!" );

        // return last command
        GN_ASSERT( !db.empty() );
        DrawCommand * dc = (DrawCommand*)( db.next - sizeof(DrawCommand) );
        return *dc;
    }
    else
    {
        DrawCommand * dc = (DrawCommand*)db.next;
        db.next += sizeof(DrawCommand);
        GN_ASSERT( db.next <= db.end );
        return *dc;
    }
}

//
//
// -----------------------------------------------------------------------------
inline void GN::engine::RenderEngine::DrawThread::submitResourceCommand(
     const GraphicsResourceCommand & command )
{
    // allocate new resource command item
    AutoObjPtr<ResourceCommandItem> item( new ResourceCommandItem );
    item->command = command;

    // append to resource command list
    mResourceMutex.lock();
    mResourceCommands.append( item.detach() );
    mResourceMutex.unlock();
}

//
//
// -----------------------------------------------------------------------------
inline void GN::engine::RenderEngine::DrawThread::dispose(
    const GraphicsResourceItem * disposeList )
{
    GraphicsResourceCommand cmd;
    cmd.op = GROP_DISPOSE;
    while( disposeList )
    {
        cmd.resourceId = disposeList->id;
        submitResourceCommand( cmd );
        disposeList = disposeList->nextItemToDispose;
    }
}
