#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::input;
using namespace GN::win;

GpuProgram * gp = 0;
VtxBuf     * vb = 0;
IdxBuf     * ib = 0;

const char * vscode =
    "uniform mat4x4 transform; \n"
    "void main() { \n"
    "}";

const char * pscode =
    "void main() { \n"
    "}";

bool init( Renderer & )
{
    GpuProgramDesc gpd;
    gpd.codeVS.lang = GPL_GLSL;
    gpd.codeVS.code = vscode;
    gpd.codePS.lang = GPL_GLSL;
    gpd.codePS.code = pscode;
    return true;
}

void quit( Renderer & )
{
}

void draw( Renderer & )
{
}

int run( Renderer & rndr )
{
    if( !init( rndr ) ) { quit( rndr ); return -1; }

    WindowHandle win = rndr.getDispDesc().windowHandle;

    bool again_again_again = true;

    while( again_again_again )
    {
        processWindowMessages( win, false );

        Input & in = gInput;

        in.processInputEvents();

        if( in.getKeyStatus( KEY_ESCAPE ).down )
        {
            again_again_again = false;
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
