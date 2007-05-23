#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::engine;
using namespace GN::app;

class MyMiniApp : public MiniApp
{
public:

    bool onInit()
    {
        return true;
    }

    bool onRendererCreate()
    {
        GN_OGL_CHECK( glClearColor( 0, 0, 1, 1 ) );
        return true;
    }

    bool onRendererRestore()
    {
        return true;
    }

    void onRendererDispose()
    {
    }

    void onRendererDestroy()
    {
    }

    void onQuit()
    {
    }

    void onFrame()
    {
        GN_OGL_CHECK( glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT ) );
    }
};

class MyApp : public SampleApp
{
    MiniAppId app;

public:

    void onDetermineInitParam( InitParam & ip )
    {
        ip.rapi = API_OGL;
    }

    bool onInit()
    {
        app = getRenderEngine().registerMiniApp( new MyMiniApp );
        return 0 != app;
    }

    void onQuit()
    {
        delete getRenderEngine().unregisterMiniApp( app );
    }

    void onUpdate()
    {
    }

    void onRender()
    {
        getRenderEngine().runMiniApp( app );
    }
};

int main( int argc, const char * argv[] )
{
    MyApp app;
    return app.run( argc, argv );
}
