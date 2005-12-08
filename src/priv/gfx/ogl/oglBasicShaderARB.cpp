#include "pch.h"
#include "oglShader.h"
#include "oglRenderer.h"

// *****************************************************************************
// Local function
// *****************************************************************************

// for GLEW multi-context support
#define glewGetContext() r.getGLEWContext()

struct ARBAutoDel
{
    GN::gfx::OGLRenderer & r;
    GLuint mProgram;

    ARBAutoDel( GN::gfx::OGLRenderer & r_, GLuint p ) : r(r_), mProgram(p) {}

    ~ARBAutoDel() { if(mProgram) glDeleteProgramsARB( 1, &mProgram ); }

    void dismiss() { mProgram = 0; }
};

//
//
// ----------------------------------------------------------------------------
static GLuint sCompileShader( GN::gfx::OGLRenderer & r, GLenum target, const GN::StrA & code )
{
    GN_GUARD;

    if( code.empty() )
    {
        GNGFX_ERROR( "shader code can't be empty!" );
        return false;
    }

    // generate new texture
    GLuint program = 0;
    glGenProgramsARB( 1, &program );
    if( 0 == program )
    {
        GNGFX_ERROR( "Fail to generate new program object!" );
        return 0;
    }
    ARBAutoDel autodel( r, program );

    // load program
    glBindProgramARB( target, program );
    glProgramStringARB( target, GL_PROGRAM_FORMAT_ASCII_ARB, (GLsizei)code.size(), code.cstr() );
    if( GL_NO_ERROR != glGetError() )
    {
        GLint        errPos;
        const char * errStr;
        glGetIntegerv( GL_PROGRAM_ERROR_POSITION_ARB, &errPos );
        errStr = (const char*)glGetString( GL_PROGRAM_ERROR_STRING_ARB );
        GNGFX_INFO(
            "\n"
            "============= ARB Shader Program Compile Error ==========\n"
            "%s\n"
            "---------------------------------------------------------\n"
            "character(%d)\n"
            "%s\n"
            "=========================================================\n",
            code.cstr(), errPos, errStr );
        return false;
    }

    // success
    autodel.dismiss();
    return program;

    GN_UNGUARD;
}

// for GLEW multi-context support
#undef glewGetContext

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLBasicShaderARB::init( const StrA & code )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::gfx::OGLBasicShaderARB, () );

    mCode = code;

    if( !deviceCreate() || !deviceRestore() ) { quit(); return selfOK(); }

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

    if( getRenderer().getOGLRC() )
    {
        getRenderer().makeCurrent();
    }

    deviceDispose();
    deviceDestroy();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// Device management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLBasicShaderARB::deviceCreate()
{
    GN_GUARD;

    GN_ASSERT( !mProgram );

    // compile shader
    mProgram = sCompileShader( getRenderer(), mTarget, mCode );
    if( 0 == mProgram )  return false;

    // get maximum uniform count
    GN_OGL_CHECK_RV(
        glGetProgramivARB( mTarget, GL_MAX_PROGRAM_LOCAL_PARAMETERS_ARB, (GLint*)&mMaxLocalUniforms ),
        false );
    GN_OGL_CHECK_RV(
        glGetProgramivARB( mTarget, GL_MAX_PROGRAM_ENV_PARAMETERS_ARB, (GLint*)&mMaxEnvUniforms ),
        false );

    // successful
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLBasicShaderARB::deviceDestroy()
{
    GN_GUARD;

    if( mProgram )
    {
        GN_ASSERT( glIsProgramARB( mProgram ) );
        glDeleteProgramsARB( 1, &mProgram );
        mProgram = 0;
    }

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

    GN_OGL_CHECK( glEnable( GL_VERTEX_PROGRAM_ARB ) );

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLBasicShaderARB::disable() const
{
    GN_GUARD_SLOW;

    GN_OGL_CHECK( glDisable( GL_VERTEX_PROGRAM_ARB ) );

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
// ----------------------------------------------------------------------------
inline void GN::gfx::OGLBasicShaderARB::applyUniform( const Uniform & u ) const
{
    GN_GUARD_SLOW;

    UniformDesc desc;

    desc.u32 = (uint32_t)u.userData;

    GN_ASSERT( desc.type );

    PFNGLPROGRAMLOCALPARAMETER4FVARBPROC fp;
    if( LOCAL_PARAMETER == desc.type )
    {
        fp = glProgramLocalParameter4fvARB;
    }
    else
    {
        GN_ASSERT( ENV_PARAMETER == desc.type );
        fp = glProgramEnvParameter4fvARB;
    }

    switch( u.type )
    {
        case UVT_FLOAT4 :
            for( size_t i = 0; i < u.valueVector4.size(); ++i )
            {
                GN_OGL_CHECK( fp(
                    mTarget,
                    (GLuint)(desc.index+i),
                    (const float * )&u.valueVector4[0] ) );
            }
            break;

        case UVT_MATRIX44 :
        case UVT_FLOAT :
        case UVT_BOOL :
        case UVT_INT :
            GNGFX_ERROR( "OGL ARB shader only supports FLOAT4 uniform, currently" );
            break;

        default:
           // program should not reach here.
           GN_UNEXPECTED();
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
    if( 1 != sscanf( name+1, "%u", &index ) )
    {
        GNGFX_ERROR(
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
                GNGFX_ERROR( "register index(%d) is too large. (max: %d)", index, mMaxEnvUniforms );
                return false;
            }
            desc.type = LOCAL_PARAMETER;
            break;

        case 'l':
        case 'L':
            if( index >= mMaxLocalUniforms )
            {
                GNGFX_ERROR( "register index(%d) is too large. (max: %d)", index, mMaxLocalUniforms );
                return false;
            }
            desc.type = ENV_PARAMETER;
            break;

        default:
            GNGFX_ERROR(
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
