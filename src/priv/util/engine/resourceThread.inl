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
inline void GN::engine::RenderEngine::ResourceThread::submitResourceLoadingCommand(
    GraphicsResourceItem   * item,
    int                      lod,
    GraphicsResourceLoader * loader )
{
    // check parameters
    GN_ASSERT( mEngine.resourceCache().check( item ) );
    GN_ASSERT( loader );

    FenceId fence = mEngine.fenceManager().getAndIncFence();

    ResourceCommand * cmd = ResourceCommand::alloc();
    if( 0 == cmd ) return;
    cmd->noerr                      = true;
    cmd->op                         = GROP_LOAD;
    cmd->resource                   = item;
    cmd->targetLod                  = lod;
    cmd->loader.set( loader );
    cmd->mustAfterThisDrawFence     = item->lastReferenceFence;
    cmd->mustAfterThisResourceFence = item->lastSubmissionFence;
    cmd->submittedAtThisFence       = fence;

    item->lastSubmissionFence = fence;
    item->lastSubmittedLoader.set( loader );
    item->lastSubmittedLod    = lod;

    GN_ASSERT( item->lastSubmissionFence > item->lastReferenceFence );
    GN_ASSERT( item->lastCompletedFence < item->lastSubmissionFence );

    submitResourceCommand( cmd );
}


