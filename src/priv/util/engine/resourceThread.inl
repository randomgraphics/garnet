//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::ResourceThread::submitResourceCommand(
    ResourceCommandItem * item )
{
    GN_ASSERT( item );
    switch( item->command.op )
    {
        case GROP_LOAD:
            mLoador.commands.submit( item );
            break;

        case GROP_DECOMPRESS:
            mDecompressor.commands.submit( item );
            break;

        default:
            GN_UNEXPECTED();
            break;
    }
}
