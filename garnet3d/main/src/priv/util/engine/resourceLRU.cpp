#include "pch.h"
#include "resourceLRU.h"
#include "resourceCache.h"
#include "drawThread.h"
#include "dump.h"

static GN::Logger * sLogger = GN::getLogger("GN.engine.RenderEngine.ResourceLRU");

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
void GN::engine::RenderEngine::ResourceLRU::insert( GraphicsResourceItem * item )
{
    GN_ASSERT( mEngine.resourceCache().check( item ) );

    GN_ASSERT( 0 == item->next && 0 == item->prev );

    mLRUList.insertBefore( mLRUList.head(), item );
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::ResourceLRU::remove( GraphicsResourceItem * item )
{
    GN_ASSERT( mEngine.resourceCache().check( item ) );
    mLRUList.remove( item );
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::ResourceLRU::realize( GraphicsResourceItem * item, bool * needReload )
{
    GN_ASSERT( mEngine.resourceCache().check( item ) );

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
        case GRT_TEXTURE :
            realizedBytes = &mRealizedTexBytes;
            maxBytes      = &mMaxTexBytes;
            break;

        case GRT_VTXBUF :
        case GRT_IDXBUF :
            realizedBytes = &mRealizedMeshBytes;
            maxBytes      = &mMaxMeshBytes;
            break;

        case GRT_SHADER   :
        case GRT_CONSTBUF :
        case GRT_VTXFMT   :
            // do nothing
            break;

        default :
            GN_UNEXPECTED();
    }

    if( maxBytes && item->bytes > *maxBytes )
    {
        GN_FATAL(sLogger)( "resource cache (%dMB) is not large enough to hold resources '%s' (%dMB).",
            *maxBytes/1024/1024, item->desc.name.cptr(), item->bytes/1024/1024 );
        GN_UNEXPECTED();
        return;
    }
    else if( realizedBytes && (*realizedBytes + item->bytes) > *maxBytes )
    {
        if( GRT_TEXTURE == item->desc.type )
        {
            GN_ASSERT( realizedBytes && maxBytes );

            GraphicsResourceItem * old = mLRUList.tail();

            while( *realizedBytes + item->bytes > *maxBytes )
            {
                GN_ASSERT( old && old != item );
                if( GRT_TEXTURE == old->desc.type )
                {
                    // TODO: check if we can reuse it.
                    dispose( old );
                }
                old = old->prev;
            }
        }
        else if( GRT_VTXBUF == item->desc.type )
        {
            GN_UNIMPL();
        }
        else if( GRT_IDXBUF == item->desc.type )
        {
            GN_UNIMPL();
        }
        else
        {
            GN_UNEXPECTED();
            return;
        }
    }

    GN_ASSERT( !realizedBytes || (*realizedBytes + item->bytes) <= *maxBytes );

    item->state = GRS_REALIZED;

    if( realizedBytes )
    {
        *realizedBytes += item->bytes;
        GN_ASSERT( *realizedBytes <= *maxBytes );
    }

    if( GN_RENDER_ENGINE_API_DUMP_ENABLED )
    {
        dumpApiString( strFormat( "<RealizeGraphicsResource %s/>", item->desc.toString().cptr() ) );
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::ResourceLRU::dispose( GraphicsResourceItem * item )
{
    GN_ASSERT( mEngine.resourceCache().check( item ) );

    if( GRS_DISPOSED == item->state ) return;

    if( GN_RENDER_ENGINE_API_DUMP_ENABLED )
    {
        dumpApiString( strFormat( "<DisposeGraphicsResource %s/>", item->desc.toString().cptr() ) );
    }

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

    mEngine.drawThread().submitResourceDisposingCommand( item );
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::ResourceLRU::disposeAll()
{
    for( GraphicsResourceItem * item = mEngine.resourceCache().first();
         item;
         item = mEngine.resourceCache().next( item ) )
    {
        dispose( item );
    }
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
inline void GN::engine::RenderEngine::ResourceLRU::markAsRecentlyUsed( GraphicsResourceItem * item )
{
    GN_ASSERT( mEngine.resourceCache().check( item ) );
    mLRUList.remove( item );
    mLRUList.insertBefore( mLRUList.head(), item );
}
