#include "pch.h"

#if GN_MSVC

#if GN_XENON
#if GN_DEBUG_BUILD
#pragma comment(lib, "d3d9d.lib")
#pragma comment( lib, "xgraphicsd.lib" )
#pragma comment(lib, "xapilibd.lib")
#else
#pragma comment(lib, "d3d9.lib")
#pragma comment( lib, "xgraphics.lib" )
#pragma comment(lib, "xapilib.lib")
#endif
#else
#if GN_DEBUG_BUILD
#pragma comment( lib, "d3dx9d.lib" )
#else
#pragma comment( lib, "d3dx9.lib" )
#endif
#endif

#endif

using namespace GN;
using namespace GN::gfx;
using namespace GN::app;

class MyApp : public SampleApp
{
    AutoComPtr<IDirect3DStateBlock9> rsb;
public:

    void onDetermineInitParam( InitParam & ip )
    {
        ip.rapi = API_D3D9;
    }

    bool onRendererRestore()
    {
        LPDIRECT3DDEVICE9 dev = (LPDIRECT3DDEVICE9)gRenderer.getD3DDevice();
        GN_DX9_CHECK( dev->CreateStateBlock( D3DSBT_ALL, &rsb ) );
        return true;
    }

    void onRendererDispose()
    {
        rsb.clear();
    }

    void onUpdate()
    {
    }

    void onRender()
    {
        LPDIRECT3DDEVICE9 dev = (LPDIRECT3DDEVICE9)gRenderer.getD3DDevice();
        dev->Clear( 0, 0, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, 0, 1.0f, 0 );
    }
};

int main( int argc, const char * argv[] )
{
    MyApp app;
    return app.run( argc, argv );
}
