#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::app;
using namespace GN::scene;

class RenderToTexture : public GN::app::SampleApp
{
    struct Vertex
    {
        float p[3];
        float n[3];
        float t[2];
    };

    ResourceId mFaces1[6], mFaces2[6];

    AutoRef<Texture> mCube;

    Matrix44f world, view, proj, pvw;

    util::ArcBall arcball;

    Vertex vb[24];
    UInt16 ib[36];

public:

    bool onAppInit()
    {
        static const float E = 150.0f;
        createBox(
            E, E, E,
            vb[0].p, sizeof(Vertex),
            vb[0].t, sizeof(Vertex),
            vb[0].n, sizeof(Vertex),
            ib, 0 );

        world.identity();
        view.translate( 0, 0, -300 );
        proj.perspectiveD3DRh( 1.0f, 4.0f/3.0f, 10.0f, 1000.0f );
        arcball.setHandness( util::ArcBall::RIGHT_HAND );
        arcball.setViewMatrix( view );

        return true;
    }

    bool onRendererCreate()
    {
        Renderer & r = gRenderer;
        ResourceManager & rm = gSceneResMgr;

        // create cube map
        mCube.attach( r.createCubeTexture( 256, 1, FMT_DEFAULT, TEXUSAGE_RENDER_TARGET ) );

        // load 2D faces
        StrA name( "media::/texture/cube1/1.bmp" );
        for( char i = 0; i < 6; ++i )
        {
            name[22] = '1' + i;
            mFaces1[i] = rm.getResourceId( name );
            if( 0 == mFaces1[i] ) return false;
        }
        name = "media::/texture/cube2/a.bmp";
        for( char i = 0; i < 6; ++i )
        {
            name[22] = 'a' + i;
            mFaces2[i] = rm.getResourceId( name );
            if( 0 == mFaces2[i] ) return false;
        }

        // initial arcball window
        const DispDesc & dd = r.getDispDesc();
        arcball.setMouseMoveWindow( 0, 0, (int)dd.width, (int)dd.height );

        return true;
    }

    void onRendererDestroy()
    {
        mCube.clear();
    }

    void onUpdate()
    {
        world = arcball.getRotationMatrix();
        pvw = proj * view * world;
    }

    void drawCube( const UInt32 * faces )
    {
        Renderer & r = gRenderer;
        ResourceManager & rm = gSceneResMgr;

        // draw to cube
        for( int i = 0; i < 6; ++i )
        {
            r.setTexture( 0, rm.getResourceT<Texture>( faces[i] ) );
            r.setDrawToTextureWithoutDepth( mCube, 0, i );
            r.draw2DTexturedQuad( 0 );
        }

        // draw cube to screen
        r.setDrawToBackBuf();
        r.clearScreen();
        r.drawIndexedUp(
            TRIANGLE_LIST,
            12, // num primitives
            24, // num vertices
            vb,
            sizeof(Vertex),
            ib );
    }

    void onRender()
    {
    }
};

int main( int argc, const char * argv[] )
{
    RenderToTexture app;
    return app.run( argc, argv );
}
