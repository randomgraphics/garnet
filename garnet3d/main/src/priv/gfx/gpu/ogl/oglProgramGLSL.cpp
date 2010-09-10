#include "pch.h"
#include "oglShader.h"
#include "oglGpu.h"
#include "oglTexture.h"

using namespace GN;
using namespace GN::gfx;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.OGL.GpuProgramGLSL");

// *****************************************************************************
// Local function
// *****************************************************************************

struct GLSLBuiltInAttribute
{
    const char * name;
};

static const GLSLBuiltInAttribute GLSL_BUILT_IN_ATTRIBUTES[] =
{
    { "gl_Color"          },
    { "gl_SecondaryColor" },
    { "gl_Normal"         },
    { "gl_Vertex"         },
    { "gl_MultiTexCoord0" },
    { "gl_MultiTexCoord1" },
    { "gl_MultiTexCoord2" },
    { "gl_MultiTexCoord3" },
    { "gl_MultiTexCoord4" },
    { "gl_MultiTexCoord5" },
    { "gl_MultiTexCoord6" },
    { "gl_MultiTexCoord7" },
    { "gl_FogCoord"       },
};

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
            out.append( stringFormat( "\n(%3d) : ", ++line ) );
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

    // check for preexisting error.
    GN_OGL_CHECK(;);

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

//
//
// -----------------------------------------------------------------------------
static bool sGetOglVertexSemantic( OGLVertexSemantic & semanticName, uint8 & semanticIndex, const char * attributeName, GLuint attributeLocation )
{
    if( -1 == attributeLocation )
    {
        // this is conventional attribute

        semanticIndex = 0;

        if( 0 == stringCompare( "gl_Vertex", attributeName ) )
        {
            semanticName = VERTEX_SEMANTIC_VERTEX;
        }
        else if( 0 == stringCompare( "gl_Normal", attributeName ) )
        {
            semanticName = VERTEX_SEMANTIC_NORMAL;
        }
        else if( 0 == stringCompare( "gl_Color", attributeName ) )
        {
            semanticName = VERTEX_SEMANTIC_COLOR;
        }
        else if( 0 == stringCompare( "gl_SecondaryColor", attributeName ) )
        {
            semanticName  = VERTEX_SEMANTIC_COLOR;
            semanticIndex = 1;
        }
        else if( 0 == stringCompare( "gl_FogCoord", attributeName ) )
        {
            semanticName = VERTEX_SEMANTIC_FOG;
        }
        else if( 0 == stringCompare( "gl_MultiTexCoord0", attributeName ) )
        {
            semanticName = VERTEX_SEMANTIC_TEXCOORD;
        }
        else if( 0 == stringCompare( "gl_MultiTexCoord1", attributeName ) )
        {
            semanticName = VERTEX_SEMANTIC_TEXCOORD;
            semanticIndex    = 1;
        }
        else if( 0 == stringCompare( "gl_MultiTexCoord2", attributeName ) )
        {
            semanticName  = VERTEX_SEMANTIC_TEXCOORD;
            semanticIndex = 2;
        }
        else if( 0 == stringCompare( "gl_MultiTexCoord3", attributeName ) )
        {
            semanticName  = VERTEX_SEMANTIC_TEXCOORD;
            semanticIndex = 3;
        }
        else if( 0 == stringCompare( "gl_MultiTexCoord4", attributeName ) )
        {
            semanticName  = VERTEX_SEMANTIC_TEXCOORD;
            semanticIndex = 4;
        }
        else if( 0 == stringCompare( "gl_MultiTexCoord5", attributeName ) )
        {
            semanticName  = VERTEX_SEMANTIC_TEXCOORD;
            semanticIndex = 5;
        }
        else if( 0 == stringCompare( "gl_MultiTexCoord6", attributeName ) )
        {
            semanticName  = VERTEX_SEMANTIC_TEXCOORD;
            semanticIndex = 6;
        }
        else if( 0 == stringCompare( "gl_MultiTexCoord7", attributeName ) )
        {
            semanticName  = VERTEX_SEMANTIC_TEXCOORD;
            semanticIndex = 7;
        }
        else
        {
            // never reach here
            GN_UNEXPECTED();
            return false;
        }
    }
    else
    {
        // this is general vertex attribute
        semanticName  = VERTEX_SEMANTIC_ATTRIBUTE;
        semanticIndex = (uint8)attributeLocation;
    }

    // success
    return true;
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

    GN_ASSERT( GpuProgramLanguage::GLSL == desc.lang );

    mVS = sCreateShader( desc.vs.source, GL_VERTEX_SHADER_ARB );
    if( 0 == mVS ) return failure();

    mPS = sCreateShader( desc.ps.source, GL_FRAGMENT_SHADER_ARB );
    if( 0 == mPS ) return failure();

    mProgram = sCreateProgram( mVS, mPS );
    if( 0 == mProgram ) return failure();

    // enumerate parameters (textures and uniforms)
    if( !enumParameters() ) return failure();

    // enumerate attributes
    if( !enumAttributes() ) return failure();

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
    mTextures.clear();
    mAttributes.clear();

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
bool GN::gfx::OGLGpuProgramGLSL::getBindingDesc(
    OGLVertexBindingDesc & result, size_t attributeIndex ) const
{
    if( attributeIndex >= mAttributes.size() )
    {
        GN_ERROR(sLogger)( "Invalid attribute index." );
        result.index = 255;
        result.semantic = (OGLVertexSemantic)-1;
        return false;
    }

    const GLSLAttributeDesc & desc = mAttributes[attributeIndex];

    result.semantic = desc.semanticName;
    result.index = desc.semanticIndex;

    // success
    return true;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLGpuProgramGLSL::applyUniforms(
    const Uniform * const * uniforms,
    size_t                  count ) const
{
    if( count != mUniforms.size() )
    {
        GN_ERROR(sLogger)( "Current GPU program requires %d uniforms. But %d are provided.", mUniforms.size(), count );
    }

    if( count > mUniforms.size() ) count = mUniforms.size();

    for( size_t i = 0; i < count; ++i )
    {
        // get uniform pointer
        const SysMemUniform * uniform = (const SysMemUniform *)uniforms[i];
        if( NULL == uniform )
        {
            GN_ERROR(sLogger)( "Null uniform pointer for GPU program uniform parameter #%d.", i );
            continue;
        }

        // get uniform descriptor
        const GLSLUniformOrTextureDesc & desc = mUniforms[i];

        // If the uniform is currently bind to OpenGL, skip it.
        if( uniform == desc.lastUniform && uniform->getTimeStamp() == desc.lastStamp )
        {
            continue;
        }

        // update time stamp
        desc.lastUniform.set( uniform );
        desc.lastStamp = uniform->getTimeStamp();

        // check parameter size
        if( getGpu().paramCheckEnabled() )
        {
            if( uniform->size() != desc.size )
            {
                GN_WARN(sLogger)(
                    "parameter %s: value size(%d) differs from size defined in shader code(%d).",
                    desc.name.cptr(),
                    uniform->size(),
                    desc.size );
            }
        }

        switch( desc.type )
        {
            case GL_FLOAT                      :
                glUniform1fvARB( desc.location, desc.count, (GLfloat*)uniform->getval() );
                break;

            case GL_FLOAT_VEC2_ARB             :
                glUniform2fvARB( desc.location, desc.count, (GLfloat*)uniform->getval() );
                break;

            case GL_FLOAT_VEC3_ARB             :
                glUniform3fvARB( desc.location, desc.count, (GLfloat*)uniform->getval() );
                break;

            case GL_FLOAT_VEC4_ARB             :
                glUniform4fvARB( desc.location, desc.count, (GLfloat*)uniform->getval() );
                break;

            case GL_INT                        :
            case GL_BOOL_ARB                   :
                glUniform1ivARB( desc.location, desc.count, (GLint*)uniform->getval() );
                break;

            case GL_INT_VEC2_ARB               :
            case GL_BOOL_VEC2_ARB              :
                glUniform2ivARB( desc.location, desc.count, (GLint*)uniform->getval() );
                break;

            case GL_INT_VEC3_ARB               :
            case GL_BOOL_VEC3_ARB              :
                glUniform3ivARB( desc.location, desc.count, (GLint*)uniform->getval() );
                break;

            case GL_INT_VEC4_ARB               :
            case GL_BOOL_VEC4_ARB              :
                glUniform4ivARB( desc.location, desc.count, (GLint*)uniform->getval() );
                break;

            case GL_FLOAT_MAT2_ARB             :
                glUniformMatrix2fvARB( desc.location, desc.count, true, (GLfloat*)uniform->getval() );
                break;

            case GL_FLOAT_MAT3_ARB             :
                glUniformMatrix3fvARB( desc.location, desc.count, true, (GLfloat*)uniform->getval() );
                break;

            case GL_FLOAT_MAT4_ARB             :
                glUniformMatrix4fvARB( desc.location, desc.count, true, (GLfloat*)uniform->getval() );
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
void GN::gfx::OGLGpuProgramGLSL::applyTextures(
    const TextureBinding * textures,
    size_t                 count ) const
{
    OGLGpu & r = getGpu();
    size_t maxStages = r.caps().maxTextures;

    // determine effective texture count
    if( count > mTextures.size() )
    {
        count = mTextures.size();
    }
    GN_ASSERT( count <= maxStages );

    // apply textures to OpenGL, one by one
    size_t i;
    for( i = 0; i < count; ++i )
    {
        const TextureBinding & b = textures[i];

        // get texture descriptor
        const GLSLUniformOrTextureDesc & desc = mTextures[i];

        if( b.texture )
        {
            r.chooseTextureStage( i );

            const OGLTexture * ogltexture = safeCastPtr<const OGLTexture>(b.texture.get());

            // bind sampler
            ogltexture->setSampler( b.sampler );

            // bind texture to current stage
            ogltexture->bind();

            // bind current texture stage to the GPU program parameter
            if( desc.lastTexStage != i )
            {
                GLint s = (GLint)i;
                GN_OGL_CHECK( glUniform1ivARB( desc.location, desc.count, &s ) );
                desc.lastTexStage = i;
            }
        }
        else
        {
            r.disableTextureStage( i );
        }
    }

    // disable remaining texture stages
    for( ; i < maxStages; ++i )
    {
        r.disableTextureStage( i );
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
        GLSLUniformOrTextureDesc u;

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
    for( size_t i = 0; i < mUniforms.size(); ++i )
    {
        GLSLUniformOrTextureDesc & u = mUniforms[i];
        u.uniformDesc.name = u.name.cptr();
        u.uniformDesc.size = u.size;
    }
    for( size_t i = 0; i < mTextures.size(); ++i )
    {
        GLSLUniformOrTextureDesc & t = mTextures[i];
        t.textureDesc.name = t.name.cptr();
    }

    // update parameter descriptor
    if( mUniforms.size() > 0 )
    {
        mParamDesc.setUniformArray(
            &mUniforms[0].uniformDesc,
            mUniforms.size(),
            sizeof(GLSLUniformOrTextureDesc) );
    }
    if( mTextures.size() > 0 )
    {
        mParamDesc.setTextureArray(
            &mTextures[0].textureDesc,
            mTextures.size(),
            sizeof(GLSLUniformOrTextureDesc) );
    }

    // check for texture capability
    OGLGpu & r = getGpu();
    if( mTextures.size() > r.caps().maxTextures )
    {
        GN_ERROR(sLogger)( "The GPU program requires more textures than current hardware supports." );
        return false;
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
        GLsizei unusedCount;
        GLenum unusedType;
        GLint location;

        GN_OGL_CHECK_RV( glGetActiveAttribARB( mProgram, i, maxLength, NULL, &unusedCount, &unusedType, nameptr ), false );
        nameptr[maxLength] = 0;

        GN_OGL_CHECK_RV( location = glGetAttribLocationARB( mProgram, nameptr ), false );

        if( !sGetOglVertexSemantic( a.semanticName, a.semanticIndex, nameptr, location ) ) return false;

        a.name = nameptr;

        mAttributes.append( a );
    }

    // initialize name and format arrays
    for( size_t i = 0; i < mAttributes.size(); ++i )
    {
        GLSLAttributeDesc & a = mAttributes[i];
        a.desc.name = a.name.cptr();
    }

    // update parameter descriptor
    mParamDesc.setAttributeArray(
        &mAttributes[0].desc,
        mAttributes.size(),
        sizeof(GLSLAttributeDesc) );

    return true;
}
