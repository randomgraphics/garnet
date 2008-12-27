#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::input;
using namespace GN::win;

struct Scene
{
    AutoRef<GpuProgram> gp;
    AutoRef<VtxBuf>     vb;
    AutoRef<IdxBuf>     ib;
};
Scene * scene = NULL;

const char * vscode =
    "attrib float4 position; \n"
    "uniform mat4x4 transform; \n"
    "void main() { \n"
    "   gl_Vertex = mul( transform, position ); \n"
    "}";

const char * pscode =
    "void main() { \n"
    "   gl_Color = float4(1,1,1,1); \n"
    "}";

bool init( Renderer & rndr )
{
    scene = new Scene;

    GpuProgramDesc gpd;
    gpd.vs.lang = GPL_GLSL;
    gpd.vs.code = vscode;
    gpd.ps.lang = GPL_GLSL;
    gpd.ps.code = pscode;
    scene->gp.attach( rndr.createGpuProgram( gpd ) );
    if( !scene->gp ) return false;

    return true;
}

void quit( Renderer & )
{
    safeDelete( scene );
}

void draw( Renderer & )
{
}

int run( Renderer & rndr )
{
    if( !init( rndr ) ) { quit( rndr ); return -1; }

    WindowHandle win = rndr.getDispDesc().windowHandle;

    bool gogogo = true;

    while( gogogo )
    {
        processWindowMessages( win, false );

        Input & in = gInput;

        in.processInputEvents();

        if( in.getKeyStatus( KEY_ESCAPE ).down )
        {
            gogogo = false;
        }

        rndr.clearScreen( Vector4f(0,0.5f,0.5f,1.0f) );
        draw( rndr );
        rndr.present();
    }

    quit( rndr );

    return 0;
}

struct InputInitiator
{
    InputInitiator( Renderer & r )
    {
        initializeInputSystem( API_NATIVE );
        const DispDesc & dd = r.getDispDesc();
        gInput.attachToWindow( dd.displayHandle, dd.windowHandle );
    }

    ~InputInitiator()
    {
        shutdownInputSystem();
    }
};

int main( int, const char *[] )
{
    enableCRTMemoryCheck();

    RendererOptions o;
    o.api = API_OGL;

    Renderer * r = createRenderer( o );
    if( NULL == r ) return -1;

    InputInitiator ii(*r);

    int result = run( *r );

    deleteRenderer( r );

    return result;
}
