#include "pch.h"
#include "meshresource.h"

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpures");

// *****************************************************************************
// Local stuff
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------

// *****************************************************************************
// GN::gfx::MeshResource::Impl - Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::MeshResource::Impl::init( const MeshResourceDesc & )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::MeshResource::Impl, () );

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MeshResource::Impl::quit()
{
    GN_GUARD;

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// GN::gfx::MeshResource::Impl - public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------

// *****************************************************************************
// GN::gfx::MeshResource::Impl - private methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------


// *****************************************************************************
// GN::gfx::MeshResource
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
class MeshResourceInternal : public MeshResource
{
    MeshResourceInternal( GpuResourceDatabase & db, GpuResourceHandle handle )
        : MeshResource( db, handle )
    {
    }

    bool init( const void * parameters )
    {
        if( NULL == parameters )
        {
            GN_ERROR(sLogger)( "Null parameter pointer." );
            return false;
        }
        return mImpl->init( *(const MeshResourceDesc*)parameters );
    }

    static GpuResource *
    createInstance( GpuResourceDatabase & db,
                    GpuResourceHandle     handle,
                    const void          * parameters )
    {
        AutoObjPtr<MeshResourceInternal> m( new MeshResourceInternal( db, handle ) );
        if( !m->init( parameters ) ) return NULL;
        return m.detach();
    }

    static void deleteInstance( GpuResource * p )
    {
        delete GpuResource::castTo<MeshResourceInternal>( p );
    }

public:

    static bool checkAndRegisterFactory( GpuResourceDatabase & db )
    {
        if( db.hasResourceFactory( guid() ) ) return true;

        GpuResourceFactory factory = { &createInstance, &deleteInstance };

        return db.registerResourceFactory( guid(), "Mesh Resource", factory );
    }
};

//
//
// -----------------------------------------------------------------------------
GN::gfx::MeshResource::MeshResource( GpuResourceDatabase & db, GpuResourceHandle h )
    : GpuResource( db, h )
    , mImpl(NULL)
{
    mImpl = new Impl(*this);
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::MeshResource::~MeshResource()
{
    delete mImpl;
}

//
//
// -----------------------------------------------------------------------------
const Guid & GN::gfx::MeshResource::guid()
{
    static const Guid MESH_GUID = { 0x892f15d5, 0x8e56, 0x4982, { 0x83, 0x1a, 0xc7, 0x1a, 0x11, 0x20, 0x4e, 0x4a } };
    return MESH_GUID;
}

//
//
// -----------------------------------------------------------------------------
GpuResourceHandle GN::gfx::MeshResource::create(
    GpuResourceDatabase    & db,
    const char             * name,
    const MeshResourceDesc & desc )
{
    if( !MeshResourceInternal::checkAndRegisterFactory( db ) ) return NULL;

    return db.createResource( MeshResource::guid(), name, &desc );
}

//
//
// -----------------------------------------------------------------------------
GpuResourceHandle GN::gfx::MeshResource::loadFromFile(
    GpuResourceDatabase & db,
    const char          * filename )
{
    if( !MeshResourceInternal::checkAndRegisterFactory( db ) ) return NULL;

    GN_UNUSED_PARAM( filename );
    GN_UNIMPL();

    return NULL;
}
