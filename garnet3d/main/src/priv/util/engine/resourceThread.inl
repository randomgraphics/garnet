//
//
// -----------------------------------------------------------------------------
inline void GN::engine::RenderEngine::ResourceThread::submitResourceCommand(
    ResourceCommand * cmd )
{
    GN_ASSERT( cmd );
    switch( cmd->op )
    {
        case GROP_LOAD:
            mLoader.commands.submit( cmd );
            break;

        case GROP_DECOMPRESS:
            mDecompressor.commands.submit( cmd );
            break;

        default:
            GN_UNEXPECTED();
            break;
    }
}

//
//
// -----------------------------------------------------------------------------
inline void GN::engine::RenderEngine::ResourceThread::loadResource(
    GraphicsResourceItem      * item,
    GraphicsResourceLoadStore * loadstore )
{
    // check parameters
    GN_ASSERT( mEngine.resourceCache().checkResource( item ) );
    GN_ASSERT( GRS_REALIZED == item->state );
    GN_ASSERT( loadstore );

    // get new fence
    FenceId fence = mEngine.fenceManager().getAndIncFence();

    // create new command
    ResourceCommand * cmd = ResourceCommand::alloc();
    if( 0 == cmd ) return;

    // setup command fields
    cmd->noerr                      = true;
    cmd->op                         = GROP_LOAD;
    cmd->resource                   = item;
    cmd->loadstore.set( loadstore );
    cmd->mustAfterThisDrawFence     = item->lastReferenceFence;
    cmd->mustAfterThisResourceFence = item->lastSubmissionFence;
    cmd->submittedAtThisFence       = fence;

    // update resource item
    item->lastSubmissionFence       = fence;

    GN_ASSERT( item->lastSubmissionFence > item->lastReferenceFence );
    GN_ASSERT( item->lastCompletedFence < item->lastSubmissionFence );

    submitResourceCommand( cmd );
}
