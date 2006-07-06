#include "pch.h"
#include "garnet/gfx/fatMesh.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::input;

class MeshViewerApp : public app::SampleApp
{
    StrA    mMeshName;
    FatMesh mFatMesh;

public:

    bool onCheckCmdLine( int argc, const char * const argv[] )
    {
        for( int i = 1; i < argc; ++i )
        {
            const char * a = argv[i];
            if( '-' != *a )
            {
                mMeshName = app::SampleResourceManager::sSearchResourceFile( a );
                if( mMeshName.empty() ) { GN_ERROR( "mesh file '%s' not found.", a ); return false; }
                break;
            }
            else GN_WARN( "unknown command line argument: %s", a );
        }
        return true;
    }

    bool onAppInit()
    {
        DiskFile fp;
        if( !fp.open( mMeshName, "rb" ) ) return false;
        if( !mFatMesh.readFrom( fp ) ) return false;
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
        mFatMesh.draw( 0 );
    }
};

int main( int argc, const char * argv[] )
{
    MeshViewerApp app;
    return app.run( argc, argv );
}
