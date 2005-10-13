#include "pch.h"
#include "oglRenderer.h"
#include <algorithm>

// ****************************************************************************
// local types/variables/functions
// ****************************************************************************

//!
//! Split a string into token list
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

//!
//! function use to determine a extension is supported or not
// ------------------------------------------------------------------------
GN_INLINE static bool
sFindExtension( const std::vector<GN::StrA> & glexts, const char * ext )
{
    return glexts.end() != std::find( glexts.begin(), glexts.end(), ext );
}
//!
//! Check required extensions
// ------------------------------------------------------------------------
static bool sCheckRequiredExtensions( const std::vector<GN::StrA> & extensions )
{
    static const char * sRequiredExtensions[] =
    {
        "GL_EXT_bgra",                    // 1.1
        "GL_ARB_multitexture",            // 1.3
        //"GL_ARB_texture_env_combine",     // 1.3
        0,
    };
    bool fail = false;
    char const * const * p = sRequiredExtensions;
    while ( *p )
    {
        if( GL_TRUE != sFindExtension( extensions, *p ) )
        {
            GNOGL_ERROR( "Required extension '%s' was not supported!", *p );
            fail = true;
        }
        // next extension
        ++p;
    }
    return !fail;
}

//!
//! initialize opengl extension
// ------------------------------------------------------------------------
#if GN_WINNT
static bool sGetOGLExtensions( std::vector<GN::StrA> & result, HDC hdc )
#else
static bool sGetOGLExtensions( std::vector<GN::StrA> & result )
#endif
{
    GN_GUARD;

    result.clear();

    // 分析OpenGL-Extentions-String
    sGetTokens( result, (const char*)glGetString(GL_EXTENSIONS) );

#if GN_WINNT
    // 分析WGL Extensions
    PFNWGLGETEXTENSIONSSTRINGARBPROC proc;
    proc = reinterpret_cast<PFNWGLGETEXTENSIONSSTRINGARBPROC>(
        ::wglGetProcAddress("wglGetExtensionsStringARB") );
    if( proc ) sGetTokens( result, (const char *)proc(hdc) );
#endif

    std::sort( result.begin(), result.end() );

    // success;
    return true;

    GN_UNGUARD;
}

//!
//! output GL implementation info.
// ------------------------------------------------------------------------
static void sOutputOGLInfo( const std::vector<GN::StrA> & glexts )
{
    GN_GUARD;

    GN::StrA info;

    // vendor info.
    const char * vendor   = (const char *)glGetString(GL_VENDOR);
    const char * renderer = (const char *)glGetString(GL_RENDERER);
    const char * version  = (const char *)glGetString(GL_VERSION);
    info = GN::strFormat(
        "\n\n"
        "===================================================\n"
        "        OpenGL Implementation Informations\n"
        "---------------------------------------------------\n"
        "    OpenGL vendor      :    %s\n"
        "    OpenGL renderer    :    %s\n"
        "    OpenGL version     :    %s\n",
        vendor, renderer, version );

    // caps. info.
    GLint ts, tu;
    GNOGL_CHECK( glGetIntegerv( GL_MAX_TEXTURE_SIZE, &ts ) );
    if( GLEW_ARB_multitexture )
        GNOGL_CHECK( glGetIntegerv( GL_MAX_TEXTURE_UNITS_ARB, &tu ) );
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

    GNOGL_INFO( info.cstr() );

    GN_UNGUARD;
}

// ****************************************************************************
// local functions that initialize individual capability
// ****************************************************************************

//
static uint32_t sCapsInit_MAX_2D_TEXTURE_SIZE()
{
    GLint result = 0;
    GNOGL_CHECK( glGetIntegerv( GL_MAX_TEXTURE_SIZE, &result ) );
    return result;
}
//
static uint32_t sCapsInit_MAX_CLIP_PLANES()
{
    GLint result = 0;
    GNOGL_CHECK( glGetIntegerv( GL_MAX_CLIP_PLANES, &result ) );
    return result;
}
//
static uint32_t sCapsInit_MAX_RENDER_TARGETS()
{
    // FIXME: this is only suit for glCopyTexImage, not real PBuffer texture
    return 4;
}
//
static uint32_t sCapsInit_MAX_PRIMITIVES()
{
    return 0x10000; // no more than 65536 elements in one DIP
}
//
static uint32_t sCapsInit_MAX_TEXTURE_STAGES()
{
    if( GLEW_ARB_multitexture )
    {
        GLint result;
        GNOGL_CHECK_RV( glGetIntegerv( GL_MAX_TEXTURE_UNITS_ARB, &result ), 1 );
        return result;
    }
    else
    {
        return 1;
    }
}
//
static uint32_t sCapsInit_PER_STAGE_CONSTANT()
{
    // OpenGL always supports this.
    return true;
}
//
static uint32_t sCapsInit_PSCAPS()
{
    uint32_t result = 0;
    if( GLEW_ARB_fragment_program ) result |= GN::gfx::PSCAPS_OGL_ARB1;
    if( GLEW_ARB_shader_objects &&
         GLEW_ARB_fragment_shader &&
         GLEW_ARB_shading_language_100 ) result |= GN::gfx::PSCAPS_OGL_GLSL;
    return result;
}
//
static uint32_t sCapsInit_VSCAPS()
{
    uint32_t result = 0;
    if( GLEW_ARB_vertex_program ) result |= GN::gfx::VSCAPS_OGL_ARB1;
    if( GLEW_ARB_shader_objects &&
         GLEW_ARB_vertex_shader &&
         GLEW_ARB_shading_language_100 ) result |= GN::gfx::VSCAPS_OGL_GLSL;
    return result;
}
//
static uint32_t sOGLCapsInit_MULTI_TEXTURE()
{
    return GLEW_ARB_multitexture;
}
//
static uint32_t sOGLCapsInit_VBO()
{
    // Note: Mesa 6.0.1 has a bug when dealing with VBO
    const char * version  = (const char *)glGetString(GL_VERSION);
    if( ::strstr( version, "Mesa 6.0.1" ) ) return 0;
    else return GLEW_ARB_vertex_buffer_object;
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

    _GN_RENDER_DEVICE_TRACE();

    // init glew
    GLenum glewErr = glewInit();
    if( GLEW_OK != glewErr )
    {
        GNOGL_ERROR( "Fail to initialize glew library : %s",
            (const char *)glewGetErrorString(glewErr) );
        return false;
    }

    // output opengl implementation info.
    std::vector<StrA> glexts;
#if GN_WINNT
    if( !sGetOGLExtensions(glexts, mDeviceContext) )
#else
    if( !sGetOGLExtensions(glexts) )
#endif
    {
        return false;
    }
    sOutputOGLInfo( glexts );

    // check required extension
    if( !sCheckRequiredExtensions( glexts ) ) return false;

    // 逐一的初始化每一个caps
    #define GNGFX_CAPS( name ) \
        setCaps(CAPS_##name, sCapsInit_##name() );
    #define GNOGL_CAPS( name ) mOGLCaps[OGLCAPS_##name].set( \
        sOGLCapsInit_##name() );
    #include "garnet/gfx/gfxCapsMeta.h"
    #include "oglCapsMeta.h"
    #undef GNGFX_CAPS
    #undef GNOGL_CAPS

    // success;
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::gfx::OGLRenderer::capsDeviceDestroy()
{
    GN_GUARD;

    _GN_RENDER_DEVICE_TRACE();

    // clear all caps
    resetAllCaps();
    for( size_t i = 0; i < NUM_OGLCAPS; ++i ) mOGLCaps[i].reset();

    GN_UNGUARD;
}

