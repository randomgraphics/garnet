#include "pch.h"
#include "resourceLRU.h"
#include "resourceCache.h"
#include "drawThread.h"

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::engine::RenderEngine::ResourceLRU::init( size_t maxTexBytes, size_t maxMeshBytes )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( ResourceLRU, () );

    // misc. initialization
    mMaxTexBytes = maxTexBytes;
    mMaxMeshBytes = maxMeshBytes;
    mRealizedTexBytes = 0;
    mRealizedMeshBytes = 0;

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::ResourceLRU::quit()
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
void GN::engine::RenderEngine::ResourceLRU::insert( GraphicsResourceId id )
{
    GraphicsResourceItem * item = mEngine.resourceCache().id2ptr( id );
    GN_ASSERT( item );
    GN_ASSERT( item->id == id );

    GN_ASSERT( 0 == item->next && 0 == item->prev );

    mLRUList.insertBefore( mLRUList.head(), item );
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::ResourceLRU::remove( GraphicsResourceId id )
{
    GraphicsResourceItem * item = mEngine.resourceCache().id2ptr( id );
    GN_ASSERT( item );
    GN_ASSERT( item->id == id );

    mLRUList.remove( item );
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::ResourceLRU::realize( GraphicsResourceId id, bool * needReload )
{
    // get resource item
    GraphicsResourceItem * item = mEngine.resourceCache().id2ptr( id );
    GN_ASSERT( item );
    GN_ASSERT( item->id == id );

    // mark as recently used.
    markAsRecentlyUsed( item );

    if( GRS_REALIZED == item->state )
    {
        // is already realized, do nothing
        if( needReload ) *needReload = false;
        return;
    }

    // need reload resource content
    if( needReload ) *needReload = true;

    // check if there's enough space to hold it.
    size_t * realizedBytes = 0;
    size_t * maxBytes = 0;
    switch( item->desc.type )
    {
        case GRT_TEXTURE:
            realizedBytes = &mRealizedTexBytes;
            maxBytes      = &mMaxTexBytes;
            break;

        case GRT_VTXBUF :
        case GRT_IDXBUF :
            realizedBytes = &mRealizedMeshBytes;
            maxBytes      = &mMaxMeshBytes;
            break;

        case GRT_SHADER :
        case GRT_CONSTBUF :
        case GRT_VTXFMT :
            // do nothing
            break;

        default :
            GN_UNEXPECTED();
    }

    if( !realizedBytes || (*realizedBytes + item->bytes) <= *maxBytes )
    {
        // Cool! There's enough space.
        item->state = GRS_REALIZED;

        if( realizedBytes )
        {
            *realizedBytes += item->bytes;
            GN_ASSERT( *realizedBytes <= *maxBytes );
        }

        return;
    }

    // no enough space in cache. We have to dispose some old resources, or try
    // reusing one of old resources.
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::ResourceLRU::dispose( GraphicsResourceId id )
{
    GraphicsResourceItem * item = mEngine.resourceCache().id2ptr( id );
    GN_ASSERT( item );

    if( GRS_DISPOSED == item->state ) return;

    item->state = GRS_DISPOSED;

    switch( item->desc.type )
    {
        case GRT_TEXTURE:
            GN_ASSERT( mRealizedTexBytes >= item->bytes );
            mRealizedTexBytes -= item->bytes;
            break;

        case GRT_VTXBUF :
        case GRT_IDXBUF :
            GN_ASSERT( mRealizedMeshBytes >= item->bytes );
            mRealizedMeshBytes -= item->bytes;
            break;

        case GRT_SHADER :
        case GRT_CONSTBUF :
        case GRT_VTXFMT :
            // do nothing
            break;

        default :
            GN_UNEXPECTED();
    }

    mEngine.drawThread().submitResourceDisposingCommand( id );
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::ResourceLRU::disposeAll()
{
    for( GraphicsResourceId id = mEngine.resourceCache().first();
         id;
         id = mEngine.resourceCache().next( id ) )
    {
        dispose( id );
    }
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::ResourceLRU::markAsRecentlyUsed( GraphicsResourceItem * item )
{
    GN_ASSERT( item );
    mLRUList.remove( item );
    mLRUList.insertBefore( mLRUList.head(), item );
}
