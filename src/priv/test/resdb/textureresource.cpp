#include "pch.h"
#include "gpures.h"

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpures");

// *****************************************************************************
// Local stuff
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
static bool
sCheckAndRegisterTextureFactory(
    GpuResourceDatabase      & db,
    const GpuResourceFactory & factory )
{
    if( db.hasResourceFactory( TextureResource::guid() ) ) return true;

    return db.registerResourceFactory( TextureResource::guid(), "Texture Resource", factory );
}

// *****************************************************************************
// GN::gfx::TextureResource
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
const Guid & GN::gfx::TextureResource::guid()
{
    static const Guid TEXTURE_GUID = {0x6ad8b59d, 0xe672, 0x4b5e, { 0x8e, 0xec, 0xf7, 0xac, 0xd4, 0xf1, 0x99, 0xdd } };
    return TEXTURE_GUID;
}

//
//
// -----------------------------------------------------------------------------
GpuResourceHandle GN::gfx::TextureResource::create(
    GpuResourceDatabase & db,
    const char          * name,
    const TextureDesc   & desc )
{
    GpuResourceFactory factory = { &createInstance, &deleteInstance };
    if( !sCheckAndRegisterTextureFactory( db, factory ) ) return NULL;

    return db.createResource( TextureResource::guid(), name, &desc );
}

//
//
// -----------------------------------------------------------------------------
GpuResourceHandle GN::gfx::TextureResource::loadFromFile(
    GpuResourceDatabase & db,
    const char          * filename )
{
    GpuResourceFactory factory = { &createInstance, &deleteInstance };
    if( !sCheckAndRegisterTextureFactory( db, factory ) ) return NULL;

    GN_UNUSED_PARAM( filename );
    GN_UNIMPL();

    return NULL;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::TextureResource::setTexture( const AutoRef<Texture> & newTexture )
{
    if( mTexture == newTexture ) return;

    mTexture = newTexture;

    sigUnderlyingResourcePointerChanged(*this);
}

//
//
// -----------------------------------------------------------------------------
GpuResource *
GN::gfx::TextureResource::createInstance(
    GpuResourceDatabase & db,
    GpuResourceHandle     handle,
    const void          * parameters )
{
    TextureResource * m = new TextureResource( db, handle );

    if( NULL == parameters )
    {
        GN_ERROR(sLogger)( "Null parameter pointer." );
        return NULL;
    }

    const TextureDesc * desc = (const TextureDesc*)parameters;

    m->mTexture.attach( db.gpu().createTexture( *desc ) );

    return m;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::TextureResource::deleteInstance( GpuResource * p )
{
    delete GpuResource::castTo<TextureResource>( p );
}
