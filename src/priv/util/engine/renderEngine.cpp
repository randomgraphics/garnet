#include "pch.h"
#include "resourceCache.h"
#include "resourceLRU.h"
#include "drawThread.h"
#include "resourceThread.h"
#include "fenceManager.h"
#include "dump.h"

static GN::Logger * sLogger = GN::getLogger("GN.engine.RenderEngine");

// *****************************************************************************
// local functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static inline void sPrepareResource(
    GN::engine::RenderEngine & engine,
    const GN::engine::GraphicsResource * res )
{
    using namespace GN::engine;

    if( 0 == res ) return;

    GN::engine::GraphicsResourceItem * item = (GN::engine::GraphicsResourceItem*)res;

    if( !engine.resourceCache().check(item) ) return;

    bool reload;

    engine.resourceLRU().realize( item, &reload );

    GN_ASSERT( GRS_REALIZED == item->state );

    if( reload )
    {
        // reload using it's current loader and lod
        GN_ASSERT( item->lastSubmittedLoader );
        engine.resourceThread().submitResourceLoadingCommand(
            item,
            item->lastSubmittedLod,
            item->lastSubmittedLoader );
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
        if( context.flags.shaderBit( i ) ) sPrepareResource( engine, context.shaders[i] );
    }
    if( context.flags.renderTargets )
    {
        for( int i = 0; i < gfx::MAX_RENDER_TARGETS; ++i )
        {
            sPrepareResource( engine, context.renderTargets.cbuffers[i].texture );
        }
        sPrepareResource( engine, context.renderTargets.zbuffer.texture );
    }
    if( context.flags.vtxfmt )
    {
        sPrepareResource( engine, context.vtxfmt );
    }
    if( context.flags.textures )
    {
        for( unsigned int i = 0; i < context.numTextures; ++i )
        {
            sPrepareResource( engine, context.textures[i] );
        }
    }
    if( context.flags.vtxbufs )
    {
        for( unsigned int i = 0; i < context.numVtxBufs; ++i )
        {
            sPrepareResource( engine, context.vtxbufs[i].buffer );
        }
    }
    if( context.flags.idxbuf )
    {
        sPrepareResource( engine, context.idxbuf );
    }
}

//
//
// -----------------------------------------------------------------------------
static inline void sSetupWaitingListAndReferenceFence(
    GN::engine::RenderEngine::GraphicsResourceCache & cache,
    const GN::engine::GraphicsResource * res,
    GN::engine::DrawCommandHeader & dr )
{
    using namespace GN::engine;

    if( 0 == res ) return;

    GN::engine::GraphicsResourceItem * item = (GN::engine::GraphicsResourceItem*)res;

    if( !cache.check( item ) ) return;

    GN_ASSERT( GRS_REALIZED == item->state );

    // reference and update can't happen at same fence.
    GN_ASSERT( item->lastReferenceFence != item->lastSubmissionFence );

    // resource is updated after being used. Now it is being used again.
    // so we have to wait for completion of the update.
    if( item->lastSubmissionFence > item->lastReferenceFence )
    {
        dr.resourceWaitingList[dr.resourceWaitingCount].resource = item;
        dr.resourceWaitingList[dr.resourceWaitingCount].waitForUpdate = item->lastSubmissionFence;
        dr.resourceWaitingCount++;
    }

    // note: this should be the only place to update lastReferenceFence
    item->lastReferenceFence = dr.fence;

    GN_ASSERT( item->lastReferenceFence != item->lastSubmissionFence );
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
                context.shaders[i],
                dr );
        }
    }
    if( context.flags.renderTargets )
    {
        for( int i = 0; i < gfx::MAX_RENDER_TARGETS; ++i )
        {
            sSetupWaitingListAndReferenceFence(
                cache,
                context.renderTargets.cbuffers[i].texture,
                dr );
        }
        sSetupWaitingListAndReferenceFence(
            cache,
            context.renderTargets.zbuffer.texture,
            dr );
    }
    if( context.flags.vtxfmt )
    {
        sSetupWaitingListAndReferenceFence(
            cache,
            context.vtxfmt,
            dr );
    }
    if( context.flags.textures )
    {
        for( unsigned int i = 0; i < context.numTextures; ++i )
        {
            sSetupWaitingListAndReferenceFence(
                cache,
                context.textures[i],
                dr );
        }
    }
    if( context.flags.vtxbufs )
    {
        for( unsigned int i = 0; i < context.numVtxBufs; ++i )
        {
            sSetupWaitingListAndReferenceFence(
                cache,
                context.vtxbufs[i].buffer,
                dr );
        }
    }
    if( context.flags.idxbuf )
    {
        sSetupWaitingListAndReferenceFence(
            cache,
            context.idxbuf,
            dr );
    }
}

///
/// loader for shader and vertex format
///
class DummyLoader : public GN::engine::GraphicsResourceLoader
{
public:
    virtual bool load( const GN::engine::GraphicsResourceDesc &, void * & outbuf, size_t & outbytes, int )
    {
        outbuf = 0;
        outbytes = 0;
        return true;
    }

    bool decompress( const GN::engine::GraphicsResourceDesc &, void * & outbuf, size_t & outbytes, const void *, size_t, int )
    {
        outbuf = 0;
        outbytes = 0;
        return true;
    }

    virtual bool copy( GN::engine::GraphicsResource &, const void * , size_t, int )
    {
        return true;
    }

    virtual void freebuf( void *, size_t )
    {
    }
};

// *****************************************************************************
// graphics resource descriptor
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static const char * sGraphicsResourceType2Str( GN::engine::GraphicsResourceType type )
{
    static const char * table[] = {
        "SHADER",
        "TEXTURE",
        "VTXBUF",
        "IDXBUF",
        "CONSTBUF",
        "VTXFMT",
    };

    if( 0 <= type && type < GN::engine::NUM_GRAPHICS_RESOURCE_TYPES ) return table[type];
    else return "INVALID";
}

//
//
// -----------------------------------------------------------------------------
GN::StrA GN::engine::GraphicsResourceDesc::toString() const
{
    StrA s;

    s.format( "type=\"%s\" name=\"%s\"",
        sGraphicsResourceType2Str(type),
        name.empty() ? "unnamed" : name.cptr() );

    return s;
}

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

///
/// Render engine API is not reentrant safe.
///
struct ApiReentrantChecker
{
    volatile SInt32 * mFlag;

    ApiReentrantChecker( volatile SInt32 * flag ) : mFlag(flag)
    {
        if( 0 != GN::atomCmpXchg32( mFlag, 1, 0 ) )
        {
            // We're doomed....
            GN_FATAL(sLogger)( "Render engine API reentrant!" );
            GN_DEBUG_BREAK();
        }
    }

    ~ApiReentrantChecker()
    {
        GN::atomDec32( mFlag );
    }
};


#if GN_RENDER_ENGINE_API_DUMP_ENABLED
#define RENDER_ENGINE_API( api ) \
    ApiReentrantChecker checker( &mApiReentrantFlag ); \
    RenderEngineApiDumper apidumper( api )
#else
#define RENDER_ENGINE_API( api ) \
    ApiReentrantChecker checker( &mApiReentrantFlag );
#endif

///
/// define to non-zero value to use fake render engine.
///
#define FAKE_RENDER_ENGINE 0

///
/// force render engine runs in serialize way.
///
#if 0
#define FORCE_SERALIZE()
#else
#define FORCE_SERALIZE() \
    if(1) { \
        mResourceThread->waitForIdle(); \
        mDrawThread->waitForIdle(); \
    } else void(0)
#endif

//
//
// -----------------------------------------------------------------------------
bool GN::engine::RenderEngine::init( const RenderEngineInitParameters & p )
{
    GN_GUARD;

    RENDER_ENGINE_API( "init" );

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

    // connect to renderer signals
    gSigRendererDispose.connect( mResourceLRU, &ResourceLRU::disposeAll );

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

    RENDER_ENGINE_API( "quit" );

    // disconnect to renderer signals
    gSigRendererDispose.disconnect( this );

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

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::clear()
{
    mResourceCache = 0;
    mResourceLRU = 0;
    mDrawThread = 0;
    mResourceThread = 0;
    mFenceManager = 0;
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
    RENDER_ENGINE_API( "reset" );

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
const GN::gfx::RendererOptions & GN::engine::RenderEngine::getRendererOptions() const
{
    RENDER_ENGINE_API( "getRenderOptions" );

    return mDrawThread->getRendererOptions();
}


//
//
// -----------------------------------------------------------------------------
const GN::gfx::DispDesc & GN::engine::RenderEngine::getDispDesc() const
{
    RENDER_ENGINE_API( "getDispDesc" );

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
    RENDER_ENGINE_API( "frameBegin" );

    return mDrawThread->frameBegin();
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::frameEnd()
{
    RENDER_ENGINE_API( "frameEnd" );

    mDrawThread->frameEnd();

    FORCE_SERALIZE();
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::setContext( const DrawContext & context )
{
    if( FAKE_RENDER_ENGINE ) return;

    RENDER_ENGINE_API( "setContext" );

    mDrawContext.mergeWith( context );

    sPrepareContextResources( *this, mDrawContext );

    DrawCommandHeader * dr = mDrawThread->submitDrawCommand1( DCT_SET_CONTEXT, context );
    if( 0 == dr ) return;

    sSetupDrawCommandWaitingList( *mResourceCache, mDrawContext, *dr );

    FORCE_SERALIZE();
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::setShaderUniform(
    GraphicsResource        * shader,
    const StrA              & uniformName,
    const gfx::UniformValue & value )
{
    if( FAKE_RENDER_ENGINE ) return;

    RENDER_ENGINE_API( "setShaderUniform" );

    GraphicsResourceItem * item = (GraphicsResourceItem*)shader;

    if( !mResourceCache->check( item ) ) return;

    // make sure the item is ready to use.
    sPrepareResource( *this, item );

    // get uniform data buffer
    const void * data;
    size_t bytes;
    switch( value.type )
    {
        case gfx::UVT_MATRIX44 :
            data = &value.matrix44s[0];
            bytes = value.matrix44s.size() * sizeof(Matrix44f);
            break;

        case gfx::UVT_VECTOR4 :
            data = &value.vector4s[0];
            bytes = value.vector4s.size() * sizeof(Vector4f);
            break;

        case gfx::UVT_BOOL :
        case gfx::UVT_INT :
        case gfx::UVT_FLOAT :
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
        GraphicsResourceItem * shader;
        char                   uniname[32];
        SInt32                 unitype;
    };


    // create new draw command
    DrawCommandHeader * dr = mDrawThread->submitDrawCommand( DCT_SET_UNIFORM, sizeof(ParamHeader) + bytes );
    if( 0 == dr ) return;
    ParamHeader * h = (ParamHeader*)dr->param();
    h->shader = item;
    memcpy( h->uniname, uniformName.cptr(), std::min<size_t>(uniformName.size()+1,32) );
    h->uniname[31] = 0;
    h->unitype = value.type;
    ++h;
    memcpy( h, data, bytes );

    // setup draw command and resource relationship
    sSetupWaitingListAndReferenceFence(
        *mResourceCache,
        item,
        *dr );

    FORCE_SERALIZE();
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::clearScreen(
    const Vector4f & c,
    float z, UInt8 s,
    BitFields flags )
{
    if( FAKE_RENDER_ENGINE ) return;

    RENDER_ENGINE_API( "clearScreen" );

    sPrepareContextResources( *this, mDrawContext );
    DrawCommandHeader * dr = mDrawThread->submitDrawCommand4( DCT_CLEAR, c, z, s, flags );
    if( 0 == dr ) return;
    sSetupDrawCommandWaitingList( *mResourceCache, mDrawContext, *dr );

    FORCE_SERALIZE();
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::drawIndexed(
    SInt32 prim,
    UInt32 numprim,
    UInt32 startvtx,
    UInt32 minvtxidx,
    UInt32 numvtx,
    UInt32 startidx )
{
    if( FAKE_RENDER_ENGINE ) return;

    RENDER_ENGINE_API( "drawIndexed" );

    sPrepareContextResources( *this, mDrawContext );
    DrawCommandHeader * dr = mDrawThread->submitDrawCommand6( DCT_DRAW_INDEXED, prim, numprim, startvtx, minvtxidx, numvtx, startidx );
    if( 0 == dr ) return;
    sSetupDrawCommandWaitingList( *mResourceCache, mDrawContext, *dr );

    FORCE_SERALIZE();
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::draw(
    SInt32 prim,
    UInt32 numprim,
    UInt32 startvtx )
{
    if( FAKE_RENDER_ENGINE ) return;

    RENDER_ENGINE_API( "draw" );

    sPrepareContextResources( *this, mDrawContext );
    DrawCommandHeader * dr = mDrawThread->submitDrawCommand3( DCT_DRAW, prim, numprim, startvtx );
    if( 0 == dr ) return;
    sSetupDrawCommandWaitingList( *mResourceCache, mDrawContext, *dr );

    FORCE_SERALIZE();
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::drawLines(
    BitFields         options,
    const void *      positions,
    size_t            stride,
    size_t            count,
    UInt32            rgba,
    const Matrix44f & model,
    const Matrix44f & view,
    const Matrix44f & proj )
{
    if( FAKE_RENDER_ENGINE ) return;

    RENDER_ENGINE_API( "drawLines" );

    sPrepareContextResources( *this, mDrawContext );

    struct Param
    {
        BitFields options;
        size_t    stride;
        size_t    count;
        UInt32    rgba;
        Matrix44f model;
        Matrix44f view;
        Matrix44f proj;
    };
    size_t bufsize = stride * count * 2;

    DrawCommandHeader * dr = mDrawThread->submitDrawCommand( DCT_DRAW_LINE, sizeof(Param)+bufsize );
    if( 0 == dr ) return;

    Param * p = (Param*)dr->param();
    p->options = options;
    p->stride = stride;
    p->count = count;
    p->rgba = rgba;
    p->model = model;
    p->view = view;
    p->proj = proj;
    memcpy( (p+1), positions, bufsize );

    sSetupDrawCommandWaitingList( *mResourceCache, mDrawContext, *dr );

    FORCE_SERALIZE();
}

// *****************************************************************************
// resource commands
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::engine::GraphicsResource *
GN::engine::RenderEngine::allocResource( const GraphicsResourceDesc & desc )
{
    RENDER_ENGINE_API( "allocResource" );

    if( GN_RENDER_ENGINE_API_DUMP_ENABLED )
    {
        dumpApiString( strFormat( "<CreateGraphicsResource %s/>", desc.toString().cptr() ) );
    }

    GraphicsResourceItem * item = mResourceCache->alloc( desc );
    if( 0 == item ) return 0;
    mResourceLRU->insert( item );
    return item;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::freeResource( GraphicsResource * res )
{
    RENDER_ENGINE_API( "freeResource" );

    GraphicsResourceItem * item = (GraphicsResourceItem*)res;

    if( !mResourceCache->check( item ) ) return;

    // TODO: check if the resource is using by current context.

    // make sure the the resource item is disposed.
    mResourceLRU->dispose( item );
    mDrawThread->waitForIdle();

    if( GN_RENDER_ENGINE_API_DUMP_ENABLED )
    {
        dumpApiString( strFormat( "<FreeGraphicsResource %s/>", res->desc.toString().cptr() ) );
    }

    mResourceLRU->remove( item );
    return mResourceCache->free( item );
}

//
//
// -----------------------------------------------------------------------------
bool GN::engine::RenderEngine::checkResource( const GraphicsResource * res ) const
{
    RENDER_ENGINE_API( "checkResource" );

    const GraphicsResourceItem * item = (const GraphicsResourceItem*)res;
    return mResourceCache->check( item );
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::disposeResource( GraphicsResource * res )
{
    if( FAKE_RENDER_ENGINE ) return;

    RENDER_ENGINE_API( "disposeResource" );

    GraphicsResourceItem * item = (GraphicsResourceItem*)res;

    if( !mResourceCache->check( item ) ) return;

    mResourceLRU->dispose( item );

    FORCE_SERALIZE();
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::disposeAllResources()
{
    if( FAKE_RENDER_ENGINE ) return;

    RENDER_ENGINE_API( "disposeAllResources" );

    mResourceLRU->disposeAll();

    FORCE_SERALIZE();
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::updateResource(
    GraphicsResource       * res,
    int                      lod,
    GraphicsResourceLoader * loader )
{
    if( FAKE_RENDER_ENGINE ) return;

    RENDER_ENGINE_API( "updateResource" );

    GraphicsResourceItem * item = (GraphicsResourceItem*)res;

    if( !mResourceCache->check( item ) ) return;

    mResourceLRU->realize( item, 0 );

    mResourceThread->submitResourceLoadingCommand( item, lod, loader );

    FORCE_SERALIZE();
}

// *****************************************************************************
// helpers
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::engine::GraphicsResource * GN::engine::RenderEngine::createShader(
    const StrA       & name,
    const ShaderDesc & sd )
{
    return createShader( name, sd.type, sd.lang, sd.code, sd.hints );
}

//
//
// -----------------------------------------------------------------------------
GN::engine::GraphicsResource * GN::engine::RenderEngine::createShader(
    const StrA         & name,
    gfx::ShaderType      type,
    gfx::ShadingLanguage lang,
    const StrA         & code,
    const StrA         & hints )
{
    GraphicsResourceDesc desc;
    desc.name = name;
    desc.type = GRT_SHADER;
    desc.sd.type = type;
    desc.sd.lang = lang;
    desc.sd.code = code;
    desc.sd.hints = hints;

    GraphicsResource * res = allocResource( desc );
    if( 0 == res ) return 0;

    AutoRef<DummyLoader> dummyloader( new DummyLoader );

    updateResource( res, 0, dummyloader );

    return res;
}

//
//
// -----------------------------------------------------------------------------
GN::engine::GraphicsResource * GN::engine::RenderEngine::createVtxFmt(
    const StrA            & name,
    const gfx::VtxFmtDesc & fd )
{
    GraphicsResourceDesc desc;
    desc.name = name;
    desc.type = GRT_VTXFMT;
    desc.fd = fd;

    GraphicsResource * res = allocResource( desc );
    if( 0 == res ) return 0;

    AutoRef<DummyLoader> dummyloader( new DummyLoader );

    updateResource( res, 0, dummyloader );

    return res;
}

//
//
// -----------------------------------------------------------------------------
GN::engine::GraphicsResource * GN::engine::RenderEngine::createVtxBuf(
    const StrA            & name,
    const gfx::VtxBufDesc & desc )
{
    return createVtxBuf( name, desc.bytes, desc.dynamic, desc.readback );
}

//
//
// -----------------------------------------------------------------------------
GN::engine::GraphicsResource * GN::engine::RenderEngine::createVtxBuf(
    const StrA & name,
    size_t       bytes,
    bool         dynamic,
    bool         readback )
{
    GraphicsResourceDesc grd;
    grd.name        = name;
    grd.type        = GRT_VTXBUF;
    grd.vd.bytes    = (UInt32)bytes;
    grd.vd.dynamic  = dynamic;
    grd.vd.readback = readback;
    return allocResource( grd );
}

//
//
// -----------------------------------------------------------------------------
GN::engine::GraphicsResource * GN::engine::RenderEngine::createIdxBuf(
    const StrA            & name,
    const gfx::IdxBufDesc & desc )
{
    return createIdxBuf( name, desc.numidx, desc.dynamic, desc.readback );
}

//
//
// -----------------------------------------------------------------------------
GN::engine::GraphicsResource * GN::engine::RenderEngine::createIdxBuf(
    const StrA & name,
    size_t       numidx,
    bool         dynamic,
    bool         readback )
{
    GraphicsResourceDesc grd;
    grd.name        = name;
    grd.type        = GRT_IDXBUF;
    grd.id.numidx   = (UInt32)numidx;
    grd.id.dynamic  = dynamic;
    grd.id.readback = readback;
    return allocResource( grd );
}

//
//
// -----------------------------------------------------------------------------
GN::engine::GraphicsResource * GN::engine::RenderEngine::createTexture(
    const StrA             & name,
    const gfx::TextureDesc & desc )
{
    GraphicsResourceDesc grd;
    grd.name        = name;
    grd.type        = GRT_TEXTURE;
    grd.td          = desc;
    return allocResource( grd );
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::clearDrawContext()
{
    static struct Local
    {
       DrawContext ctx;

       Local() { ctx.resetToDefault(); }
    } local;

    setContext( local.ctx );
}
