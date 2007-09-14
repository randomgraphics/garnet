#ifndef __GN_ENGINE2_RESOURCEITEM_H__
#define __GN_ENGINE2_RESOURCEITEM_H__
// *****************************************************************************
/// \file
/// \brief   Graphics resource cache
/// \author  chenli@@FAREAST (2007.4.27)
// *****************************************************************************

#include "garnet/base/linkedlist.h"
#include "fence.h"

namespace GN { namespace engine
{
    ///
    /// graphics resource states
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
        /// constant resource item properties
        //@{

        ///
        /// estimated resource size in bytes.
        ///
        const size_t bytes;

        //@}

        /// managed by LRU manager, which runs in render engine's thread.
        ///
        //@{

        ///
        /// resource state.
        ///
        GraphicsResourceState state;

        ///
        /// This is used to mark realize time stamp. Note that 0 is reserved for newly created resource.
        ///
        int realizeStamp;

        ///
        /// this is used to store dispose resource list returned by makeRoomForResource()
        ///
        GraphicsResourceItem * nextItemToDispose;

        //@}

        /// managemented by render engine
        //@{

        ///
        /// resources that this resource depends on
        ///
        DynaArray<GraphicsResourceItem*> prerequisites;

        ///
        /// resources that depent on this resource
        ///
        DynaArray<GraphicsResourceItem*> dependents;

        ///
        /// resource loader
        ///
        AutoRef<GraphicsResourceLoader> loader;

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
        FenceId lastCompletedFence;

        //@}

        ///
        /// ctor
        ///
        GraphicsResourceItem(
                RenderEngine & engine_,
                const GraphicsResourceDesc & desc_,
                size_t bytes_ )
            : GraphicsResource( engine_, desc_ )
            , bytes( bytes_ )
            , state( GRS_DISPOSED )
            , realizeStamp( 0 )
            , lastSubmissionFence( 0 )
            , lastReferenceFence( 0 )
            , lastCompletedFence( 0 )
        {}

        ///
        /// dtor
        ///
        ~GraphicsResourceItem()
        {
            // remove itself from prerequisits' dependent list
            for( size_t i = 0; i < prerequisites.size(); ++i )
            {
                GraphicsResourceItem * p = prerequisites[i];
                std::remove( p->dependents.begin(), p->dependents.end(), this );
                p->dependents.popBack();
            }

            // remove itself from dependents' prerequisit list
            for( size_t i = 0; i < dependents.size(); ++i )
            {
                GraphicsResourceItem * d = dependents[i];
                std::remove( d->prerequisites.begin(), d->prerequisites.end(), this );
                d->prerequisites.popBack();
            }
        }
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_ENGINE2_RESOURCEITEM_H__
