#include "pch.h"
#include "oglShader.h"
#include "oglRenderer.h"

#ifdef HAS_CG_OGL

#if GN_MSVC
#pragma warning(disable:4100)
#endif

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.OGL");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLBasicShaderCg::init( const StrA & code, const StrA & hints )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::OGLBasicShaderCg, () );

    // get the latest profile
    CGprofile prof = cgGLGetLatestProfile( mProfileClass );
    if( CG_PROFILE_UNKNOWN == prof )
    {
        GN_ERROR(sLogger)( "Fail to get the lastest profile!" );
        return failure();
    }

    // parse hints
    Registry reg( hints );
    StrA entry = reg.gets( "entry", "main" );

    // create the shader
    if( !mShader.init( getRenderer().getCgContext(), prof, code, entry ) ) return failure();

    // load the program
    GN_CG_CHECK_RV( cgGLLoadProgram( mShader.getProgram() ), failure() );

    // success
    return success();

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
