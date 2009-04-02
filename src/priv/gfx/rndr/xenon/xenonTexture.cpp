#include "pch.h"
#include "xenonRenderer.h"
#include "xenonTexture.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.Xenon");

// *****************************************************************************
// local functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------

// ****************************************************************************
//  init / quit functions
// ****************************************************************************

//
//
// ----------------------------------------------------------------------------
bool GN::gfx::XenonTexture::init( const TextureDesc & desc )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::XenonTexture, () );

    if( !setDesc( desc ) ) return failure();

    // determine texture format
    mD3DFormat = (D3DFORMAT)colorFormat2XenonFormat( desc.format );
    if( D3DFMT_UNKNOWN == mD3DFormat )
    {
        GN_ERROR(sLogger)( "Fail to convert color format '%s' to D3DFORMAT.", desc.format.toString().cptr() );
        return failure();
    }

    // rendertarget and depth texture requires tiled format.
    if( desc.usages.rendertarget || desc.usages.depth )
    {
        GN_UNIMPL();
    }

    // TODO: determine D3D usage

    // TODO: create texture
    GN_UNIMPL_WARNING();

    // success
    return success();

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
void GN::gfx::XenonTexture::quit()
{
    GN_GUARD;

    safeRelease( mD3DTexture );

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// ****************************************************************************
// from class Texture
// ****************************************************************************

//
//
// ----------------------------------------------------------------------------
void GN::gfx::XenonTexture::updateMipmap(
    size_t              /*face*/,
    size_t              /*level*/,
    const Box<UInt32> * /*area*/,
    size_t              /*rowPitch*/,
    size_t              /*slicePitch*/,
    const void        * /*data*/,
    SurfaceUpdateFlag   /*flag*/ )
{
    GN_GUARD_SLOW;

    GN_UNIMPL_WARNING();

    GN_UNGUARD_SLOW;
}
