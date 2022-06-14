#include "pch.h"
#include "oglGpu.h"
#include <algorithm>

using namespace GN;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.gpu.OGL");

// ****************************************************************************
// local types/variables/functions
// ****************************************************************************

///
/// Split a string into token list
// ------------------------------------------------------------------------
static void sGetTokens(DynaArray<StrA> & tokens, const char * str) {
    if (str::isEmpty(str)) return;
    const char * p1 = str;
    const char * p2 = p1;

    while (*p1) {
        while (*p2 && *p2 != ' ') ++p2;

        tokens.append(StrA(p1, p2 - p1));

        while (*p2 && *p2 == ' ') ++p2;

        p1 = p2;
    }
}

///
/// function use to determine a extension is supported or not
// ------------------------------------------------------------------------
static inline bool sFindExtension(const DynaArray<StrA> & glexts, const char * ext) { return glexts.end() != std::find(glexts.begin(), glexts.end(), ext); }

///
/// Get OpenGL version number
// ------------------------------------------------------------------------
static void sGetOpenGLVersion(const char * version, int * major, int * minor, int * release) {
    // According to OpenGL standard, the version must starts with: major.minor or major.minor.release

    StrA str;

    // get major version
    const char * p = version;
    while ('.' != *p) ++p;
    if (major) *major = str::toInetger<int>(StrA(version, p - version).rawptr(), 0);

    // get minor version
    version = p + 1;
    p       = version;
    while (*p && '.' != *p && ' ' != *p) ++p;
    if (minor) *minor = str::toInetger<int>(StrA(version, p - version).rawptr(), 0);

    // get release version
    if (*p && '.' == *p) {
        version = p + 1;
        p       = version;
        while (*p && ' ' != *p) ++p;
        if (release) *release = str::toInetger<int>(StrA(version, p - version).rawptr(), 0);
    } else {
        if (release) *release = 0;
    }
}

///
/// Check required extensions
// ------------------------------------------------------------------------
static bool sCheckRequiredExtensions(const DynaArray<StrA> & extensions) {
    static const char * sRequiredExtensions[] = {
        "GL_EXT_bgra",                 // 1.1
        "GL_ARB_vertex_buffer_object", // 1.4
        0,
    };
    bool                 fail = false;
    char const * const * p    = sRequiredExtensions;
    while (*p) {
        if (GL_TRUE != sFindExtension(extensions, *p)) {
            GN_ERROR(sLogger)("Required extension '%s' was not supported!", *p);
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
#if GN_WINPC
static bool sGetOGLExtensions(HDC hdc, DynaArray<StrA> & result)
#else
static bool sGetOGLExtensions(Display * disp, DynaArray<StrA> & result)
#endif
{
    GN_GUARD;

    result.clear();

    // OpenGL-Extentions-String
    sGetTokens(result, (const char *) glGetString(GL_EXTENSIONS));

#if GN_WINPC
    // WGL Extensions
    PFNWGLGETEXTENSIONSSTRINGARBPROC proc;
    proc = reinterpret_cast<PFNWGLGETEXTENSIONSSTRINGARBPROC>(::wglGetProcAddress("wglGetExtensionsStringARB"));
    if (proc) sGetTokens(result, (const char *) proc(hdc));
#elif GN_POSIX
    // GLX Extensions
    // TODO: query server extension string
    sGetTokens(result, (const char *) glXGetClientString(disp, GLX_EXTENSIONS));
#endif

    std::sort(result.begin(), result.end());

    // success;
    return true;

    GN_UNGUARD;
}

///
/// output GL implementation info.
// ------------------------------------------------------------------------
static void sOutputOGLInfo(intptr_t disp, const DynaArray<StrA> & glexts) {
    GN_GUARD;

    StrA info;

    // vendor and version info.
    GN_UNUSED_PARAM(disp);
    const char * vendor   = (const char *) glGetString(GL_VENDOR);
    const char * version  = (const char *) glGetString(GL_VERSION);
    const char * renderer = (const char *) glGetString(GL_RENDERER);
    const char * glsl     = "N/A";

    int major;
    sGetOpenGLVersion(version, &major, NULL, NULL);
    if (major >= 2) { glsl = (const char *) glGetString(GL_SHADING_LANGUAGE_VERSION); }

    info = GN::str::format("\n\n"
                           "===================================================\n"
                           "        OpenGL Implementation Informations\n"
                           "---------------------------------------------------\n"
                           "    OpenGL vendor      :    %s\n"
                           "    OpenGL version     :    %s\n"
                           "    OpenGL renderer    :    %s\n"
                           "    GLSL version       :    %s\n",
                           vendor, version, renderer, glsl);

    // caps. info.
    GLint ts, tu;
    GN_OGL_CHECK(glGetIntegerv(GL_MAX_TEXTURE_SIZE, &ts));
    if (GLEW_ARB_multitexture)
        GN_OGL_CHECK(glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &tu));
    else
        tu = 1;
    info += str::format("---------------------------------------------------\n"
                        "    Max size of texture             :    %d\n"
                        "    Max number of texture stages    :    %d\n"
                        "===================================================\n"
                        "\n\n",
                        ts, tu);
    GN_INFO(sLogger)(info.rawptr());

    // extension info.
    info = "\n\n"
           "===================================================\n"
           "              OpenGL Extension List\n"
           "---------------------------------------------------\n";
    for (size_t i = 0; i < glexts.size(); ++i) { info += "     " + glexts[i] + "\n"; }
    info += "===================================================\n"
            "\n\n";

    GN_VERBOSE(sLogger)(info.rawptr());

    GN_UNGUARD;
}

// *****************************************************************************
// device management
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLGpu::capsInit() {
    GN_GUARD;

    // output opengl implementation info.
    DynaArray<StrA> glexts;
#if GN_WINPC
    if (!sGetOGLExtensions(mDeviceContext, glexts))
#else
    if (!sGetOGLExtensions((Display *) getDispDesc().displayHandle, glexts))
#endif
    {
        return false;
    }
    sOutputOGLInfo(getDispDesc().displayHandle, glexts);

    // Get Opengl Major version
    int majorVersion, miniorVersion;
    sGetOpenGLVersion((const char *) glGetString(GL_VERSION), &majorVersion, &miniorVersion, NULL);
    if (majorVersion < 2) {
        GN_ERROR(sLogger)("OpenGL version 2.0+ is required.");
        return false;
    }

    // check required extension
    if (!sCheckRequiredExtensions(glexts)) return false;

    // clear all caps
    memset(&mCaps, 0, sizeof(mCaps));

    // max texture size
    GLuint maxTexSize;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, (GLint *) &maxTexSize);
    mCaps.maxTex1DSize[0] = maxTexSize;
    mCaps.maxTex1DSize[1] = 1;
    mCaps.maxTex2DSize[0] = mCaps.maxTex2DSize[1] = maxTexSize;
    mCaps.maxTex2DSize[2]                         = 1;

    // handle case where multi-texture extension is not supported
    GN_OGL_CHECK_R(glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, (GLint *) &mCaps.maxTextures), false);

    // TODO: MRT support
    mCaps.maxColorRenderTargets = 1;

    // max vertex attributes
    if (GLEW_ARB_vertex_program || GLEW_ARB_vertex_shader) {
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS_ARB, (GLint *) &mCaps.maxVertexAttributes);
    } else {
        mCaps.maxVertexAttributes = 0;
    }

    // shader caps
    mCaps.shaderModels |= ShaderModel::GLSL_1_10; // GLSL 110 is part of GL 2.0+
    int glslMajor, glslMinor;
    sGetOpenGLVersion((const char *) glGetString(GL_SHADING_LANGUAGE_VERSION), &glslMajor, &glslMinor, NULL);
    if (glslMajor > 3 || (glslMajor == 3 && glslMinor >= 30)) { mCaps.shaderModels |= ShaderModel::GLSL_3_30; }

    // success;
    return true;

    GN_UNGUARD;
}

// *****************************************************************************
// from Gpu
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::gfx::OGLGpu::checkTextureFormatSupport(ColorFormat /*format*/, TextureUsage /*usages*/) const {
    GN_UNIMPL_WARNING();
    return true;
}
