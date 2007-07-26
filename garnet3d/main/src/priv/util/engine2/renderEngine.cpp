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
static void sDeleteAllResources(
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
        // dispose it first
        lru.dispose( item );
        dt.submitResourceDisposingCommand( item );
        dt.waitForIdle();

        // then remove from LRU
        lru.remove( item );

        // finally, delete from cache
        cache.deleteResource( item );
    }

    GN_UNGUARD;
}

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
static inline void sPrepareResource(
    GN::engine2::RenderEngine::ResourceCache  & cache,
    GN::engine2::RenderEngine::ResourceLRU    & lru,
    GN::engine2::RenderEngine::ResourceThread & rt,
    GN::engine2::GraphicsResource             * res )
{
    using namespace GN;
    using namespace GN::engine2;

    GN_ASSERT( res );
    GN_ASSERT( cache.checkResource(res) );

    GraphicsResourceItem * item = safeCastPtr<GraphicsResourceItem>( res );

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

//
//
// -----------------------------------------------------------------------------
static inline void sSetupWaitingListAndReferenceFence(
    GN::engine2::RenderEngine::ResourceCache & cache,
    GN::engine2::GraphicsResource            * res,
    GN::engine2::DrawCommandHeader           & dr )
{
    using namespace GN;
    using namespace GN::engine2;

    GN_ASSERT( res );
    GN_ASSERT( cache.checkResource(res) );

    GraphicsResourceItem * item = safeCastPtr<GraphicsResourceItem>( res );

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

    if( ok() )
    {
        // delete all resources
        sDeleteAllResources( *mResourceCache, *mResourceLRU, *mDrawThread );
    }

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

    GraphicsResourceItem * item = mResourceCache->createResource( desc );

    if( 0 == item ) return 0;

    mResourceLRU->insert( item );
    GN_UNIMPL();

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
void GN::engine2::RenderEngine::draw(
    GraphicsResource * kernel,
    GraphicsResource * param,
    GraphicsResource * binding )
{
    GN_GUARD_SLOW;

    RENDER_ENGINE_API();

    // check parameter
    if( 0 == kernel || mResourceCache->checkResource( kernel ) || GRT_KERNEL != kernel->desc.type )
    {
        GN_ERROR(sLogger)( "invalid kernel resource" );
        return;
    }
    if( 0 == param || mResourceCache->checkResource( param ) || GRT_PARAMETER_SET != param->desc.type )
    {
        GN_ERROR(sLogger)( "invalid parameter set resource" );
        return;
    }
    if( 0 != binding && ( !mResourceCache->checkResource( binding ) || GRT_PORT_BINDING != binding->desc.type ) )
    {
        GN_ERROR(sLogger)( "invalid port binding resource" );
        return;
    }

    // prepare resources, make sure that they are usable.
    sPrepareResource( *mResourceCache, *mResourceLRU, *mResourceThread, kernel );
    sPrepareResource( *mResourceCache, *mResourceLRU, *mResourceThread, param );
    if( binding ) sPrepareResource( *mResourceCache, *mResourceLRU, *mResourceThread, binding );

    // submit new draw command
    DrawCommandHeader * dr = mDrawThread->submitDrawCommand3( DCT_DRAW, kernel, param, binding );
    if( 0 == dr ) return;

    // setup resource waiting list, to make sure draw command happens after resource updating.
    sSetupWaitingListAndReferenceFence( *mResourceCache, kernel, *dr );
    sSetupWaitingListAndReferenceFence( *mResourceCache, param, *dr );
    if( binding ) sSetupWaitingListAndReferenceFence( *mResourceCache, binding, *dr );

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

    grd.name         = resname;
    grd.type         = GRT_PARAMETER_SET;
    grd.param.kernel = kernel;

    return createResource( grd );
}

//
//
// -----------------------------------------------------------------------------
GN::engine2::GraphicsResource *
GN::engine2::RenderEngine::createPortBinding( const StrA & resname, const StrA & kernel, const gfx::KernelPortBindingDesc & desc )
{
    GraphicsResourceDesc grd;

    grd.name           = resname;
    grd.type           = GRT_PORT_BINDING;
    grd.binding.kernel = kernel;
    grd.binding.desc   = desc;

    return createResource( grd );
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

    return createResource( grd );
}

// *****************************************************************************
// ClearScreen class
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::engine2::ClearScreen::init( RenderEngine & re )
{
    GN_GUARD;

    mKernel = re.getKernel( "CLEAR_SCREEN" );
    if( 0 == mKernel ) return false;

    mParam = re.createParameterSet( StrA::EMPTYSTR, "CLEAR_SCREEN" );
    if( 0 == mParam ) return false;

    const gfx::KernelReflection & refl = gfx::getKernelReflection( "CLEAR_SCREEN" );

    // get parameter index

#define GET_INDEX( x ) \
    x = refl.parameters.name2idx( #x ); \
    if( (size_t)-1 == x ) return false;

    GET_INDEX( CLEAR_COLOR   );
    GET_INDEX( CLEAR_DEPTH   );
    GET_INDEX( CLEAR_STENCIL );
    GET_INDEX( COLOR         );
    GET_INDEX( DEPTH         );
    GET_INDEX( STENCIL       );

#undef GET_INDEX

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine2::ClearScreen::setClearColor( bool enabled, float r, float g, float b, float a )
{
    GN_ASSERT( mParam );

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
