#ifndef __GN_ENGINE_RESOURCECACHE_H__
#define __GN_ENGINE_RESOURCECACHE_H__
// *****************************************************************************
//! \file    engine/resourceCache.h
//! \brief   Graphics resource cache
//! \author  chenli@@FAREAST (2007.4.27)
// *****************************************************************************

#include "garnet/base/linkedlist.h"

namespace GN { namespace engine
{
    ///
    /// ...
    ///
    enum GraphicsResourceState
    {
        //@{
        GRS_REALIZED,
        GRS_DISPOSED,
        //@}
    };

    ///
    /// graphics resource item
    ///
    struct GraphicsResourceItem : public GraphicsResource, public DoubleLinkedItem<GraphicsResourceItem>
    {
        ///
        /// Should be one of GraphicsResourceState.
        ///
        /// Note that neither GraphicsResourceCache::realize() nor GraphicsResourceCache::dispose() will change resource state.
        ///
        GraphicsResourceState state;

        ///
        /// means that the resource will be available to update and/or dispose, after this draw fence.
        ///
        FenceId fence;

        ///
        /// this is used to store dispose resource list returned by makeRoomFromResource()
        ///
        GraphicsResourceItem * nextItemToDispose;

        ///
        /// ctor
        ///
        GraphicsResourceItem( GraphicsResourceId i, GraphicsResourceType t )
            : GraphicsResource( i, t )
        {}
    };

    ///
    /// Graphics resource reusable cache. This class is <b>_NOT_</b> thread safe, to ensure maxinum performance.
    ///
    /// Be sure to syncornize your calls into the cache.
    ///
    class RenderEngine::GraphicsResourceCache
    {
    public:

        //@{

        ///
        /// ctor
        ///
        GraphicsResourceCache( UInt32 maxtexbytes, UInt32 maxmeshbytes );

        //@}

        ///
        /// these methods manage resource instances, but do not touch resource state.
        //@{
        GraphicsResourceId     alloc( const GraphicsResourceCreationParameter & );
        void                   free( GraphicsResourceId );
        GraphicsResourceItem * id2ptr( GraphicsResourceId );
        bool                   realize( GraphicsResourceId, bool * needReload );
        void                   dispose( GraphicsResourceId );
        //@}

        ///
        /// these methods operators on resource LRU list and resource state.
        //@{

        ///
        /// mark the resource as realized. Adjust total realized bytes and LRU list as well.
        ///
        void mark_as_realized_and_recently_used( GraphicsResourceId );

        ///
        /// This function will lookup in resource cache, in reverse order of LRU list,
        /// for items that are:
        /// - in GRS_REALIZED state.
        /// - available at user specified draw fence.
        ///
        /// Each found item will be marked to as GRS_DISPOSED, then returned in result list.
        ///
        GraphicsResourceItem * makeRoomForResource( GraphicsResourceId, FenceId );

        //@}

    private:

        typedef HandleManager<GraphicsResourceItem*,GraphicsResourceId> ResourceHandleManager;
        typedef DoubleLinkedList<GraphicsResourceItem> ResourceLRUList;

        ResourceHandleManager mResources;
        ResourceLRUList       mLRUList;
    };
}}

// *****************************************************************************
//                           End of resourceCache.h
// *****************************************************************************
#endif // __GN_ENGINE_RESOURCECACHE_H__
