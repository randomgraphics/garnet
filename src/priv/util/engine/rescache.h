#ifndef __GN_ENGINE_RESCACHE_H__
#define __GN_ENGINE_RESCACHE_H__
// *****************************************************************************
//! \file    engine/rescache.h
//! \brief   Graphics resource cache
//! \author  chenli@@FAREAST (2007.4.27)
// *****************************************************************************

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

    struct GraphicsResourceItem : public GraphicsResource
    {
        ///
        /// Should be one of GraphicsResourceState.
        ///
        /// Note that resource state is totally controlled/updated by render engine class.
        ///
        /// Neither GraphicsResourceCache::realize() nor GraphicsResourceCache::dispose() will change resource state.
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
    };

    ///
    /// Graphics resource reusable cache. This class is <b>_NOT_</b> thread safe, to ensure maxinum performance.
    ///
    /// Be sure to syncornize your calls into the cache.
    ///
    class GraphicsResourceCache
    {
    public:

        ///
        /// ctor
        ///
        GraphicsResourceCache( UInt32 maxtexbytes, UInt32 maxmeshbytes );

        //@{
        GraphicsResourceId     alloc( const GraphicsResourceCreationParameter & );
        void                   free( GraphicsResourceId );
        GraphicsResourceItem * id2ptr( GraphicsResourceId );
        bool                   realize( GraphicsResourceId, bool * needReload );
        void                   dispose( GraphicsResourceId );
        //@}


        ///
        /// mark the resource as being recently used.
        ///
        void markAsRecentlyUsed( GraphicsResourceId );

        ///
        /// make enough room to hold the incoming resource, before specific draw fence happens.
        ///
        /// return a list of resource item that has to be disposed.
        ///
        GraphicsResourceItem * makeRoomFromResource( GraphicsResourceId, FenceId );
    };
}}

// *****************************************************************************
//                           End of rescache.h
// *****************************************************************************
#endif // __GN_ENGINE_RESCACHE_H__
