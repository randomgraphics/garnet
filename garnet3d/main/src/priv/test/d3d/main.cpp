#include "pch.h"
#include "garnet/GNd3d.h"

bool init()
{
    GN::d3d::gTexMgr.getResource( "haha.bmp" );
    return true;
}

void render()
{
    gD3D.getDevice()->Clear( 0, 0, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0, 1.0f, 0 );
}

int main()
{
    GN::d3d::App myApp;
    myApp.setDevCreateFunc( &init );
    myApp.setRenderFunc( &render );
    return myApp.run();
}
