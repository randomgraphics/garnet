#include "pch.h"
#include "resourceLRU.h"
#include "resourceCache.h"
#include "resourceThread.h"
#include "drawThread.h"

static GN::Logger * sLogger = GN::getLogger("GN.engine.RenderEngine.ResourceLRU");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::engine::RenderEngine::ResourceLRU::init( size_t capacity )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( ResourceLRU, () );

    mCapacity = capacity;
    mRealizedBytes = 0;
    mRealizeStamp = 0;

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
    GN_ASSERT( mEngine.resourceCache().checkResource( item ) );

    GN_ASSERT( 0 == item->next && 0 == item->prev );

    mLRUList.insertBefore( mLRUList.head(), item );
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::ResourceLRU::remove( GraphicsResourceItem * item )
{
    GN_ASSERT( mEngine.resourceCache().checkResource( item ) );
    mLRUList.remove( item );
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::ResourceLRU::realize( GraphicsResourceItem * item )
{
    ++mRealizeStamp;
    if( 0 == mRealizeStamp ) ++ mRealizeStamp;

    doRealize( item );
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::ResourceLRU::dispose( GraphicsResourceItem * item )
{
    GN_ASSERT( mEngine.resourceCache().checkResource( item ) );

    GN_ASSERT( GRS_REALIZED == item->state );

    DrawThread & dt = mEngine.drawThread();

    // dispose dependents
    for( size_t i = 0; i < item->dependents.size(); ++i )
    {
        GraphicsResourceItem * d = item->dependents[i];

        GN_ASSERT( mEngine.resourceCache().checkResource( d ) );

        if( GRS_REALIZED == d->state )
        {
            d->state = GRS_DISPOSED;
            GN_ASSERT( mRealizedBytes >= d->bytes );
            mRealizedBytes -= d->bytes;
            dt.submitResourceDisposeCommand( d );
        }
    }

    // then dispose itself
    item->state = GRS_DISPOSED;
    GN_ASSERT( mRealizedBytes >= item->bytes );
    mRealizedBytes -= item->bytes;
    dt.submitResourceDisposeCommand( item );
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
inline void GN::engine::RenderEngine::ResourceLRU::doRealize( GraphicsResourceItem * item )
{
    GN_ASSERT( mEngine.resourceCache().checkResource( item ) );

    if( GRS_REALIZED == item->state ) return;

    // recursive realize all prerequites, setup resource waiting list
    DynaArray<ResourceCommandWaitItem> waitingList;
    for( size_t i = 0; i < item->prerequisites.size(); ++i )
    {
        GraphicsResourceItem * p = item->prerequisites[i];

        doRealize( p );

        waitingList.resize( waitingList.size() + 1 );
        waitingList.back().item  = p;
        waitingList.back().fence = p->lastSubmissionFence;
    }

    // move to head of LRU list
    markAsRecentlyUsed( item );

    // renew time stamp
    GN_ASSERT( item->realizeStamp != mRealizeStamp );
    item->realizeStamp = mRealizeStamp;

    if( (mRealizedBytes + item->bytes) > mCapacity )
    {
        GraphicsResourceItem * old = mLRUList.tail();

        GN_ASSERT( old && old != item );

        while( mRealizedBytes + item->bytes > mCapacity && old )
        {
            // skip prerequisites
            if( old->realizeStamp != mRealizeStamp )
            {
                dispose( old );
            }

            old = old->prev;
        }
    }

    if( (mRealizedBytes + item->bytes) > mCapacity )
    {
        GN_FATAL(sLogger)( "There's no enough space in resource cache, to hold the resource xxxx and its all prerequisites." );
        GN_UNEXPECTED();
        return;
    }

    item->state = GRS_REALIZED;

    mRealizedBytes += item->bytes;

    GN::AutoRef<GraphicsResourceLoader> newLoader;
    item->sigReload( item, newLoader );

    if( newLoader )
    {
        item->loader = newLoader;
        mEngine.resourceThread().submitResourceLoadCommand( item, &waitingList );
    }
    else if( item->loader )
    {
        mEngine.resourceThread().submitResourceLoadCommand( item, &waitingList  );
    }
    else
    {
        mEngine.drawThread().submitResourceCreateCommand( item, &waitingList  );
    }
}

//
//
// -----------------------------------------------------------------------------
inline void GN::engine::RenderEngine::ResourceLRU::markAsRecentlyUsed( GraphicsResourceItem * item )
{
    GN_ASSERT( mEngine.resourceCache().checkResource( item ) );
    mLRUList.remove( item );
    mLRUList.insertBefore( mLRUList.head(), item );
}
