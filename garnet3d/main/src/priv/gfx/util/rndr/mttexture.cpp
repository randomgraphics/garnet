#include "pch.h"
#include "mttexture.h"

using namespace GN::gfx;

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::MultiThreadTexture::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::MultiThreadTexture, () );

    // Do custom init here

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadTexture::quit()
{
    GN_GUARD;

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// from Texture
// *****************************************************************************

#pragma warning( disable : 4100 )

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadTexture::updateMipmap(
    size_t              face,
    size_t              level,
    const Box<UInt32> * area,
    size_t              rowPitch,
    size_t              slicePitch,
    const void        * data,
    UpdateFlag          flag )
{
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadTexture::readMipmap( size_t face, size_t level, MipmapData & data )
{
    GN_UNIMPL();
}


//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadTexture::blobWrite( const void * data, size_t length )
{
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
size_t GN::gfx::MultiThreadTexture::blobRead( void * data )
{
    GN_UNIMPL();
    return 0;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::MultiThreadTexture::generateMipmapPyramid()
{
    GN_UNIMPL();
}


//
//
// -----------------------------------------------------------------------------
void * GN::gfx::MultiThreadTexture::getAPIDependentData() const
{
    GN_UNIMPL();
    return 0;
}
