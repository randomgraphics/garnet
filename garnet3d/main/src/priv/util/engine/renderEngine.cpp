#include "pch.h"
#include "fenceManager.h"
#include "resourceCache.h"
#include "resourceLRU.h"
#include "resourceThread.h"
#include "drawThread.h"

static GN::Logger * sLogger = GN::getLogger("GN.engine.RenderEngine");

// *****************************************************************************
// local classes
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


///
/// dummy loader that does nothing.
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

    virtual bool load( const GN::engine::GraphicsResourceDesc &, GN::DynaArray<UInt8> & )
    {
        return true;
    }

    bool decompress( const GN::engine::GraphicsResourceDesc &, GN::DynaArray<UInt8> &, GN::DynaArray<UInt8> & )
    {
        return true;
    }

    virtual bool copy( GN::engine::GraphicsResource &, GN::DynaArray<UInt8> & )
    {
        return true;
    }
};

///
/// kernel stream loader
///
class StreamSourceLoader : public DummyLoader
{
    size_t               mIndex;
    GN::DynaArray<UInt8> mData;

public:

    StreamSourceLoader( size_t streamIndex, size_t bytes, const void * data ) : mIndex(streamIndex), mData( bytes )
    {
        memcpy( mData.cptr(), data, bytes );
    }

    virtual bool copy( GN::engine::GraphicsResource & res, GN::DynaArray<UInt8> & )
    {
        GN_ASSERT( GN::engine::GRT_KERNEL == res.desc.type );
        GN_ASSERT( res.kernel );
        res.kernel->getStream( mIndex )->push( mData.cptr(), mData.size() );
        return true;
    }
};

///
/// kernel parameter loader
///
class KernelParameterLoader : public DummyLoader
{
    size_t               mIndex;
    size_t               mOffset;
    GN::DynaArray<UInt8> mValues;

public:

    KernelParameterLoader( size_t index, size_t offset, size_t bytes, const void * data )
        : mIndex( index )
        , mOffset( offset )
        , mValues( bytes )
    {
        memcpy( mValues.cptr(), data, bytes );
    }

    virtual bool copy( GN::engine::GraphicsResource & res, GN::DynaArray<UInt8> & )
    {
        GN_ASSERT( GN::engine::GRT_PARAMETER_SET == res.desc.type );
        GN_ASSERT( res.paramset );
        res.paramset->get( mIndex ).set( mOffset, mValues.size(), mValues.cptr() );
        return true;
    }
};

///
/// kernel boolean parameter loader
///
template<size_t START, size_t COUNT>
struct BoolParameterLoader : public DummyLoader
{
    size_t index;
    bool   values[COUNT];

    virtual bool copy( GN::engine::GraphicsResource & res, GN::DynaArray<UInt8> & )
    {
        GN_ASSERT( GN::engine::GRT_PARAMETER_SET == res.desc.type );
        GN_ASSERT( res.paramset );
        res.paramset->get( index ).set( START, COUNT, values );
        return true;
    }
};

///
/// kernel float parameter loader
///
template<size_t START, size_t COUNT>
struct FloatParameterLoader : public DummyLoader
{
    size_t index;
    float  values[COUNT];

    virtual bool copy( GN::engine::GraphicsResource & res, GN::DynaArray<UInt8> & )
    {
        GN_ASSERT( GN::engine::GRT_PARAMETER_SET == res.desc.type );
        GN_ASSERT( res.paramset );
        res.paramset->get( index ).set( START*4, COUNT*4, values );
        return true;
    }
};

// *****************************************************************************
// local functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static void sDisposeAllResources(
    GN::engine::RenderEngine::ResourceCache & cache,
    GN::engine::RenderEngine::ResourceLRU   & lru,
    GN::engine::RenderEngine::DrawThread    & dt )
{
    GN_GUARD;

    using namespace GN::engine;

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
static void sDeleteAllResources(
    GN::engine::RenderEngine::ResourceCache & cache,
    GN::engine::RenderEngine::ResourceLRU   & lru,
    GN::engine::RenderEngine::DrawThread    & dt )
{
    GN_GUARD;

    using namespace GN::engine;

    // dispose all of them
    sDisposeAllResources( cache, lru, dt );

    // wait for completion of dipose
    dt.waitForIdle();

    // remove from LRU
    for( GraphicsResourceItem * item = cache.firstResource();
         item;
         item = cache.nextResource( item ) )
    {
        lru.remove( item );
    }

    // clear cache
    cache.deleteAllResources();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
template< typename RESOURCE_ARRAY >
static inline void sPrepareResources(
    GN::engine::RenderEngine::ResourceLRU    & lru,
    GN::engine::RenderEngine::ResourceThread & rt,
    const RESOURCE_ARRAY                      & resources )
{
    using namespace GN;
    using namespace GN::engine;

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
template< typename RESOURCE_ARRAY >
static inline void sSetupWaitingListAndReferenceFence(
    GN::engine::DrawCommandHeader & dr,
    const RESOURCE_ARRAY           & resources )
{
    using namespace GN;
    using namespace GN::engine;

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
// Render engine
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::engine::RenderEngine::RenderEngine() : mApiReentrantFlag(0)
{
    clear();
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

    RENDER_ENGINE_API();

    // standard init procedure
    GN_STDCLASS_INIT( GN::engine::RenderEngine, () );

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
void GN::engine::RenderEngine::quit()
{
    GN_GUARD;

    RENDER_ENGINE_API();

    mDrawContexts.clear();

    if( ok() )
    {
        sDeleteAllResources( *mResourceCache, *mResourceLRU, *mDrawThread );
    }

    mKernels.clear();

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
void GN::engine::RenderEngine::clear()
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
bool GN::engine::RenderEngine::reset( const gfx::GraphicsSystemCreationParameter & gscp )
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
GN::engine::RenderEngine::getGraphicsSystemCreationParameter() const
{
    RENDER_ENGINE_API();

    return mDrawThread->getGraphicsSystemCreationParameter();
}

//
//
// -----------------------------------------------------------------------------
const GN::gfx::GraphicsSystemDesc &
GN::engine::RenderEngine::getGraphicsSystemDesc() const
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
GN::engine::GraphicsResource *
GN::engine::RenderEngine::createResource( const GraphicsResourceDesc & desc )
{
    GN_GUARD;

    RENDER_ENGINE_API();

    // special cases
    if( GRT_KERNEL == desc.type )
    {
        GraphicsResource * res = mKernels.get( desc.kernel.kernel );
        if( res ) return res;
    }
    else if( GRT_PORT_BINDING == desc.type )
    {
        // realize disposed surfaces before creating port binding
        std::map<GN::StrA,SurfaceResourceView>::const_iterator iter = desc.binding.views.begin();
        for( ; iter != desc.binding.views.end(); ++iter )
        {
            GraphicsResourceItem * item = safeCastPtr<GraphicsResourceItem>( iter->second.surf );

            if( item && GRS_DISPOSED == item->state )
            {
                mResourceLRU->realize( item, 0 );
                if( item->lastSubmittedLoader )
                {
                    mResourceThread->submitResourceLoadingCommand( item, item->lastSubmittedLoader );
                }
                else
                {
                    mResourceThread->submitResourceLoadingCommand( item, DummyLoader::sGetInstance() );
                }
            }
        }
    }

    // create new resource item
    GraphicsResourceItem * item = mResourceCache->createResource( desc );
    if( 0 == item ) return 0;
    mResourceLRU->insert( item );

    // special for kernel: insert to local cache.
    if( GRT_KERNEL == desc.type )
    {
        mKernels.add( desc.kernel.kernel, item );
    }

    return item;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::deleteResource( GraphicsResource * res )
{
    GN_GUARD;

    RENDER_ENGINE_API();

    if( 0 == res ) return;

    GraphicsResourceItem * item = safeCastPtr<GraphicsResourceItem>( res );

    if( !mResourceCache->checkResource( item ) ) return;

    // special for kernel: try remove it from local cache first.
    if( GRT_KERNEL == item->desc.type )
    {
        if( mKernels.del( item->desc.kernel.kernel, item ) > 0 )
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
void GN::engine::RenderEngine::deleteAllResources()
{
    sDeleteAllResources( *mResourceCache, *mResourceLRU, *mDrawThread );
    mKernels.clear();
}

//
//
// -----------------------------------------------------------------------------
bool GN::engine::RenderEngine::checkResource( const GraphicsResource * res ) const
{
    GN_GUARD;

    RENDER_ENGINE_API();
    return mResourceCache->checkResource( safeCastPtr<const GraphicsResourceItem>( res ) );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::disposeResource( GraphicsResource * res )
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
void GN::engine::RenderEngine::disposeAllResources()
{
    GN_GUARD;

    RENDER_ENGINE_API();

    sDisposeAllResources( *mResourceCache, *mResourceLRU, *mDrawThread );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::updateResource(
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
UIntPtr GN::engine::RenderEngine::createRenderContext(
    GraphicsResource * kernel,
    GraphicsResource * paramset,
    GraphicsResource * binding )
{
    // check arguments
    if( !mResourceCache->checkResource( kernel, GRT_KERNEL ) ) return 0;
    if( !mResourceCache->checkResource( paramset, GRT_PARAMETER_SET ) ) return 0;
    if( kernel->desc.kernel.kernel != paramset->desc.paramset.kernel )
    {
        GN_ERROR(sLogger)( "parameter resource belongs to another kernel then the input one." );
        return 0;
    }
    if( 0 != binding )
    {
        if( !mResourceCache->checkResource( binding, GRT_PORT_BINDING ) ) return false;
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
void GN::engine::RenderEngine::deleteRenderContext( UIntPtr context )
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
void GN::engine::RenderEngine::render( UIntPtr context )
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
    sSetupWaitingListAndReferenceFence( *dr, dc.resources );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::render( GraphicsResource * kernel, GraphicsResource * paramset, GraphicsResource * binding )
{
    GN_GUARD_SLOW;

    RENDER_ENGINE_API();

    // check arguments
    if( !mResourceCache->checkResource( kernel, GRT_KERNEL ) ) return;
    if( !mResourceCache->checkResource( paramset, GRT_PARAMETER_SET ) ) return;
    if( kernel->desc.kernel.kernel != paramset->desc.paramset.kernel )
    {
        GN_ERROR(sLogger)( "parameter resource belongs to another kernel then the input one." );
        return;
    }
    if( 0 != binding )
    {
        if( !mResourceCache->checkResource( binding, GRT_PORT_BINDING ) ) return;
        if( kernel->desc.kernel.kernel != binding->desc.binding.kernel )
        {
            GN_ERROR(sLogger)( "binding resource belongs to another kernel then the input one." );
            return;
        }
    }

    // build resource array
    StackArray<GraphicsResource*,DrawCommandHeader::MAX_RESOURCES_PER_DRAW> resources;
    resources.append( kernel );
    resources.append( paramset );
    if( binding )
    {
        std::map<StrA,SurfaceResourceView>::const_iterator iter = binding->desc.binding.views.begin();
        for( ; iter != binding->desc.binding.views.end(); ++iter )
        {
            if( iter->second.surf )
            {
                resources.append( iter->second.surf );
            }
        }
        resources.append( binding );
    }

    // prepare resources, make sure that they are usable.
    sPrepareResources( *mResourceLRU, *mResourceThread, resources );

    // submit new draw command
    DrawCommandHeader * dr = mDrawThread->submitDrawCommand3( DCT_DRAW, kernel, paramset, binding );
    if( 0 == dr ) return;

    // setup resource waiting list, to make sure draw command happens after resource updating.
    sSetupWaitingListAndReferenceFence( *dr, resources );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::present()
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
GN::engine::GraphicsResource *
GN::engine::RenderEngine::getKernel( const StrA & kernel )
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

//
//
// -----------------------------------------------------------------------------
GN::engine::GraphicsResource *
GN::engine::RenderEngine::createSurface( const StrA & resname, const gfx::SurfaceCreationParameter & creation )
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
GN::engine::GraphicsResource *
GN::engine::RenderEngine::createParameterSet( const StrA & resname, const StrA & kernel )
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
GN::engine::GraphicsResource *
GN::engine::RenderEngine::createParameterSet( const StrA & resname, const GraphicsResource & kernel )
{
    if( !mResourceCache->checkResource( &kernel, GRT_KERNEL ) ) return 0;

    return createParameterSet( resname, kernel.desc.kernel.kernel );
}

//
//
// -----------------------------------------------------------------------------
GN::engine::GraphicsResource *
GN::engine::RenderEngine::createPortBinding( const StrA & resname, const StrA & kernel, const std::map<StrA,SurfaceResourceView> & views )
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
GN::engine::GraphicsResource *
GN::engine::RenderEngine::createPortBinding( const StrA & resname, const GraphicsResource & kernel, const std::map<StrA,SurfaceResourceView> & views )
{
    if( !mResourceCache->checkResource( &kernel ) ) return 0;

    if( GRT_KERNEL != kernel.desc.type )
    {
        GN_ERROR(sLogger)( "input graphics resource is not a kernel." );
        return 0;
    }

    return createPortBinding( resname, kernel.desc.kernel.kernel, views );
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::pushStreamData(
    GraphicsResource * kernel,
    size_t             streamIndex,
    size_t             bytes,
    const void       * data )
{
    if( !mResourceCache->checkResource( kernel, GRT_KERNEL ) ) return;

    AutoRef<StreamSourceLoader> loader( new StreamSourceLoader( streamIndex, bytes, data ) );

    updateResource( kernel, loader );
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::pushStreamData(
    GraphicsResource * kernel,
    const StrA &       streamName,
    size_t             bytes,
    const void       * data )
{
    if( !mResourceCache->checkResource( kernel, GRT_KERNEL ) ) return;

    size_t index = gfx::getKernelReflection( kernel->desc.kernel.kernel ).streams.name2idx(streamName);
    if( (size_t)-1 == index )
    {
        GN_ERROR(sLogger)( "invalid stream name: %s", streamName.cptr() );
        return;
    }

    AutoRef<StreamSourceLoader> loader( new StreamSourceLoader( index, bytes, data ) );

    updateResource( kernel, loader );
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::setParameter(
    GraphicsResource * paramset,
    size_t             index,
    size_t             offset,
    size_t             bytes,
    const void       * data )
{
    if( !mResourceCache->checkResource( paramset, GRT_PARAMETER_SET ) ) return;

    AutoRef<KernelParameterLoader> loader( new KernelParameterLoader( index, offset, bytes, data ) );

    updateResource( paramset, loader );
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::setParameter(
    GraphicsResource * paramset,
    const StrA       & name,
    size_t             offset,
    size_t             bytes,
    const void       * data )
{
    if( !mResourceCache->checkResource( paramset, GRT_PARAMETER_SET ) ) return;

    const gfx::KernelReflection & refl = gfx::getKernelReflection( paramset->desc.paramset.kernel );

    size_t index = refl.parameters.name2idx( name );

    AutoRef<KernelParameterLoader> loader( new KernelParameterLoader( index, offset, bytes, data ) );

    updateResource( paramset, loader );
}

// *****************************************************************************
// NamedResourceManager class
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::NamedResourceManager::add( const StrA & name, GraphicsResource * res )
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
size_t GN::engine::RenderEngine::NamedResourceManager::del( const StrA & name, GraphicsResource * res )
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
GN::engine::GraphicsResource * GN::engine::RenderEngine::NamedResourceManager::get( const StrA & name )
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
bool GN::engine::ClearScreen::init( RenderEngine & re, GraphicsResource * binding )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::engine::ClearScreen, () );

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
void GN::engine::ClearScreen::quit()
{
    GN_GUARD;

    if( mContext )
    {
        GN_ASSERT( mKernel );
        mKernel->engine.deleteRenderContext( mContext );
    }

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::ClearScreen::setClearColor( bool enabled, float r, float g, float b, float a )
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
