#include "pch.h"
#include "fenceManager.h"
#include "resourceCache.h"
#include "resourceLRU.h"
#include "resourceThread.h"
#include "drawThread.h"

static GN::Logger * sLogger = GN::getLogger("GN.engine2.RenderEngine");

// *****************************************************************************
// local classes
// *****************************************************************************

///
/// dummy loader that does nothing.
///
class DummyLoader : public GN::engine2::GraphicsResourceLoader
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

    virtual bool load( const GN::engine2::GraphicsResourceDesc &, GN::DynaArray<UInt8> & )
    {
        return true;
    }

    bool decompress( const GN::engine2::GraphicsResourceDesc &, GN::DynaArray<UInt8> &, const GN::DynaArray<UInt8> & )
    {
        return true;
    }

    virtual bool copy( GN::engine2::GraphicsResource &, const GN::DynaArray<UInt8> & )
    {
        return true;
    }
};

///
/// kernel boolean parameter loader
///
template<size_t OFFSET, size_t COUNT>
struct BoolParameterLoader : public DummyLoader
{
    size_t index;
    bool   values[COUNT];

    virtual bool copy( GN::engine2::GraphicsResource & res, const GN::DynaArray<UInt8> & )
    {
        GN_ASSERT( GN::engine2::GRT_PARAMETER_SET == res.desc.type );
        GN_ASSERT( res.paramset );
        res.paramset->get( index ).setb( OFFSET, COUNT, values );
        return true;
    }
};

///
/// kernel float parameter loader
///
template<size_t OFFSET, size_t COUNT>
struct FloatParameterLoader : public DummyLoader
{
    size_t index;
    float  values[COUNT];

    virtual bool copy( GN::engine2::GraphicsResource & res, const GN::DynaArray<UInt8> & )
    {
        GN_ASSERT( GN::engine2::GRT_PARAMETER_SET == res.desc.type );
        GN_ASSERT( res.paramset );
        res.paramset->get( index ).setf( OFFSET, COUNT, values );
        return true;
    }
};

/*
        }
        else if( KERNEL_PARAMETER_TYPE_INT == TYPE )
        {
            res.paramset->get( index ).seti( OFFSET, COUNT, values );
        }
        else if( KERNEL_PARAMETER_TYPE_FLOAT == TYPE )
        {
            res.paramset->get( index ).setf( OFFSET, COUNT, values );
        }
        else if( KERNEL_PARAMETER_TYPE_STRING == TYPE )
        {
            const char * str[COUNT];

            for( size_t i = 0; i < COUNT; ++i ) str[i] = values[i].cstr();

            res.paramset->get( index ).sets( OFFSET, COUNT, str );
        }

        return true;
    }
};*/

// *****************************************************************************
// local functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static void sDisposeAllResources(
    GN::engine2::RenderEngine::ResourceCache & cache,
    GN::engine2::RenderEngine::ResourceLRU   & lru,
    GN::engine2::RenderEngine::DrawThread    & dt )
{
    GN_GUARD;

    using namespace GN::engine2;

    for( GraphicsResourceItem * item = cache.firstResource();
         item;
         item = cache.nextResource( item ) )
    {
        lru.dispose( item );

        dt.submitResourceDisposingCommand( item );
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
static inline void sPrepareResources(
    GN::engine2::RenderEngine::ResourceLRU               & lru,
    GN::engine2::RenderEngine::ResourceThread            & rt,
    const GN::DynaArray<GN::engine2::GraphicsResource *> & resources )
{
    using namespace GN;
    using namespace GN::engine2;

    for( size_t i = 0; i < resources.size(); ++i )
    {
        GN_ASSERT( resources[i] );

        GraphicsResourceItem * item = safeCastPtr<GraphicsResourceItem>( resources[i] );

        bool reload;

        lru.realize( item, &reload );

        GN_ASSERT( GRS_REALIZED == item->state );

        if( reload )
        {
            // reload using last loader
            GN_ASSERT( item->lastSubmittedLoader );
            rt.submitResourceLoadingCommand( item, item->lastSubmittedLoader );
        }
    }
}

//
//
// -----------------------------------------------------------------------------
static inline void sSetupWaitingListAndReferenceFence(
    const GN::DynaArray<GN::engine2::GraphicsResource *> & resources,
    GN::engine2::DrawCommandHeader                       & dr )
{
    using namespace GN;
    using namespace GN::engine2;

    for( size_t i = 0; i < resources.size(); ++i )
    {
        GN_ASSERT( resources[i] );

        GraphicsResourceItem * item = safeCastPtr<GraphicsResourceItem>( resources[i] );

        GN_ASSERT( GRS_REALIZED == item->state );

        // reference and update can't happen at same fence.
        GN_ASSERT( item->lastReferenceFence != item->lastSubmissionFence );

        if( item->lastSubmissionFence > item->lastReferenceFence )
        {
            // resource is updated after last time used. Now it is being used again.
            // So we have to wait for completion of last update.
            dr.resourceWaitingList[dr.resourceWaitingCount].resource = item;
            dr.resourceWaitingList[dr.resourceWaitingCount].waitForUpdate = item->lastSubmissionFence;
            dr.resourceWaitingCount++;
        }

        // note: this should be the only place to modify lastReferenceFence
        item->lastReferenceFence = dr.fence;

        GN_ASSERT( item->lastReferenceFence != item->lastSubmissionFence );
    }
}

// *****************************************************************************
// graphics resource
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::StrA GN::engine2::GraphicsResource::toString() const
{
    struct Local
    {
        static const char * sType2Str( GN::engine2::GraphicsResourceType type )
        {
            static const char * table[] = {
                "SURFACE",
                "STREAM",
                "PARAMETER_SET",
                "PORT_BINDING",
                "KERNEL",
            };

            if( 0 <= type && type < GN::engine2::NUM_GRAPHICS_RESOURCE_TYPES )
                return table[type];
            else
                return "INVALID";
        }
    };

    StrA s;

    s.format( "type=\"%s\" name=\"%s\"",
        Local::sType2Str( desc.type ),
        desc.name.empty() ? "unnamed" : desc.name.cptr() );

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
#define RENDER_ENGINE_API() \
    ApiReentrantChecker checker( &mApiReentrantFlag ); \
    RenderEngineApiDumper apidumper( GN_FUNCTION )
#else
#define RENDER_ENGINE_API() \
    ApiReentrantChecker checker( &mApiReentrantFlag );
#endif

// *****************************************************************************
// ctor / dtor
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::engine2::RenderEngine::RenderEngine() : mApiReentrantFlag(0)
{
    clear();
}

//
//
// -----------------------------------------------------------------------------
GN::engine2::RenderEngine::~RenderEngine()
{
    quit();
}

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::engine2::RenderEngine::init( const RenderEngineInitParameters & p )
{
    GN_GUARD;

    RENDER_ENGINE_API();

    // standard init procedure
    GN_STDCLASS_INIT( GN::engine2::RenderEngine, () );

    // create sub components
    mFenceManager = new FenceManager( *this );

    mResourceCache = new ResourceCache( *this );
    if( !mResourceCache->init() ) return failure();

    mResourceLRU = new ResourceLRU( *this );
    if( !mResourceLRU->init( p.resourceCapacityInBytes ) ) return failure();

    mResourceThread = new ResourceThread( *this );
    if( !mResourceThread->init() ) return failure();

    mDrawThread = new DrawThread( *this );
    if( !mDrawThread->init( p.maxDrawCommandBufferBytes ) ) return failure();

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine2::RenderEngine::quit()
{
    GN_GUARD;

    RENDER_ENGINE_API();

    mDrawContexts.clear();

    if( ok() )
    {
        // delete all resources
        for( GraphicsResourceItem * item = mResourceCache->firstResource();
             item;
             item = mResourceCache->nextResource( item ) )
        {
            // dispose it first
            mResourceLRU->dispose( item );
            mDrawThread->submitResourceDisposingCommand( item );
            mDrawThread->waitForIdle();

            // then remove from LRU
            mResourceLRU->remove( item );

            // finally, delete from cache
            mResourceCache->deleteResource( item );
        }
    }

    mKernels.clear();
    mStreams.clear();

    safeDelete( mDrawThread );
    safeDelete( mResourceThread );
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
void GN::engine2::RenderEngine::clear()
{
    mFenceManager = 0;
    mResourceCache = 0;
    mResourceLRU = 0;
    mResourceThread = 0;
    mDrawThread = 0;
}

// *****************************************************************************
// renderer management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::engine2::RenderEngine::reset( const gfx::GraphicsSystemCreationParameter & gscp )
{
    GN_GUARD;

    RENDER_ENGINE_API();

    // dispose all graphics resources
    sDisposeAllResources( *mResourceCache, *mResourceLRU, *mDrawThread );
    mDrawThread->waitForIdle();

    // do reset
    return mDrawThread->resetGraphicsSystem( gscp );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
const GN::gfx::GraphicsSystemCreationParameter &
GN::engine2::RenderEngine::getGraphicsSystemCreationParameter() const
{
    RENDER_ENGINE_API();

    return mDrawThread->getGraphicsSystemCreationParameter();
}

//
//
// -----------------------------------------------------------------------------
const GN::gfx::GraphicsSystemDesc &
GN::engine2::RenderEngine::getGraphicsSystemDesc() const
{
    RENDER_ENGINE_API();

    return mDrawThread->getGraphicsSystemDesc();
}

// *****************************************************************************
// resource commands
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::engine2::GraphicsResource *
GN::engine2::RenderEngine::createResource( const GraphicsResourceDesc & desc )
{
    GN_GUARD;

    RENDER_ENGINE_API();

    // special for kernel and stream: try get it from local cache first.
    StrA streamName;
    if( GRT_KERNEL == desc.type )
    {
        GraphicsResource * res = mKernels.get( desc.kernel.kernel );
        if( res ) return res;
    }
    else if( GRT_STREAM == desc.type )
    {
        streamName.format( "%s::%s", desc.stream.kernel.cptr(), desc.stream.stream.cptr() );
        GraphicsResource * res = mStreams.get( streamName );
        if( res ) return res;
    }

    // create new resource item
    GraphicsResourceItem * item = mResourceCache->createResource( desc );
    if( 0 == item ) return 0;
    mResourceLRU->insert( item );

    // special for kernel and stream: insert to local cache.
    if( GRT_KERNEL == desc.type )
    {
        mKernels.add( desc.kernel.kernel, item );
    }
    else if( GRT_STREAM == desc.type )
    {
        mStreams.add( streamName, item );
    }

    return item;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine2::RenderEngine::deleteResource( GraphicsResource * res )
{
    GN_GUARD;

    RENDER_ENGINE_API();

    if( 0 == res ) return;

    GraphicsResourceItem * item = safeCastPtr<GraphicsResourceItem>( res );

    if( !mResourceCache->checkResource( item ) ) return;

    // special for kernel and stream: try remove it from local cache first.
    if( GRT_KERNEL == item->desc.type )
    {
        if( mKernels.del( item->desc.kernel.kernel, item ) > 0 )
        {
            return;
        }
    }
    else if( GRT_STREAM == item->desc.type )
    {
        StrA streamName;
        streamName.format( "%s::%s", item->desc.stream.kernel.cptr(), item->desc.stream.stream.cptr() );
        if( mStreams.del( streamName, item ) > 0 )
        {
            return;
        }
    }

    // dispose it first
    mResourceLRU->dispose( item );
    mDrawThread->submitResourceDisposingCommand( item );
    mDrawThread->waitForIdle();

    // then remove from LRU
    mResourceLRU->remove( item );

    // finally, delete from cache
    return mResourceCache->deleteResource( item );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::engine2::RenderEngine::checkResource( const GraphicsResource * res ) const
{
    GN_GUARD;

    RENDER_ENGINE_API();
    return mResourceCache->checkResource( safeCastPtr<const GraphicsResourceItem>( res ) );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine2::RenderEngine::disposeResource( GraphicsResource * res )
{
    GN_GUARD;

    RENDER_ENGINE_API();

    GraphicsResourceItem * item = safeCastPtr<GraphicsResourceItem>( res );

    if( !mResourceCache->checkResource( item ) ) return;

    mResourceLRU->dispose( item );

    mDrawThread->submitResourceDisposingCommand( item );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine2::RenderEngine::disposeAllResources()
{
    GN_GUARD;

    RENDER_ENGINE_API();

    sDisposeAllResources( *mResourceCache, *mResourceLRU, *mDrawThread );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine2::RenderEngine::updateResource(
    GraphicsResource       * res,
    GraphicsResourceLoader * loader )
{
    GN_GUARD;

    RENDER_ENGINE_API();

    GraphicsResourceItem * item = safeCastPtr<GraphicsResourceItem>( res );

    if( !mResourceCache->checkResource( item ) ) return;

    mResourceLRU->realize( item, 0 );

    mResourceThread->submitResourceLoadingCommand( item, loader );

    GN_UNGUARD;
}

// *****************************************************************************
// draw request management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
UIntPtr GN::engine2::RenderEngine::createRenderContext(
    GraphicsResource * kernel,
    GraphicsResource * paramset,
    GraphicsResource * binding )
{
    // check arguments
    if( !mResourceCache->checkResource( kernel ) )
    {
        GN_ERROR(sLogger)( "invalid kernel pointer" );
        return 0;
    }
    if( GRT_KERNEL != kernel->desc.type )
    {
        GN_ERROR(sLogger)( "kernel pointer points to non-kernel resource." );
        return 0;
    }
    if( !mResourceCache->checkResource( paramset ) )
    {
        GN_ERROR(sLogger)( "invalid parameter pointer" );
        return 0;
    }
    if( GRT_PARAMETER_SET != paramset->desc.type )
    {
        GN_ERROR(sLogger)( "parameter pointer points to non-parameter resource." );
        return 0;
    }
    if( kernel->desc.kernel.kernel != paramset->desc.paramset.kernel )
    {
        GN_ERROR(sLogger)( "parameter resource belongs to another kernel then the input one." );
        return 0;
    }
    if( 0 != binding )
    {
        if( !mResourceCache->checkResource( binding ) )
        {
            GN_ERROR(sLogger)( "invalid binding pointer" );
            return 0;
        }
        if( GRT_PARAMETER_SET != binding->desc.type )
        {
            GN_ERROR(sLogger)( "binding pointer points to non-binding resource." );
            return 0;
        }
        if( kernel->desc.kernel.kernel != binding->desc.binding.kernel )
        {
            GN_ERROR(sLogger)( "binding resource belongs to another kernel then the input one." );
            return 0;
        }
    }

    // build draw context
    DrawContext dc;

    dc.resources.append( kernel );
    dc.resources.append( paramset );
    if( binding )
    {
        std::map<StrA,SurfaceResourceView>::const_iterator iter = binding->desc.binding.views.begin();
        for( ; iter != binding->desc.binding.views.end(); ++iter )
        {
            if( iter->second.surf )
            {
                dc.resources.append( iter->second.surf );
            }
        }
        dc.resources.append( binding );
    }

    // create draw context handle
    return mDrawContexts.add( dc );
}

//
//
// -----------------------------------------------------------------------------
void GN::engine2::RenderEngine::deleteRenderContext( UIntPtr context )
{
    if( 0 == context ) return; // silently ignore NULL context.

    if( !mDrawContexts.validHandle( context ) )
    {
        GN_ERROR(sLogger)( "invalid context handle." );
        return;
    }

    mDrawContexts.remove( context );
}

//
//
// -----------------------------------------------------------------------------
void GN::engine2::RenderEngine::render( UIntPtr context )
{
    GN_GUARD_SLOW;

    RENDER_ENGINE_API();

    const DrawContext & dc = mDrawContexts[context];

    // prepare resources, make sure that they are usable.
    sPrepareResources( *mResourceLRU, *mResourceThread, dc.resources );

    // submit new draw command
    GraphicsResource * kernel   = dc.resources[0]; // the first resource in draw context is always kernel
    GraphicsResource * paramset = dc.resources[1]; // the second is always parameter set
    GraphicsResource * binding  = dc.resources.size() > 2 ? dc.resources.back() : 0; // the last is always biding, if have.
    DrawCommandHeader * dr = mDrawThread->submitDrawCommand3( DCT_DRAW, kernel, paramset, binding );
    if( 0 == dr ) return;

    // setup resource waiting list, to make sure draw command happens after resource updating.
    sSetupWaitingListAndReferenceFence( dc.resources, *dr );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine2::RenderEngine::present()
{
    GN_GUARD_SLOW;

    RENDER_ENGINE_API();

    mDrawThread->present();

    // profile frame time
#if GN_PROFILE_ENABLED
    mFrameProfiler.nextFrame();
#endif

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// helpers
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::engine2::GraphicsResource *
GN::engine2::RenderEngine::createSurface( const StrA & resname, const gfx::SurfaceCreationParameter & creation )
{
    GraphicsResourceDesc grd;

    grd.name             = resname;
    grd.type             = GRT_SURFACE;
    grd.surface.creation = creation;

    return createResource( grd );
}

//
//
// -----------------------------------------------------------------------------
GN::engine2::GraphicsResource *
GN::engine2::RenderEngine::createParameterSet( const StrA & resname, const StrA & kernel )
{
    GraphicsResourceDesc grd;

    grd.name            = resname;
    grd.type            = GRT_PARAMETER_SET;
    grd.paramset.kernel = kernel;

    GraphicsResource * res = createResource( grd );
    if( 0 == res ) return 0;

    updateResource( res, DummyLoader::sGetInstance() );

    return res;
}

//
//
// -----------------------------------------------------------------------------
GN::engine2::GraphicsResource *
GN::engine2::RenderEngine::createPortBinding( const StrA & resname, const StrA & kernel, const std::map<StrA,SurfaceResourceView> & views )
{
    GraphicsResourceDesc grd;

    grd.name           = resname;
    grd.type           = GRT_PORT_BINDING;
    grd.binding.kernel = kernel;
    grd.binding.views  = views;

    GraphicsResource * res = createResource( grd );
    if( 0 == res ) return 0;

    updateResource( res, DummyLoader::sGetInstance() );

    return res;
}

//
//
// -----------------------------------------------------------------------------
GN::engine2::GraphicsResource *
GN::engine2::RenderEngine::getStream( const StrA & kernel, const StrA & stream )
{
    GraphicsResourceDesc grd;

    grd.name          = strFormat( "stream %s::%s", kernel.cptr(), stream.cptr() );
    grd.type          = GRT_STREAM;
    grd.stream.kernel = kernel;
    grd.stream.stream = stream;

    return createResource( grd );
}

//
//
// -----------------------------------------------------------------------------
GN::engine2::GraphicsResource *
GN::engine2::RenderEngine::getKernel( const StrA & kernel )
{
    GraphicsResourceDesc grd;

    grd.name          = kernel;
    grd.type          = GRT_KERNEL;
    grd.kernel.kernel = kernel;

    GraphicsResource * res = createResource( grd );
    if( 0 == res ) return 0;

    updateResource( res, DummyLoader::sGetInstance() );

    return res;
}

// *****************************************************************************
// NamedResourceManager class
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::engine2::RenderEngine::NamedResourceManager::add( const StrA & name, GraphicsResource * res )
{
    GN_ASSERT( resources.end() == resources.find( name ) );
    GN_ASSERT( res );

    RefCountedResource & rcr = resources[name];

    rcr.resource = res;
    rcr.refcounter = 1;
}

//
//
// -----------------------------------------------------------------------------
size_t GN::engine2::RenderEngine::NamedResourceManager::del( const StrA & name, GraphicsResource * res )
{
    GN_ASSERT( resources.end() != resources.find( name ) );
    GN_ASSERT( res );

    RefCountedResource & rcr = resources[name];

    GN_ASSERT( rcr.refcounter > 0 );

    size_t ret = --rcr.refcounter;

    if( 0 == ret )
    {
        resources.erase( name );
    }

    return ret;
}

//
//
// -----------------------------------------------------------------------------
GN::engine2::GraphicsResource * GN::engine2::RenderEngine::NamedResourceManager::get( const StrA & name )
{
    std::map<StrA,RefCountedResource>::iterator i = resources.find( name );

    if( resources.end() == i ) return 0;

    GN_ASSERT( i->second.resource );
    GN_ASSERT( i->second.refcounter > 0 );

    ++(i->second.refcounter);

    return i->second.resource;
}

// *****************************************************************************
// ClearScreen class
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::engine2::ClearScreen::init( RenderEngine & re, GraphicsResource * binding )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::engine2::ClearScreen, () );

    mKernel = re.getKernel( "CLEAR_SCREEN" );
    if( 0 == mKernel ) return failure();

    if( GN_ASSERT_ENABLED )
    {
        // make sure kenrel cache is working
        GraphicsResource * res = re.getKernel( "CLEAR_SCREEN" );
        GN_ASSERT( res == mKernel );
        re.deleteResource( res );
    }

    mParam = re.createParameterSet( StrA::EMPTYSTR, "CLEAR_SCREEN" );
    if( 0 == mParam ) return failure();

    // get parameter index

    const gfx::KernelReflection & refl = gfx::getKernelReflection( "CLEAR_SCREEN" );

#define GET_INDEX( x ) \
    x = refl.parameters.name2idx( #x ); \
    if( (size_t)-1 == x ) return failure();

    GET_INDEX( CLEAR_COLOR   );
    GET_INDEX( CLEAR_DEPTH   );
    GET_INDEX( CLEAR_STENCIL );
    GET_INDEX( COLOR         );
    GET_INDEX( DEPTH         );
    GET_INDEX( STENCIL       );

#undef GET_INDEX

    // create context
    mContext = re.createRenderContext( mKernel, mParam, binding );
    if( 0 == mContext ) return failure();

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine2::ClearScreen::quit()
{
    GN_GUARD;

    if( mContext )
    {
        GN_ASSERT( mKernel );
        mKernel->engine.deleteRenderContext( mContext );
    }

    safeDeleteGraphicsResource( mKernel );
    safeDeleteGraphicsResource( mParam );

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine2::ClearScreen::setClearColor( bool enabled, float r, float g, float b, float a )
{
    GN_ASSERT( mKernel && mParam );

    {
        AutoRef< BoolParameterLoader<0,1> > loader( new BoolParameterLoader<0,1> );
        loader->index = CLEAR_COLOR;
        loader->values[0] = enabled;
        mKernel->engine.updateResource( mParam, loader );
    }

    if( enabled )
    {
        AutoRef< FloatParameterLoader<0,4> > loader( new FloatParameterLoader<0,4> );
        loader->index = COLOR;
        loader->values[0] = r;
        loader->values[1] = g;
        loader->values[2] = b;
        loader->values[3] = a;
        mKernel->engine.updateResource( mParam, loader );
    }
};
