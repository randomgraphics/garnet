#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::util;

class RenderToTexture : public SampleApp
{
    AutoRef<TextureResource> faces[6];

    AutoRef<Texture> cubemap;

    AutoRef<ModelResource> box;

    Matrix44f world, view, proj, pvw;

    ArcBall arcball;

public:

    bool onInit()
    {
        world.identity();
        view.translate( 0, 0, -200 );
        proj.perspectiveD3DRh( 1.0f, 4.0f/3.0f, 100.0f, 1000.0f );
        arcball.setHandness( util::RIGHT_HAND );
        arcball.setViewMatrix( view );
        arcball.connectToInput();

        RenderEngine & re = getRenderEngine();

        // load 2D faces
        StrA name = "media::/texture/cube2/a.bmp";
        for( unsigned char i = 0; i < 6; ++i )
        {
            name[22] = 'a' + i;
            faces[i] = loadTextureFromFile( re, name );
            if( 0 == faces[i] ) return false;
        }

        // create cube render target
        cubemap = re.createCubeRenderTargetTexture( "cube", 512, 512 );
        if( 0 == cubemap ) return false;

        // load box
        if( !box.loadFromXmlFile( getEntityManager(), re, "media::cube/cube_on_cube.drawable.xml" ) ) return false;
        box.textures["cube"].texture = cubemap;

        // initial arcball window
        const DispDesc & dd = re.getDispDesc();
        arcball.setMouseMoveWindow( 0, 0, (int)dd.width, (int)dd.height );

        // setup context
        ctx.resetToDefault();

        return true;
    }

    void onQuit()
    {
    }

    void onUpdate()
    {
        world = arcball.getRotationMatrix44();
        pvw = proj * view * world;
        box.uniforms["pvw"].value = pvw;
    }

    void onRender()
    {
        RenderEngine & re = getRenderEngine();
        QuadRenderer & qr = getQuadRenderer();

        // draw to cube
        for( int i = 0; i < 6; ++i )
        {
            ctx.setDrawToTextureWithoutDepth( cubemap, 0, i );
            re.setContext( ctx );
            qr.drawSingleTexturedQuad( faces[i], 0 );
        }

        // draw cube to screen
        ctx.setDrawToBackBuf();
        re.setContext( ctx );
        re.clearScreen();
        box.draw();
    }
};

int main( int argc, const char * argv[] )
{
    RenderToTexture app;
    return app.run( argc, argv );
}
