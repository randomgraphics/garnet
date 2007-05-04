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
            mLoador.commands.submit( cmd );
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
    GraphicsResourceId       id,
    int                      lod,
    GraphicsResourceLoader * loader )
{
    // check parameters
    GN_ASSERT( lod > 0 );
    GN_ASSERT( loader );

    // get resource item
    GraphicsResourceItem * res = mEngine.resourceCache().id2ptr( id );
    GN_ASSERT( res );
    GN_ASSERT( res->id == id );

    FenceId fence = mEngine.fenceManager().getAndIncFence();

    ResourceCommand * cmd = ResourceCommand::alloc();
    if( 0 == cmd ) return;
    cmd->noerr                = true;
    cmd->op                   = GROP_LOAD;
    cmd->resourceId           = id;
    cmd->targetLod            = lod;
    cmd->loader.attach( loader );
    cmd->mustAfterThisFence   = res->lastReferenceFence;
    cmd->submittedAtThisFence = fence;

    res->lastSubmissionFence = fence;
    res->lastSubmittedLoader.attach( loader );
    res->lastSubmittedLod    = lod;

    GN_ASSERT( res->lastSubmissionFence > res->lastReferenceFence );

    submitResourceCommand( cmd );
}


