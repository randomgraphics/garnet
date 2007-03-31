#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::scene;
using namespace GN::input;

static GN::Logger * sLogger = GN::getLogger("GN.gfx.tool.meshViewer");

class MeshViewerApp : public app::SampleApp
{
    StrA mObjName;

    Drawable mObj;

public:

    bool onCheckCmdLine( int argc, const char * const argv[] )
    {
        for( int i = 1; i < argc; ++i )
        {
            const char * a = argv[i];
            if( '-' != *a )
            {
                mObjName = a;
                break;
            }
            else GN_WARN(sLogger)( "unknown command line argument: %s", a );
        }
        return true;
    }

    bool onAppInit()
    {
        return mObj.loadFromXmlFile( mObjName );
    }

    bool onRendererRestore()
    {
        return true;
    }

    void onKeyPress( input::KeyEvent ke )
    {
        GN_GUARD_SLOW;

        app::SampleApp::onKeyPress( ke );

        GN_UNGUARD_SLOW;
    }

    //void onAxisMove( input::Axis a, int d )
    //{
    //    GN_GUARD_SLOW;
    //
    //    GN_UNGUARD_SLOW;
    //}

    void onUpdate()
    {
    }

    void onRender()
    {
        Renderer & r = gRenderer;
        r.clearScreen();

        mObj.draw();
    }
};

int main( int argc, const char * argv[] )
{
    MeshViewerApp app;
    return app.run( argc, argv );
}
