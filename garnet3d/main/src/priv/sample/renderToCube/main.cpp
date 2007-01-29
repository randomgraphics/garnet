#include "pch.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::app;

class RenderToTexture : public GN::app::SampleApp
{
    struct Vertex
    {
        float p[3];
        float n[3];
        float t[2];
    };

    UInt32 mFaces1[6], mFaces2[6];

    AutoRef<Texture> mCube;

    RendererContext mDrawToCube;
    RendererContext mDrawCubeToScreen;

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
    }

    bool onRendererCreate()
    {
        Renderer & r = gRenderer;
        SampleResourceManager & rm = getResMgr();

        // create cube map
        mCube.attach( r.createCubeTexture( 256, 1, FMT_DEFAULT, TEXUSAGE_RENDER_TARGET ) );

        // load 2D faces
        char * name = "texture/cube1/1.bmp";
        for( int i = 0; i < 6; ++i )
        {
            name[12] = '1' + i;
            mFaces1[i] = rm.textures.getResourceHandle( name );
        }
        name = "textures/cube2/a.bmp";
        for( int i = 0; i < 6; ++i )
        {
            name[12] = 'a' + i;
            mFaces2[i] = rm.textures.getResourceHandle( name );
        }

        // initialize renderer context
        mDrawToCube.resetToDefault();
        mDrawToCube.flags.renderTargets = 1;
        mDrawToCube.renderTargets.count = 1;
        mDrawToCube.renderTargets.aa = 0;
        mDrawToCube.renderTargets.setcbuf( 0, mCube );

        mDrawCubeToScreen.resetToDefault();

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
        SampleResourceManager & rm = getResMgr();

        // draw to cube
        mDrawToCube.setTexture( 0, faces[i] );
        for( int i = 0; i < 6; ++i )
        {
            mDrawToCube.renderTargets.cbuffers[0].face = i;
            r.setContext( mDrawToCube );
            r.draw2DTexturedQuad( 0 );
        }

        // draw cube to screen
        mDrawCubeToScreen.setTexture( 0, rm.textures.getResource( mCube ) );
        r.setContext( mDrawCubeToScreen );
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
