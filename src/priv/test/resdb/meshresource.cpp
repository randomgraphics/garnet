#include "pch.h"
#include "meshresource.h"

using namespace GN;
using namespace GN::gfx;

// *****************************************************************************
// local classes and functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::ModelResource::Impl::init( const ModelResourceDesc & desc )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( ModelResource::Impl, () );

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::ModelResource::Impl::quit()
{
    GN_GUARD;

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::ModelResource::Impl::setTexture( const StrA & effectParameterName, GpuResourceHandle handle )
{
    UInt32 h = mTextures.name2handle( effectParameterName );
    if( 0 == h )
    {
        GN_ERROR(sLogger)( "Invalid effect parameter name: %s", effectParameterName.cptr() );
        return;
    }

    TextureItem & ti = mTextures[h];

    // check for redundant call
    if( ti.handle == handle ) return;

    delete ti.change;
    ti.change = new TextureChange( mOwner.database(), handle );
    if( NULL == ti.change ) return;
    ti.change->dowork();

    ti.handle = handle;
}

//
//
// -----------------------------------------------------------------------------
GpuResourceHandle
GN::gfx::ModelResource::Impl::getTexture( const StrA & effectParameterName ) const
{
}

// *****************************************************************************
// private methods
// *****************************************************************************

