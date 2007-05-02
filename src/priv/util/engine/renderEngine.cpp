#include "pch.h"
#include "resourceCache.h"
#include "drawThread.h"
#include "resourceThread.h"

#pragma warning(disable:4100)

// *****************************************************************************
// local functions
// *****************************************************************************

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

    // initialize other data
    mSubmitFence = 0;

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
void GN::engine::RenderEngine::setContext( const DrawContext & context )
{
    DrawCommand & dr = mDrawThread->newDrawCommand();
    dr.type = DCT_SET_CONTEXT;
    dr.fence = mSubmitFence++;
    dr.context = context;
    dr.resourceWaitingCount = 0;

    for( int i = 0; i < gfx::NUM_SHADER_TYPES; ++i )
    {
        if( context.shader[i].shader )
        {
            ensureUsableResource( context.shader[i], dr.fence );
        }
    }

    // TODO: associate other resources as well
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::clearScreen(
    const Vector4f & c,
    float z, UInt8 s,
    BitFields flags )
{
    DrawCommand & dr = mDrawThread->newDrawCommand();
    dr.type = DCT_CLEAR;
    dr.fence = mSubmitFence++;
    dr.clear.color() = c;
    dr.clear.z = z;
    dr.clear.s = s;
    dr.clear.flags = flags;
}

//
//
// -----------------------------------------------------------------------------
bool GN::engine::RenderEngine::ensureUsableResource(
    GraphicsResourceId id,
    FenceId fence )
{
    GN_ASSERT( 0 != id );

    GraphicsResourceItem * res = mResourceCache->id2ptr(id);
    GN_ASSERT( res );

    bool wait = false;

    if( res->updateFence > res->referenceFence )
    {
        // The resource is updated at lease once, after being used.
        // Now it is used again. So this draw must wait
        // the last update to complete.
        wait = true;
    }

    if( GRS_DISPOSED == res->state )
    {
        // the resource is disposed. we have to reload it,
        // using it's current loader and lod
        loadResource( fence, id, res->updateLod, res->updateLoader );

        res->updateFence = fence;
        wait = true;
    }

    // update reference fence
    GN_ASSERT( res->updateFence <= fence );
    res->referenceFence = fence;

    return wait;
}


// *****************************************************************************
// resource commands
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

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::updateres(
    GraphicsResourceId       id,
    int                      lod,
    GraphicsResourceLoader * loader )
{
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::loadResource(
    FenceId                   fence,
    GraphicsResourceId        id,
    int                       lod,
    GraphicsResourceLoader  * loader )
{
    GN_TODO( "what happend, if a resource is updated seveval times, may or may not at same fence, before using." );

    // get resource item
    GraphicsResourceItem * res = mResourceCache->id2ptr( id );
    if( 0 == res ) return;
    GN_ASSERT( res->id == id );

    // update the update fence
    res->updateFence = fence;

    // check if the resource is disposed.
    const GraphicsResourceItem * to_be_disposed = 0;
    if( GRS_DISPOSED == res->state )
    {
        to_be_disposed = mResourceCache->makeRoomForResource( id, fence );
        mResourceCache->mark_as_realized_and_recently_used( id );
    }

    // submit dispose commands
    if( to_be_disposed )
    {
        ResourceCommandItem * item = ResourceCommandItem::alloc();
        if( 0 == item ) return;
        item->command.op = GROP_DISPOSE;
        while( to_be_disposed )
        {
            item->command.resourceId = to_be_disposed->id;
            item->command.waitForDrawFence = to_be_disposed->referenceFence;
            mDrawThread->submitResourceCommand( item );
            to_be_disposed = to_be_disposed->nextItemToDispose;
        }
    }

    // submit load command
    ResourceCommandItem * item = ResourceCommandItem::alloc();
    if( 0 == item ) return;
    item->command.op = GROP_LOAD;
    item->command.resourceId = id;
    item->command.waitForDrawFence = res->referenceFence;
    item->command.targetLod = lod;
    item->command.loader.attach( loader );
    mResourceThread->submitResourceCommand( item );
}


