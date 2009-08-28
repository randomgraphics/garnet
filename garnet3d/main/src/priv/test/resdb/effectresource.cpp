#include "pch.h"
#include "effectresource.h"

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
// GN::gfx::EffectResource::Impl - Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::EffectResource::Impl::init( const EffectResourceDesc & )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::EffectResource::Impl, () );

    // Do custom init here

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::EffectResource::Impl::quit()
{
    GN_GUARD;

    // Do custom shutting down here

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// GN::gfx::EffectResource::Impl - public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
size_t GN::gfx::EffectResource::Impl::getNumPasses() const
{
    GN_UNIMPL_WARNING();
    return 0;
}

//
//
// -----------------------------------------------------------------------------
size_t GN::gfx::EffectResource::Impl::getNumTextures() const
{
    GN_UNIMPL_WARNING();
    return 0;
}

//
//
// -----------------------------------------------------------------------------
size_t GN::gfx::EffectResource::Impl::findTexture( const char * name ) const
{
    GN_UNIMPL_WARNING();
    GN_UNUSED_PARAM( name );
    return PARAMETER_NOT_FOUND;
}

//
//
// -----------------------------------------------------------------------------
const GN::gfx::EffectResource::TextureProperties &
GN::gfx::EffectResource::Impl::getTextureProperties( size_t parameterIndex ) const
{
    GN_UNIMPL();
    GN_UNUSED_PARAM( parameterIndex );
    static TextureProperties dummy;
    return dummy;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::EffectResource::Impl::applyGpuProgramAndRenderStates( size_t pass, GpuContext & gc ) const
{
    GN_UNUSED_PARAM( pass );
    GN_UNUSED_PARAM( gc );
    GN_UNIMPL_WARNING();
}

// *****************************************************************************
// GN::gfx::EffectResource::Impl - private methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------

// *****************************************************************************
// GN::gfx::EffectResource
// *****************************************************************************

class EffectResourceInternal : public EffectResource
{
    EffectResourceInternal( GpuResourceDatabase & db, GpuResourceHandle handle )
        : EffectResource( db, handle )
    {
    }

    bool init( const void * parameters )
    {
        if( NULL == parameters )
        {
            GN_ERROR(sLogger)( "Null parameter pointer." );
            return false;
        }
        return mImpl->init( *(const EffectResourceDesc*)parameters );
    }

    static GpuResource *
    createInstance( GpuResourceDatabase & db,
                    GpuResourceHandle     handle,
                    const void          * parameters )
    {
        AutoObjPtr<EffectResourceInternal> m( new EffectResourceInternal( db, handle ) );
        if( !m->init( parameters ) ) return NULL;
        return m.detach();
    }

    static void deleteInstance( GpuResource * p )
    {
        delete GpuResource::castTo<EffectResourceInternal>( p );
    }

public:

    static bool checkAndRegisterFactory( GpuResourceDatabase & db )
    {
        if( db.hasResourceFactory( guid() ) ) return true;

        GpuResourceFactory factory = { &createInstance, &deleteInstance };

        return db.registerResourceFactory( guid(), "Effect Resource", factory );
    }
};

//
//
// -----------------------------------------------------------------------------
GN::gfx::EffectResource::EffectResource( GpuResourceDatabase & db, GpuResourceHandle h )
    : GpuResource( db, h )
    , mImpl(NULL)
{
    mImpl = new Impl(*this);
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::EffectResource::~EffectResource()
{
    delete mImpl;
}

//
//
// -----------------------------------------------------------------------------
const Guid & GN::gfx::EffectResource::guid()
{
    static const Guid EFFECT_GUID = { 0x621c00ed, 0xcd51, 0x4cc5, { 0x89, 0x9f, 0xd4, 0xd5, 0xb1, 0xd5, 0xb2, 0xa4 } };
    return EFFECT_GUID;
}

//
//
// -----------------------------------------------------------------------------
GpuResourceHandle GN::gfx::EffectResource::create(
    GpuResourceDatabase      & db,
    const char               * name,
    const EffectResourceDesc & desc )
{
    if( !EffectResourceInternal::checkAndRegisterFactory( db ) ) return NULL;

    return db.createResource( EffectResource::guid(), name, &desc );
}

//
//
// -----------------------------------------------------------------------------
GpuResourceHandle GN::gfx::EffectResource::loadFromFile(
    GpuResourceDatabase & db,
    const char          * filename )
{
    if( !EffectResourceInternal::checkAndRegisterFactory( db ) ) return NULL;

    GN_UNUSED_PARAM( filename );
    GN_UNIMPL();

    return NULL;
}

//
//
// -----------------------------------------------------------------------------
size_t GN::gfx::EffectResource::getNumPasses() const { return mImpl->getNumPasses(); }
size_t GN::gfx::EffectResource::getNumTextures() const { return mImpl->getNumTextures(); }
size_t GN::gfx::EffectResource::findTexture( const char * name ) const { return mImpl->findTexture( name ); }
const GN::gfx::EffectResource::TextureProperties & GN::gfx::EffectResource::getTextureProperties( size_t i ) const { return mImpl->getTextureProperties( i ); }
void GN::gfx::EffectResource::applyGpuProgramAndRenderStates( size_t pass, GpuContext & gc ) const { return mImpl->applyGpuProgramAndRenderStates( pass, gc ); }
