#include "pch.h"
#include "uniformresource.h"

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpures");

// *****************************************************************************
// Local stuff
// *****************************************************************************

struct UniformDesc
{
    size_t       length;
    const void * initialData;
};

// *****************************************************************************
// GN::gfx::UniformResource
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
const Guid & GN::gfx::UniformResource::guid()
{
    static const Guid UNIFORM_GUID = { 0xd140dbea, 0x9ce3, 0x4bb9, { 0x9c, 0xa7, 0xb8, 0x14, 0xf3, 0x83, 0xab, 0x95 } };
    return UNIFORM_GUID;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::UniformResource::reset(
    size_t                length,
    const void          * initialData )
{
    AutoRef<Uniform> u( database().gpu().createUniform( length ) );
    if( !u ) return false;

    if( initialData ) u->update( 0, 0, initialData );

    setUniform( u );

    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::UniformResource::setUniform( const AutoRef<Uniform> & newUniform )
{
    if( mUniform == newUniform ) return;

    mUniform = newUniform;

    sigResourceChanged(*this);
}

// *****************************************************************************
// GN::gfx::UniformResourceInternal
// *****************************************************************************

class UniformResourceInternal : public UniformResource
{
    //
    //
    // -----------------------------------------------------------------------------
    UniformResourceInternal( GpuResourceDatabase & db, GpuResourceHandle h )
        : UniformResource( db, h )
    {
    }

    //
    //
    // -----------------------------------------------------------------------------
    ~UniformResourceInternal()
    {
    }

    //
    //
    // -----------------------------------------------------------------------------
    static GpuResource * sCreateInstance(
        GpuResourceDatabase & db,
        GpuResourceHandle     handle )
    {
        return new UniformResourceInternal( db, handle );
    }

    //
    //
    // -----------------------------------------------------------------------------
    static void sDeleteInstance( GpuResource * p )
    {
        delete GpuResource::castTo<UniformResourceInternal>( p );
    }

public:

    //
    //
    // -----------------------------------------------------------------------------
    static bool sRegisterFactory( GpuResourceDatabase & db )
    {
        GpuResourceFactory factory = { &sCreateInstance, &sDeleteInstance };

        if( db.hasResourceFactory( UniformResource::guid() ) ) return true;

        return db.registerResourceFactory( UniformResource::guid(), "Uniform Resource", factory );
    }
};

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::registerUniformResourceFactory( GpuResourceDatabase & db )
{
    return UniformResourceInternal::sRegisterFactory( db );
}
