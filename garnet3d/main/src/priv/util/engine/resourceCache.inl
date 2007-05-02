//
//
// -----------------------------------------------------------------------------
inline GN::engine::GraphicsResourceItem *
GN::engine::RenderEngine::GraphicsResourceCache::id2ptr( GraphicsResourceId id ) const
{
    mResourceMutex.lock();

    if( !mResources.validHandle( id ) )
    {
        mResourceMutex.unlock();
        static GN::Logger * sLogger = GN::getLogger("GN.engine.RenderEngine.ResourceCache");
        GN_ERROR(sLogger)( "invalid resource ID: %d", id );
        return 0;
    }

    GraphicsResourceItem * item = mResources[id];

    GN_ASSERT( item );

    mResourceMutex.unlock();

    return item;
}

//
//
// -----------------------------------------------------------------------------
inline GN::engine::GraphicsResourceId
GN::engine::RenderEngine::GraphicsResourceCache::first() const
{
    return mResources.first();
}

//
//
// -----------------------------------------------------------------------------
inline GN::engine::GraphicsResourceId
GN::engine::RenderEngine::GraphicsResourceCache::next( GraphicsResourceId id ) const
{
    return mResources.next( id );
}
