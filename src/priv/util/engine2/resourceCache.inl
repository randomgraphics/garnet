//
//
// -----------------------------------------------------------------------------
inline bool
GN::engine2::RenderEngine::ResourceCache::checkResource( const GraphicsResourceItem * item ) const
{
    ScopeMutex<SpinLoop> lock(mResourceMutex);

    if( 0 == item )
    {
        mResourceMutex.unlock();
        static GN::Logger * sLogger = GN::getLogger("GN.engine2.RenderEngine.ResourceCache");
        GN_ERROR(sLogger)( "invalid graphics resource pointer : 0x%p", item );
        GN_ASSERT_EX( 0, "invalid graphics resource pointer" );
        return false;
    }

    if( &mEngine != &item->engine ||
        !mResources.validHandle( item->id ) ||
        mResources[item->id] != item )
    {
        mResourceMutex.unlock();
        static GN::Logger * sLogger = GN::getLogger("GN.engine2.RenderEngine.ResourceCache");
        GN_ERROR(sLogger)( "invalid graphics resource pointer : 0x%p", item );
        GN_ASSERT_EX( 0, "invalid graphics resource pointer" );
        return false;
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
inline GN::engine2::GraphicsResourceItem *
GN::engine2::RenderEngine::ResourceCache::firstResource() const
{
    UInt32 id = mResources.first();
    return id ? mResources[id] : 0;
}

//
//
// -----------------------------------------------------------------------------
inline GN::engine2::GraphicsResourceItem *
GN::engine2::RenderEngine::ResourceCache::nextResource( const GraphicsResourceItem * item ) const
{
    GN_ASSERT( checkResource( item ) );
    UInt32 nextid = mResources.next( item->id );
    return nextid ? mResources[nextid] : 0;
}
