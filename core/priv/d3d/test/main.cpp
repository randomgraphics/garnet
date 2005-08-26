#include "pch.h"
#include "garnet/GNd3d.h"

bool init()
{
    GN::d3d::gTexMgr.getResource( "haha.bmp" );
    return true;
}

void quit()
{
}

void render()
{
    gD3D.present();
}

int main()
{
    //
    // 如果是Debug版本，则启用CRT的内存泄漏检测功能
    //
#if GN_DEBUG
    int tmpDbgFlag;
    tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
    _CrtSetDbgFlag(tmpDbgFlag);
#endif

    GN::d3d::D3DInitParams d3dip;
    GN::d3d::D3D d3d;
    if( !d3d.init(d3dip) ) return -1;

    if( !init() ) { quit(); return -1; }

    // run
    while( !d3d.windowClosed() )
    {
        render();
    }

    quit();

    return 0;
}
