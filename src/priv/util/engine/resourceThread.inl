//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::ResourceThread::submitResourceCommand(
    const GraphicsResourceCommand & cmd )
{
    switch( cmd.op )
    {
        case GROP_LOAD:
            mLoador.commands.submit( cmd );
            break;

        case GROP_DECOMPRESS:
            mDecompressor.commands.submit( cmd );
            break;

        case GROP_COPY:
            mPopulator.commands.submit( cmd );
            break;

        default:
            GN_UNEXPECTED();
            break;
    }
}
