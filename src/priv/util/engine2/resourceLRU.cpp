#include "pch.h"
#include "resourceLRU.h"
#include "resourceCache.h"

static GN::Logger * sLogger = GN::getLogger("GN.engine2.RenderEngine.ResourceLRU");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::engine2::RenderEngine::ResourceLRU::init( size_t capacity )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( ResourceLRU, () );

    mCapacity = capacity;
    mRealizedBytes = 0;

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine2::RenderEngine::ResourceLRU::quit()
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
void GN::engine2::RenderEngine::ResourceLRU::insert( GraphicsResourceItem * item )
{
    GN_ASSERT( mEngine.resourceCache().checkResource( item ) );

    GN_ASSERT( 0 == item->next && 0 == item->prev );

    mLRUList.insertBefore( mLRUList.head(), item );
}

//
//
// -----------------------------------------------------------------------------
void GN::engine2::RenderEngine::ResourceLRU::remove( GraphicsResourceItem * item )
{
    GN_ASSERT( mEngine.resourceCache().checkResource( item ) );
    mLRUList.remove( item );
}

//
//
// -----------------------------------------------------------------------------
void GN::engine2::RenderEngine::ResourceLRU::realize( GraphicsResourceItem * item, bool * needReload )
{
    GN_ASSERT( mEngine.resourceCache().checkResource( item ) );

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

    // checkResource if there's enough space to hold it.

    if( item->bytes > mCapacity )
    {
        GN_FATAL(sLogger)( "resource cache (%dMB) is not large enough to hold resources '%s' (%dMB).",
            mCapacity/1024/1024, item->desc.name.cptr(), item->bytes/1024/1024 );
        GN_UNEXPECTED();
        return;
    }
    else if( (mRealizedBytes + item->bytes) > mCapacity )
    {
        GraphicsResourceItem * old = mLRUList.tail();

        while( mRealizedBytes + item->bytes > mCapacity )
        {
            GN_ASSERT( old && old != item );
            dispose( old );
            old = old->prev;
        }
    }

    GN_ASSERT( (mRealizedBytes + item->bytes) <= mCapacity );

    item->state = GRS_REALIZED;

    mRealizedBytes += item->bytes;

    GN_ASSERT( mRealizedBytes <= mCapacity );
}

//
//
// -----------------------------------------------------------------------------
void GN::engine2::RenderEngine::ResourceLRU::dispose( GraphicsResourceItem * item )
{
    GN_ASSERT( mEngine.resourceCache().checkResource( item ) );

    if( GRS_DISPOSED == item->state ) return;

    item->state = GRS_DISPOSED;

    GN_ASSERT( mRealizedBytes >= item->bytes );
    mRealizedBytes -= item->bytes;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
inline void GN::engine2::RenderEngine::ResourceLRU::markAsRecentlyUsed( GraphicsResourceItem * item )
{
    GN_ASSERT( mEngine.resourceCache().checkResource( item ) );
    mLRUList.remove( item );
    mLRUList.insertBefore( mLRUList.head(), item );
}
