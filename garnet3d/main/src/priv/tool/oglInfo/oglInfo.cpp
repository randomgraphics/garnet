#include "pch.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.tool.oglInfo");

///
/// Split a string into token list
// ------------------------------------------------------------------------
static void
sGetTokens( GN::DynaArray<GN::StrA> & tokens, const char * str )
{
    if( GN::stringEmpty(str) ) return;
    const char * p1 = str;
    const char * p2 = p1;

    while( *p1 )
    {
        while( *p2 && *p2 != ' ' ) ++p2;

        tokens.append( GN::StrA(p1, p2-p1) );

        while( *p2 && *p2 == ' ' ) ++p2;

        p1 = p2;
    }
}

///
/// initialize opengl extension
// ------------------------------------------------------------------------
#if GN_MSWIN
bool sGetOGLExtensions( HDC hdc, GN::DynaArray<GN::StrA> & result )
#else
bool sGetOGLExtensions( Display * disp, GN::DynaArray<GN::StrA> & result )
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

void printOglInfo( GN::HandleType disp, int index )
{
    GN_GUARD;

    GN::DynaArray<GN::StrA> glexts;
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

    info = GN::stringFormat(
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
    info += GN::stringFormat(
        "---------------------------------------------------\n"
        "    Max size of texture             :    %d\n"
        "    Max number of texture stages    :    %d\n",
        ts,tu );

    // extension info.
    info +=
        "---------------------------------------------------\n";
    for ( size_t i = 0; i < glexts.size(); ++i )
    {
        info += glexts[i] + "\n";
    }
    info +=
        "\n"
        "===================================================\n"
        "\n\n";

    GN_INFO(sLogger)( info.cptr() );

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

void createWindow( int pfdIndex )
{
    GN::AutoObjPtr<GN::win::Window> oglWindow( GN::win::createWindow( GN::win::WCP_WINDOWED_RENDER_WINDOW ) );
    if( !oglWindow ) return;

    HWND hwnd = (HWND)oglWindow->getWindowHandle();
    HDC hdc;
    GN_MSW_CHECK_RETURN_VOID( hdc = ::GetDC(hwnd) );
    createOGL( hdc, pfdIndex );
    ::ReleaseDC( hwnd, hdc );
}

int choosePixelFormat( HDC hdc, int total )
{
    GN_GUARD;

    PIXELFORMATDESCRIPTOR pfd;

    DWORD required_flags = PFD_DRAW_TO_WINDOW |
        PFD_SUPPORT_OPENGL |
        PFD_DOUBLEBUFFER   ;

    // flags that can not exist
    DWORD xxx_flags = PFD_NEED_PALETTE; // we're aiming for a RGB device

    int candidates[4] =
    {
        0, // ICD
        0, // MCD
        0, // ???
        0, // software
    };

    for ( int i = 1; i <= total; i++ )
    {
        if (!DescribePixelFormat(hdc, i, sizeof(pfd), &pfd))
        {
            GN_ERROR(sLogger)( "can't get the description of the %dth pixelformat!", i );
            return 0;
        }

        // check pfd flags;
        if( (pfd.dwFlags & required_flags) != required_flags ) continue;
        if( (pfd.dwFlags & xxx_flags) != 0 ) continue;

        // check pixel type
        if( PFD_TYPE_RGBA != pfd.iPixelType ) continue;

        // check z-buffer
        if( 0 == pfd.cDepthBits ) continue;

        // check stencil buffer
        if( 0 == pfd.cStencilBits ) continue;

        // check acceleration flag
        if( PFD_GENERIC_ACCELERATED & pfd.dwFlags )
        {
            if( PFD_GENERIC_FORMAT & pfd.dwFlags )
            {
                // mixed device
                if( 0 == candidates[2] ) candidates[2] = i;
            }
            else
            {
                // MCD device
                if( 0 == candidates[1] ) candidates[1] = i;
            }
        }
        else if( PFD_GENERIC_FORMAT & pfd.dwFlags )
        {
            // software device
            if( 0 == candidates[3] ) candidates[3] = i;
        }
        else
        {
            // might be ICD device
            if( 0 == candidates[0] ) candidates[0] = i;
        }

        if( candidates[0] > 0 && candidates[1] > 0 && candidates[2] > 0 )
        {
            // no need to iterate more formats
            break;
        }
    }

    // prefer hardware than mixed, than software
    if( candidates[0] > 0 )
    {
        return candidates[0];
    }
    else if( candidates[1] > 0 )
    {
        return candidates[1];
    }
    else if( candidates[2] > 0 )
    {
        return candidates[2];
    }
    else if( candidates[3] > 0 )
    {
        return candidates[3];
    }

    // error
    GN_ERROR(sLogger)( "no appropriate pixelformat!" );
    return 0;

    GN_UNGUARD;
}

int main()
{
    GN::win::Window * mainWindow = GN::win::createWindow( GN::win::WCP_WINDOWED_RENDER_WINDOW );
    if( 0 == mainWindow ) return -1;

    HWND hwnd = (HWND)mainWindow->getWindowHandle();
    HDC hdc;
    GN_MSW_CHECK_RETURN( hdc = ::GetDC(hwnd), -1 );

    GN_INFO(sLogger)( "Enumerating pixelformats..." );
    int count = DescribePixelFormat(hdc, 1, 0, 0);
    GN_INFO(sLogger)( "%d pixelformats in total.", count );

    int bestPixelFormat = choosePixelFormat( hdc, count );

    ::ReleaseDC( hwnd, hdc );

    //for( int i = 1; i <= count; ++i )
    //{
    //    createWindow( i );
    //}
    createWindow( bestPixelFormat );

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
