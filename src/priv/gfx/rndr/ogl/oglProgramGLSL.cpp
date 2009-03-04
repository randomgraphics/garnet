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
static GN::StrA sAddLineCount( const GN::StrA & in )
{
    using namespace GN;

    GN::StrA out( "(  1) : " );

    int line = 1;
    for( const char * s = in.cptr(); *s; ++s )
    {
        if( '\n' == *s )
        {
            out.append( strFormat( "\n(%3d) : ", ++line ) );
        }
        else
        {
            out.append( *s );
        }
    }

    return out;
}

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
            sAddLineCount(code_str).cptr(), buf );
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
static inline size_t
sGetUniformSize( GLenum type )
{
    switch( type )
    {
        case GL_FLOAT                      : return sizeof(float);
        case GL_FLOAT_VEC2_ARB             : return sizeof(float) * 2;
        case GL_FLOAT_VEC3_ARB             : return sizeof(float) * 3;
        case GL_FLOAT_VEC4_ARB             : return sizeof(float) * 4;
        case GL_INT                        :
        case GL_BOOL_ARB                   : return sizeof(int);
        case GL_INT_VEC2_ARB               :
        case GL_BOOL_VEC2_ARB              : return sizeof(int) * 2;
        case GL_INT_VEC3_ARB               :
        case GL_BOOL_VEC3_ARB              : return sizeof(int) * 3;
        case GL_INT_VEC4_ARB               :
        case GL_BOOL_VEC4_ARB              : return sizeof(int) * 4;
        case GL_FLOAT_MAT2_ARB             : return sizeof(float) * 4;
        case GL_FLOAT_MAT3_ARB             : return sizeof(float) * 9;
        case GL_FLOAT_MAT4_ARB             : return sizeof(float) * 16;
        case GL_SAMPLER_1D_ARB             :
        case GL_SAMPLER_2D_ARB             :
        case GL_SAMPLER_3D_ARB             :
        case GL_SAMPLER_CUBE_ARB           :
        case GL_SAMPLER_1D_SHADOW_ARB      :
        case GL_SAMPLER_2D_SHADOW_ARB      :
        case GL_SAMPLER_2D_RECT_ARB        :
        case GL_SAMPLER_2D_RECT_SHADOW_ARB :
            GN_UNEXPECTED_EX( "Texture parameter is not supposed to be handled by this function." )
            return sizeof(GLint);

        default :
            GN_UNEXPECTED_EX( "invalid uniform type" );
            return 0;
    }
}

//
//
// -----------------------------------------------------------------------------
static bool
sIsTextureUniform( GLenum type )
{
    return GL_SAMPLER_1D_ARB <= type && type <= GL_SAMPLER_2D_RECT_SHADOW_ARB;
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

    GN_ASSERT( GPL_GLSL == desc.lang );

    mVS = sCreateShader( desc.vs.code, GL_VERTEX_SHADER_ARB );
    if( 0 == mVS ) return failure();

    mPS = sCreateShader( desc.ps.code, GL_FRAGMENT_SHADER_ARB );
    if( 0 == mPS ) return failure();

    mProgram = sCreateProgram( mVS, mPS );
    if( 0 == mProgram ) return failure();

    // enumerate parameters (textures and uniforms)
    if( !enumParameters() ) return failure();

    // enumerate attributes
    if( !enumAttributes() ) return failure();

    // initialize parameter descriptor
    mParamDesc.numUniforms      = mUniforms.size();
    mParamDesc.uniformNames     = mUniformNames.cptr();
    mParamDesc.uniformSizes     = mUniformSizes.cptr();
    mParamDesc.numTextures      = mTextures.size();
    mParamDesc.textureNames     = mTextureNames.cptr();
    mParamDesc.numAttributes    = mAttributes.size();
    mParamDesc.attributeNames   = mAttributeNames.cptr();
 
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

    mUniforms.clear();
    mUniformNames.clear();
    mUniformSizes.clear();
    mTextures.clear();
    mTextureNames.clear();

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
void GN::gfx::OGLGpuProgramGLSL::applyUniforms( const OGLUniform * const * uniforms, size_t count ) const
{
    for( size_t i = 0; i < count; ++i )
    {
        if( i >= mUniforms.size() )
        {
            GN_WARN(sLogger)( "there are more GPU parameters than the shader needs." );
            return;
        }

        const OGLUniform * u = uniforms[i];

        if( NULL == u )
        {
            GN_ERROR(sLogger)( "Null uniform pointer." );
            continue;
        }

        const GLSLUniformAndTextureDesc & d = mUniforms[i];

        if( u == d.lastUniform && u->getTimeStamp() == d.lastStamp )
        {
            // ignore redundant parameters
            continue;
        }

        // update time stamp
        d.lastUniform.set( u );
        d.lastStamp = u->getTimeStamp();

        // check parameter size
        if( getRenderer().paramCheckEnabled() )
        {
            if( u->size() != d.size )
            {
                GN_WARN(sLogger)(
                    "parameter %s: value size(%d) differs from size defined in shader code(%d).",
                    d.name.cptr(),
                    u->size(),
                    d.size );
            }
        }

        switch( d.type )
        {
            case GL_FLOAT                      :
                glUniform1fvARB( d.location, d.count, (GLfloat*)u->getval() );
                break;

            case GL_FLOAT_VEC2_ARB             :
                glUniform2fvARB( d.location, d.count, (GLfloat*)u->getval() );
                break;

            case GL_FLOAT_VEC3_ARB             :
                glUniform3fvARB( d.location, d.count, (GLfloat*)u->getval() );
                break;

            case GL_FLOAT_VEC4_ARB             :
                glUniform4fvARB( d.location, d.count, (GLfloat*)u->getval() );
                break;

            case GL_INT                        :
            case GL_BOOL_ARB                   :
                glUniform1ivARB( d.location, d.count, (GLint*)u->getval() );
                break;

            case GL_INT_VEC2_ARB               :
            case GL_BOOL_VEC2_ARB              :
                glUniform2ivARB( d.location, d.count, (GLint*)u->getval() );
                break;

            case GL_INT_VEC3_ARB               :
            case GL_BOOL_VEC3_ARB              :
                glUniform3ivARB( d.location, d.count, (GLint*)u->getval() );
                break;

            case GL_INT_VEC4_ARB               :
            case GL_BOOL_VEC4_ARB              :
                glUniform4ivARB( d.location, d.count, (GLint*)u->getval() );
                break;

            case GL_FLOAT_MAT2_ARB             :
                glUniformMatrix2fvARB( d.location, d.count, true, (GLfloat*)u->getval() );
                break;

            case GL_FLOAT_MAT3_ARB             :
                glUniformMatrix3fvARB( d.location, d.count, true, (GLfloat*)u->getval() );
                break;

            case GL_FLOAT_MAT4_ARB             :
                glUniformMatrix4fvARB( d.location, d.count, true, (GLfloat*)u->getval() );
                break;

            case GL_SAMPLER_1D_ARB             :
            case GL_SAMPLER_2D_ARB             :
            case GL_SAMPLER_3D_ARB             :
            case GL_SAMPLER_CUBE_ARB           :
            case GL_SAMPLER_1D_SHADOW_ARB      :
            case GL_SAMPLER_2D_SHADOW_ARB      :
            case GL_SAMPLER_2D_RECT_ARB        :
            case GL_SAMPLER_2D_RECT_SHADOW_ARB :
                GN_UNEXPECTED_EX( "sampler/texture is not handled by this function." );
                break;
        }
    }

    GN_OGL_CHECK( (void)0 );
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLGpuProgramGLSL::applyTexture( const char * name, size_t stage ) const
{
    GN_ASSERT( name );

    size_t idx = getTextureIndex( name );

    if( PARAMETER_NOT_FOUND != idx )
    {
        GN_ASSERT( idx < mTextures.size() );

        const GLSLUniformAndTextureDesc & t = mTextures[idx];

        if( t.lastTexStage != stage )
        {
            GLint s = (GLint)stage;
            GN_OGL_CHECK( glUniform1ivARB( t.location, t.count, &s ) );
            t.lastTexStage = stage;
        }
    }
    else
    {
        GN_ERROR(sLogger)( "texture name '%s' is not found in current GPU program." );
    }
}

// *****************************************************************************
// private methods
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool
GN::gfx::OGLGpuProgramGLSL::enumParameters()
{
    GN_GUARD;

    // get parameter count
    GLint numParameters;
    GN_OGL_CHECK_RV( glGetObjectParameterivARB( mProgram, GL_OBJECT_ACTIVE_UNIFORMS_ARB, &numParameters ), false );

    // get maxinum length of parameter name;
    GLint maxLength;
    GN_OGL_CHECK_RV( glGetObjectParameterivARB( mProgram, GL_OBJECT_ACTIVE_UNIFORM_MAX_LENGTH_ARB, &maxLength ), false );

    // enumerate all parameters
    char * nameptr = (char*)alloca( maxLength+1 );
    mUniforms.clear();
    for( GLint i = 0; i < numParameters; ++i )
    {
        GLSLUniformAndTextureDesc u;

        GN_OGL_CHECK_RV( glGetActiveUniformARB( mProgram, i, maxLength, NULL, &u.count, &u.type, nameptr ), false );
        nameptr[maxLength] = 0;

        GN_OGL_CHECK_RV( u.location = glGetUniformLocationARB( mProgram, nameptr ), false );

        u.name = nameptr;

        if( sIsTextureUniform( u.type ) )
        {
            mTextures.append( u );
        }
        else
        {
            u.size = sGetUniformSize(u.type) * u.count;
            mUniforms.append( u );
        }
    }

    // initialize name and size arrays
    mUniformNames.resize( mUniforms.size() );
    mUniformSizes.resize( mUniforms.size() );
    for( size_t i = 0; i < mUniforms.size(); ++i )
    {
        mUniformNames[i] = mUniforms[i].name.cptr();
        mUniformSizes[i] = mUniforms[i].size;
    }

    mTextureNames.resize( mTextures.size() );
    for( size_t i = 0; i < mTextures.size(); ++i )
    {
        mTextureNames[i] = mTextures[i].name.cptr();
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool
GN::gfx::OGLGpuProgramGLSL::enumAttributes()
{
    // get attribute count
    GLint numAttributes;
    GN_OGL_CHECK_RV( glGetObjectParameterivARB( mProgram, GL_OBJECT_ACTIVE_ATTRIBUTES_ARB, &numAttributes ), false );

    // get maxinum length of attribute name;
    GLint maxLength;
    GN_OGL_CHECK_RV( glGetObjectParameterivARB( mProgram, GL_OBJECT_ACTIVE_ATTRIBUTE_MAX_LENGTH_ARB, &maxLength ), false );

    // enumerate all attributes
    char * nameptr = (char*)alloca( maxLength+1 );
    mAttributes.clear();
    for( GLint i = 0; i < numAttributes; ++i )
    {
        GLSLAttributeDesc a;

        GN_OGL_CHECK_RV( glGetActiveAttribARB( mProgram, i, maxLength, NULL, &a.count, &a.type, nameptr ), false );
        nameptr[maxLength] = 0;

        GN_OGL_CHECK_RV( a.location = glGetAttribLocationARB( mProgram, nameptr ), false );

        a.name = nameptr;

        mAttributes.append( a );
    }

    // initialize name and format arrays
    mAttributeNames.resize( mAttributes.size() );
    for( size_t i = 0; i < mAttributes.size(); ++i )
    {
        mAttributeNames[i]   = mAttributes[i].name.cptr();
    }

    return true;
}
