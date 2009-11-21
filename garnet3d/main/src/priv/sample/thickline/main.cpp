#include "pch.h"
#include "garnet/GNd3d9.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::util;
using namespace GN::d3d9;

static GN::Logger * sLogger = GN::getLogger("GN.sample.thickline");

class ThickLineDemo : public D3D9Application
{
public:

    ThickLineDemo()
    {
    }

    bool onCreate()
    {
        return true;
    }

    bool onRestore()
    {
        return true;
    }

    void onDispose()
    {
    }

    void onDestroy()
    {
    }

    void onUpdate()
    {
    }

    void onDraw()
    {
        IDirect3DDevice9 & dev = d3d9dev();

        dev.Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, 0, 1.0f, 0 );

        dev.Present( 0, 0, 0, 0 );
    }
};

int main()
{
    ThickLineDemo app;
    return app.run();
}
