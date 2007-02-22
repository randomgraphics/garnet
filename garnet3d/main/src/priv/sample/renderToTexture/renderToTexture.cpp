#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::app;
using namespace GN::scene;

class RenderToTexture : public GN::app::SampleApp
{
    AutoRef<Texture> mRt0, mRt1;

    ResourceId mTex0, mTex1;

public:

    bool onRendererCreate()
    {
        Renderer & r = gRenderer;
        ResourceManager & rm = gSceneResMgr;

        mRt0.attach( r.create2DTexture( 256, 256, 1, FMT_DEFAULT, TEXUSAGE_RENDER_TARGET ) );
        mRt1.attach( r.create2DTexture( 256, 256, 1, FMT_DEFAULT, TEXUSAGE_RENDER_TARGET ) );
        if( mRt0.empty() || mRt1.empty() ) return false;

        mTex0 = rm.getResourceId( "media::texture/rabit.png" );
        mTex1 = rm.getResourceId( "media::texture/earth.jpg" );
        if( 0 == mTex0 || 0 == mTex1 ) return false;

        return true;
    }

    void onRendererDestroy()
    {
        mRt0.clear();
        mRt1.clear();
    }

    void onRender()
    {
        Renderer & r = gRenderer;
        ResourceManager & rm = gSceneResMgr;

        // draw to RT0
        r.setTexture( 0, rm.getResourceT<Texture>( mTex0 ) );
        r.setDrawToTextures( 1, mRt0 );
        r.clearScreen();
        r.draw2DTexturedQuad( 0 );

        // draw to RT1
        r.setTexture( 0, rm.getResourceT<Texture>( mTex1 ) );
        r.setDrawToTextures( 1, mRt1 );
        r.clearScreen();
        r.draw2DTexturedQuad( 0 );

        // draw 2 RTs to screen
        r.setDrawToBackBuf();
        r.clearScreen();
        r.setTexture( 0, mRt0 );
        r.draw2DTexturedQuad( 0, 0, 0.0, 0.0, 0.5, 1.0 );
        r.setTexture( 0, mRt1 );
        r.draw2DTexturedQuad( 0, 0, 0.5, 0.0, 1.0, 1.0 );
    }
};

int main( int argc, const char * argv[] )
{
    RenderToTexture app;
    return app.run( argc, argv );
}
