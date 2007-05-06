#include "pch.h"
#include "resourceCache.h"
#include "drawThread.h"

static GN::Logger * sLogger = GN::getLogger("GN.engine.RenderEngine.ResourceCache");

#pragma warning(disable:4100)

// *****************************************************************************
// local functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static size_t sEstimateTextureSize( const GN::gfx::TextureDesc & desc )
{
    using namespace GN::gfx;

    TextureDesc d = desc;

    if( !d.validate() ) return 0;

    size_t basebytes = desc.width * desc.height * desc.depth * desc.faces * getClrFmtDesc(desc.format).bits / 8;

    return basebytes * 4 / 3; // that is: 1 / ( 1 - 1/4 ) 
}

//
//
// -----------------------------------------------------------------------------
static size_t sEstimateResourceSize( const GN::engine::GraphicsResourceDesc & desc )
{
    using namespace GN::engine;

    switch( desc.type )
    {
        case GRT_SHADER :
            return 1; // we assume that shader take no video memory

        case GRT_TEXTURE :
            return sEstimateTextureSize( desc.td );

        case GRT_VTXBUF :
            if( 0 == desc.vd.bytes )
            {
                GN_ERROR(sLogger)( "can't create vertex buffer with zero size." );
                return 0;
            }
            return desc.vd.bytes;

        case GRT_IDXBUF :
            if( 0 == desc.id.numidx )
            {
                GN_ERROR(sLogger)( "can't create index buffer with zero size." );
                return 0;
            }
            return desc.id.numidx * 2;

        case GRT_CONSTBUF :
            return 1;
    }
    return 0;
}

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::engine::RenderEngine::GraphicsResourceCache::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::engine::RenderEngine::GraphicsResourceCache, () );

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::GraphicsResourceCache::quit()
{
    GN_GUARD;

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::engine::GraphicsResourceId
GN::engine::RenderEngine::GraphicsResourceCache::alloc(
    const GraphicsResourceDesc & desc )
{
    size_t bytes = sEstimateResourceSize( desc );
    if( 0 == bytes ) return 0;

    mResourceMutex.lock();

    GraphicsResourceId id = mResources.newItem();

    GraphicsResourceItem * item = new GraphicsResourceItem( id, desc, bytes, mEngine.fenceManager().getAndIncFence() );
    item->prev = item->next = 0;

    mResources[id] = item;

    mResourceMutex.unlock();

    return id;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::GraphicsResourceCache::free( GraphicsResourceId id )
{
    GraphicsResourceItem * item = id2ptr( id );
    if( 0 == item ) return;

    // wait until the resouce is not used any more
    while( mEngine.drawThread().getCurrentDrawFence() < item->lastReferenceFence
        || item->lastCompletedFence < item->lastSubmissionFence )
    {
        sleepCurrentThread( 0 );
    }

    mResourceMutex.lock();

    mResources.remove( id );
    delete item;

    mResourceMutex.unlock();
}

//
//
// -----------------------------------------------------------------------------
GN::StrA GN::engine::RenderEngine::GraphicsResourceCache::id2name( GraphicsResourceId id ) const
{
    GraphicsResourceItem * item = id2ptr( id );
    if( 0 == item ) return "INVALID_RESOURCE";
    else return item->desc.name;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------


