#include "pch.h"
#include "oglShader.h"

#ifdef HAS_CG_OGL

#if GN_MSVC
#pragma warning(disable:4100)
#endif

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLBasicShaderCg::init( const StrA & code )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::OGLBasicShaderCg, () );

    // success
    return selfOK();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLBasicShaderCg::quit()
{
    GN_GUARD;

    mShader.quit();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// ****************************************************************************
// from OGLBasicShader
// ****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLBasicShaderCg::enable() const
{
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLBasicShaderCg::disable() const
{
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLBasicShaderCg::apply() const
{
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLBasicShaderCg::applyDirtyUniforms() const
{
    GN_UNIMPL();
}

// ****************************************************************************
// private functions
// ****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLBasicShaderCg::queryDeviceUniform(
    const char * name, HandleType & userData ) const
{
    GN_UNIMPL();
    return false;
}

#endif
