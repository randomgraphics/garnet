#include "pch.h"
#include "fenceManager.h"
#include "resourceCache.h"
#include "resourceLRU.h"
#include "resourceThread.h"
#include "drawThread.h"

static GN::Logger * sLogger = GN::getLogger("GN.engine2.RenderEngine");

#pragma warning( disable : 4100 )

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
/// kernel parameter loader
///
template<typename T, size_t COUNT>
struct ParameterLoader : public GN::engine2::GraphicsResourceLoader
{
    size_t index;         ///< parameter index
    T      values[COUNT]; ///< parameter value
};

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
    const GraphicsResource * kernel,
    const GraphicsResource * param,
    const GraphicsResource * binding )
{
    GN_GUARD_SLOW;

    RENDER_ENGINE_API();
    GN_UNIMPL();

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

GN::engine2::GraphicsResource * GN::engine2::RenderEngine::createSurface( const gfx::SurfaceCreationParameter & ) { GN_UNIMPL(); return 0; }
GN::engine2::GraphicsResource * GN::engine2::RenderEngine::getStream( const StrA & kernel, const StrA & stream ) { GN_UNIMPL(); return 0; }
GN::engine2::GraphicsResource * GN::engine2::RenderEngine::getKernel( const StrA & name ) { GN_UNIMPL(); return 0; }
GN::engine2::GraphicsResource * GN::engine2::RenderEngine::createParameterSet( const StrA & kernel ) { GN_UNIMPL(); return 0; }
GN::engine2::GraphicsResource * GN::engine2::RenderEngine::createPortBinding( const StrA & kernel, const gfx::KernelPortBindingDesc & ) { GN_UNIMPL(); return 0; }

// *****************************************************************************
// ClearScreen class
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::engine2::ClearScreen::init( RenderEngine & re )
{
    mKernel = re.getKernel( "CLEAR_SCREEN" );
    if( 0 == mKernel ) return false;

    mParam = re.createParameterSet( "CLEAR_SCREEN" );
    if( 0 == mParam ) return false;

    //mBinding = re.createPortBinding( "CLEAR_SCREEN" );

    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine2::ClearScreen::setClearColor( bool enabled, float r, float g, float b, float a )
{
    GN_ASSERT( mParam );

    GN_UNUSED_PARAM( enabled );
    GN_UNUSED_PARAM( r );
    GN_UNUSED_PARAM( g );
    GN_UNUSED_PARAM( b );
    GN_UNUSED_PARAM( a );

    GN_UNIMPL();
};
