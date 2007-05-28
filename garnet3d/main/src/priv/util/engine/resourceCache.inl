//
//
// -----------------------------------------------------------------------------
inline bool
GN::engine::RenderEngine::GraphicsResourceCache::check( const GraphicsResourceItem * item ) const
{
    mResourceMutex.lock();

    if( 0 == item )
    {
        mResourceMutex.unlock();
        static GN::Logger * sLogger = GN::getLogger("GN.engine.RenderEngine.ResourceCache");
        GN_ERROR(sLogger)( "invalid graphics resource pointer : 0x%p", item );
        GN_ASSERT_EX( 0, "invalid graphics resource pointer" );
        return false;
    }

    if( &mEngine != &item->engine ||
        !mResources.validHandle( item->id ) ||
        mResources[item->id] != item )
    {
        mResourceMutex.unlock();
        static GN::Logger * sLogger = GN::getLogger("GN.engine.RenderEngine.ResourceCache");
        GN_ERROR(sLogger)( "invalid graphics resource pointer : 0x%p", item );
        GN_ASSERT_EX( 0, "invalid graphics resource pointer" );
        return false;
    }

    mResourceMutex.unlock();

    return true;
}

//
//
// -----------------------------------------------------------------------------
inline GN::engine::GraphicsResourceItem *
GN::engine::RenderEngine::GraphicsResourceCache::first() const
{
    UInt32 id = mResources.first();
    return id ? mResources[id] : 0;
}

//
//
// -----------------------------------------------------------------------------
inline GN::engine::GraphicsResourceItem *
GN::engine::RenderEngine::GraphicsResourceCache::next( const GraphicsResourceItem * item ) const
{
    GN_ASSERT( check( item ) );
    UInt32 nextid = mResources.next( item->id );
    return nextid ? mResources[nextid] : 0;
}
