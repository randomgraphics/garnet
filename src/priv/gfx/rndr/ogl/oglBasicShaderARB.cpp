#include "pch.h"
#include "oglShader.h"
#include "oglRenderer.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.OGL");

// *****************************************************************************
// Local function
// *****************************************************************************

struct ARBAutoDel
{
    GLuint mProgram;

    ARBAutoDel( GLuint p ) : mProgram(p) {}

    ~ARBAutoDel() { if(mProgram) glDeleteProgramsARB( 1, &mProgram ); }

    void dismiss() { mProgram = 0; }
};

//
//
// ----------------------------------------------------------------------------
static GLuint sCompileShader( GLenum target, const GN::StrA & code )
{
    GN_GUARD;

    if( code.empty() )
    {
        GN_ERROR(sLogger)( "shader code can't be empty!" );
        return false;
    }

    // generate new texture
    GLuint program = 0;
    glGenProgramsARB( 1, &program );
    if( 0 == program )
    {
        GN_ERROR(sLogger)( "Fail to generate new program object!" );
        return 0;
    }
    ARBAutoDel autodel( program );

    // trim leading spaces in shader code
    const char * ptr = code.cptr();
    GLsizei      len = (GLsizei)code.size();
    while( len > 0 &&
        ( ' '==*ptr || '\t' == *ptr || '\n' == *ptr ) )
    {
        ++ptr;
        --len;
    }

    // load program
    glBindProgramARB( target, program );
    glProgramStringARB( target, GL_PROGRAM_FORMAT_ASCII_ARB, len, ptr );
    if( GL_NO_ERROR != glGetError() )
    {
        GLint        errPos;
        const char * errStr;
        glGetIntegerv( GL_PROGRAM_ERROR_POSITION_ARB, &errPos );
        errStr = (const char*)glGetString( GL_PROGRAM_ERROR_STRING_ARB );
        GN_ERROR(sLogger)(
            "\n"
            "============= ARB Shader Program Compile Error ==========\n"
            "%s\n"
            "---------------------------------------------------------\n"
            "character(%d)\n"
            "%s\n"
            "=========================================================\n",
            code.cptr(), errPos, errStr );
        return false;
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
bool GN::gfx::OGLBasicShaderARB::init( const StrA & code )
{
    GN_GUARD;

    OGLAutoAttribStack autoAttribStack;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::OGLBasicShaderARB, () );

    // create shader
    if( !createShader( code ) ) { quit(); return selfOK(); }

    // success
    return selfOK();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLBasicShaderARB::quit()
{
    GN_GUARD;

    OGLAutoAttribStack autoAttribStack;

    if( mProgram )
    {
        GN_ASSERT( glIsProgramARB( mProgram ) );
        glDeleteProgramsARB( 1, &mProgram );
        mProgram = 0;
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
void GN::gfx::OGLBasicShaderARB::enable() const
{
    GN_GUARD_SLOW;

    GN_OGL_CHECK( glEnable( mTarget ) );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLBasicShaderARB::disable() const
{
    GN_GUARD_SLOW;

    GN_OGL_CHECK( glDisable( mTarget ) );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLBasicShaderARB::apply() const
{
    GN_GUARD_SLOW;

    GN_ASSERT( glIsProgramARB(mProgram) );
    GN_OGL_CHECK( glBindProgramARB( mTarget, mProgram ) );

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
void GN::gfx::OGLBasicShaderARB::applyDirtyUniforms() const
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

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLBasicShaderARB::createShader( const StrA & code )
{
    GN_GUARD;

    GN_ASSERT( !mProgram );

    // check device caps
    if( GL_VERTEX_PROGRAM_ARB == mTarget )
    {
        if( !GLEW_ARB_vertex_program )
        {
            GN_ERROR(sLogger)( "do not support ARB vertex program!" );
            return false;
        }
    }
    else
    {
        GN_ASSERT( GL_FRAGMENT_PROGRAM_ARB == mTarget );
        if( !GLEW_ARB_fragment_program )
        {
            GN_ERROR(sLogger)( "do not support ARB fragment program!" );
            return false;
        }
    }

    // compile shader
    mProgram = sCompileShader( mTarget, code );
    if( 0 == mProgram )  return false;

    // get maximum uniform count
    GN_OGL_CHECK_RV(
        glGetProgramivARB( mTarget, GL_MAX_PROGRAM_LOCAL_PARAMETERS_ARB, (GLint*)&mMaxLocalUniforms ),
        false );
    GN_OGL_CHECK_RV(
        glGetProgramivARB( mTarget, GL_MAX_PROGRAM_ENV_PARAMETERS_ARB, (GLint*)&mMaxEnvUniforms ),
        false );
    GN_OGL_CHECK_RV(
        glGetIntegerv( GL_MAX_PROGRAM_MATRICES_ARB, (GLint*)&mMaxMatrixUniforms ),
        false );

    // successful
    return true;

    GN_UNGUARD;
}


//
//
// ----------------------------------------------------------------------------
inline void GN::gfx::OGLBasicShaderARB::applyUniform( const Uniform & u ) const
{
    GN_GUARD_SLOW;

    UniformDesc desc;

    desc.u32 = (uint32_t)u.userData;

    if( ENV_PARAMETER == desc.type )
    {
        switch( u.value.type )
        {
            case UVT_VECTOR4 :
                for( size_t i = 0; i < u.value.vector4s.size(); ++i )
                {
                    GN_OGL_CHECK( glProgramEnvParameter4fvARB(
                        mTarget,
                        (GLuint)(desc.index+i),
                        (const float * )&u.value.vector4s[0] ) );
                }
                break;

            case UVT_MATRIX44 :
            case UVT_FLOAT :
            case UVT_BOOL :
            case UVT_INT :
                GN_ERROR(sLogger)( "OGL ARB shader only supports FLOAT4 uniform, currently" );
                break;

            default:
               // program should not reach here.
               GN_UNEXPECTED();
        }
    }
    else if( LOCAL_PARAMETER == desc.type )
    {
        switch( u.value.type )
        {
            case UVT_VECTOR4 :
                for( size_t i = 0; i < u.value.vector4s.size(); ++i )
                {
                    GN_OGL_CHECK( glProgramLocalParameter4fvARB(
                        mTarget,
                        (GLuint)(desc.index+i),
                        (const float * )u.value.vector4s[i] ) );
                }
                break;

            case UVT_MATRIX44 :
                for( size_t i = 0; i < u.value.matrix44s.size(); ++i )
                {
                    GLuint idx = desc.index + i * 4;
                    GN_OGL_CHECK( glProgramLocalParameter4fvARB(
                        mTarget,
                        idx,
                        (const float * )u.value.matrix44s[i][0] ) );
                    GN_OGL_CHECK( glProgramLocalParameter4fvARB(
                        mTarget,
                        idx + 1,
                        (const float * )u.value.matrix44s[i][1] ) );
                    GN_OGL_CHECK( glProgramLocalParameter4fvARB(
                        mTarget,
                        idx + 2,
                        (const float * )u.value.matrix44s[i][2] ) );
                    GN_OGL_CHECK( glProgramLocalParameter4fvARB(
                        mTarget,
                        idx + 3,
                        (const float * )u.value.matrix44s[i][3] ) );
                }
                break;

            case UVT_FLOAT :
            case UVT_BOOL :
            case UVT_INT :
                GN_ERROR(sLogger)( "OGL ARB shader only supports FLOAT4 uniform, currently" );
                break;

            default:
               // program should not reach here.
               GN_UNEXPECTED();
        }
    }
    else
    {
        GN_ASSERT( MATRIX_PARAMETER == desc.type );

        if( UVT_MATRIX44 == u.value.type )
        {
            for( size_t i = 0; i < u.value.matrix44s.size(); ++i )
            {
                GN_OGL_CHECK( glMatrixMode( (GLenum)(GL_MATRIX0_ARB+desc.index+i) ) );
                GN_OGL_CHECK( glLoadMatrixf( Matrix44f::sTranspose(u.value.matrix44s[i])[0] ) );
            }
        }
        else
        {
            GN_ERROR(sLogger)( "Maxtrix uniform can only accept uniform type of matrix." );
        }
    }

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
// from Shader
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLBasicShaderARB::queryDeviceUniform( const char * name, HandleType & userData ) const
{
    GN_GUARD;

    GN_ASSERT( !strEmpty(name) );

    // get uniform index
    unsigned int index;
    if( !str2Int<unsigned int>( index, name+1 ) )
    {
        GN_ERROR(sLogger)(
            "Invalid parameter name: %s. It must be Exxx, exxx, Lxxx or lxxx. \n"
            "E means ENV parameter; L means LOCAL parameter. xxx is the register index.",
            name );
        return false;
    }

    UniformDesc desc;

    switch( name[0] )
    {
        case 'e':
        case 'E':
            if( index >= mMaxEnvUniforms )
            {
                GN_ERROR(sLogger)( "global uniform index(%d) is too large. (max: %d)", index, mMaxEnvUniforms );
                return false;
            }
            desc.type = ENV_PARAMETER;
            break;

        case 'l':
        case 'L':
            if( index >= mMaxLocalUniforms )
            {
                GN_ERROR(sLogger)( "local uniform index(%d) is too large. (max: %d)", index, mMaxLocalUniforms );
                return false;
            }
            desc.type = LOCAL_PARAMETER;
            break;

        case 'm':
        case 'M':
            if( index >= mMaxMatrixUniforms )
            {
                GN_ERROR(sLogger)( "matrix uniform index(%d) is too large. (max: %d)", index, mMaxMatrixUniforms );
                return false;
            }
            desc.type = MATRIX_PARAMETER;
            break;

        default:
            GN_ERROR(sLogger)(
                "Invalid parameter name: %s. It must be Exxx, exxx, Lxxx or lxxx. \n"
                "E means ENV parameter; L means LOCAL parameter. xxx is the register index.",
                name );
            return false;
    }

    // set user data
    desc.index = index;
    userData = (HandleType)desc.u32;

    // success
    return true;

    GN_UNGUARD;
}
