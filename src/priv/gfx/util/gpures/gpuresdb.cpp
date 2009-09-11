#include "pch.h"
#include "gpuresdb.h"
#include "textureresource.h"
#include "uniformresource.h"
#include "meshresource.h"
#include "effectresource.h"
#include "modelresource.h"

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpures");

// *****************************************************************************
// GpuResource::Impl public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::GpuResource::Impl::Impl( GpuResourceDatabase::Impl & db, GpuResource & res )
    : database(db)
    , resource(res)
    , handle(0)
{
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::GpuResource::Impl::~Impl()
{
    if( 0 != handle.u32() )
    {
        database.onResourceDelete( handle );
    }
}

// *****************************************************************************
// GpuResource public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::gfx::GpuResource::GpuResource( GpuResourceDatabase & db )
    : mDatabase(db), mImpl(NULL)
{
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::GpuResource::~GpuResource()
{
    delete mImpl;
}

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

    if( NULL == factory.createResource )
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
    return NULL != getManager(type);
}

//
//
// -----------------------------------------------------------------------------
AutoRef<GpuResource>
GpuResourceDatabase::Impl::createResource(
    const Guid & type,
    const char * name )
{
    // get the manager for the resource type
    ResourceManager * mgr = getManager( type );
    if( NULL == mgr )
    {
        GN_ERROR(sLogger)( "Invalid resouce type: %s", type.toStr() );
        return AutoRef<GpuResource>::NULLREF;
    }

    // compose a unique name for unnamed resource
    char unnamed[256];
    if( 0 == name || 0 == *name )
    {
        static int i = 0;
        ++i;
        strPrintf( unnamed, GN_ARRAY_COUNT(unnamed), "Unnamed %s #%d", mgr->desc.cptr(), i );
        name = unnamed;

        GN_ASSERT( !mgr->resources.validName( name ) );
    }

    // check if the resource with same name exisits already.
    if( mgr->resources.validName( name ) )
    {
        GN_ERROR(sLogger)( "Resource named \"%s\" exists already.", name );
        return AutoRef<GpuResource>::NULLREF;
    }

    // create new resource instance
    AutoRef<GpuResource> newres( mgr->factory.createResource( mDatabase ) );
    if( !newres ) return AutoRef<GpuResource>::NULLREF;

    // create new instance of resource implementation class, assign handle to it
    GpuResource::Impl * resimpl = new GpuResource::Impl( *this, *newres );
    if( !resimpl ) return AutoRef<GpuResource>::NULLREF;
    newres->mImpl = resimpl;

    // create new handle
    UInt32 internalHandle = mgr->resources.add( name, resimpl );
    if( 0 ==internalHandle ) return AutoRef<GpuResource>::NULLREF;
    resimpl->handle.set( (UInt32)mgr->index, internalHandle );

    // done
    return newres;
}

//
//
// -----------------------------------------------------------------------------
AutoRef<GpuResource>
GpuResourceDatabase::Impl::findResource( const Guid & type, const char * name ) const
{
    const ResourceManager * mgr = getManager( type );
    if( NULL == mgr ) return AutoRef<GpuResource>::NULLREF;

    UInt32 internalHandle = mgr->resources.name2handle(name);
    if( 0 == internalHandle ) return AutoRef<GpuResource>::NULLREF;

    GpuResource::Impl * resimpl = mgr->resources[internalHandle];
    GN_ASSERT( resimpl );

    AutoRef<GpuResource> result;
    result.set( &resimpl->resource );

    return result;
}

//
//
// -----------------------------------------------------------------------------
bool GpuResourceDatabase::Impl::validResource( const Guid & type, const GpuResource * resource ) const
{
    if( NULL == resource ) return false;

    if( this != resource->database().mImpl ) return false;

    const ResourceManager * mgr = getManager( type );
    if( NULL == mgr ) return false;

    return mgr->resources.validHandle( resource->mImpl->handle.internalHandle() );
}

//
//
// -----------------------------------------------------------------------------
bool GpuResourceDatabase::Impl::validResource( const GpuResource * resource ) const
{
    if( NULL == resource ) return false;

    if( this != resource->database().mImpl ) return false;

    for( size_t i = 0; i < mManagers.size(); ++i )
    {
        const ResourceManager & mgr = mManagers[i];

        if( mgr.resources.validHandle( resource->mImpl->handle.internalHandle() ) ) return true;
    }

    return false;
}

//
//
// -----------------------------------------------------------------------------
const char *
GpuResourceDatabase::Impl::getResourceName( const GpuResource * resource ) const
{
    GpuResource::Impl * resimpl = getResourceImpl( resource );
    if( NULL == resimpl ) return NULL;

    const ResourceManager & mgr = mManagers[resimpl->handle.managerIndex()];

    const char * name = mgr.resources.handle2name( resimpl->handle.internalHandle() );

    if( NULL == name )
    {
        GN_ERROR(sLogger)( "Fail to get reosource name: Invalid resource pointer." );
    }

    return name;
}

//
//
// -----------------------------------------------------------------------------
const Guid &
GpuResourceDatabase::Impl::getResourceType( const GpuResource * resource ) const
{
    GpuResource::Impl * resimpl = getResourceImpl( resource );

    if( NULL == resimpl )
    {
        static const Guid INVALID_TYPE = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };
        return INVALID_TYPE;
    }

    const ResourceManager & mgr = mManagers[resimpl->handle.managerIndex()];

    return mgr.guid;
}

//
//
// -----------------------------------------------------------------------------
void GpuResourceDatabase::Impl::onResourceDelete( GpuResourceHandle handle )
{
    GN_ASSERT( handle.u32() );

    ResourceManager & mgr = mManagers[handle.managerIndex()];

    mgr.resources.remove( handle.internalHandle() );
}

// *****************************************************************************
// GpuResourceDatabase::Impl private methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
inline const GpuResourceDatabase::Impl::ResourceManager *
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

    return NULL;
}

//
//
// -----------------------------------------------------------------------------
inline GpuResourceDatabase::Impl::ResourceManager *
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

    return NULL;
}

//
//
// -----------------------------------------------------------------------------
inline GpuResource::Impl *
GpuResourceDatabase::Impl::getResourceImpl( const GpuResource * resource ) const
{
    if( NULL == resource )
    {
        GN_ERROR(sLogger)( "Null resource pointer." );
        return NULL;
    }

    if( this != resource->database().mImpl )
    {
        GN_ERROR(sLogger)( "The resource belongs to another database." );
        return NULL;
    }

    GpuResource::Impl * impl = resource->mImpl;

    GN_ASSERT( impl );
    GN_ASSERT( impl->handle.managerIndex() < mManagers.size() );
    GN_ASSERT( mManagers[impl->handle.managerIndex()].resources.validHandle(impl->handle.internalHandle()) );

    return impl;
}

// *****************************************************************************
// GpuResourceDatabase
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GpuResourceDatabase::GpuResourceDatabase( Gpu & g ) : mImpl(NULL)
{
    mImpl = new Impl(*this,g);

    // register built-in resource factories
    GN_VERIFY( registerTextureResourceFactory( *this ) );
    GN_VERIFY( registerUniformResourceFactory( *this ) );
    GN_VERIFY( registerMeshResourceFactory( *this ) );
    GN_VERIFY( registerEffectResourceFactory( *this ) );
    GN_VERIFY( registerModelResourceFactory( *this ) );
}

GpuResourceDatabase::~GpuResourceDatabase() { delete mImpl; }
Gpu                & GpuResourceDatabase::gpu() const { return mImpl->gpu(); }
bool                 GpuResourceDatabase::registerResourceFactory( const Guid & type, const char * desc, GpuResourceFactory factory ) { return mImpl->registerResourceFactory( type, desc, factory ); }
bool                 GpuResourceDatabase::hasResourceFactory( const Guid & type ) { return mImpl->hasResourceFactory( type ); }
AutoRef<GpuResource> GpuResourceDatabase::createResource( const Guid & type, const char * name ) { return mImpl->createResource( type, name ); }
AutoRef<GpuResource> GpuResourceDatabase::findResource( const Guid & type, const char * name ) const { return mImpl->findResource( type, name ); }
bool                 GpuResourceDatabase::validResource( const Guid & type, const GpuResource * resource ) const { return mImpl->validResource( type, resource ); }
bool                 GpuResourceDatabase::validResource( const GpuResource * resource ) const { return mImpl->validResource( resource ); }
const char         * GpuResourceDatabase::getResourceName( const GpuResource * resource ) const { return mImpl->getResourceName(resource); }
const Guid         & GpuResourceDatabase::getResourceType( const GpuResource * resource ) const { return mImpl->getResourceType(resource); }
