#include "pch.h"

using namespace GN;
using namespace GN::gfx;

class RenderToTexture : public GN::app::SampleApp
{
    AutoRef<Texture> mRt0, mRt1;

    uint32_t mTex0, mTex1;

public:

    bool onRendererCreate()
    {
        Renderer & r = gRenderer;
        
        mRt0.attach( r.create2DTexture( 256, 256, 1, FMT_DEFAULT, TEXUSAGE_RENDER_TARGET ) );
        mRt1.attach( r.create2DTexture( 256, 256, 1, FMT_DEFAULT, TEXUSAGE_RENDER_TARGET ) );
        if( mRt0.empty() || mRt1.empty() ) return false;

        mTex0 = gTexDict.getResourceHandle( "texture/rabit.png" );
        mTex1 = gTexDict.getResourceHandle( "texture/earth.jpg" );

        return true;
    }

    void onRendererDestroy()
    {
        mRt0.reset();
        mRt1.reset();
    }

    void onRender()
    {
        Renderer & r = gRenderer;

        // draw to RT0
        r.bindTexture( 0, gTexDict.getResource(mTex0) );
        r.setRenderTarget( 0, mRt0 );
        r.clearScreen();
        r.draw2DQuad( 0 );

        // draw to RT1
        r.bindTexture( 0, gTexDict.getResource(mTex1) );
        r.setRenderTarget( 0, mRt1 );
        r.clearScreen();
        r.draw2DQuad( 0 );

        // draw 2 RTs to screen
        r.setRenderTarget( 0, 0 );
        r.clearScreen();
        r.bindTexture( 0, mRt0 );
        r.draw2DQuad( 0, 0.0, 0.0, 0.5, 1.0 );
        r.bindTexture( 0, mRt1 );
        r.draw2DQuad( 0, 0.5, 0.0, 1.0, 1.0 );//*/
    }
};

int main( int argc, const char * argv[] )
{
    RenderToTexture app;
    return app.run( argc, argv );
}
