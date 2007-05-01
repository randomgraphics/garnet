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
    mResourceCache = new GraphicsResourceCache( *this, p.maxTexBytes, p.maxMeshBytes );

    mDrawThread = new DrawThread( *this );
    if( !mDrawThread->init(p.maxDrawCommandBufferBytes) ) return failure();

    mResourceThread = new ResourceThread( *this );
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
    safeDelete( mResourceCache );

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// renderer management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::engine::RenderEngine::resetRenderer(
    gfx::RendererAPI api,
    const gfx::RendererOptions & ro )
{
    // make sure that all resource and draw commands are executed.
    mResourceThread->waitForIdle();
    mDrawThread->waitForIdle();

    return mDrawThread->resetRenderer( api, ro );
}

//
//
// -----------------------------------------------------------------------------
const GN::gfx::DispDesc & GN::engine::RenderEngine::getDispDesc() const
{
    return mDrawThread->getDispDesc();
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
void GN::engine::RenderEngine::clearScreen(
    const Vector4f & c,
    float z, UInt8 s,
    BitFields flags )
{
    DrawCommand & dr = newDrawCommand();
    dr.pendingResources = 0;
    dr.action = 0; // clear
    dr.clear.color() = c;
    dr.clear.z = z;
    dr.clear.s = s;
    dr.clear.flags = flags;
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
void GN::engine::RenderEngine::submitResourceCommand(
    DrawCommand &             dr,
    GraphicsResourceId        resourceid,
    int                       lod,
    GraphicsResourceLoader  * loader,
    bool                      reload )
{
    GraphicsResourceItem * res = mResourceCache->id2ptr( resourceid );
    if( 0 == res ) return;
    GN_ASSERT( res->id == resourceid );

    const GraphicsResourceItem * to_be_disposed = 0;

    if( GRS_DISPOSED == res->state )
    {
        to_be_disposed = mResourceCache->makeRoomForResource( resourceid, dr.fence );

        if( GN_ASSERT_ENABLED )
        {
            for( const GraphicsResourceItem * p = to_be_disposed; p; p = p->nextItemToDispose )
            {
                GN_ASSERT( GRS_DISPOSED == p->state );
                GN_ASSERT( p->fence < dr.fence );
            }
        }

        mResourceCache->mark_as_realized_and_recently_used( resourceid );

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
        item->command.loader.attach( loader );
        item->command.pendingResources = &dr.pendingResources;
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
    return mResourceCache->alloc( param );
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::freeres( GraphicsResourceId id )
{
    return mResourceCache->free( id );
}

//
//
// -----------------------------------------------------------------------------
GN::engine::GraphicsResource *
GN::engine::RenderEngine::id2res( GraphicsResourceId id )
{
    return mResourceCache->id2ptr( id );
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
