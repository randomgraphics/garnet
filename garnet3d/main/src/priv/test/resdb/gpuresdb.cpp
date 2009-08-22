#include "pch.h"
#include "gpuresdb.h"

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpures");

// *****************************************************************************
// local functions
// *****************************************************************************

// *****************************************************************************
// GpuResourceDatabase::Impl
// *****************************************************************************


//
//
// -----------------------------------------------------------------------------
GpuResourceDatabase::Impl::Impl( GPU & g )
    : mGpu(g)
{
}

//
//
// -----------------------------------------------------------------------------
GpuResourceDatabase::Impl::~Impl()
{
    clear();
}

//
//
// -----------------------------------------------------------------------------
void GpuResourceDatabase::Impl::clear()
{
    for( int = 0; i < GpuResourceType::NUM_TYPES; ++i )
    {
        mResources[i].clear();
    }

    removeAllResourceLoaders();
}

//
//
// -----------------------------------------------------------------------------
void GpuResourceDatabase::Impl::prependResourceLoader( const StrA & /*loaderName*/, GpuResourceLoaderFactory /*factory*/ )
{
}

//
//
// -----------------------------------------------------------------------------
void GpuResourceDatabase::Impl::appendResourceLoader( const StrA & /*loaderName*/, GpuResourceLoaderFactory /*factory*/ )
{
}

//
//
// -----------------------------------------------------------------------------
void GpuResourceDatabase::Impl::removeResourceLoader( const StrA & /*loaderName*/ )
{
}

//
//
// -----------------------------------------------------------------------------
void GpuResourceDatabase::Impl::removeAllResourceLoaders()
{
}

//
//
// -----------------------------------------------------------------------------
GpuResourceHandle
GpuResourceDatabase::Impl::addResource( GpuResourceType type, const char * name, const GpuResourceLoadingParameters & lp )
{
}

//
//
// -----------------------------------------------------------------------------
void GpuResourceDatabase::Impl::removeResource( GpuResourceHandle );

//
//
// -----------------------------------------------------------------------------
GpuResourceHandle
GpuResourceDatabase::Impl::getResourceHandle( GpuResourceType type, const char * name )
{
    if( type < 0 || type >= GpuResourceType::NUM_TYPES )
    {
        GN_ERROR(sLogger)( "Invalid resource type: %d", type );
        return 0;
    }

    UInt32 internalHandle = mResources[type].name2handle(name);
    if( 0 == internalHandle )
    {
        return 0;
    }

    return composeGpuResourceHandle( type, internalHandle );
}

//
//
// -----------------------------------------------------------------------------
GpuResource
GpuResourceDatabase::Impl::getResource( GpuResourceHandle handle )
{
    ResourceItem * r = getResourceItem( handle );
    if( NULL == r ) return AutoRef<GpuResource>::NULLREF;

    if( NULL == r->resource )
    {
        GN_TODO( "Load the resource." );
    }

    return &r->resource;
}

//
//
// -----------------------------------------------------------------------------
void GpuResourceDatabase::Impl::reloadResource( GpuResourceHandle handle )
{
    ResourceItem * r = getResourceItem( handle );
    if( NULL == r ) return;

    r->resource.clear();
}

//
//
// -----------------------------------------------------------------------------
void GpuResourceDatabase::Impl::reloadAllResources()
{
    for( int i = 0; i < GpuResourceType::NUM_TYPES; ++i )
    {
        NamedGpuResMgr & mgr = mResources[i];

        for( UInt32 h = mgr.first(); 0 != h; h = mgr.next(h) )
        {
            mgr[h].resource.clear();
        }
    }
}

//
//
// -----------------------------------------------------------------------------
GpuResourceDatabase::Impl::ResourceItem *
GpuResourceDatabase::Impl::getResourceItem( GpuResourceHandle handle )
{
    GpuResourceType type = retriveGpuResourceInternalHandle( handle );

    if( type < 0 || type >= GpuResourceType::NUM_TYPES )
    {
        GN_ERROR(sLogger)( "Invalid resource handle: %d", handle );
        return NULL;
    }

    NamedGpuResMgr & mgr = mResources[type];

    UInt32 internalHandle = retriveGpuResourceInternalHandle( handle );

    if( !mgr.validHandle(internalHandle) )
    {
        GN_ERROR(sLogger)( "Invalid resource handle: %d", handle );
        return NULL;
    }

    return &mgr.get(internalHandle);
}

// *****************************************************************************
// GpuResourceDatabase
// *****************************************************************************

GpuResourceDatabase::GpuResourceDatabase( GPU & g ) : mImpl( new Impl(g) ) {}
GpuResourceDatabase::~GpuResourceDatabase() { delete mImpl; }
void GpuResourceDatabase::clear() { mImpl->clear(); }
void GpuResourceDatabase::prependResourceLoader( const StrA & loaderName, GpuResourceLoaderFactory factory ) { mImpl->insertLoader( mImpl->getFirstLoaderName(), loaderName, factory ); }
void GpuResourceDatabase::appendResourceLoader( const StrA & loaderName, GpuResourceLoaderFactory factory ) { mImpl->insertLoader( NULL, loaderName, factory ); }
void GpuResourceDatabase::removeResourceLoader( const StrA & loaderName ) { mImpl->removeLoader( loaderName ); }
void GpuResourceDatabase::removeAllResourceLoaders() { mImpl->removeAllResourceLoaders(); }
GpuResourceHandle    GpuResourceDatabase::addResource( GpuResourceType type, const char * name, const GpuResourceLoadingParameters & lp ) { return mImpl->addResource( type, name, lp ); }
void                 GpuResourceDatabase::removeResource( GpuResourceHandle handle ) { mImpl->removeResource( handle ); }
GpuResourceHandle    GpuResourceDatabase::getResourceHandle( GpuResourceType type, const char * name ) { return mImpl->getResourceHande( type, name ); }
GpuResourceType      GpuResourceDatabase::getResourceType( GpuResourceHandle handle ) { return mImpl->getResourceType(handle); }
const char *         GpuResourceDatabase::getResourceName( GpuResourceHandle handle ) { return mImpl->getResourceName(handle); }
AutoRef<GpuResource> GpuResourceDatabase::getResource( GpuResourceHandle handle ) { return mImpl->getResource(handle); }
void                 GpuResourceDatabase::reloadResource( GpuResourceHandle handle ) { return mImpl->reloadResource( handle ); }
void                 GpuResourceDatabase::reloadAllResources() { return mImpl->reloadAllResources(); }
