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

    size_t basebytes = d.width * d.height * d.depth * d.faces * getClrFmtDesc(d.format).bits / 8;

    GN_ASSERT( basebytes > 0 );

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
        case GRT_SHADER   :
        case GRT_CONSTBUF :
        case GRT_VTXFMT   :
            return 1; // we assume that these resources won't take video memory

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

        default :
            GN_UNEXPECTED();
            return 0;
    }
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

    // free all resources
    mResourceMutex.lock();
    UInt32 id = mResources.first();
    UInt32 id2;
    while( id )
    {
        GraphicsResourceItem * item = mResources.get( id );

        id2 = mResources.next( id );

        mResources.remove( id );
        delete item;

        id = id2;
    }
    mResourceMutex.unlock();

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
GN::engine::GraphicsResourceItem *
GN::engine::RenderEngine::GraphicsResourceCache::alloc(
    const GraphicsResourceDesc & desc )
{
    size_t bytes = sEstimateResourceSize( desc );
    if( 0 == bytes ) return 0;

    mResourceMutex.lock();

    UInt32 id = mResources.newItem();

    GraphicsResourceItem * item = new GraphicsResourceItem( mEngine, id, desc, bytes );
    item->prev = item->next = 0;

    mResources[id] = item;

    mResourceMutex.unlock();

    return item;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::GraphicsResourceCache::free( GraphicsResourceItem * item )
{
    if( 0 == item ) return;

    GN_ASSERT( check( item ) );

    mEngine.drawThread().waitForResource( item );

    mResourceMutex.lock();

    mResources.remove( item->id );
    delete item;

    mResourceMutex.unlock();
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------


