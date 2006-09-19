#include "pch.h"

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

    bool onRendererCreate()
    {
        GN_OGL_CHECK( glClearColor( 1, 1, 1, 1 ) );
        return true;
    }

    void onRendererDestroy()
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
