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
bool GN::gfx::OGLGpuProgramGLSL::init( const GpuProgramDesc & )
{
    GN_GUARD;

    OGLAutoAttribStack autoAttribStack;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::OGLGpuProgramGLSL, () );

    if( !createProgram() ) return failure();

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLGpuProgramGLSL::quit()
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
bool GN::gfx::OGLGpuProgramGLSL::createProgram()
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

    // TODO: attach shaders to program
    /*for( size_t i = 0; i < mShaders.size(); ++i )
    {
        GN_OGL_CHECK_RV(
            glAttachObjectARB( mProgram, mShaders[i]->getHandleARB() ),
            false );
    }*/

    // link program
    GN_OGL_CHECK( glLinkProgramARB(mProgram) );
    int linkOk;
    GN_OGL_CHECK_RV( glGetObjectParameterivARB(
        mProgram, GL_OBJECT_LINK_STATUS_ARB, &linkOk ),
        false );
    if( !linkOk )
    {
        char buf[4096];

        GN_OGL_CHECK( glGetInfoLogARB( mProgram, 4095, NULL, buf ) );

        GN_ERROR(sLogger)(
            "\n========== GLSL program link error =========\n"
            "%s\n"
            "==============================================\n",
            buf );
    }

    // success
    return true;

    GN_UNGUARD;
}
