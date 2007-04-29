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
    mResourceCommands.submit( command );
}
