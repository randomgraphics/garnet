#ifndef __GN_ENGINE_RESOURCEITEM_H__
#define __GN_ENGINE_RESOURCEITEM_H__
// *****************************************************************************
//! \file    engine/resourceItem.h
//! \brief   Graphics resource cache
//! \author  chenli@@FAREAST (2007.4.27)
// *****************************************************************************

#include "garnet/base/linkedlist.h"
#include "fence.h"

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
        /// estimated resource size in bytes.
        ///
        const size_t bytes;

        /// managed by LRU manager, which runs in render engine's thread.
        ///
        //@{

        ///
        /// One of GraphicsResourceState.
        ///
        /// Note that neither GraphicsResourceCache::realize() nor GraphicsResourceCache::dispose() will change resource state.
        ///
        GraphicsResourceState state;

        ///
        /// this is used to store dispose resource list returned by makeRoomFromResource()
        ///
        GraphicsResourceItem * nextItemToDispose;

        //@}

        /// managemented by render engine
        //@{

        ///
        /// the loader used by the lasted update command.
        ///
        AutoRef<GraphicsResourceLoader,Mutex> lastSubmittedLoader;

        ///
        /// the LOD level used by the last update command.
        ///
        /// \note Don't confuse this with GraphicsResource::lod.
        ///
        int lastSubmittedLod;

        ///
        /// the fence of the lastest update command.
        ///
        FenceId lastSubmissionFence;

        ///
        /// last used/referenced at this fence.
        ///
        FenceId lastReferenceFence;

        //@}

        //@{

        ///
        /// When resource update is complete. Draw thread will copy the submission fence
        /// value in the resource request to here, which means that the resource request
        /// submitted at this fence is done.
        ///
        /// \note
        ///     - This value should be less or equal then lastSubmission all the time.
        ///
        volatile FenceId lastCompletedFence;

        //@}

        ///
        /// ctor
        ///
        GraphicsResourceItem(
                GraphicsResourceId id_,
                const GraphicsResourceDesc & desc_,
                size_t bytes_ )
            : GraphicsResource( id_, desc_ )
            , bytes( bytes_ )
            , state( GRS_DISPOSED )
            , lastSubmittedLod( 0 )
            , lastSubmissionFence( 0 )
            , lastReferenceFence( 0 )
            , lastCompletedFence( 0 )
        {}
    };
}}

// *****************************************************************************
//                           End of resourceItem.h
// *****************************************************************************
#endif // __GN_ENGINE_RESOURCEITEM_H__
