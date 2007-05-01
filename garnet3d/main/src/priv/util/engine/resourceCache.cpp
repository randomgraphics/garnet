#include "pch.h"
#include "resourceCache.h"

#pragma warning(disable:4100)

// *****************************************************************************
// local functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------

// *****************************************************************************
// ctor / dtor
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::engine::RenderEngine::GraphicsResourceCache::GraphicsResourceCache(
    RenderEngine& engine, UInt32 maxTexBytes, UInt32 maxMeshBytes )
    : mEngine( engine )
{
}

//
//
// -----------------------------------------------------------------------------
GN::engine::RenderEngine::GraphicsResourceCache::~GraphicsResourceCache()
{
}


// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::engine::GraphicsResourceId
GN::engine::RenderEngine::GraphicsResourceCache::alloc(
    const GraphicsResourceCreationParameter & )
{
    return 0;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::GraphicsResourceCache::free( GraphicsResourceId )
{
}

//
//
// -----------------------------------------------------------------------------
GN::engine::GraphicsResourceItem *
GN::engine::RenderEngine::GraphicsResourceCache::id2ptr( GraphicsResourceId )
{
    return 0;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::GraphicsResourceCache::mark_as_realized_and_recently_used( GraphicsResourceId )
{
}

//
//
// -----------------------------------------------------------------------------
GN::engine::GraphicsResourceItem *
GN::engine::RenderEngine::GraphicsResourceCache::makeRoomForResource( GraphicsResourceId, FenceId )
{
    return 0;
}

//
//
// -----------------------------------------------------------------------------
bool GN::engine::RenderEngine::GraphicsResourceCache::createDeviceData( GraphicsResourceId )
{
    return false;
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::GraphicsResourceCache::deleteDeviceData( GraphicsResourceId )
{
}

//
//
// -----------------------------------------------------------------------------
void GN::engine::RenderEngine::GraphicsResourceCache::deleteAllDeviceData()
{
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------


