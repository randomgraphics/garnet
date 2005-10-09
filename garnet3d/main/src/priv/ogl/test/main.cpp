#include "pch.h"
#include "garnet/GNogl.h"

bool init()
{
    glClearColor( 0.0f, 0.0f, 1.0f, 1.0f );
    return true;
}

void quit()
{
}

void render()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    gOGL.present();
}

int main()
{
    GN_GUARD_ALWAYS;

    //
    // 如果是Debug版本，则启用CRT的内存泄漏检测功能
    //
#if GN_MSVC && GN_DEBUG
    int tmpDbgFlag;
    tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
    _CrtSetDbgFlag(tmpDbgFlag);
#endif

    GN::ogl::OGLInitParams ip;

    GN::ogl::OGL ogl;
    if( !ogl.init(ip) ) return -1;

    if( !init() ) return -1;

    while( !ogl.windowClosed() )
    {
        render();
    }

    quit();

    // success
    return 0;

    // failed
    GN_UNGUARD_ALWAYS_NO_THROW;
    return -1;
}
