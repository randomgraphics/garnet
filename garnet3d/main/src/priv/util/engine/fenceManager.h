#ifndef __GN_ENGINE_FENCEMANAGER_H__
#define __GN_ENGINE_FENCEMANAGER_H__
// *****************************************************************************
//! \file    engine/fenceManager.h
//! \brief   fence manager
//! \author  chen@@CHENLI-HP (2007.5.2)
// *****************************************************************************

#include "fence.h"
#include "resourceThread.h"
#include "drawThread.h"

namespace GN { namespace engine
{
    ///
    /// managing command fence, shared by resource and draw commands.
    ///
    class RenderEngine::FenceManager
    {
        RenderEngine & mEngine;

        FenceId mFence;

    public:

        ///
        /// ctor
        ///
        FenceManager( RenderEngine & engine ) : mEngine(engine), mFence(0) {}

        //@{

        DrawCommand & submitDrawCommand()
        {
            DrawCommand & dr = mEngine.drawThread().newDrawCommand();
            dr.fence = mFence++;
            return dr;
        }

        //@}

        //@{

        void submitResourceLoadingCommand(
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

            ResourceCommand * cmd = ResourceCommand::alloc();
            if( 0 == cmd ) return;
            cmd->noerr = true;
            cmd->op = GROP_LOAD;
            cmd->resourceId = id;
            cmd->targetLod = lod;
            cmd->loader.attach( loader );
            cmd->mustAfterThisFence = res->lastReferenceFence;
            cmd->submittedAtThisFence = mFence;

            res->lastSubmissionFence = mFence++;
            res->lastSubmittedLoader.attach( loader );
            res->lastSubmittedLod = lod;

            mEngine.resourceThread().submitResourceCommand( cmd );

            GN_ASSERT( res->lastSubmissionFence > res->lastReferenceFence );
        }

        void submitResourceDisposingCommand( GraphicsResourceId id )
        {
            GraphicsResourceItem * res = mEngine.resourceCache().id2ptr( id );
            GN_ASSERT( res );
            GN_ASSERT( res->id == id );
            GN_ASSERT( GRS_DISPOSED == res->state );

            ResourceCommand * cmd = ResourceCommand::alloc();
            if( 0 == cmd ) return;

            cmd->noerr = true;
            cmd->op = GROP_DISPOSE;
            cmd->resourceId = id;
            cmd->mustAfterThisFence = res->lastReferenceFence;
            cmd->submittedAtThisFence = mFence;

            res->lastSubmissionFence = mFence++;

            mEngine.drawThread().submitResourceCommand( cmd );
        }

        //@}
    };
}}

// *****************************************************************************
//                           End of fenceManager.h
// *****************************************************************************
#endif // __GN_ENGINE_FENCEMANAGER_H__
