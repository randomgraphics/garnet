#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::engine;
using namespace GN::app;
using namespace GN::scene;



class RenderToTexture : public SampleApp
{
    AutoGraphicsResource mRt0, mRt1, mTex0, mTex1;

    DrawContext ctx0, ctx1, ctx2;

public:

    bool onInit()
    {
        RenderEngine & re = getRenderEngine();

        mRt0.attach( re.create2DRenderTargetTexture( "RenderToTexture::rt0", 256, 256, 1, FMT_RGBA32 ) );
        mRt1.attach( re.create2DRenderTargetTexture( "RenderToTexture::rt1", 256, 256, 1, FMT_RGBA32 ) );
        if( mRt0.empty() || mRt1.empty() ) return false;

        mTex0.attach( loadTextureFromFile( re, "media::texture/rabit.png" ) );
        mTex1.attach( loadTextureFromFile( re, "media::texture/earth.jpg" ) );
        if( 0 == mTex0 || 0 == mTex1 ) return false;

        // draw to rt0
        ctx0.resetToDefault();
        ctx0.setDrawToTextures( 1, mRt0 );

        // draw to rt1
        ctx1.clearToNull();
        ctx1.setDrawToTextures( 1, mRt1 );

        // draw to backbuf
        ctx2.clearToNull();
        ctx2.setDrawToBackBuf();

        return true;
    }

    void onQuit()
    {
        mRt0.clear();
        mRt1.clear();
        mTex0.clear();
        mTex1.clear();
    }

    void onUpdate()
    {
    }

    void onRender()
    {
        RenderEngine & re = getRenderEngine();
        QuadRenderer & qr = getQuadRenderer();

        // draw to RT0
        re.setContext( ctx0 );
        re.clearScreen();
        qr.drawSingleTexturedQuad( mTex0, 0 );

        // draw to RT1
        re.setContext( ctx1 );
        re.clearScreen();
        qr.drawSingleTexturedQuad( mTex1, 0 );

        // draw 2 RTs to screen
        re.setContext( ctx2 );
        re.clearScreen();
        qr.drawSingleTexturedQuad( mRt0, 0, 0, 0.0, 0.0, 0.5, 1.0 );
        qr.drawSingleTexturedQuad( mRt1, 0, 0, 0.5, 0.0, 1.0, 1.0 );
    }
};

int main( int argc, const char * argv[] )
{
    RenderToTexture app;
    return app.run( argc, argv );
}
