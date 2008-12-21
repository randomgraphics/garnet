#include "pch.h"
#include "oglShader.h"
#include "oglRenderer.h"

using namespace GN;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.OGL.GpuProgramGLSL");

// *****************************************************************************
// Local function
// *****************************************************************************

class AutoARBObjectDel
{
    GLhandleARB mHandle;

public:

    AutoARBObjectDel( GLhandleARB h ) : mHandle(h) {}

    ~AutoARBObjectDel()
    {
        if( mHandle ) glDeleteObjectARB( mHandle );
    }

    void dismiss() { mHandle = 0; }
};

//
//
// -----------------------------------------------------------------------------
GLhandleARB
sCreateShader( const StrA & code, GLenum usage )
{
    GN_GUARD;

    if( code.empty() )
    {
        GN_ERROR(sLogger)( "shader code cannot be empty string." );
        return 0;
    }

    // check device caps
    if( GL_VERTEX_SHADER_ARB == usage )
    {
        if( !GLEW_ARB_vertex_shader ||
            !GLEW_ARB_shader_objects ||
            !GLEW_ARB_shading_language_100 )
        {
            GN_ERROR(sLogger)( "do not support GLSL vertex shader!" );
            return 0;
        }
    }
    else
    {
        GN_ASSERT( GL_FRAGMENT_SHADER_ARB == usage );
        if( !GLEW_ARB_fragment_shader ||
            !GLEW_ARB_shader_objects ||
            !GLEW_ARB_shading_language_100 )
        {
            GN_ERROR(sLogger)( "do not support GLSL fragment program!" );
            return 0;
        }
    }

    // generate new shader
    GLhandleARB sh = glCreateShaderObjectARB( usage );
    if( 0 == sh )
    {
        GN_ERROR(sLogger)( "Fail to generate new program object!" );
        return 0;
    }
    AutoARBObjectDel autodel( sh );

    // set shader code
    const char * code_str = code.cptr();
    GLint code_size = static_cast<GLint>( code.size() );
    GN_OGL_CHECK_RV(
        glShaderSourceARB( sh, 1, &code_str, &code_size ),
        0 );

    // compile shader
    GN_OGL_CHECK_RV( glCompileShaderARB( sh ), 0 );
    int compile_ok;
    GN_OGL_CHECK_RV( glGetObjectParameterivARB(
        sh, GL_OBJECT_COMPILE_STATUS_ARB, &compile_ok ), 0 );
    if( !compile_ok )
    {
        char buf[4096];
        GN_OGL_CHECK( glGetInfoLogARB( sh, 4095, NULL, buf ) );
        GN_ERROR(sLogger)(
            "\n========== GLSL shader =========\n"
            "%s\n"
            "\n========= compile error ========\n"
            "%s\n"
            "==================================\n",
            code_str, buf );
        return false;
    }

    // success
    autodel.dismiss();
    return sh;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GLhandleARB sCreateProgram( GLhandleARB vs, GLhandleARB ps )
{
    GN_GUARD;

    // check GL extensions
    if( !GLEW_ARB_shader_objects )
    {
        GN_ERROR(sLogger)( "no GLSL shader support!" );
        return 0;
    }

    // create GLSL program
    GLhandleARB program =glCreateProgramObjectARB();
    if( 0 == program )
    {
        GN_ERROR(sLogger)( "fail to create OpenGL shading language program object!" );
        return 0;
    }
    AutoARBObjectDel autodel( program );

    // attach shaders to program
    GN_OGL_CHECK_RV( glAttachObjectARB( program, vs ), 0 );
    GN_OGL_CHECK_RV( glAttachObjectARB( program, ps ), 0 );

    // link program
    GN_OGL_CHECK( glLinkProgramARB(program) );
    int linkOk;
    GN_OGL_CHECK_RV( glGetObjectParameterivARB(
        program, GL_OBJECT_LINK_STATUS_ARB, &linkOk ),
        0 );
    if( !linkOk )
    {
        char buf[4096];

        GN_OGL_CHECK( glGetInfoLogARB( program, 4095, NULL, buf ) );

        GN_ERROR(sLogger)(
            "\n========== GLSL program link error =========\n"
            "%s\n"
            "==============================================\n",
            buf );

        return 0;
    }

    // success
    autodel.dismiss();
    return program;

    GN_UNGUARD;
}

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLGpuProgramGLSL::init( const GpuProgramDesc & desc )
{
    GN_GUARD;

    OGLAutoAttribStack autoAttribStack;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::OGLGpuProgramGLSL, () );

    mVS = sCreateShader( desc.codeVS.code, GL_VERTEX_SHADER_ARB );
    if( 0 == mVS ) return failure();

    mPS = sCreateShader( desc.codePS.code, GL_FRAGMENT_SHADER_ARB );
    if( 0 == mPS ) return failure();

    mProgram = sCreateProgram( mVS, mPS );
    if( 0 == mProgram ) return failure();

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

    if( mProgram ) glDeleteObjectARB( mProgram ), mProgram = 0;
    if( mPS ) glDeleteObjectARB( mPS ), mPS = 0;
    if( mVS ) glDeleteObjectARB( mVS ), mVS = 0;

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}
