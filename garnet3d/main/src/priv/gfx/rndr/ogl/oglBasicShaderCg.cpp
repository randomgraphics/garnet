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
    mProfile = cgGLGetLatestProfile( mProfileClass );
    if( CG_PROFILE_UNKNOWN == mProfile )
    {
        GN_ERROR(sLogger)( "Fail to get the lastest profile!" );
        return failure();
    }

    // parse hints
    Registry reg( hints );
    StrA entry = reg.gets( "entry", "main" );

    // create the shader
    if( !mShader.init( getRenderer().getCgContext(), mProfile, code, entry ) ) return failure();

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
    GN_GUARD_SLOW;
    GN_ASSERT( ok() );
    GN_CG_CHECK( cgGLDisableProfile( mProfile ) );
    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLBasicShaderCg::apply() const
{
    GN_GUARD_SLOW;

    GN_ASSERT( ok() );

    // enable the shader
    GN_CG_CHECK( cgGLEnableProfile( mProfile ) );
    GN_CG_CHECK( cgGLBindProgram( mShader.getProgram() ) );

    // apply ALL uniforms to D3D device
    uint32_t handle = getFirstUniform();
    while( handle )
    {
        applyUniform( getUniform( handle ) );
        handle = getNextUniform( handle );
    }
    clearDirtySet();

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLBasicShaderCg::applyDirtyUniforms() const
{
    GN_GUARD_SLOW;
    const std::set<uint32_t> dirtySet = getDirtyUniforms();
    std::set<uint32_t>::const_iterator i, e = dirtySet.end();
    for( i = dirtySet.begin(); i != e; ++i )
    {
        applyUniform( getUniform( *i ) );
    }
    clearDirtySet();
    GN_UNGUARD_SLOW;
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
    GN_GUARD;
    
    GN_ASSERT( !strEmpty(name) );

    CGparameter param = mShader.getUniformHandle( name );
    if( 0 == param ) return false;

    // success
    userData = (HandleType)param;
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLBasicShaderCg::applyUniform( const Uniform & u ) const
{
    GN_GUARD_SLOW;

    CGparameter param = (CGparameter)u.userData;

    GN_ASSERT( cgIsParameter( param ) );

    switch( u.value.type )
    {
        case UVT_VECTOR4 :
            if( 1 == u.value.vector4s.size() )
            {
                GN_CG_CHECK( cgGLSetParameter4fv(
                    param,
                    u.value.vector4s[0] ) );
            }
            else
            {
                GN_CG_CHECK( cgGLSetParameterArray4f(
                    param,
                    0,
                    (long)u.value.vector4s.size(),
                    u.value.vector4s[0] ) );
            }
            break;

        case UVT_MATRIX44 :
            if( 1 == u.value.matrix44s.size() )
            {
                GN_CG_CHECK( cgGLSetMatrixParameterfr(
                    param,
                    u.value.matrix44s[0][0] ) );
            }
            else
            {
                GN_CG_CHECK( cgGLSetMatrixParameterArrayfr(
                    param,
                    0,
                    (long)u.value.matrix44s.size(),
                    u.value.matrix44s[0][0] ) );
            }
            break;

        case UVT_FLOAT :
            if( 1 == u.value.floats.size() )
            {
                GN_CG_CHECK( cgGLSetParameter1f(
                    param,
                    u.value.floats[0] ) );
            }
            else
            {
                GN_CG_CHECK( cgGLSetParameterArray1f(
                    param,
                    0,
                    (long)u.value.floats.size(),
                    &u.value.floats[0] ) );
            }
            break;

        case UVT_BOOL :
        case UVT_INT :
            GN_ERROR(sLogger)( "OGL Cg shader does not support boolean and integer uniforms." );
            break;

        default:
           // program should not reach here.
           GN_UNEXPECTED();
    }

    GN_UNGUARD;
}

#endif
