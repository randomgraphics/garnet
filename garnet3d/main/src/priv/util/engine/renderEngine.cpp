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

    virtual bool load( const GN::engine::GraphicsResourceDesc &, GN::DynaArray<UInt8> & )
    {
        return true;
    }

    bool decompress( const GN::engine::GraphicsResourceDesc &, GN::DynaArray<UInt8> &, GN::DynaArray<UInt8> & )
    {
        return true;
    }

    virtual bool download( GN::engine::GraphicsResource &, GN::DynaArray<UInt8> & )
    {
        return true;
    }
};

///
/// static texture loader: load texture from image file
///
class StaticTextureLoader : public DummyLoader
{
    const GN::StrA mFileName;

public:

    StaticTextureLoader( const GN::StrA & name ) : mFileName( name ) {}

    bool load( const GN::engine::GraphicsResourceDesc &, GN::DynaArray<UInt8> & outbuf )
    {
        using namespace GN;
        using namespace GN::gfx;

        ImageReader ir;

        AutoObjPtr<File> file( core::openFile( mFileName, "rb" ) );

        if( !file ) return false;

        if( !ir.reset( *file ) ) return false;

        ImageDesc id;
        if( !ir.readHeader( id ) ) return false;

        size_t mdbytes  = id.numFaces * id.numLevels * sizeof(MipmapDesc);
        size_t imgbytes = id.getTotalBytes();
        size_t bytes    = sizeof(id) + mdbytes + imgbytes;

        outbuf.resize( bytes );

        memcpy( &outbuf[0], &id, sizeof(id) );
        memcpy( &outbuf[sizeof(id)], id.mipmaps, mdbytes );

        if( !ir.readImage( &outbuf[sizeof(id)+mdbytes] ) ) return false;

        return true;
    }

    bool decompress( const GN::engine::GraphicsResourceDesc &, GN::DynaArray<UInt8> & outbuf, GN::DynaArray<UInt8> & inbuf )
    {
        outbuf.swap( inbuf );
        return true;
    }

    bool download( GN::engine::GraphicsResource & res, GN::DynaArray<UInt8> & inbuf )
    {
        using namespace GN;
        using namespace GN::gfx;

        GN_ASSERT( engine::GRT_SURFACE == res.desc.type );
        GN_ASSERT( res.surface );

        SafeArrayAccessor<UInt8> saa( inbuf.cptr(), inbuf.size() );

        ImageDesc * id = (ImageDesc*)saa.subrange( 0, sizeof(ImageDesc) );

        size_t mdbytes  = id->numFaces * id->numLevels * sizeof(MipmapDesc);

        id->mipmaps = (MipmapDesc *)saa.subrange( sizeof(ImageDesc), mdbytes );

        size_t imgbytes = id->getTotalBytes();

        GN_ASSERT( inbuf.size() == sizeof(ImageDesc) + mdbytes + imgbytes );

        SafeArrayAccessor<UInt8> imgdata( saa.subrange( sizeof(ImageDesc)+mdbytes, imgbytes ), imgbytes );

        for( size_t f = 0; f < id->numFaces; ++f )
        for( size_t l = 0; l < id->numLevels; ++l )
        {
            const GN::gfx::MipmapDesc & mmd = id->getMipmap( f, l );
            res.surface->download(
                GN::gfx::calcSubSurfaceIndex( f, l, id->numLevels ),
                0,
                imgdata.subrange( id->getMipmapOffset( f, l ), mmd.slicePitch ),
                mmd.rowPitch,
                mmd.slicePitch );
        }
        return true;
    }
};

///
/// kernel stream updater
///
class StreamLoader : public DummyLoader
{
    size_t               mIndex;
    GN::DynaArray<UInt8> mData;

public:

    StreamLoader( size_t streamIndex, size_t bytes, const void * data ) : mIndex(streamIndex), mData( bytes )
    {
        memcpy( mData.cptr(), data, bytes );
    }

    virtual bool download( GN::engine::GraphicsResource & res, GN::DynaArray<UInt8> & )
    {
        GN_ASSERT( GN::engine::GRT_KERNEL == res.desc.type );
        GN_ASSERT( res.kernel );
        res.kernel->getStream( mIndex )->push( mData.cptr(), mData.size() );
        return true;
    }
};

///
/// kernel parameter updater
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

    virtual bool download( GN::engine::GraphicsResource & res, GN::DynaArray<UInt8> & )
    {
        GN_ASSERT( GN::engine::GRT_PARAMETER_SET == res.desc.type );
        GN_ASSERT( res.paramset );
        res.paramset->get( mIndex ).set( mOffset, mValues.size(), mValues.cptr() );
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
    GN::engine::RenderEngine::ResourceLRU   & lru )
{
    GN_GUARD;

    using namespace GN::engine;

    for( GraphicsResourceItem * item = cache.firstResource();
         item;
         item = cache.nextResource( item ) )
    {
        if( GRS_REALIZED == item->state ) lru.dispose( item );
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
    sDisposeAllResources( cache, lru );

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
static void sDoRender(
    GN::engine::RenderEngine::ResourceLRU    & lru,
    GN::engine::RenderEngine::DrawThread     & dt,
    const RESOURCE_ARRAY                     & resources )
{
    GN_GUARD_SLOW;

    using namespace GN;
    using namespace GN::engine;

    // prepare resources, make sure that they are usable.
    for( size_t i = 0; i < resources.size(); ++i )
    {
        GN_ASSERT( resources[i] );

        GraphicsResourceItem * item = safeCastPtr<GraphicsResourceItem>( resources[i] );

        lru.realize( item );
    }

    // submit new draw command
    GraphicsResource * kernel   = resources[0]; // the first resource in draw context is always kernel
    GraphicsResource * paramset = resources[1]; // the second is always parameter set
    GraphicsResource * binding  = resources.size() > 2 ? resources.back() : 0; // the last is always biding, if have.
    DrawCommandHeader * dr = dt.submitDrawCommand3( DCT_DRAW, kernel, paramset, binding );
    if( 0 == dr ) return;

    // setup resource waiting list, to make sure draw command happens after resource updating.
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
            dr->resourceWaitingList[dr->resourceWaitingCount].resource = item;
            dr->resourceWaitingList[dr->resourceWaitingCount].waitForUpdate = item->lastSubmissionFence;
            dr->resourceWaitingCount++;
        }

        // note: this should be the only place to modify lastReferenceFence
        item->lastReferenceFence = dr->fence;

        GN_ASSERT( item->lastReferenceFence != item->lastSubmissionFence );
    }

    GN_UNGUARD_SLOW;
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
    sDisposeAllResources( *mResourceCache, *mResourceLRU );
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
GN::engine::RenderEngine::createResource(
    const GraphicsResourceDesc & desc,
    GraphicsResourceLoader     * loader )
{
    GN_GUARD;

    RENDER_ENGINE_API();

    if( GRT_KERNEL == desc.type )
    {
        GraphicsResource * res = mKernels.get( desc.kernel.kernel );
        if( res ) return res;
    }

    // create new resource item
    GraphicsResourceItem * item = mResourceCache->createResource( desc );
    if( 0 == item ) return 0;
    item->loader.set( loader );

    // add to LRU list
    mResourceLRU->insert( item );

    // special for kernel: insert to local cache.
    if( GRT_KERNEL == desc.type )
    {
        mKernels.add( desc.kernel.kernel, item );
    }
    // for port binding: setup dependencies
    else if( GRT_PORT_BINDING == desc.type )
    {
        // realize disposed surfaces before creating port binding
        std::map<GN::StrA,SurfaceResourceView>::const_iterator iter = desc.binding.views.begin();
        for( ; iter != desc.binding.views.end(); ++iter )
        {
            GraphicsResourceItem * surf = safeCastPtr<GraphicsResourceItem>( iter->second.surf );

            if( mResourceCache->checkResource( surf, GRT_SURFACE ) )
            {
                if( item->prerequisites.end() == std::find( item->prerequisites.begin(), item->prerequisites.end(), surf ) )
                {
                    item->prerequisites.append( surf );

                    GN_ASSERT( surf->dependents.end() == std::find( surf->dependents.begin(), surf->dependents.end(), item ) );

                    surf->dependents.append( item );
                }
            }
            else
            {
                GN_ERROR(sLogger)( "invalid binding!" );
            }
        }
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
        if( mKernels.del( item->desc.kernel.kernel ) > 0 )
        {
            return;
        }
    }

    // dispose it first
    if( GRS_REALIZED == item->state )
    {
        mResourceLRU->dispose( item );
        mDrawThread->waitForIdle();
    }

    // then remove from LRU
    mResourceLRU->remove( item );

    // finally, delete from cache
    mResourceCache->deleteResource( item );

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
void GN::engine::RenderEngine::updateResource(
    GraphicsResource       * res,
    GraphicsResourceLoader * loader )
{
    GN_GUARD_SLOW;

    RENDER_ENGINE_API();

    GraphicsResourceItem * item = safeCastPtr<GraphicsResourceItem>( res );

    if( !mResourceCache->checkResource( item ) ) return;

    if( 0 == loader )
    {
        GN_ERROR(sLogger)( "NULL loader pointer!" );
        return;
    }

    // realize the resource, if it is disposed.
    mResourceLRU->realize( item );

    // update item loaders
    item->loader.set( loader );

    // submit loading command to resource thread.
    mResourceThread->submitResourceLoadCommand( item );

    GN_UNGUARD_SLOW;
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

    if( GRS_DISPOSED == item->state ) return;

    mResourceLRU->dispose( item );

    GN_ASSERT( GN::engine::GRS_DISPOSED == item->state );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::disposeAllResources()
{
    GN_GUARD;

    RENDER_ENGINE_API();

    sDisposeAllResources( *mResourceCache, *mResourceLRU );

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

    sDoRender( *mResourceLRU, *mDrawThread, mDrawContexts[context].resources );

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

    sDoRender( *mResourceLRU, *mDrawThread, resources );

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

    return createResource( grd );
}

//
//
// -----------------------------------------------------------------------------
GN::engine::GraphicsResource *
GN::engine::RenderEngine::createSurface(
    const StrA & resname,
    const gfx::SurfaceCreationParameter & creation,
    GraphicsResourceLoader * loader )
{
    GraphicsResourceDesc grd;

    grd.name             = resname;
    grd.type             = GRT_SURFACE;
    grd.surface.creation = creation;

    return createResource( grd, loader );
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

    return createResource( grd );
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

    return createResource( grd );
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
GN::engine::GraphicsResource *
GN::engine::RenderEngine::createVtxBuf(
    const StrA                      & name,
    const gfx::SurfaceElementFormat & format,
    size_t                            count,
    GraphicsResourceLoader          * loader )
{
    using namespace GN::gfx;

    SurfaceCreationParameter scp;

    scp.bindTo( "STANDARD_RESOURCES", "VERTEX_BUFFER" );
    scp.desc.access = SURFACE_ACCESS_HOST_WRITE;
    scp.desc.layout.dim = SURFACE_DIMENSION_1D;
    scp.desc.layout.levels = 1;
    scp.desc.layout.faces = 1;
    scp.desc.layout.basemap.width = count;
    scp.desc.layout.basemap.height = 1;
    scp.desc.layout.basemap.depth = 1;
    scp.desc.layout.basemap.rowBytes = count * format.stride;
    scp.desc.layout.basemap.sliceBytes = scp.desc.layout.basemap.rowBytes;
    scp.desc.layout.format = format;

    return createSurface( name, scp, loader );
}

//
//
// -----------------------------------------------------------------------------
GN::engine::GraphicsResource *
GN::engine::RenderEngine::createIdxBuf(
    const StrA                & name,
    size_t                      count,
    GraphicsResourceLoader    * loader )
{
    using namespace GN::gfx;

    SurfaceCreationParameter scp;

    scp.bindTo( "STANDARD_RESOURCES", "INDEX_BUFFER" );
    scp.desc.access = SURFACE_ACCESS_HOST_WRITE;
    scp.desc.layout.dim = SURFACE_DIMENSION_1D;
    scp.desc.layout.levels = 1;
    scp.desc.layout.faces = 1;
    scp.desc.layout.basemap.width = count;
    scp.desc.layout.basemap.height = 1;
    scp.desc.layout.basemap.depth = 1;
    scp.desc.layout.basemap.rowBytes = count * sizeof(UInt16);
    scp.desc.layout.basemap.sliceBytes = scp.desc.layout.basemap.rowBytes;
    scp.desc.layout.format.attribs.resize( 1 );
    scp.desc.layout.format.attribs[0].semantic.set( "INDEX" );
    scp.desc.layout.format.attribs[0].offset = 0;
    scp.desc.layout.format.attribs[0].format = FMT_R_16_UINT;
    scp.desc.layout.format.stride = sizeof(short);

    return createSurface( name, scp, loader );
}

//
//
// -----------------------------------------------------------------------------
GN::engine::GraphicsResource *
GN::engine::RenderEngine::createTextureFromImageFile( const StrA & filename )
{
    GN_GUARD;

    using namespace GN::gfx;

    // open file
    AutoObjPtr<File> file;
    file.attach( core::openFile( filename, "rb" ) );
    if( !file ) return false;

    // get image information
    ImageReader ir;
    ImageDesc   id;
    if( !ir.reset( *file ) ) return false;
    if( !ir.readHeader( id ) ) return false;

    // setup texture creation parameters
    SurfaceCreationParameter scp;
    scp.bindTo( "STANDARD_RESOURCES", "TEXTURE" );
     scp.desc.layout.dim = SURFACE_DIMENSION_2D;
    scp.desc.layout.levels = id.numLevels;
    scp.desc.layout.faces  = id.numFaces;
    scp.desc.layout.basemap.width  = id.mipmaps[0].width;
    scp.desc.layout.basemap.height = id.mipmaps[0].height;
    scp.desc.layout.basemap.depth  = id.mipmaps[0].depth;
    scp.desc.layout.basemap.rowBytes = id.mipmaps[0].rowPitch;
    scp.desc.layout.basemap.sliceBytes = id.mipmaps[0].slicePitch;
    scp.desc.layout.format.attribs.resize( 1 );
    scp.desc.layout.format.attribs[0].semantic.set( "TEXEL" );
    scp.desc.layout.format.attribs[0].offset = 0;
    scp.desc.layout.format.attribs[0].format = id.format;
    scp.desc.layout.format.stride = getClrFmtDesc(id.format).bits / 8;

    // create loader
    AutoRef<StaticTextureLoader> loader( new StaticTextureLoader( filename ) );

    // create texture
    GraphicsResource * tex = createSurface( filename, scp, loader );
    if( 0 == tex ) return 0;

    // sucess
    return tex;

    GN_UNGUARD;
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

    AutoRef<StreamLoader> loader( new StreamLoader( streamIndex, bytes, data ) );

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

    AutoRef<StreamLoader> loader( new StreamLoader( index, bytes, data ) );

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
size_t GN::engine::RenderEngine::NamedResourceManager::del( const StrA & name )
{
    GN_ASSERT( resources.end() != resources.find( name ) );

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

    mKernel->engine.setParameterT( mParam, CLEAR_COLOR, enabled );

    if( enabled )
    {
        Vector4f rgba( r, g, b, a );
        mKernel->engine.setParameterT( mParam, COLOR, rgba );
    }
};
