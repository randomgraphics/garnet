#include "pch.h"
#include "oglShader.h"
#include "oglRenderer.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.OGL");

// *****************************************************************************
// Local functions
// *****************************************************************************

class AutoShaderDel
{
    PFNGLDELETEOBJECTARBPROC mFunc;
    GLhandleARB              mHandle;

public:

    AutoShaderDel( PFNGLDELETEOBJECTARBPROC f, GLhandleARB h ) : mFunc(f), mHandle(h) {}

    ~AutoShaderDel()
    {
        GN_ASSERT( mFunc );
        if( mHandle ) mFunc( mHandle );
    }

    void dismiss() { mHandle = 0; }
};

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLBasicShaderGLSL::init( const StrA & code )
{
    GN_GUARD;

    OGLAutoAttribStack autoAttribStack;

    // standard init procedure
    GN_STDCLASS_INIT( OGLBasicShaderGLSL, () );

    if( !createShader( code ) ) return failure();

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLBasicShaderGLSL::quit()
{
    GN_GUARD;

    OGLAutoAttribStack autoAttribStack;

    // delete shader object
    if( 0 != mHandle )
    {
        glDeleteObjectARB( mHandle );
        mHandle = 0;
        getRenderer().removeGLSLShader( getType(), this );
    }

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// from OGLBasicShader
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLBasicShaderGLSL::disable() const
{
    GN_GUARD_SLOW;

    GN_OGL_CHECK( glUseProgramObjectARB( 0 ) );

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// from Shader
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLBasicShaderGLSL::queryDeviceUniform(
    const char * name, HandleType & userData ) const
{
    GN_UNUSED_PARAM(name);
    GN_UNUSED_PARAM(userData);
    return true; // always return true
}

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLBasicShaderGLSL::applyDirtyUniforms( GLhandleARB program ) const
{
    GN_GUARD_SLOW;

    const std::set<uint32_t> dirtySet = getDirtyUniforms();
    std::set<uint32_t>::const_iterator i, e = dirtySet.end();
    for( i = dirtySet.begin(); i != e; ++i )
    {
        const Uniform & u = getUniform( *i );

        GLint location;
        
        GN_OGL_CHECK_DO(
            location = glGetUniformLocationARB( program, u.name.cptr() ),
            continue; );

        if( location >= 0 )
        {
            // update uniform value
            switch( u.value.type )
            {
                case UVT_VECTOR4 :
                    GN_OGL_CHECK( glUniform4fvARB(
                        location,
                        (GLsizei)u.value.vector4s.size(),
                        (const float * )&u.value.vector4s[0] ) );
                    break;

                case UVT_MATRIX44 :
                    GN_OGL_CHECK( glUniformMatrix4fvARB(
                        location,
                        (GLsizei)u.value.matrix44s.size(),
                        GL_TRUE, // row major
                        (const float * )&u.value.matrix44s[0] ) );
                    break;

                case UVT_FLOAT :
                    GN_OGL_CHECK( glUniform1fvARB(
                        location,
                        (GLsizei)u.value.floats.size(),
                        &u.value.floats[0] ) );
                    break;

                case UVT_BOOL :
                    GN_OGL_CHECK( glUniform1ivARB(
                        location,
                        (GLsizei)u.value.bools.size(),
                        (const GLint*)&u.value.bools[0] ) );
                    break;

                case UVT_INT :
                    GN_OGL_CHECK( glUniform1ivARB(
                        location,
                        (GLsizei)u.value.ints.size(),
                        (const GLint*)&u.value.ints[0] ) );
                    break;

                default:
                   // program should not reach here.
                   GN_UNEXPECTED();
            }
        }
        else
        {
            GN_ERROR(sLogger)( "'%s' is not a valid GLSL uniform.", u.name.cptr() );
        }
    }
    clearDirtySet();


    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLBasicShaderGLSL::createShader( const StrA & code )
{
    GN_GUARD;

    GN_ASSERT( !code.empty() );

    // check device caps
    if( GL_VERTEX_SHADER_ARB == mUsage )
    {
        if( !GLEW_ARB_vertex_shader ||
            !GLEW_ARB_shader_objects ||
            !GLEW_ARB_shading_language_100 )
        {
            GN_ERROR(sLogger)( "do not support GLSL vertex shader!" );
            return false;
        }
    }
    else
    {
        GN_ASSERT( GL_FRAGMENT_SHADER_ARB == mUsage );
        if( !GLEW_ARB_fragment_shader ||
            !GLEW_ARB_shader_objects ||
            !GLEW_ARB_shading_language_100 )
        {
            GN_ERROR(sLogger)( "do not support GLSL fragment program!" );
            return false;
        }
    }

    // generate new shader
    GLhandleARB sh = glCreateShaderObjectARB( mUsage );
    if( 0 == sh )
    {
        GN_ERROR(sLogger)( "Fail to generate new program object!" );
        return 0;
    }
    AutoShaderDel autodel( glDeleteObjectARB, sh );

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
    mHandle = sh;
    return true;

    GN_UNGUARD;
}
