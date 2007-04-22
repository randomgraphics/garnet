#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::app;
using namespace GN::scene;

class RenderToTexture : public GN::app::SampleApp
{
    ResourceId faces[6];

    ResourceId cubemap;

    Drawable box;

    Matrix44f world, view, proj, pvw;

    util::ArcBall arcball;

public:

    bool onAppInit()
    {
        world.identity();
        view.translate( 0, 0, -200 );
        proj.perspectiveD3DRh( 1.0f, 4.0f/3.0f, 100.0f, 1000.0f );
        arcball.setHandness( util::RIGHT_HAND );
        arcball.setViewMatrix( view );
        arcball.connectToInput();
        return true;
    }

    bool onRendererCreate()
    {
        Renderer & r = gRenderer;
        ResourceManager & rm = gSceneResMgr;

        // load 2D faces
        StrA name = "media::/texture/cube2/a.bmp";
        for( unsigned char i = 0; i < 6; ++i )
        {
            name[22] = 'a' + i;
            faces[i] = rm.getResourceId( name );
            if( 0 == faces[i] ) return false;
        }

        // load cubemap
        cubemap = rm.getResourceId( "media::texture/cube_512_rendertarget.texture.xml" );
        if( 0 == cubemap ) return false;

        // load box
        if( !loadFromXmlFile( box, "media::cube/cube_on_cube.drawable.xml" ) ) return false;
        box.textures["cube"].texid = cubemap;

        // initial arcball window
        const DispDesc & dd = r.getDispDesc();
        arcball.setMouseMoveWindow( 0, 0, (int)dd.width, (int)dd.height );

        return true;
    }

    void onRendererDestroy()
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
        Renderer & r = gRenderer;
        ResourceManager & rm = gSceneResMgr;

        // draw to cube
        Texture * rt = rm.getResourceT<Texture>( cubemap );
        for( int i = 0; i < 6; ++i )
        {
            r.setDrawToTextureWithoutDepth( rt, 0, i );
            gQuadRenderer.drawSingleTexturedQuad( rm.getResourceT<Texture>( faces[i] ), 0 );
        }

        // draw cube to screen
        r.setDrawToBackBuf();
        r.clearScreen();
        box.draw();
    }
};

int main( int argc, const char * argv[] )
{
    RenderToTexture app;
    return app.run( argc, argv );
}
