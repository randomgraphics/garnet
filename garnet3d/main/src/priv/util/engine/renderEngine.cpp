#include "pch.h"
#include "resourceCache.h"
#include "resourceLRU.h"
#include "drawThread.h"
#include "resourceThread.h"
#include "fenceManager.h"

static GN::Logger * sLogger = GN::getLogger("GN.engine.RenderEngine");

// *****************************************************************************
// local functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static inline void sPrepareResource(
    GN::engine::RenderEngine & engine,
    GN::engine::GraphicsResourceId id )
{
    using namespace GN::engine;

    if( 0 == id ) return;

    GraphicsResourceItem * res = engine.resourceCache().id2ptr(id);
    if( 0 == res ) return;

    bool reload;

    engine.resourceLRU().realize( id, &reload );

    if( reload )
    {
        // reload using it's current loader and lod
        GN_ASSERT( res->lastSubmittedLoader );
        engine.resourceThread().submitResourceLoadingCommand(
            id,
            res->lastSubmittedLod,
            res->lastSubmittedLoader );
    }
}

//
//
// -----------------------------------------------------------------------------
static inline void sPrepareContextResources(
    GN::engine::RenderEngine & engine,
    const GN::engine::DrawContext & context )
{
    using namespace GN;
    using namespace GN::engine;

    // make sure all resources referenced in contex is ready to use.
    for( int i = 0; i < gfx::NUM_SHADER_TYPES; ++i )
    {
        if( context.flags.shaderBit( i ) ) sPrepareResource( engine, (GraphicsResourceId)context.shaders[i] );
    }
    if( context.flags.renderTargets )
    {
        for( int i = 0; i < gfx::MAX_RENDER_TARGETS; ++i )
        {
            sPrepareResource( engine, (GraphicsResourceId)context.renderTargets.cbuffers[i].texture );
        }
        sPrepareResource( engine, (GraphicsResourceId)context.renderTargets.zbuffer.texture );
    }
    if( context.flags.textures )
    {
        for( unsigned int i = 0; i < context.numTextures; ++i )
        {
            sPrepareResource( engine, (GraphicsResourceId)context.textures[i] );
        }
    }
    if( context.flags.vtxBufs )
    {
        for( unsigned int i = 0; i < context.numVtxBufs; ++i )
        {
            sPrepareResource( engine, (GraphicsResourceId)context.vtxBufs[i].buffer );
        }
    }
    if( context.flags.idxBuf )
    {
        sPrepareResource( engine, (GraphicsResourceId)context.idxBuf );
    }
}

//
//
// -----------------------------------------------------------------------------
static inline void sSetupWaitingListAndReferenceFence(
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

//
//
// -----------------------------------------------------------------------------
static void sSetupDrawCommandWaitingList(
    GN::engine::RenderEngine::GraphicsResourceCache & cache,
    const GN::engine::DrawContext & context,
    GN::engine::DrawCommandHeader & dr )
{
    using namespace GN;
    using namespace GN::engine;

    // update reference fence of resources in the context
    for( int i = 0; i < gfx::NUM_SHADER_TYPES; ++i )
    {
        if( context.flags.shaderBit( i ) )
        {
            sSetupWaitingListAndReferenceFence(
                cache,
                (GraphicsResourceId)context.shaders[i],
                dr );
        }
    }
    if( context.flags.renderTargets )
    {
        for( int i = 0; i < gfx::MAX_RENDER_TARGETS; ++i )
        {
            sSetupWaitingListAndReferenceFence(
                cache,
                (GraphicsResourceId)context.renderTargets.cbuffers[i].texture,
                dr );
        }
        sSetupWaitingListAndReferenceFence(
            cache,
            (GraphicsResourceId)context.renderTargets.zbuffer.texture,
            dr );
    }
    if( context.flags.textures )
    {
        for( unsigned int i = 0; i < context.numTextures; ++i )
        {
            sSetupWaitingListAndReferenceFence(
                cache,
                (GraphicsResourceId)context.textures[i],
                dr );
        }
    }
    if( context.flags.vtxBufs )
    {
        for( unsigned int i = 0; i < context.numVtxBufs; ++i )
        {
            sSetupWaitingListAndReferenceFence(
                cache,
                (GraphicsResourceId)context.vtxBufs[i].buffer,
                dr );
        }
    }
    if( context.flags.idxBuf )
    {
        sSetupWaitingListAndReferenceFence(
            cache,
            (GraphicsResourceId)context.idxBuf,
            dr );
    }
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
        mResourceThread->waitForIdle();
        mResourceLRU->disposeAll();
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

    // clear context
    mDrawContext.clearToNull();

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
    sPrepareContextResources( *this, context );

    DrawCommandHeader * dr = mDrawThread->submitDrawCommand1( DCT_SET_CONTEXT, context );
    if( 0 == dr ) return;

    sSetupDrawCommandWaitingList( *mResourceCache, context, *dr );

    mDrawContext.mergeWith( context );
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::setShaderUniform(
    GraphicsResourceId        shader,
    const StrA              & uniformName,
    const gfx::UniformValue & value )
{
    // make sure the shader is ready to use.
    sPrepareResource( *this, shader );

    // get uniform data buffer
    const void * data;
    size_t bytes;
    switch( value.type )
    {
        case gfx::UVT_MATRIX44 :
            data = &value.matrix44s[0];
            bytes = value.matrix44s.size() * sizeof(Matrix44f);
            break;

        case gfx::UVT_BOOL :
        case gfx::UVT_INT :
        case gfx::UVT_FLOAT :
        case gfx::UVT_VECTOR4 :
            GN_UNIMPL();
            data = 0;
            bytes = 0;
            break;

        default:
            GN_ERROR(sLogger)( "invalid uniform value!" );
            return;
    }

    struct ParamHeader
    {
        GraphicsResourceId shader;
        char               uniname[32];
        SInt32             unitype;
    } header;

    header.shader = shader;
    memcpy( header.uniname, uniformName.cptr(), 32 );
    header.uniname[31] = 0;
    header.unitype = value.type;

    // create new draw command
    DrawCommandHeader * dr = mDrawThread->submitDrawCommand( DCT_SET_UNIFORM, sizeof(ParamHeader) + bytes );
    if( 0 == dr ) return;
    ParamHeader * h = (ParamHeader*)dr->param();
    *h = header;
    ++h;
    memcpy( h, data, bytes );

    // setup draw command and resource relationship
    sSetupWaitingListAndReferenceFence(
        *mResourceCache,
        shader,
        *dr );
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::clearScreen(
    const Vector4f & c,
    float z, UInt8 s,
    BitFields flags )
{
    sPrepareContextResources( *this, mDrawContext );
    DrawCommandHeader * dr = mDrawThread->submitDrawCommand4( DCT_CLEAR, c, z, s, flags );
    if( 0 == dr ) return;
    sSetupDrawCommandWaitingList( *mResourceCache, mDrawContext, *dr );
}

void GN::engine::RenderEngine::drawIndexed(
    SInt32 prim,
    UInt32 numprim,
    UInt32 startvtx,
    UInt32 minvtxidx,
    UInt32 numvtx,
    UInt32 startidx )
{
    sPrepareContextResources( *this, mDrawContext );
    DrawCommandHeader * dr = mDrawThread->submitDrawCommand6( DCT_DRAW_INDEXED, prim, numprim, startvtx, minvtxidx, numvtx, startidx );
    if( 0 == dr ) return;
    sSetupDrawCommandWaitingList( *mResourceCache, mDrawContext, *dr );
}

void GN::engine::RenderEngine::draw(
    SInt32 prim,
    UInt32 numprim,
    UInt32 startvtx )
{
    sPrepareContextResources( *this, mDrawContext );
    DrawCommandHeader * dr = mDrawThread->submitDrawCommand3( DCT_DRAW, prim, numprim, startvtx );
    if( 0 == dr ) return;
    sSetupDrawCommandWaitingList( *mResourceCache, mDrawContext, *dr );
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
void GN::engine::RenderEngine::disposeResource( GraphicsResourceId id )
{
    mResourceLRU->dispose( id );
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::disposeAllResources()
{
    mResourceLRU->disposeAll();
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
