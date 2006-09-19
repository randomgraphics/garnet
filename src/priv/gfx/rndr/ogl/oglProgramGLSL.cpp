#include "pch.h"
#include "oglShader.h"
#include "oglRenderer.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.OGL");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLProgramGLSL::init( const OGLBasicShaderGLSL * vs, const OGLBasicShaderGLSL * ps )
{
    GN_GUARD;

    OGLAutoAttribStack autoAttribStack;

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

    // delete program
    if( mProgram ) glDeleteObjectARB( mProgram );

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
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
        GN_ERROR(sLogger)( "no GLSL shader support!" );
        return false;
    }
    
    // create GLSL program
    GN_OGL_CHECK( mProgram = glCreateProgramObjectARB() );
    if( 0 == mProgram )
    {
        GN_ERROR(sLogger)( "fail to create OpenGL shading language program object!" );
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
        GN_ERROR(sLogger)(
            "\n========== GLSL program link error =========\n"
            "%s\n"
            "==============================================\n",
            getProgramInfoLog(mProgram).cptr() );
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
