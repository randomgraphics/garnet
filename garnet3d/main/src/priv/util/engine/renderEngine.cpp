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
    GN::engine::GraphicsResourceItem * item )
{
    using namespace GN::engine;

    if( 0 == item ) return;

    if( !engine.resourceCache().check(item) ) return;

    bool reload;

    engine.resourceLRU().realize( item, &reload );

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
        if( context.flags.shaderBit( i ) ) sPrepareResource( engine, (GraphicsResourceItem*)context.shaders[i] );
    }
    if( context.flags.renderTargets )
    {
        for( int i = 0; i < gfx::MAX_RENDER_TARGETS; ++i )
        {
            sPrepareResource( engine, (GraphicsResourceItem*)context.renderTargets.cbuffers[i].texture );
        }
        sPrepareResource( engine, (GraphicsResourceItem*)context.renderTargets.zbuffer.texture );
    }
    if( context.flags.vtxFmt )
    {
        sPrepareResource( engine, (GraphicsResourceItem*)context.vtxFmt );
    }
    if( context.flags.textures )
    {
        for( unsigned int i = 0; i < context.numTextures; ++i )
        {
            sPrepareResource( engine, (GraphicsResourceItem*)context.textures[i] );
        }
    }
    if( context.flags.vtxBufs )
    {
        for( unsigned int i = 0; i < context.numVtxBufs; ++i )
        {
            sPrepareResource( engine, (GraphicsResourceItem*)context.vtxBufs[i].buffer );
        }
    }
    if( context.flags.idxBuf )
    {
        sPrepareResource( engine, (GraphicsResourceItem*)context.idxBuf );
    }
}

//
//
// -----------------------------------------------------------------------------
static inline void sSetupWaitingListAndReferenceFence(
    GN::engine::RenderEngine::GraphicsResourceCache & cache,
    GN::engine::GraphicsResourceItem * item,
    GN::engine::DrawCommandHeader & dr )
{
    using namespace GN::engine;

    if( 0 == item ) return;

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
                (GraphicsResourceItem*)context.shaders[i],
                dr );
        }
    }
    if( context.flags.renderTargets )
    {
        for( int i = 0; i < gfx::MAX_RENDER_TARGETS; ++i )
        {
            sSetupWaitingListAndReferenceFence(
                cache,
                (GraphicsResourceItem*)context.renderTargets.cbuffers[i].texture,
                dr );
        }
        sSetupWaitingListAndReferenceFence(
            cache,
            (GraphicsResourceItem*)context.renderTargets.zbuffer.texture,
            dr );
    }
    if( context.flags.vtxFmt )
    {
        sSetupWaitingListAndReferenceFence(
            cache,
            (GraphicsResourceItem*)context.vtxFmt,
            dr );
    }
    if( context.flags.textures )
    {
        for( unsigned int i = 0; i < context.numTextures; ++i )
        {
            sSetupWaitingListAndReferenceFence(
                cache,
                (GraphicsResourceItem*)context.textures[i],
                dr );
        }
    }
    if( context.flags.vtxBufs )
    {
        for( unsigned int i = 0; i < context.numVtxBufs; ++i )
        {
            sSetupWaitingListAndReferenceFence(
                cache,
                (GraphicsResourceItem*)context.vtxBufs[i].buffer,
                dr );
        }
    }
    if( context.flags.idxBuf )
    {
        sSetupWaitingListAndReferenceFence(
            cache,
            (GraphicsResourceItem*)context.idxBuf,
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
    GraphicsResource        * shader,
    const StrA              & uniformName,
    const gfx::UniformValue & value )
{
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
        GraphicsResourceItem * shader;
        char                   uniname[32];
        SInt32                 unitype;
    } header;

    header.shader = item;
    memcpy( header.uniname, uniformName.cptr(), std::min<size_t>(uniformName.size()+1,32) );
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
        item,
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
GN::engine::GraphicsResource *
GN::engine::RenderEngine::allocResource( const GraphicsResourceDesc & desc )
{
    if( GN_ENGINE_DUMP_ENABLED )
    {
        dumpString( strFormat( "<CreateGraphicsResource %s/>", desc.toString().cptr() ) );
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
    GraphicsResourceItem * item = (GraphicsResourceItem*)res;

    if( !mResourceCache->check( item ) ) return;

    // TODO: check if the resource is using by current context.

    // make sure the the resource item is disposed.
    mResourceLRU->dispose( item );
    mDrawThread->waitForIdle();

    if( GN_ENGINE_DUMP_ENABLED )
    {
        dumpString( strFormat( "<FreeGraphicsResource %s/>", res->desc.toString().cptr() ) );
    }

    mResourceLRU->remove( item );
    return mResourceCache->free( item );
}

//
//
// -----------------------------------------------------------------------------
bool GN::engine::RenderEngine::checkResource( GraphicsResource * res )
{
    GraphicsResourceItem * item = (GraphicsResourceItem*)res;
    return mResourceCache->check( item );
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::disposeResource( GraphicsResource * res )
{
    GraphicsResourceItem * item = (GraphicsResourceItem*)res;

    if( !mResourceCache->check( item ) ) return;

    mResourceLRU->dispose( item );
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
    GraphicsResource       * res,
    int                      lod,
    GraphicsResourceLoader * loader )
{
    GraphicsResourceItem * item = (GraphicsResourceItem*)res;

    if( !mResourceCache->check( item ) ) return;

    mResourceLRU->realize( item, 0 );

    mResourceThread->submitResourceLoadingCommand( item, lod, loader );
}

// *****************************************************************************
// global helper functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::engine::GraphicsResource * GN::engine::createShader(
    RenderEngine     & eng,
    const ShaderDesc & sd,
    const StrA       & name )
{
    return createShader( eng, sd.type, sd.lang, sd.code, sd.hints, name );
}

//
//
// -----------------------------------------------------------------------------
GN::engine::GraphicsResource * GN::engine::createShader(
    RenderEngine       & eng,
    gfx::ShaderType      type,
    gfx::ShadingLanguage lang,
    const StrA         & code,
    const StrA         & hints,
    const StrA         & name)
{
    GraphicsResourceDesc desc;
    desc.name = name;
    desc.type = GRT_SHADER;
    desc.sd.type = type;
    desc.sd.lang = lang;
    desc.sd.code = code;
    desc.sd.hints = hints;

    GraphicsResource * res = eng.allocResource( desc );
    if( 0 == res ) return 0;

    AutoRef<DummyLoader> dummyloader( new DummyLoader );

    eng.updateResource( res, 0, dummyloader );

    return res;
}

//
//
// -----------------------------------------------------------------------------
GN::engine::GraphicsResource * GN::engine::createVtxFmt(
    GN::engine::RenderEngine & eng,
    const gfx::VtxFmtDesc    & fd,
    const StrA               & name )
{
    GraphicsResourceDesc desc;
    desc.name = name;
    desc.type = GRT_VTXFMT;
    desc.fd = fd;

    GraphicsResource * res = eng.allocResource( desc );
    if( 0 == res ) return 0;

    AutoRef<DummyLoader> dummyloader( new DummyLoader );

    eng.updateResource( res, 0, dummyloader );

    return res;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::clearDrawContext( RenderEngine & eng )
{
    static struct Local
    {
       DrawContext ctx;

       Local() { ctx.resetToDefault(); }
    } local;

    eng.setContext( local.ctx );
}
