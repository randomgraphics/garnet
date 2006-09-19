#include "pch.h"

#if GN_MSVC
#if GN_DEBUG_BUILD
#pragma comment( lib, "d3dx10d.lib" )
#else
#pragma comment( lib, "d3dx10.lib" )
#endif
#endif

using namespace GN;
using namespace GN::gfx;
using namespace GN::app;

class MyApp : public SampleApp
{
public:

    void onDetermineInitParam( InitParam & ip )
    {
        ip.rapi = API_D3D10;
        ip.ro.reference = true;
    }

    bool onRendererCreate()
    {
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
        gRenderer.clearScreen( Vector4f(1,0,0,1) );
    }
};

int main( int argc, const char * argv[] )
{
    MyApp app;
    return app.run( argc, argv );
}
