#include "pch.h"
#include "gpures.h"

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

//
//
// -----------------------------------------------------------------------------
static bool
sCheckAndRegisterUniformFactory(
    GpuResourceDatabase      & db,
    const GpuResourceFactory & factory )
{
    if( db.hasResourceFactory( UniformResource::guid() ) ) return true;

    return db.registerResourceFactory( UniformResource::guid(), "Uniform Resource", factory );
}

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
GpuResourceHandle GN::gfx::UniformResource::create(
    GpuResourceDatabase & db,
    const char          * name,
    size_t                length,
    const void          * initialData )
{
    GpuResourceFactory factory = { &createInstance, &deleteInstance };
    if( !sCheckAndRegisterUniformFactory( db, factory ) ) return NULL;

    UniformDesc desc = { length, initialData };
    return db.createResource( UniformResource::guid(), name, &desc );
}

//
//
// -----------------------------------------------------------------------------
GpuResourceHandle GN::gfx::UniformResource::loadFromFile(
    GpuResourceDatabase & db,
    const char          * filename )
{
    GpuResourceFactory factory = { &createInstance, &deleteInstance };
    if( !sCheckAndRegisterUniformFactory( db, factory ) ) return NULL;

    GN_UNUSED_PARAM( filename );
    GN_UNIMPL();

    return NULL;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::UniformResource::setUniform( const AutoRef<Uniform> & newUniform )
{
    if( mUniform == newUniform ) return;

    mUniform = newUniform;

    sigUnderlyingResourcePointerChanged(*this);
}

//
//
// -----------------------------------------------------------------------------
GpuResource *
GN::gfx::UniformResource::createInstance(
    GpuResourceDatabase & db,
    GpuResourceHandle     handle,
    const void          * parameters )
{
    UniformResource * m = new UniformResource( db, handle );

    if( NULL == parameters )
    {
        GN_ERROR(sLogger)( "Null parameter pointer." );
        return NULL;
    }

    const UniformDesc * desc = (const UniformDesc*)parameters;

    m->mUniform.attach( db.gpu().createUniform( desc->length ) );

    if( m->mUniform && desc->initialData )
    {
        m->mUniform->update( 0, 0, desc->initialData );
    }

    return m;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::UniformResource::deleteInstance( GpuResource * p )
{
    delete GpuResource::castTo<UniformResource>( p );
}

