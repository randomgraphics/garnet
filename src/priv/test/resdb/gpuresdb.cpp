#include "pch.h"
#include "gpuresdb.h"

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpures");

// *****************************************************************************
// local functions
// *****************************************************************************

// *****************************************************************************
// GpuResourceDatabase::Impl public methods
// *****************************************************************************


//
//
// -----------------------------------------------------------------------------
GpuResourceDatabase::Impl::Impl( GpuResourceDatabase & db, Gpu & g )
    : mDatabase(db), mGpu(g)
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
    deleteAllResources();

    // unregister all factories
    mManagers.clear();
}

//
//
// -----------------------------------------------------------------------------
bool GpuResourceDatabase::Impl::registerResourceFactory(
    const Guid       & type,
    const char       * descriptiveName,
    GpuResourceFactory factory )
{
    if( hasResourceFactory( type ) )
    {
        GN_ERROR(sLogger)( "Resource type exisits already!" );
        return false;
    }

    if( NULL == factory.createResource ||
        NULL == factory.deleteResource )
    {
        GN_ERROR(sLogger)( "Resource factory has NULL function pointer(s)." );
        return false;
    }

    if( mManagers.size() == mManagers.MAX_SIZE )
    {
        GN_ERROR(sLogger)( "Resource manager pool is full. Cannot register more resource types!" );
        return false;
    }

    mManagers.resize( mManagers.size() + 1 );

    ResourceManager & mgr = mManagers.back();

    mgr.guid = type;
    mgr.desc = descriptiveName ? descriptiveName : "unnamed resource";
    mgr.index = mManagers.size() - 1;
    mgr.factory = factory;
    GN_ASSERT( mgr.resources.empty() );

    return true;
}

//
//
// -----------------------------------------------------------------------------
bool GpuResourceDatabase::Impl::hasResourceFactory( const Guid & type )
{
    for( size_t i = 0; i < mManagers.size(); ++i )
    {
        ResourceManager & m = mManagers[i];
        if( type == m.guid ) return true;
    }

    return false;
}

//
//
// -----------------------------------------------------------------------------
GpuResourceHandle
GpuResourceDatabase::Impl::createResource(
    const Guid & type,
    const char * name,
    const void * parameters )
{
    if( 0 == name || 0 == *name )
    {
        GN_ERROR(sLogger)( "Invalid name: empty or null string pointer is not allowed." );
        return 0;
    }

    ResourceManager * mgr = getManager( type );
    if( NULL == mgr ) return 0;

    if( mgr->resources.validName( name ) )
    {
        GN_ERROR(sLogger)( "Resource named \"%s\" exists already.", name );
        return 0;
    }

    UInt32 internalHandle = mgr->resources.add( name );
    if( 0 == internalHandle ) return 0;

    GpuResourceHandleStruct hs;
    hs.type = mgr->index;
    hs.internalHandle = internalHandle;

    ResourceItem & item = mgr->resources[internalHandle];
    item.resource = mgr->factory.createResource( mDatabase, hs.u32, parameters );
    if( NULL == item.resource )
    {
        mgr->resources.remove( internalHandle );
        return 0;
    }

    return hs.u32;
}

//
//
// -----------------------------------------------------------------------------
void GpuResourceDatabase::Impl::deleteResource( GpuResourceHandle handle )
{
    ResourceItem * r = getResourceItem( handle );
    if( NULL == r ) return;

    GpuResourceHandleStruct hs;
    hs.u32 = handle;

    ResourceManager & mgr = mManagers[hs.type];

    GN_ASSERT( r->resource );
    mgr.factory.deleteResource( r->resource );

    // note: this will invalidate variable "r".
    mgr.resources.remove( hs.internalHandle );
}

//
//
// -----------------------------------------------------------------------------
void GpuResourceDatabase::Impl::deleteAllResources()
{
    for( size_t i = 0; i < mManagers.size(); ++i )
    {
        ResourceManager & mgr = mManagers[i];

        for( UInt32 i = mgr.resources.first(); i != 0; i = mgr.resources.next( i ) )
        {
            GpuResource * r = mgr.resources[i].resource;
            GN_ASSERT( r );
            mgr.factory.deleteResource( r );
        }

        mgr.resources.clear();
    }
}

//
//
// -----------------------------------------------------------------------------
GpuResourceHandle
GpuResourceDatabase::Impl::findResource( const Guid & type, const char * name )
{
    ResourceManager * mgr = getManager( type );
    if( NULL == mgr ) return 0;

    UInt32 internalHandle = mgr->resources.name2handle(name);
    if( 0 == internalHandle )
    {
        GN_ERROR(sLogger)( "Invalid resource name: %s", name ? name : "NULL name pointer!" );
        return 0;
    }

    GpuResourceHandleStruct hs;
    hs.type = mgr->index;
    hs.internalHandle = internalHandle;
    return hs.u32;
}

//
//
// -----------------------------------------------------------------------------
const char *
GpuResourceDatabase::Impl::getResourceName( GpuResourceHandle handle )
{
    ResourceItem * r = getResourceItem( handle );
    if( NULL == r ) return NULL;

    GpuResourceHandleStruct hs;
    hs.u32 = handle;

    ResourceManager & mgr = mManagers[hs.type];

    return mgr.resources.handle2name(hs.internalHandle);
}

//
//
// -----------------------------------------------------------------------------
GpuResource *
GpuResourceDatabase::Impl::getResource( GpuResourceHandle handle )
{
    ResourceItem * r = getResourceItem( handle );
    if( NULL == r ) return NULL;

    return r->resource;
}

// *****************************************************************************
// GpuResourceDatabase::Impl private methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
const GpuResourceDatabase::Impl::ResourceManager *
GpuResourceDatabase::Impl::getManager( const Guid & type ) const
{
    for( size_t i = 0; i < mManagers.size(); ++i )
    {
        const ResourceManager & m = mManagers[i];
        if( type == m.guid )
        {
            return &m;
        }
    }

    GN_ERROR(sLogger)( "Invalid resource type guid." );

    return NULL;
}

//
//
// -----------------------------------------------------------------------------
GpuResourceDatabase::Impl::ResourceManager *
GpuResourceDatabase::Impl::getManager( const Guid & type )
{
    for( size_t i = 0; i < mManagers.size(); ++i )
    {
        ResourceManager & m = mManagers[i];
        if( type == m.guid )
        {
            return &m;
        }
    }

    GN_ERROR(sLogger)( "Invalid resource type guid." );

    return NULL;
}

//
//
// -----------------------------------------------------------------------------
GpuResourceDatabase::Impl::ResourceItem *
GpuResourceDatabase::Impl::getResourceItem( GpuResourceHandle handle ) const
{
    GpuResourceHandleStruct hs;
    hs.u32 = handle;

    if( hs.type >= mManagers.size() )
    {
        GN_ERROR(sLogger)( "Invalid resource handle %d : invalid type", handle );
        return NULL;
    }

    const ResourceManager & mgr = mManagers[hs.type];

    if( !mgr.resources.validHandle(hs.internalHandle) )
    {
        GN_ERROR(sLogger)( "Invalid resource handle %d : invalid internal handle", handle );
        return NULL;
    }

    return &mgr.resources[hs.internalHandle];
}

// *****************************************************************************
// GpuResourceDatabase
// *****************************************************************************

GpuResourceDatabase::GpuResourceDatabase( Gpu & g ) : mImpl(NULL) { mImpl = new Impl(*this,g); }
GpuResourceDatabase::~GpuResourceDatabase() { delete mImpl; }
void GpuResourceDatabase::clear() { mImpl->clear(); }
bool GpuResourceDatabase::registerResourceFactory( const Guid & type, const char * desc, GpuResourceFactory factory ) { return mImpl->registerResourceFactory( type, desc, factory ); }
bool GpuResourceDatabase::hasResourceFactory( const Guid & type ) { return mImpl->hasResourceFactory( type ); }
GpuResourceHandle    GpuResourceDatabase::createResource( const Guid & type, const char * name, const void * parameters ) { return mImpl->createResource( type, name, parameters ); }
void                 GpuResourceDatabase::deleteResource( GpuResourceHandle handle ) { mImpl->deleteResource( handle ); }
void                 GpuResourceDatabase::deleteAllResources() { mImpl->deleteAllResources(); }
GpuResourceHandle    GpuResourceDatabase::findResource( const Guid & type, const char * name ) { return mImpl->findResource( type, name ); }
const char         * GpuResourceDatabase::getResourceName( GpuResourceHandle handle ) { return mImpl->getResourceName(handle); }
GpuResource        * GpuResourceDatabase::getResource( GpuResourceHandle handle ) { return mImpl->getResource(handle); }
