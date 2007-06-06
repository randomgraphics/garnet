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
        for( unsigned int i = 0; i < context.renderTargets.count; ++i )
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
        for( unsigned int i = 0; i < context.renderTargets.count; ++i )
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
/// loader for shader and vertex format and render target texture
///
class DummyLoader : public GN::engine::GraphicsResourceLoader
{
protected:

    DummyLoader() {}
    ~DummyLoader() {}

public:

    static DummyLoader * sGetInstance()
    {
        static GN::AutoRef<DummyLoader> sInstance( new DummyLoader );
        return sInstance;
    }

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

///
/// basic loader for vertex and index buffer
///
class BasicLoader : public GN::engine::GraphicsResourceLoader
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

///
/// static vertex buffer loader
///
class StaticVtxBufLoader : public BasicLoader
{
    GN::DynaArray<UInt8> mData;

public:

    StaticVtxBufLoader( const void * data, size_t bytes )
    {
        mData.resize( bytes );
        memcpy( mData.cptr(), data, bytes );
    }

    virtual bool copy( GN::engine::GraphicsResource & res, const void * , size_t, int )
    {
        GN_ASSERT( GN::engine::GRT_VTXBUF == res.desc.type );
        GN_ASSERT( res.desc.vd.bytes == mData.size() );
        GN::gfx::VtxBuf * vb = res.vtxbuf;
        void * data = vb->lock( 0, 0, GN::gfx::LOCK_DISCARD );
        if( 0 == data ) return false;
        memcpy( data, mData.cptr(), mData.size() );
        vb->unlock();
        return true;
    }
};

///
/// static index buffer loader
///
class StaticIdxBufLoader : public BasicLoader
{
    GN::DynaArray<UInt8> mData;

public:

    StaticIdxBufLoader( const void * data, size_t bytes )
    {
        mData.resize( bytes );
        memcpy( mData.cptr(), data, bytes );
    }

    virtual bool copy( GN::engine::GraphicsResource & res, const void * , size_t, int )
    {
        GN_ASSERT( GN::engine::GRT_IDXBUF == res.desc.type );
        GN_ASSERT( res.desc.id.numidx*2 == mData.size() );
        GN::gfx::IdxBuf * ib = res.idxbuf;
        void * data = ib->lock( 0, 0, GN::gfx::LOCK_DISCARD );
        if( 0 == data ) return false;
        memcpy( data, mData.cptr(), mData.size() );
        ib->unlock();
        return true;
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
// Render engine
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
/// uncomment this macro to force render engine runs in serialize way
///
#define FORCE_SERALIZE() // mDrawThread->waitForIdle();

// *****************************************************************************
// ctor / dtor
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::engine::RenderEngine::RenderEngine() : mApiReentrantFlag(0)
{
    clear();

    // connect to renderer signals
    gSigRendererCreate.connect( this, &RenderEngine::onRendererCreate );
    gSigRendererRestore.connect( this, &RenderEngine::onRendererRestore );
    gSigRendererDispose.connect( this, &RenderEngine::onRendererDispose );
    gSigRendererDestroy.connect( this, &RenderEngine::onRendererDestroy );
    gSigRendererWindowSizeMove.connect( this, &RenderEngine::onRenderWindowSizeMove );
}

//
//
// -----------------------------------------------------------------------------
GN::engine::RenderEngine::~RenderEngine()
{
    quit();
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

    return internalResetRenderer( api, ro );
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

//
//
// -----------------------------------------------------------------------------
UInt32 GN::engine::RenderEngine::getCaps( SInt32 c ) const
{
    return gRenderer.getCaps( c );
}

//
//
// -----------------------------------------------------------------------------
bool GN::engine::RenderEngine::supportShader( const StrA & profile )
{
    return gRenderer.supportShader( profile );
}

//
//
// -----------------------------------------------------------------------------
bool GN::engine::RenderEngine::supportTextureFormat( gfx::TexDim type, BitFields usage, gfx::ClrFmt format )
{
    return gRenderer.supportTextureFormat( type, usage, format );
}

//
//
// -----------------------------------------------------------------------------
bool GN::engine::RenderEngine::internalResetRenderer(
    gfx::RendererAPI api,
    const gfx::RendererOptions & ro )
{
    // dispose all graphics resources
    mResourceLRU->disposeAll();
    mResourceThread->waitForIdle();
    mDrawThread->waitForIdle();

    // clear context
    mDrawContext.clearToNull();

    // renderer signal queue should be empty now.
    // Note that here we don't need to use mutex, because draw thread must be idle now.
    GN_ASSERT( mRendererSignals.empty() );

    // reset the renderer
    bool result = mDrawThread->resetRenderer( api, ro );

    // clear renderer signals
    // Do not have to use mutex, for the same reason as above.
    mRendererSignals.clear();

    // done.
    return result;
}

// *****************************************************************************
// draw request management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::setContext( const DrawContext & context )
{
    RENDER_ENGINE_API( "setContext" );

    if( FAKE_RENDER_ENGINE ) return;

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
    RENDER_ENGINE_API( "setShaderUniform" );

    if( FAKE_RENDER_ENGINE ) return;

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
    RENDER_ENGINE_API( "clearScreen" );

    if( FAKE_RENDER_ENGINE ) return;

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
    size_t numprim,
    size_t startvtx,
    size_t minvtxidx,
    size_t numvtx,
    size_t startidx )
{
    RENDER_ENGINE_API( "drawIndexed" );

    if( FAKE_RENDER_ENGINE ) return;

    sPrepareContextResources( *this, mDrawContext );
    DrawCommandHeader * dr = mDrawThread->submitDrawCommand6( DCT_DRAW_INDEXED, prim, (UInt32)numprim, (UInt32)startvtx, (UInt32)minvtxidx, (UInt32)numvtx, (UInt32)startidx );
    if( 0 == dr ) return;
    sSetupDrawCommandWaitingList( *mResourceCache, mDrawContext, *dr );

    FORCE_SERALIZE();
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::draw(
    SInt32 prim,
    size_t numprim,
    size_t startvtx )
{
    RENDER_ENGINE_API( "draw" );

    if( FAKE_RENDER_ENGINE ) return;

    sPrepareContextResources( *this, mDrawContext );
    DrawCommandHeader * dr = mDrawThread->submitDrawCommand3( DCT_DRAW, prim, (UInt32)numprim, (UInt32)startvtx );
    if( 0 == dr ) return;
    sSetupDrawCommandWaitingList( *mResourceCache, mDrawContext, *dr );

    FORCE_SERALIZE();
}

void GN::engine::RenderEngine::drawIndexedUp(
    gfx::PrimitiveType prim,
    size_t             numprim,
    size_t             numvtx,
    const void       * vertexData,
    size_t             strideInBytes,
    const UInt16     * indexData )
{
    RENDER_ENGINE_API( "draw" );

    if( FAKE_RENDER_ENGINE ) return;

    sPrepareContextResources( *this, mDrawContext );

    struct Param
    {
        SInt32 prim;
        size_t numprim;
        size_t numvtx;
        size_t vtxstride;
        size_t numidx;
    };

    size_t numidx  = gfx::calcVertexCount( prim, numprim );
    size_t vbsize  = numvtx * strideInBytes;
    size_t ibsize  = numidx * sizeof(UInt16);
    size_t bufsize = sizeof(Param) + vbsize + ibsize;

    DrawCommandHeader * dr = mDrawThread->submitDrawCommand( DCT_DRAW_INDEXED_UP, bufsize );
    if( 0 == dr ) return;

    Param * p = (Param*)dr->param();
    p->prim      = prim;
    p->numprim   = numprim;
    p->numvtx    = numvtx;
    p->vtxstride = strideInBytes;
    p->numidx    = numidx;

    UInt8 * vertices = (UInt8*)(p+1);
    void * indices   = vertices + vbsize;

    memcpy( vertices, vertexData, vbsize );
    memcpy( indices, indexData, ibsize );

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
    RENDER_ENGINE_API( "drawLines" );

    if( FAKE_RENDER_ENGINE ) return;

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

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::present()
{
    RENDER_ENGINE_API( "present" );

    mDrawThread->present();

    // handle renderer signals
    mRendererSignalMutex.lock();
    bool hasSignals = !mRendererSignals.empty();
    mRendererSignalMutex.unlock();
    if( hasSignals )
    {
        // make a local copy of the signals.
        DynaArray<RendererSignal> signals;
        {
            ScopeMutex<SpinLoop> lock(mRendererSignalMutex);

            signals.resize( mRendererSignals.size() );
            memcpy(
                signals.cptr(),
                mRendererSignals.cptr(),
                sizeof(RendererSignal)*mRendererSignals.size() );
            mRendererSignals.clear();
        }

        bool needDispose = false;
        bool needReset = false;
        gfx::RendererOptions o;
        for( size_t i = 0; i < signals.size(); ++i )
        {
            const RendererSignal & s = signals[i];
            switch( s.type )
            {
                case RENDERER_SIZEMOVE :
                    needReset = true;
                    o = mDrawThread->getRendererOptions();
                    o.monitorHandle = s.sizemove.monitor;
                    o.windowedWidth = s.sizemove.width;
                    o.windowedHeight = s.sizemove.height;
                    break;

                case RENDERER_DISPOSE  :
                    needDispose = true;
                    break;

                case RENDERER_CREATE   :
                case RENDERER_RESTORE  :
                case RENDERER_DESTROY  :
                    break; // do nothing

                default                :
                    GN_UNEXPECTED();
            }
        }

        if( needReset )
        {
            if( !internalResetRenderer( mDrawThread->getRendererApi(), o ) )
            {
                GN_THROW( "fail to handle renderer sizemove signal!" );
            }
        } else if( needDispose )
        {
           mResourceLRU->disposeAll();
        }
    }

    FORCE_SERALIZE();

    // profile frame time
#if GN_PROFILE_ENABLED
    mFrameProfiler.nextFrame();
#endif
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
    RENDER_ENGINE_API( "disposeResource" );

    if( FAKE_RENDER_ENGINE ) return;

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
    RENDER_ENGINE_API( "disposeAllResources" );

    if( FAKE_RENDER_ENGINE ) return;

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
    RENDER_ENGINE_API( "updateResource" );

    if( FAKE_RENDER_ENGINE ) return;

    GraphicsResourceItem * item = (GraphicsResourceItem*)res;

    if( !mResourceCache->check( item ) ) return;

    mResourceLRU->realize( item, 0 );

    mResourceThread->submitResourceLoadingCommand( item, lod, loader );

    FORCE_SERALIZE();
}

// *****************************************************************************
// Mini application management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::engine::MiniAppId GN::engine::RenderEngine::registerMiniApp( MiniApp * ma )
{
    RENDER_ENGINE_API( "registerMiniApp" );

    if( 0 == ma )
    {
        GN_ERROR(sLogger)( "NULL application pointer!" );
        return 0;
    }
    if( mMiniApps.find( ma ) )
    {
        GN_ERROR(sLogger)( "redundant mini-application registration: 0x%p!", ma );
        return 0;
    }

    MiniAppId id = mMiniApps.add( ma );

    mDrawThread->submitDrawCommand1( DCT_MINIAPP_CTOR, ma );
    mDrawThread->submitDrawCommand1( DCT_MINIAPP_CREATE, ma );
    mDrawThread->submitDrawCommand1( DCT_MINIAPP_RESTORE, ma );

    FORCE_SERALIZE();

    return id;
}

//
//
// -----------------------------------------------------------------------------
GN::engine::MiniApp * GN::engine::RenderEngine::unregisterMiniApp( MiniAppId id )
{
    RENDER_ENGINE_API( "unregisterMiniApp" );

    if( !mMiniApps.validHandle( id ) )
    {
        GN_ERROR(sLogger)( "Invalid mini application id: %d", id );
        return 0;
    }

    MiniApp * ma = mMiniApps.get( id );
    GN_ASSERT( ma );
    mMiniApps.remove( id );

    mDrawThread->submitDrawCommand1( DCT_MINIAPP_DISPOSE, ma );
    mDrawThread->submitDrawCommand1( DCT_MINIAPP_DESTROY, ma );
    mDrawThread->submitDrawCommand1( DCT_MINIAPP_DTOR, ma );

    mResourceThread->waitForIdle();
    mDrawThread->waitForIdle();

    FORCE_SERALIZE();

    return ma;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::runMiniApp( MiniAppId id )
{
    if( FAKE_RENDER_ENGINE ) return;

    RENDER_ENGINE_API( "runMiniApp" );

    if( !mMiniApps.validHandle( id ) )
    {
        GN_ERROR(sLogger)( "Invalid mini application id: %d", id );
        return;
    }

    mDrawThread->submitDrawCommand1( DCT_MINIAPP_RUN, mMiniApps[id] );

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

    updateResource( res, 0, DummyLoader::sGetInstance() );

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

    updateResource( res, 0, DummyLoader::sGetInstance() );

    return res;
}

//
//
// -----------------------------------------------------------------------------
GN::engine::GraphicsResource * GN::engine::RenderEngine::createVtxBuf(
    const StrA            & name,
    const gfx::VtxBufDesc & desc,
    const void            * initialData )
{
    GraphicsResourceDesc grd;
    grd.name = name;
    grd.type = GRT_VTXBUF;
    grd.vd   = desc;
    GraphicsResource * res = allocResource( grd );

    if( res && initialData )
    {
        AutoRef<StaticVtxBufLoader> loader( new StaticVtxBufLoader( initialData, grd.vd.bytes ) );
        updateResource( res, 0, loader );
    }

    return res;
}

//
//
// -----------------------------------------------------------------------------
GN::engine::GraphicsResource * GN::engine::RenderEngine::createVtxBuf(
    const StrA & name,
    size_t       bytes,
    bool         dynamic,
    bool         readback,
    const void * initialData )
{
    gfx::VtxBufDesc desc;
    desc.bytes    = (UInt32)bytes;
    desc.dynamic  = dynamic;
    desc.readback = readback;
    return createVtxBuf( name, desc, initialData );
}

//
//
// -----------------------------------------------------------------------------
GN::engine::GraphicsResource * GN::engine::RenderEngine::createIdxBuf(
    const StrA            & name,
    const gfx::IdxBufDesc & desc,
    const void            * initialData )
{
    GraphicsResourceDesc grd;
    grd.name = name;
    grd.type = GRT_IDXBUF;
    grd.id   = desc;
    GraphicsResource * res = allocResource( grd );

    if( res && initialData )
    {
        AutoRef<StaticIdxBufLoader> loader( new StaticIdxBufLoader( initialData, grd.id.numidx*2 ) );
        updateResource( res, 0, loader );
    }

    return res;
}

//
//
// -----------------------------------------------------------------------------
GN::engine::GraphicsResource * GN::engine::RenderEngine::createIdxBuf(
    const StrA & name,
    size_t       numidx,
    bool         dynamic,
    bool         readback,
    const void * initialData )
{
    gfx::IdxBufDesc id;
    id.numidx   = (UInt32)numidx;
    id.dynamic  = dynamic;
    id.readback = readback;
    return createIdxBuf( name, id, initialData );
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
GN::engine::GraphicsResource * GN::engine::RenderEngine::createTexture( const StrA & name,
    gfx::TexDim  dim,
    size_t       sx,
    size_t       sy,
    size_t       sz,
    size_t       faces,
    size_t       levels,
    gfx::ClrFmt  format,
    BitFields    usage )
{
    gfx::TextureDesc desc =
    {
        dim,
        (UInt32)sx, (UInt32)sy, (UInt32)sz,
        (UInt32)faces, (UInt32)levels,
        gfx::FMT_UNKNOWN == format ? gRenderer.getDefaultTextureFormat( dim, usage ) : format,
        { usage }
    };
    return createTexture( name, desc );
}

//
//
// -----------------------------------------------------------------------------
GN::engine::GraphicsResource *
GN::engine::RenderEngine::create2DRenderTargetTexture(
    const StrA & name,
    size_t       sx,
    size_t       sy,
    size_t       levels,
    gfx::ClrFmt  format )
{
    GraphicsResource * res = createTexture( name, gfx::TEXDIM_2D, sx, sy, 1, 1, levels, format, gfx::TEXUSAGE_RENDER_TARGET );

    if( res )
    {
        updateResource( res, 0, DummyLoader::sGetInstance() );
    }

    return res;
}

//
//
// -----------------------------------------------------------------------------
GN::engine::GraphicsResource *
GN::engine::RenderEngine::createCubeRenderTargetTexture(
    const StrA & name,
    size_t       sx,
    size_t       levels,
    gfx::ClrFmt  format )
{
    GraphicsResource * res = createTexture( name, gfx::TEXDIM_CUBE, sx, sx, 1, 6, levels, format, gfx::TEXUSAGE_RENDER_TARGET );

    if( res )
    {
        updateResource( res, 0, DummyLoader::sGetInstance() );
    }

    return res;
}

GN::engine::GraphicsResource *
GN::engine::RenderEngine::create2DDepthTexture(
    const StrA & name,
    size_t       sx,
    size_t       sy,
    size_t       levels,
    gfx::ClrFmt  format )
{
    GraphicsResource * res = createTexture( name, gfx::TEXDIM_2D, sx, sy, 1, 1, levels, format, gfx::TEXUSAGE_DEPTH );

    if( res )
    {
        updateResource( res, 0, DummyLoader::sGetInstance() );
    }

    return res;
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

//
//
// -----------------------------------------------------------------------------
GN::Matrix44f &
GN::engine::RenderEngine::composePerspectiveMatrixLh( Matrix44f & result,
                            float fovy,
                            float ratio,
                            float znear,
                            float zfar ) const
{
    return gfx::API_OGL == mDrawThread->getRendererApi()
        ? result.perspectiveOGLLh( fovy, ratio, znear, zfar )
        : result.perspectiveD3DLh( fovy, ratio, znear, zfar );
}

//
//
// -----------------------------------------------------------------------------
GN::Matrix44f &
GN::engine::RenderEngine::composePerspectiveMatrixRh( Matrix44f & result,
                            float fovy,
                            float ratio,
                            float znear,
                            float zfar ) const
{
    return gfx::API_OGL == mDrawThread->getRendererApi()
        ? result.perspectiveOGLRh( fovy, ratio, znear, zfar )
        : result.perspectiveD3DRh( fovy, ratio, znear, zfar );
}

//
//
// -----------------------------------------------------------------------------
GN::Matrix44f &
GN::engine::RenderEngine::composePerspectiveMatrix( Matrix44f & result,
                          float fovy,
                          float ratio,
                          float znear,
                          float zfar ) const
{
    return gfx::API_OGL == mDrawThread->getRendererApi()
        ? result.perspectiveOGL( fovy, ratio, znear, zfar )
        : result.perspectiveD3D( fovy, ratio, znear, zfar );
}

//
//
// -----------------------------------------------------------------------------
GN::Matrix44f &
GN::engine::RenderEngine::composeOrthoMatrix( Matrix44f & result,
                    float left,
                    float bottom,
                    float width,
                    float height,
                    float znear,
                    float zfar ) const
{
    return gfx::API_OGL == mDrawThread->getRendererApi()
        ? result.orthoOGL( left, left+width, bottom, bottom+height, znear, zfar )
        : result.orthoD3D( left, left+width, bottom, bottom+height, znear, zfar );
}

// *****************************************************************************
// private methods
// *****************************************************************************

// Note : onRendererXXXX() methods must be called in draw thread.

//
//
// -----------------------------------------------------------------------------
bool GN::engine::RenderEngine::onRendererCreate()
{
    GN_ASSERT( mDrawThread->isDrawThread() );

    ScopeMutex<SpinLoop> lock(mRendererSignalMutex);

    mRendererSignals.append( RENDERER_CREATE );

    for( MiniAppId i = mMiniApps.first(); 0 != i ; i = mMiniApps.next( i ) )
    {
        MiniApp * ma = mMiniApps[i];
        GN_ASSERT( ma );
        if( ma->noerr ) ma->noerr = ma->onRendererCreate();
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
bool GN::engine::RenderEngine::onRendererRestore()
{
    GN_ASSERT( mDrawThread->isDrawThread() );

    ScopeMutex<SpinLoop> lock(mRendererSignalMutex);

    mRendererSignals.append( RENDERER_RESTORE );

    for( MiniAppId i = mMiniApps.first(); 0 != i ; i = mMiniApps.next( i ) )
    {
        MiniApp * ma = mMiniApps[i];
        GN_ASSERT( ma );
        if( ma->noerr ) ma->noerr = ma->onRendererRestore();
    }

    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::onRendererDispose()
{
    GN_ASSERT( mDrawThread->isDrawThread() );

    ScopeMutex<SpinLoop> lock(mRendererSignalMutex);

    mRendererSignals.append( RENDERER_DISPOSE );

    for( MiniAppId i = mMiniApps.first(); 0 != i ; i = mMiniApps.next( i ) )
    {
        MiniApp * ma = mMiniApps[i];
        GN_ASSERT( ma );
        ma->onRendererDispose();
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::onRendererDestroy()
{
    GN_ASSERT( mDrawThread->isDrawThread() );

    ScopeMutex<SpinLoop> lock(mRendererSignalMutex);

    mRendererSignals.append( RENDERER_DESTROY );

    for( MiniAppId i = mMiniApps.first(); 0 != i ; i = mMiniApps.next( i ) )
    {
        MiniApp * ma = mMiniApps[i];
        GN_ASSERT( ma );
        ma->onRendererDestroy();
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::onRenderWindowSizeMove( HandleType m, UInt32 w, UInt32 h )
{
    GN_ASSERT( mDrawThread->isDrawThread() );

    ScopeMutex<SpinLoop> lock(mRendererSignalMutex);

    RendererSignal s( RENDERER_SIZEMOVE );

    s.sizemove.monitor = m;
    s.sizemove.width = w;
    s.sizemove.height = h;

    mRendererSignals.append( s );
}
