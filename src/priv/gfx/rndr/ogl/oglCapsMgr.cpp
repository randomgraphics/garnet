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
static inline bool
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

    // ∑÷ŒˆOpenGL-Extentions-String
    sGetTokens( result, (const char*)glGetString(GL_EXTENSIONS) );

#if GN_MSWIN
    // ∑÷ŒˆWGL Extensions
    PFNWGLGETEXTENSIONSSTRINGARBPROC proc;
    proc = reinterpret_cast<PFNWGLGETEXTENSIONSSTRINGARBPROC>(
        ::wglGetProcAddress("wglGetExtensionsStringARB") );
    if( proc ) sGetTokens( result, (const char *)proc(hdc) );
#elif GN_POSIX
    // ∑÷ŒˆGLX Extensions
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

// *****************************************************************************
// device management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLRenderer::capsInit()
{
    GN_GUARD;

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

    // clear all caps
    memset( &mCaps, 0, sizeof(mCaps) );

    // max texture size
    GLuint maxTexSize;
    glGetIntegerv( GL_MAX_TEXTURE_SIZE, (GLint*)&maxTexSize );
    mCaps.maxTex1DSize[0] = maxTexSize; mCaps.maxTex1DSize[1] = 1;
    mCaps.maxTex2DSize[0] = mCaps.maxTex2DSize[1] = maxTexSize; mCaps.maxTex2DSize[2] = 1;

    // handle case where multi-texture extension is not supported
    if( GLEW_ARB_multitexture )
    {
        GN_OGL_CHECK_RV( glGetIntegerv( GL_MAX_TEXTURE_UNITS_ARB, (GLint*)&mCaps.maxTextures ), false );
    }
    else
    {
        glActiveTextureARB = sFake_glActiveTexture;
        glClientActiveTextureARB = sFake_glClientActiveTexture;
        mCaps.maxTextures = 1;
    }

    // max render targets
    mCaps.maxColorRenderTargets = 1;

    // max vertex attributes
    if( GLEW_ARB_vertex_program || GLEW_ARB_vertex_shader )
    {
        glGetIntegerv( GL_MAX_VERTEX_ATTRIBS_ARB, (GLint*)&mCaps.maxVertexAttributes );
    }
    else
    {
        mCaps.maxVertexAttributes = 0;
    }

    // vertex shader flags
    mCaps.vsProfiles |= (!!GLEW_ARB_vertex_program) ? RendererCaps::GPP_OGL_ARB1 : 0;
    mCaps.vsProfiles |= ( GLEW_ARB_shader_objects && GLEW_ARB_vertex_shader && GLEW_ARB_shading_language_100 )
                        ? RendererCaps::GPP_OGL_GLSL : 0;
#ifdef HAS_CG_OGL
    mCaps.vsProfiles |= (CG_PROFILE_UNKNOWN != cgGLGetLatestProfile( CG_GL_VERTEX )) ? RendererCaps::GPP_CG : 0;
#endif

    // note: OGL renderer does not support GS yet.

    // pixel shader flags
    mCaps.psProfiles |= (!!GLEW_ARB_fragment_program) ? RendererCaps::GPP_OGL_ARB1 : 0;
    mCaps.psProfiles |= (GLEW_ARB_shader_objects && GLEW_ARB_fragment_shader && GLEW_ARB_shading_language_100)
                        ? RendererCaps::GPP_OGL_GLSL : 0;
#ifdef HAS_CG_OGL
    mCaps.psProfiles |= (CG_PROFILE_UNKNOWN != cgGLGetLatestProfile( CG_GL_FRAGMENT )) ? RendererCaps::GPP_CG : 0;
#endif

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
bool
GN::gfx::OGLRenderer::checkTextureFormatSupport(
    ColorFormat   /*format*/,
    TextureUsages /*usages*/ ) const
{
    GN_UNIMPL_WARNING();
    return true;
}

//
//
// -----------------------------------------------------------------------------
GN::gfx::ColorFormat
GN::gfx::OGLRenderer::getDefaultTextureFormat( TextureUsages usages ) const
{
    if( usages.depth )
    {
        return ColorFormat::R_32_UINT;
    }
    else
    {
        return ColorFormat::RGBA_8_8_8_8_UNORM;
    }
}
