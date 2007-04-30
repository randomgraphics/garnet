#include "pch.h"
#include "resourceCache.h"
#include "drawThread.h"
#include "resourceThread.h"

#pragma warning(disable:4100)

// *****************************************************************************
// local functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::engine::RenderEngine::init( const RenderEngineInitParameters & p )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::engine::RenderEngine, () );

    // create sub components
    mGfxResCache = new GraphicsResourceCache( p.maxTexBytes, p.maxMeshBytes );

    mDrawThread = new DrawThread;
    if( !mDrawThread->init(p.maxDrawCommandBufferBytes) ) return failure();

    mResourceThread = new ResourceThread;
    if( !mResourceThread->init() ) return failure();

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::quit()
{
    GN_GUARD;

    safeDelete( mResourceThread );
    safeDelete( mDrawThread );
    safeDelete( mGfxResCache );

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// draw request management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::frameBegin()
{
    return mDrawThread->frameBegin();
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::frameEnd()
{
    return mDrawThread->frameEnd();
}

//
//
// -----------------------------------------------------------------------------
GN::engine::DrawCommand & GN::engine::RenderEngine::newDrawCommand()
{
    return mDrawThread->newDrawCommand();
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::composeAndSubmitResourceCommand(
    DrawCommand &             dr,
    GraphicsResourceOperation op,
    GraphicsResourceId        resourceid,
    int                       lod,
    GraphicsResourceLoader  * loader,
    bool                      reload )
{
    GraphicsResourceItem * res = mGfxResCache->id2ptr( resourceid );
    if( 0 == res ) return;
    GN_ASSERT( res->id == resourceid );

    const GraphicsResourceItem * to_be_disposed = 0;

    if( GRS_DISPOSED == res->state )
    {
        to_be_disposed = mGfxResCache->makeRoomForResource( resourceid, dr.fence );

        if( GN_ASSERT_ENABLED )
        {
            for( const GraphicsResourceItem * p = to_be_disposed; p; p = p->nextItemToDispose )
            {
                GN_ASSERT( GRS_DISPOSED == p->state );
                GN_ASSERT( p->fence < dr.fence );
            }
        }

        mGfxResCache->mark_as_realized_and_recently_used( resourceid );

        reload = true;
    }

    res->fence = dr.fence;

    if( to_be_disposed )
    {
        ResourceCommandItem * item = ResourceCommandItem::alloc();
        if( 0 == item ) return;
        item->command.op = GROP_DISPOSE;
        while( to_be_disposed )
        {
            item->command.waitForDrawFence = to_be_disposed->fence;
            item->command.resourceId = to_be_disposed->id;
            mDrawThread->submitResourceCommand( item );
            to_be_disposed = to_be_disposed->nextItemToDispose;
        }
    }

    if( reload )
    {
        dr.incPendingResourceCount();

        GN_ASSERT( dr.getPendingResourceCount() > 0 );

        ResourceCommandItem * item = ResourceCommandItem::alloc();
        if( 0 == item ) return;
        item->command.op = GROP_LOAD;
        item->command.resourceId = resourceid;
        item->command.waitForDrawFence = dr.fence;
        item->command.targetLod = lod;
        item->command.loader = loader;
        dr.attachResourceCommand( item->command );
        mResourceThread->submitResourceCommand( item );
    }
}

// *****************************************************************************
// cache functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::engine::GraphicsResourceId
GN::engine::RenderEngine::allocres( const GraphicsResourceCreationParameter & param )
{
    return mGfxResCache->alloc( param );
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::freeres( GraphicsResourceId id )
{
    return mGfxResCache->free( id );
}

//
//
// -----------------------------------------------------------------------------
GN::engine::GraphicsResource *
GN::engine::RenderEngine::id2res( GraphicsResourceId id )
{
    return mGfxResCache->id2ptr( id );
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
