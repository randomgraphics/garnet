#include "pch.h"
#include "oglShader.h"
#include "oglRenderer.h"

using namespace GN;
using namespace GN::gfx;

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

//
//
// -----------------------------------------------------------------------------
static bool
sEnumUniforms(
    DynaArray<GLSLUniformDesc> & uniforms,
    GLhandleARB                  program )
{
    GN_GUARD;

    // get uniform count
    GLint numUniforms;
    GN_OGL_CHECK_RV( glGetObjectParameterivARB( program, GL_OBJECT_ACTIVE_UNIFORMS_ARB, &numUniforms ), false );

    // get maxinum length of uniform name;
    GLint maxLength;
    GN_OGL_CHECK_RV( glGetObjectParameterivARB( program, GL_OBJECT_ACTIVE_UNIFORM_MAX_LENGTH_ARB, &maxLength ), false );

    // enumerate all uniforms
    char * nameptr = (char*)alloca( maxLength+1 );
    uniforms.resize( numUniforms );
    for( GLuint i = 0; i < uniforms.size(); ++i )
    {
        GLSLUniformDesc & u = uniforms[i];

        GN_OGL_CHECK_RV( glGetActiveUniformARB( program, i, maxLength, NULL, &u.count, &u.type, nameptr ), false );

        GN_OGL_CHECK_RV( u.location = glGetUniformLocationARB( program, nameptr ), false );

        u.name = nameptr;

        u.value.clear();

        u.nextDirty = NULL;
    }

    // success
    return true;

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

    mVS = sCreateShader( desc.vs.code, GL_VERTEX_SHADER_ARB );
    if( 0 == mVS ) return failure();


    mPS = sCreateShader( desc.ps.code, GL_FRAGMENT_SHADER_ARB );
    if( 0 == mPS ) return failure();

    mProgram = sCreateProgram( mVS, mPS );
    if( 0 == mProgram ) return failure();

    if( !sEnumUniforms( mUniforms, mProgram ) ) return failure();

    // setup parameter array
    mParams.resize( mUniforms.size() );
    for( size_t i = 0; i < mParams.size(); ++i )
    {
        GpuProgramParameterDesc & p = mParams[i];
        GLSLUniformDesc & u = mUniforms[i];
        p.name = u.name.cptr();
    }

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

// *****************************************************************************
// public methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLGpuProgramGLSL::setParameter( size_t index, const void * value, size_t length )
{
    if( index >= mUniforms.size() )
    {
        GN_ERROR(sLogger)( "Invalid parameter index : %d", index );
        return;
    }

    GLSLUniformDesc & u = mUniforms[index];

    if( length > u.value.size() )
    {
        GN_WARN(sLogger)(
            "update parameter %s: value length(%d) exceeds maxinum allowed value(%d) defined in shader code.",
            u.name.cptr(),
            length,
            u.value.size() );
        length = u.value.size();
    }
    else if( 0 == length )
    {
        length = u.value.size();
    }
    else if( length < u.value.size() )
    {
        GN_VERBOSE(sLogger)( "Partial update to shader parameter %s", u.name.cptr() );
    }

    memcpy( u.value.cptr(), value, length );

    // attach to dirty uniform list
    u.nextDirty = mDirtyList;
    mDirtyList = &u;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLGpuProgramGLSL::applyDirtyParameters() const
{
    while( mDirtyList )
    {
        const GLSLUniformDesc & u = *mDirtyList;

        switch( u.type )
        {
            case GL_FLOAT                      :
                glUniform1fvARB( u.location, u.count, (GLfloat*)u.value.cptr() );
                break;

            case GL_FLOAT_VEC2_ARB             :
                glUniform2fvARB( u.location, u.count, (GLfloat*)u.value.cptr() );
                break;

            case GL_FLOAT_VEC3_ARB             :
                glUniform3fvARB( u.location, u.count, (GLfloat*)u.value.cptr() );
                break;

            case GL_FLOAT_VEC4_ARB             :
                glUniform4fvARB( u.location, u.count, (GLfloat*)u.value.cptr() );
                break;

            case GL_INT                        :
            case GL_BOOL_ARB                   :
                glUniform1ivARB( u.location, u.count, (GLint*)u.value.cptr() );
                break;

            case GL_INT_VEC2_ARB               :
            case GL_BOOL_VEC2_ARB              :
                glUniform2ivARB( u.location, u.count, (GLint*)u.value.cptr() );
                break;

            case GL_INT_VEC3_ARB               :
            case GL_BOOL_VEC3_ARB              :
                glUniform3ivARB( u.location, u.count, (GLint*)u.value.cptr() );
                break;

            case GL_INT_VEC4_ARB               :
            case GL_BOOL_VEC4_ARB              :
                glUniform4ivARB( u.location, u.count, (GLint*)u.value.cptr() );
                break;

            case GL_FLOAT_MAT2_ARB             :
                glUniformMatrix2fvARB( u.location, u.count, false, (GLfloat*)u.value.cptr() );
                break;

            case GL_FLOAT_MAT3_ARB             :
                glUniformMatrix3fvARB( u.location, u.count, false, (GLfloat*)u.value.cptr() );
                break;

            case GL_FLOAT_MAT4_ARB             :
                glUniformMatrix4fvARB( u.location, u.count, false, (GLfloat*)u.value.cptr() );
                break;

            case GL_SAMPLER_1D_ARB             :
            case GL_SAMPLER_2D_ARB             :
            case GL_SAMPLER_3D_ARB             :
            case GL_SAMPLER_CUBE_ARB           :
            case GL_SAMPLER_1D_SHADOW_ARB      :
            case GL_SAMPLER_2D_SHADOW_ARB      :
            case GL_SAMPLER_2D_RECT_ARB        :
            case GL_SAMPLER_2D_RECT_SHADOW_ARB :
                GN_ERROR(sLogger)( "Fail to apply GLSL parameters: sampler parameter is not supported yet." );
                break;
        }

        // next dirty uniform
        mDirtyList = mDirtyList->nextDirty;
    }
}
