#include "pch.h"
#include "resourceCache.h"
#include "resourceLRU.h"
#include "drawThread.h"
#include "resourceThread.h"
#include "fenceManager.h"

#pragma warning(disable:4100)

// *****************************************************************************
// local functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static inline void sUpdateWaitingListAndReferenceFence(
    GN::engine::RenderEngine::GraphicsResourceCache & cache,
    GN::engine::GraphicsResourceId id,
    GN::engine::DrawCommandHeader & dr )
{
    using namespace GN::engine;

    if( 0 == id ) return;

    GN::engine::GraphicsResourceItem * res = cache.id2ptr( id );

    if( 0 == res ) return;

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

    // note: this should be the only place to update lastReferenceFence
    res->lastReferenceFence = dr.fence;

    GN_ASSERT( res->lastReferenceFence != res->lastSubmissionFence );
}


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
    mFenceManager = new FenceManager( *this );

    mResourceCache = new GraphicsResourceCache( *this );
    if( !mResourceCache->init() ) return failure();

    mResourceLRU = new ResourceLRU( *this );
    if( !mResourceLRU->init( p.maxTexBytes, p.maxMeshBytes ) ) return failure();

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

    // dispose all resources
    if( ok() )
    {
        mResourceLRU->disposeAll();
        mResourceThread->waitForIdle();
        mDrawThread->waitForIdle();
    }

    safeDelete( mResourceThread );
    safeDelete( mDrawThread );
    safeDelete( mResourceLRU );
    safeDelete( mResourceCache );
    safeDelete( mFenceManager );

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
    // dispose all resources
    mResourceLRU->disposeAll();
    mResourceThread->waitForIdle();
    mDrawThread->waitForIdle();

    // then reset the renderer
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
        prepareResource( context.shaders[i].shader );
    }
    // TODO: associate other resources as well

    // create new draw command
    DrawCommandHeader * dr = mDrawThread->submitDrawCommand1( DCT_SET_CONTEXT, context );
    if( 0 == dr ) return;

    // update reference fence of resources in the context
    for( int i = 0; i < gfx::NUM_SHADER_TYPES; ++i )
    {
        sUpdateWaitingListAndReferenceFence(
            *mResourceCache,
            context.shaders[i].shader,
            *dr );
    }
}

/*
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::updateShaderUniforms(
    GraphicsResourceId        shader,
    const StrA              & uniformName,
    const gfx::UniformValue & value )
{
    DrawCommand & dr = mFenceManager->submitDrawCommand();
    dr.type = DCT_SET_UNIFORM;
    dr.resourceWaitingCount = 0;
    dr.clear.color() = c;
    dr.clear.z = z;
    dr.clear.s = s;
    dr.clear.flags = flags;
}*/

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::clearScreen(
    const Vector4f & c,
    float z, UInt8 s,
    BitFields flags )
{
    mDrawThread->submitDrawCommand4( DCT_CLEAR, c, z, s, flags );
}

//
//
// -----------------------------------------------------------------------------
inline void GN::engine::RenderEngine::prepareResource( GraphicsResourceId id )
{
    if( 0 == id ) return;

    GraphicsResourceItem * res = mResourceCache->id2ptr(id);
    if( 0 == res ) return;

    bool reload;

    mResourceLRU->realize( id, &reload );

    if( reload )
    {
        // reload using it's current loader and lod
        GN_ASSERT( res->lastSubmittedLod > 0 );
        GN_ASSERT( res->lastSubmittedLoader );
        mResourceThread->submitResourceLoadingCommand(
            id,
            res->lastSubmittedLod,
            res->lastSubmittedLoader );
    }
}

// *****************************************************************************
// resource commands
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::engine::GraphicsResourceId
GN::engine::RenderEngine::allocResource( const GraphicsResourceDesc & desc )
{
    GraphicsResourceId id = mResourceCache->alloc( desc );
    if( 0 == id ) return 0;
    mResourceLRU->insert( id );
    return id;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::freeResource( GraphicsResourceId id )
{
    mResourceLRU->remove( id );
    return mResourceCache->free( id );
}

//
//
// -----------------------------------------------------------------------------
GN::engine::GraphicsResource *
GN::engine::RenderEngine::getResourceById( GraphicsResourceId id )
{
    return mResourceCache->id2ptr( id );
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::updateResource(
    GraphicsResourceId       id,
    int                      lod,
    GraphicsResourceLoader * loader )
{
    mResourceLRU->realize( id, 0 );
    mResourceThread->submitResourceLoadingCommand( id, lod, loader );
}
