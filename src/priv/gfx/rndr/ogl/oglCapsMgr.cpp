#include "pch.h"
#include "oglRenderer.h"
#include <algorithm>

static GN::Logger * sLogger = GN::getLogger("GN.gfx.rndr.OGL");

// ****************************************************************************
// local types/variables/functions
// ****************************************************************************

///
/// Split a string into token list
// ------------------------------------------------------------------------
static void
sGetTokens( std::vector<GN::StrA> & tokens, const char * str )
{
    if( GN::strEmpty(str) ) return;
    const char * p1 = str;
    const char * p2 = p1;

    while( *p1 )
    {
        while( *p2 && *p2 != ' ' ) ++p2;

        tokens.push_back( GN::StrA(p1, p2-p1) );

        while( *p2 && *p2 == ' ' ) ++p2;

        p1 = p2;
    }
}

///
/// function use to determine a extension is supported or not
// ------------------------------------------------------------------------
static GN_INLINE bool
sFindExtension( const std::vector<GN::StrA> & glexts, const char * ext )
{
    return glexts.end() != std::find( glexts.begin(), glexts.end(), ext );
}
///
/// Check required extensions
// ------------------------------------------------------------------------
static bool sCheckRequiredExtensions( const std::vector<GN::StrA> & extensions )
{
    static const char * sRequiredExtensions[] =
    {
        "GL_EXT_bgra",                    // 1.1
        //"GL_ARB_multitexture",            // 1.3
        //"GL_ARB_texture_env_combine",     // 1.3
        0,
    };
    bool fail = false;
    char const * const * p = sRequiredExtensions;
    while ( *p )
    {
        if( GL_TRUE != sFindExtension( extensions, *p ) )
        {
            GN_ERROR(sLogger)( "Required extension '%s' was not supported!", *p );
            fail = true;
        }
        // next extension
        ++p;
    }
    return !fail;
}

///
/// initialize opengl extension
// ------------------------------------------------------------------------
#if GN_MSWIN
static bool sGetOGLExtensions( HDC hdc, std::vector<GN::StrA> & result )
#else
static bool sGetOGLExtensions( Display * disp, std::vector<GN::StrA> & result )
#endif
{
    GN_GUARD;

    result.clear();

    // 分析OpenGL-Extentions-String
    sGetTokens( result, (const char*)glGetString(GL_EXTENSIONS) );

#if GN_MSWIN
    // 分析WGL Extensions
    PFNWGLGETEXTENSIONSSTRINGARBPROC proc;
    proc = reinterpret_cast<PFNWGLGETEXTENSIONSSTRINGARBPROC>(
        ::wglGetProcAddress("wglGetExtensionsStringARB") );
    if( proc ) sGetTokens( result, (const char *)proc(hdc) );
#elif GN_POSIX
    // 分析GLX Extensions
    // TODO: query server extension string
    sGetTokens( result, (const char*)glXGetClientString( disp, GLX_EXTENSIONS) );
#endif

    std::sort( result.begin(), result.end() );

    // success;
    return true;

    GN_UNGUARD;
}

///
/// output GL implementation info.
// ------------------------------------------------------------------------
static void sOutputOGLInfo( GN::HandleType disp, const std::vector<GN::StrA> & glexts )
{
    GN_GUARD;

    GN::StrA info;

    // vendor info.

#if GN_POSIX
    const char * vendor   = (const char *)glXGetClientString( (Display*)disp, GLX_VENDOR );
    const char * version  = (const char *)glXGetClientString( (Display*)disp, GLX_VERSION );
#else
    GN_UNUSED_PARAM( disp );
    const char * vendor   = (const char *)glGetString(GL_VENDOR);
    const char * version  = (const char *)glGetString(GL_VERSION);
#endif
    const char * renderer = (const char *)glGetString(GL_RENDERER);

    info = GN::strFormat(
        "\n\n"
        "===================================================\n"
        "        OpenGL Implementation Informations\n"
        "---------------------------------------------------\n"
        "    OpenGL vendor      :    %s\n"
        "    OpenGL version     :    %s\n"
        "    OpenGL renderer    :    %s\n",
        vendor, version, renderer );

    // caps. info.
    GLint ts, tu;
    GN_OGL_CHECK( glGetIntegerv( GL_MAX_TEXTURE_SIZE, &ts ) );
    if( GLEW_ARB_multitexture )
        GN_OGL_CHECK( glGetIntegerv( GL_MAX_TEXTURE_UNITS_ARB, &tu ) );
    else
        tu = 1;
    info += GN::strFormat(
        "---------------------------------------------------\n"
        "    Max size of texture             :    %d\n"
        "    Max number of texture stages    :    %d\n",
        ts,tu );

    // extension info.
    info +=
        "---------------------------------------------------\n";
    for ( size_t i = 0; i < glexts.size(); ++i )
    {
        info += "     " + glexts[i] + "\n";
    }

    info +=
        "===================================================\n"
        "\n\n";

    GN_INFO(sLogger)( info.cptr() );

    GN_UNGUARD;
}

#ifndef GLAPIENTRY
#define GLAPIENTRY
#endif
static void GLAPIENTRY sFake_glActiveTexture(GLenum) {}
static void GLAPIENTRY sFake_glClientActiveTexture(GLenum) {}

// ****************************************************************************
// local functions that initialize individual capability
// ****************************************************************************

//
static UInt32 sCapsInit_MAX_2D_TEXTURE_SIZE()
{
    GLint result = 0;
    GN_OGL_CHECK( glGetIntegerv( GL_MAX_TEXTURE_SIZE, &result ) );
    return result;
}
//
static UInt32 sCapsInit_MAX_CLIP_PLANES()
{
    GLint result = 0;
    GN_OGL_CHECK( glGetIntegerv( GL_MAX_CLIP_PLANES, &result ) );
    return result;
}
//
static UInt32 sCapsInit_MAX_RENDER_TARGETS()
{
    // FIXME: this is only suit for glCopyTexImage, not real PBuffer texture
    return 1;
}
//
static UInt32 sCapsInit_MAX_PRIMITIVES()
{
    return 0x10000; // no more than 65536 elements in one DIP
}
//
static UInt32 sCapsInit_MAX_TEXTURE_STAGES()
{
    if( GLEW_ARB_multitexture )
    {
        GLint result;
        GN_OGL_CHECK_RV( glGetIntegerv( GL_MAX_TEXTURE_UNITS_ARB, &result ), 1 );
        return result;
    }
    else
    {
        return 1;
    }
}

// *****************************************************************************
// device management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLRenderer::capsDeviceCreate()
{
    GN_GUARD;

    _GNGFX_DEVICE_TRACE();

    // output opengl implementation info.
    std::vector<StrA> glexts;
#if GN_MSWIN
    if( !sGetOGLExtensions( mDeviceContext, glexts ) )
#else
    if( !sGetOGLExtensions( (Display*)getDispDesc().displayHandle, glexts) )
#endif
    {
        return false;
    }
    sOutputOGLInfo( getDispDesc().displayHandle, glexts );

    // check required extension
    if( !sCheckRequiredExtensions( glexts ) ) return false;

    // 逐一的初始化每一个caps
    #define GNGFX_CAPS( name ) \
        mCaps[CAPS_##name] = sCapsInit_##name();
    #include "garnet/gfx/rendererCapsMeta.h"
    #undef GNGFX_CAPS

    // special case for multi-texture
    if( !GLEW_ARB_multitexture )
    {
        glActiveTextureARB = sFake_glActiveTexture;
        glClientActiveTextureARB = sFake_glClientActiveTexture;
    }

    // success;
    return true;

    GN_UNGUARD;
}

// *****************************************************************************
// from Renderer
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLRenderer::supportShader( const StrA & profile )
{
    GN_GUARD;

    if( "arbvp1" == profile ) return !!GLEW_ARB_vertex_program;
    else if( "arbfp1" == profile ) return !!GLEW_ARB_fragment_program;
    else if( "glslvs" == profile ) return GLEW_ARB_shader_objects &&
                                          GLEW_ARB_vertex_shader &&
                                          GLEW_ARB_shading_language_100;
    else if( "glslps" == profile ) return GLEW_ARB_shader_objects &&
                                          GLEW_ARB_fragment_shader &&
                                          GLEW_ARB_shading_language_100;
#ifdef HAS_CG_OGL
    else if( "cgvs" == profile ) return CG_PROFILE_UNKNOWN != cgGLGetLatestProfile( CG_GL_VERTEX );
    else if( "cgps" == profile ) return CG_PROFILE_UNKNOWN != cgGLGetLatestProfile( CG_GL_FRAGMENT );
#endif
    else return false;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLRenderer::supportTextureFormat( TexType, BitFields, ClrFmt ) const
{
    GN_UNIMPL_WARNING();
    return true;
}
