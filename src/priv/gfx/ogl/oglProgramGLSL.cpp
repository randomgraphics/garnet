#include "pch.h"
#include "oglShader.h"
#include "oglRenderer.h"

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLProgramGLSL::init( const OGLBasicShaderGLSL * vs, const OGLBasicShaderGLSL * ps )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::OGLProgramGLSL, () );

    // Note: vs and ps can't be both NULL.
    GN_ASSERT( vs || ps );

    if( vs ) mShaders.push_back( vs );
    if( ps ) mShaders.push_back( ps );

    if( !createProgram() ) { quit(); return selfOK(); }

    // success
    return selfOK();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLProgramGLSL::quit()
{
    GN_GUARD;

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLProgramGLSL::apply()
{
    GN_GUARD_SLOW;

    GN_OGL_CHECK( glUseProgramObjectARB( mProgram ) );

    GN_DO_ONCE( GN_WARN( "TODO: apply dirty uniforms" ) );

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLProgramGLSL::createProgram()
{
    GN_GUARD;

    // check GL extensions
    if( !GLEW_ARB_shader_objects )
    {
        GNGFX_ERROR( "no GLSL shader support!" );
        return false;
    }
    
    // create GLSL program
    GN_OGL_CHECK( mProgram = glCreateProgramObjectARB() );
    if( 0 == mProgram )
    {
        GNGFX_ERROR( "fail to create OpenGL shading language program object!" );
        return false;
    }

    // attach shaders to program
    for( size_t i = 0; i < mShaders.size(); ++i )
    {
        GN_OGL_CHECK_RV(
            glAttachObjectARB( mProgram, mShaders[i]->getHandleARB() ),
            false );
    }

    // link program
    GN_OGL_CHECK( glLinkProgramARB(mProgram) );
    int linkOk;
    GN_OGL_CHECK_RV( glGetObjectParameterivARB(
        mProgram, GL_OBJECT_LINK_STATUS_ARB, &linkOk ),
        false );
    if( !linkOk )
    {
        GNGFX_ERROR(
            "\n========== GLSL program link error =========\n"
            "%s\n"
            "==============================================\n",
            getProgramInfoLog(mProgram).cstr() );
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::StrA GN::gfx::OGLProgramGLSL::getProgramInfoLog( GLhandleARB program )
{
    GN_GUARD;
    char buf[4096];
    GN_OGL_CHECK( glGetInfoLogARB( program, 4095, NULL, buf ) );
    return buf;
    GN_UNGUARD;
}
