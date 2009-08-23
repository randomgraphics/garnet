#include "pch.h"
#include "gpuresdb.h"

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpures");

// *****************************************************************************
// local functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static inline bool
sCheckType( GpuResourceType type )
{
    if( type < 0 || type >= GpuResourceType::NUM_TYPES )
    {
        GN_ERROR(sLogger)( "Invalid resource type: %d", type );
        return false;
    }
    else
    {
        return true;
    }
}

// *****************************************************************************
// CreationParam public methods
// *****************************************************************************


//
//
// -----------------------------------------------------------------------------
GpuResourceDatabase::Impl::CreationParam &
GpuResourceDatabase::Impl::CreationParam::operator=( const GpuResourceCreationParameters & )
{
    GN_UNIMPL();
    return *this;
}

// *****************************************************************************
// GpuResourceDatabase::Impl public methods
// *****************************************************************************


//
//
// -----------------------------------------------------------------------------
GpuResourceDatabase::Impl::Impl( Gpu & g )
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
    removeAllResources();
}

//
//
// -----------------------------------------------------------------------------
GpuResourceHandle
GpuResourceDatabase::Impl::addResource(
    GpuResourceType                       type,
    const char                          * name,
    const GpuResourceCreationParameters & cp )
{
    if( !sCheckType( type ) ) return 0;

    if( 0 == name || 0 == *name )
    {
        GN_ERROR(sLogger)( "Invalid name: empty or null string pointer is not allowed." );
        return 0;
    }

    NamedGpuResMgr & mgr = mResources[type];

    if( mgr.validName( name ) )
    {
        GN_ERROR(sLogger)( "Resource named \"%s\" exists already.", name );
        return 0;
    }

    ResourceItem item;
    item.resource = createResourceInstance( type, cp );
    item.cp       = cp;
    if( NULL == item.resource )
    {
        return 0;
    }

    UInt32 internalHandle = mgr.add( name, item );
    if( 0 == internalHandle ) return 0;

    return composeGpuResourceHandle( type, internalHandle );
}

//
//
// -----------------------------------------------------------------------------
void GpuResourceDatabase::Impl::removeResource( GpuResourceHandle handle )
{
    ResourceItem * r = getResourceItem( handle );
    if( NULL == r ) return;

    GN_ASSERT( r->resource );
    delete r->resource;

    GpuResourceType type = retrieveGpuResourceType( handle );
    UInt32 internalHandle = retrieveGpuResourceInternalHandle( handle );

    mResources[type].remove( internalHandle );
}

//
//
// -----------------------------------------------------------------------------
void GpuResourceDatabase::Impl::removeAllResources()
{
    for( int i = 0; i < GpuResourceType::NUM_TYPES; ++i )
    {
        NamedGpuResMgr & mgr = mResources[i];

        for( UInt32 i = mgr.first(); i != 0; i = mgr.next( i ) )
        {
            GN_ASSERT( mgr[i].resource );
            delete mgr[i].resource;
        }

        mgr.clear();
    }
}

//
//
// -----------------------------------------------------------------------------
GpuResourceHandle
GpuResourceDatabase::Impl::getResourceHandle( GpuResourceType type, const char * name )
{
    if( !sCheckType(type) ) return 0;

    UInt32 internalHandle = mResources[type].name2handle(name);
    if( 0 == internalHandle )
    {
        GN_ERROR(sLogger)( "Invalid resource name: %s", name ? name : "NULL name pointer!" );
        return 0;
    }

    return composeGpuResourceHandle( type, internalHandle );
}

//
//
// -----------------------------------------------------------------------------
GpuResourceType
GpuResourceDatabase::Impl::getResourceType( GpuResourceHandle handle )
{
    ResourceItem * r = getResourceItem( handle );
    if( NULL == r ) return GpuResourceType::INVALID;

    return retrieveGpuResourceType( handle );
}

//
//
// -----------------------------------------------------------------------------
const char *
GpuResourceDatabase::Impl::getResourceName( GpuResourceHandle handle )
{
    ResourceItem * r = getResourceItem( handle );
    if( NULL == r ) return NULL;

    NamedGpuResMgr & mgr = mResources[retrieveGpuResourceType(handle)];

    UInt32 internalHandle = retrieveGpuResourceInternalHandle( handle );

    return mgr.handle2name(internalHandle);
}

//
//
// -----------------------------------------------------------------------------
GpuResource *
GpuResourceDatabase::Impl::getResource( GpuResourceHandle handle )
{
    ResourceItem * r = getResourceItem( handle );
    if( NULL == r ) return NULL;

    GN_ASSERT( r->resource );

    return r->resource;
}

//
//
// -----------------------------------------------------------------------------
void GpuResourceDatabase::Impl::reloadResource( GpuResourceHandle handle )
{
    ResourceItem * r = getResourceItem( handle );
    if( NULL == r ) return;

    reloadResourceItem( *r );
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
            reloadResourceItem( mgr[h] );
        }
    }
}

// *****************************************************************************
// GpuResourceDatabase::Impl private methods
// *****************************************************************************


//
//
// -----------------------------------------------------------------------------
GpuResourceDatabase::Impl::ResourceItem *
GpuResourceDatabase::Impl::getResourceItem( GpuResourceHandle handle )
{
    GpuResourceType type = retrieveGpuResourceType( handle );
    if( !sCheckType(type) ) return NULL;

    NamedGpuResMgr & mgr = mResources[type];

    UInt32 internalHandle = retrieveGpuResourceInternalHandle( handle );

    if( !mgr.validHandle(internalHandle) )
    {
        GN_ERROR(sLogger)( "Invalid resource handle: %d", handle );
        return NULL;
    }

    return &mgr[internalHandle];
}

GpuResource *
GpuResourceDatabase::Impl::createResourceInstance(
    GpuResourceType                       /*type*/,
    const GpuResourceCreationParameters & /*cp*/ )
{
    GN_UNIMPL();
    return NULL;
}

void GpuResourceDatabase::Impl::reloadResourceItem( ResourceItem & )
{
    GN_UNIMPL();
}

// *****************************************************************************
// GpuResourceDatabase
// *****************************************************************************

GpuResourceDatabase::GpuResourceDatabase( Gpu & g ) : mImpl( new Impl(g) ) {}
GpuResourceDatabase::~GpuResourceDatabase() { delete mImpl; }
void GpuResourceDatabase::clear() { mImpl->clear(); }
GpuResourceHandle    GpuResourceDatabase::addResource( GpuResourceType type, const char * name, const GpuResourceCreationParameters & lp ) { return mImpl->addResource( type, name, lp ); }
void                 GpuResourceDatabase::removeResource( GpuResourceHandle handle ) { mImpl->removeResource( handle ); }
GpuResourceHandle    GpuResourceDatabase::getResourceHandle( GpuResourceType type, const char * name ) { return mImpl->getResourceHandle( type, name ); }
GpuResourceType      GpuResourceDatabase::getResourceType( GpuResourceHandle handle ) { return mImpl->getResourceType(handle); }
const char *         GpuResourceDatabase::getResourceName( GpuResourceHandle handle ) { return mImpl->getResourceName(handle); }
GpuResource        * GpuResourceDatabase::getResource( GpuResourceHandle handle ) { return mImpl->getResource(handle); }
void                 GpuResourceDatabase::reloadResource( GpuResourceHandle handle ) { return mImpl->reloadResource( handle ); }
void                 GpuResourceDatabase::reloadAllResources() { return mImpl->reloadAllResources(); }
