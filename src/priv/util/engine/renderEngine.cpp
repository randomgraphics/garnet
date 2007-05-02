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
    mFence = 0;

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
    // make sure all resources referenced in contex is ready to use.
    for( int i = 0; i < gfx::NUM_SHADER_TYPES; ++i )
    {
        reloadDisposedResource( context.shaders[i].shader );
    }
    // TODO: associate other resources as well

    // create new draw command
    DrawCommand & dr = mDrawThread->newDrawCommand();
    dr.type = DCT_SET_CONTEXT;
    dr.fence = mFence++;
    dr.context = context;
    dr.resourceWaitingCount = 0;

    // update reference fence of resources in the context
    for( int i = 0; i < gfx::NUM_SHADER_TYPES; ++i )
    {
        useResource( context.shaders[i].shader, dr );
    }
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
    dr.fence = mFence++;
    dr.resourceWaitingCount = 0;
    dr.clear.color() = c;
    dr.clear.z = z;
    dr.clear.s = s;
    dr.clear.flags = flags;
}

//
//
// -----------------------------------------------------------------------------
inline void GN::engine::RenderEngine::reloadDisposedResource( GraphicsResourceId id )
{
    if( 0 == id ) return;

    GraphicsResourceItem * res = mResourceCache->id2ptr(id);
    GN_ASSERT( res );

    if( GRS_DISPOSED == res->state )
    {
        // the resource is disposed. we have to reload it,
        // using it's current loader and lod
        GN_ASSERT( res->lastSubmittedLod > 0 );
        GN_ASSERT( res->lastSubmittedLoader );
        updateres( id, res->lastSubmittedLod, res->lastSubmittedLoader );

        GN_ASSERT( res->lastSubmissionFence > res->lastReferenceFence );
    }
}

//
//
// -----------------------------------------------------------------------------
inline void GN::engine::RenderEngine::useResource( GraphicsResourceId id, DrawCommand & dr )
{
    if( 0 == id ) return;

    GraphicsResourceItem * res = mResourceCache->id2ptr(id);

    GN_ASSERT( res );

    GN_ASSERT( GRS_REALIZED == res->state );

    // reference and update can't happen at same fence.
    GN_ASSERT( res->lastReferenceFence != res->lastSubmissionFence );

    // resource is updated after being used. Now it is being used again.
    // so we have to wait for completion of the update.
    if( res->lastSubmissionFence > res->lastReferenceFence )
    {
        dr.resourceWaitingList[dr.resourceWaitingCount].id = id;
        dr.resourceWaitingList[dr.resourceWaitingCount].waitForUpdate = res->lastSubmissionFence;
        dr.resourceWaitingCount++;
    }

    res->lastReferenceFence = dr.fence;

    GN_ASSERT( res->lastReferenceFence != res->lastSubmissionFence );
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
    // check parameters
    GN_ASSERT( lod > 0 );
    GN_ASSERT( loader );

    // get resource item
    GraphicsResourceItem * res = mResourceCache->id2ptr( id );
    if( 0 == res ) return;
    GN_ASSERT( res->id == id );

    // check if the resource is disposed.
    GraphicsResourceItem * to_be_disposed = 0;
    if( GRS_DISPOSED == res->state )
    {
        to_be_disposed = mResourceCache->makeRoomForResource( id, mFence );
        mResourceCache->mark_as_realized_and_recently_used( id );
    }

    // submit dispose commands
    if( to_be_disposed )
    {
        ResourceCommandItem * item = ResourceCommandItem::alloc();
        if( 0 == item ) return;
        item->noerr = true;
        item->command.op = GROP_DISPOSE;
        while( to_be_disposed )
        {
            item->command.resourceId = to_be_disposed->id;
            item->command.mustAfterThisFence = to_be_disposed->lastReferenceFence;
            item->command.submittedAtThisFence = mFence;
            to_be_disposed->lastSubmissionFence = mFence++;
            mDrawThread->submitResourceCommand( item );
            to_be_disposed = to_be_disposed->nextItemToDispose;
        }
    }

    // submit load command
    ResourceCommandItem * item = ResourceCommandItem::alloc();
    if( 0 == item ) return;
    item->noerr = true;
    item->command.op = GROP_LOAD;
    item->command.resourceId = id;
    item->command.targetLod = lod;
    item->command.loader.attach( loader );
    item->command.mustAfterThisFence = res->lastReferenceFence;
    item->command.submittedAtThisFence = mFence;
    res->lastSubmissionFence = mFence++;
    res->lastSubmittedLoader.attach( loader );
    res->lastSubmittedLod = lod;
    mResourceThread->submitResourceCommand( item );
}
