#include "pch.h"
#include "oglShader.h"
#include "oglRenderer.h"

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

    // standard init procedure
    GN_STDCLASS_INIT( OGLBasicShaderGLSL, () );

    mCode = code;

    if( !deviceCreate() || !deviceRestore() ) { quit(); return selfOK(); }

    // success
    return selfOK();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLBasicShaderGLSL::quit()
{
    GN_GUARD;

    deviceDispose();
    deviceDestroy();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// from OGLResource
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLBasicShaderGLSL::deviceCreate()
{
    GN_GUARD;

    GN_ASSERT( !mCode.empty() );

    // generate new texture
    mHandle = glCreateShaderObjectARB( mUsage );
    if( 0 == mHandle )
    {
        GN_ERROR( "Fail to generate new program object!" );
        return 0;
    }
    AutoShaderDel autodel( glDeleteObjectARB, mHandle );

    // set shader mCode
    const char * code_str = mCode.cptr();
    GLint code_size = static_cast<GLint>( mCode.size() );
    GN_OGL_CHECK_RV(
        glShaderSourceARB( mHandle, 1, &code_str, &code_size ),
        0 );

    // compile shader
    GN_OGL_CHECK_RV( glCompileShaderARB( mHandle ), 0 );
    int compile_ok;
    GN_OGL_CHECK_RV( glGetObjectParameterivARB(
        mHandle, GL_OBJECT_COMPILE_STATUS_ARB, &compile_ok ), 0 );
    if( !compile_ok )
    {
        char buf[4096];
        GN_OGL_CHECK( glGetInfoLogARB( mHandle, 4095, NULL, buf ) );
        GN_INFO(
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
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLBasicShaderGLSL::deviceDestroy()
{
    GN_GUARD;

    // delete shader object
    if( 0 != mHandle )
    {
        glDeleteObjectARB( mHandle );
        mHandle = 0;
        getRenderer().removeGLSLShader( getType(), this );
    }

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
            GN_ERROR( "'%s' is not a valid GLSL uniform.", u.name.cptr() );
        }
    }
    clearDirtySet();


    GN_UNGUARD_SLOW;
}
