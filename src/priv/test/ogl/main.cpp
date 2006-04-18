#include "pch.h"
#include "glew.h"

#if GN_MSVC
#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glu32.lib" )
#endif

using namespace GN;
using namespace GN::gfx;
using namespace GN::app;

class MyApp : public SampleApp
{
public:

    void onDetermineInitParam( InitParam & ip )
    {
        ip.rapi = API_OGL;
    }

    bool onRendererInit()
    {
        return true;
    }

    void onRendererQuit()
    {
    }

    void onUpdate()
    {
    }

    void onRender()
    {
        GN_OGL_CHECK( glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT ) );
    }
};

int main( int argc, const char * argv[] )
{
    MyApp app;
    return app.run( argc, argv );
}
