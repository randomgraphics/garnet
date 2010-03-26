#include "pch.h"

using namespace GN;

static GN::Logger * sLogger = GN::GetLogger("GN.gfx.tool.oglInfo");

///
/// Split a string into token list
// ------------------------------------------------------------------------
static void
sGetTokens( DynaArray<GN::StrA> & tokens, const char * str )
{
    if( GN::IsStringEmpty(str) ) return;
    const char * p1 = str;
    const char * p2 = p1;

    while( *p1 )
    {
        while( *p2 && *p2 != ' ' ) ++p2;

        tokens.Append( GN::StrA(p1, p2-p1) );

        while( *p2 && *p2 == ' ' ) ++p2;

        p1 = p2;
    }
}

///
/// initialize opengl extension
// ------------------------------------------------------------------------
#if GN_MSWIN
bool sGetOGLExtensions( HDC hdc, DynaArray<GN::StrA> & result )
#else
bool sGetOGLExtensions( Display * disp, DynaArray<GN::StrA> & result )
#endif
{
    GN_GUARD;

    result.Clear();

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

    std::sort( result.Begin(), result.End() );

    // success;
    return true;

    GN_UNGUARD;
}

void printOglInfo( GN::HandleType disp, int index )
{
    GN_GUARD;

    DynaArray<GN::StrA> glexts;
    GN::StrA info;

#if GN_POSIX
    sGetOGLExtensions( (Display*)disp, glexts);
    const char * vendor   = (const char *)glXGetClientString( (Display*)disp, GLX_VENDOR );
    const char * version  = (const char *)glXGetClientString( (Display*)disp, GLX_VERSION );
#else
    sGetOGLExtensions( (HDC)disp, glexts );
    const char * vendor   = (const char *)glGetString(GL_VENDOR);
    const char * version  = (const char *)glGetString(GL_VERSION);
#endif
    const char * renderer = (const char *)glGetString(GL_RENDERER);

    info = GN::StringFormat(
        "\n\n"
        "===================================================\n"
        "        OpenGL Implementation Informations(%d)\n"
        "---------------------------------------------------\n"
        "    OpenGL vendor      :    %s\n"
        "    OpenGL version     :    %s\n"
        "    OpenGL renderer    :    %s\n",
        index,
        vendor, version, renderer );

    // caps. info.
    GLint ts, tu;
    GN_OGL_CHECK( glGetIntegerv( GL_MAX_TEXTURE_SIZE, &ts ) );
    if( GLEW_ARB_multitexture )
        GN_OGL_CHECK( glGetIntegerv( GL_MAX_TEXTURE_UNITS_ARB, &tu ) );
    else
        tu = 1;
    info += GN::StringFormat(
        "---------------------------------------------------\n"
        "    Max size of texture             :    %d\n"
        "    Max number of texture stages    :    %d\n",
        ts,tu );

    // extension info.
    info +=
        "---------------------------------------------------\n";
    for ( size_t i = 0; i < glexts.Size(); ++i )
    {
        info += glexts[i] + " ";
    }
    info +=
        "\n"
        "===================================================\n"
        "\n\n";

    GN_INFO(sLogger)( info.ToRawPtr() );

    GN_UNGUARD;
}


#if GN_MSWIN

#if GN_MSVC
#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glu32.lib" )
#endif

void createOGL( HDC hdc, int pfdIndex )
{
    // Set the pixel format for the device context
    PIXELFORMATDESCRIPTOR pfd;
    GN_MSW_CHECK_RETURN_VOID( SetPixelFormat( hdc, pfdIndex, &pfd ) );

    // create OGL render context
    HGLRC hrc;
    GN_MSW_CHECK_RETURN_VOID( hrc = ::wglCreateContext( hdc ) );
    ::wglMakeCurrent( hdc, hrc);

    // init GLEW
    glewInit();

    // print OGL info
    printOglInfo( hdc, pfdIndex );

    // destroy OGL render context
    ::wglMakeCurrent(0, 0);
    ::wglDeleteContext( hrc );
}

void NewWindow( int pfdIndex )
{
    GN::AutoObjPtr<GN::win::Window> oglWindow( GN::win::NewWindow( GN::win::WCP_WINDOWED_RENDER_WINDOW ) );
    if( !oglWindow ) return;

    HWND hwnd = (HWND)oglWindow->GetWindowHandle();
    HDC hdc;
    GN_MSW_CHECK_RETURN_VOID( hdc = ::GetDC(hwnd) );
    createOGL( hdc, pfdIndex );
    ::ReleaseDC( hwnd, hdc );
}

int main()
{
    GN::win::Window * mainWindow = GN::win::NewWindow( GN::win::WCP_WINDOWED_RENDER_WINDOW );
    if( 0 == mainWindow ) return -1;

    HWND hwnd = (HWND)mainWindow->GetWindowHandle();
    HDC hdc;
    GN_MSW_CHECK_RETURN( hdc = ::GetDC(hwnd), -1 );

    GN_INFO(sLogger)( "Enumerating pixelformats..." );
    int count = DescribePixelFormat(hdc, 1, 0, 0);
    GN_INFO(sLogger)( "%d pixelformats in total.", count );

    ::ReleaseDC( hwnd, hdc );

    for( int i = 1; i <= count; ++i )
    {
        NewWindow( i );
    }

    // success
    return 0;
}

#else

int main()
{
    GN_INFO(sLogger)( "Not implemented on platform other than MSWIN." );
    return 0;
}

#endif
