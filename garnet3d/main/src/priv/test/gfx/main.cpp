#include "pch.h"
#if 0
using namespace GN;
using namespace GN::gfx;
using namespace GN::app;
using namespace GN::util;
using namespace GN::scene;

bool gAnimation = true;

class TestScene
{
    SampleApp & app;
    
    UInt32 tex0;

    UInt32 eff0;

    Scene scene;
    Actor box;

    Matrix44f world, view, proj;

    ArcBall arcball;

public:

    TestScene( SampleApp & a ) : app(a), box(scene) {}

    ~TestScene() { quit(); }

    bool init()
    {
        scene::ResourceManager & rm = gSceneResMgr;

        // load box
        if( !loadFromXmlFile( box, "media::cube/cube.actor.xml" ) ) return false;

        // load texture
        tex0 = rm.getResourceId( "media::texture/rabit.png" );
        if( 0 == tex0 ) return false;

        // load effect
        eff0 = rm.getResourceId( "media::effect/sprite.xml" );
        if( 0 == eff0 ) return false;

        // initialize matrices
        float r = box.getBoundingSphere().radius * 1.0f;
        world.identity();
        view.lookAtRh( Vector3f(r,r,r), Vector3f(0,0,0), Vector3f(0,1,0) );
        gRenderer.composePerspectiveMatrixRh( proj, 1.0f, 4.0f/3.0f, r/100.0f, r*10.0f );

        // initialize arcball.
        arcball.setHandness( util::RIGHT_HAND );
        arcball.setViewMatrix( view );
        const DispDesc & dd = gRenderer.getDispDesc();
        arcball.setMouseMoveWindow( 0, 0, (int)dd.width, (int)dd.height );
        arcball.connectToInput();

        // initialize scene
        scene.setProj( proj );
        scene.setView( view );

        // success
        return true;
    }

    void quit()
    {
    }

    void update()
    {
        scene::ResourceManager & rm = gSceneResMgr;

        // update box rotation
        box.setRotation( arcball.getRotation() );

        // update color
        static int r = 0; static int rr = 1;
        static int g = 80; static int gg = 1;
        static int b = 160; static int bb = 1;
        r += rr; if( 0 == r || 255 == r ) rr = -rr;
        g += gg; if( 0 == g || 255 == g ) gg = -gg;
        b += bb; if( 0 == b || 255 == b ) bb = -bb;
        GN::gfx::UniformValue u;
        u.setV( GN::Vector4f( r/255.0f, g/255.0f, b/255.0f, 1.0f ) );
        rm.getResourceT<Effect>(eff0)->setUniformByName( "color", u );
    }

    void draw()
    {
        Renderer & r = gRenderer;
        scene::ResourceManager & rm = gSceneResMgr;

        // quad 1
        gQuadRenderer.drawBegin( rm.getResourceT<Texture>(tex0), QuadRenderer::OPT_DEPTH_WRITE );
        gQuadRenderer.drawTextured( 0, 0, 0, 0.5f, 0.5f );
        gQuadRenderer.drawEnd();

        // quad 2,3
        gQuadRenderer.drawSolidBegin();
        gQuadRenderer.drawSolid( GN_RGBA32(0,255,0,255), 0, 0.5, 0.0, 1.0, 0.5 );
        gQuadRenderer.drawSolid( GN_RGBA32(255,0,0,255), 0, 0.0, 0.5, 0.5, 1.0 );
        gQuadRenderer.drawEnd();

        // quad 4
        RendererContext ctx;
        ctx.clearToNull();
        Effect * eff = rm.getResourceT<Effect>( eff0 );
        for( size_t i = 0; i < eff->getNumPasses(); ++i )
        {
            eff->passBegin( ctx, i );
            eff->commitChanges();
            r.setContext( ctx );
            gQuadRenderer.drawSingleTexturedQuad( 0, QuadRenderer::OPT_USER_CONTEXT, 0, 0.5, 0.5, 1.0, 1.0 );
            eff->passEnd();
        }

        // draw box
        box.draw();

        /* a wireframe box
        {
            static const float E = 80.0f;
            static float lines[] =
            {
                -E, -E, -E,  E, -E, -E,
                 E, -E, -E,  E,  E, -E,
                 E,  E, -E, -E,  E, -E,
                -E,  E, -E, -E, -E, -E,

				-E, -E, -E, -E, -E,  E,
                 E, -E, -E,  E, -E,  E,
                 E,  E, -E,  E,  E,  E,
                -E,  E, -E, -E,  E,  E,

                -E, -E,  E,  E, -E,  E,
                 E, -E,  E,  E,  E,  E,
                 E,  E,  E, -E,  E,  E,
                -E,  E,  E, -E, -E,  E,
			};
            static size_t count = sizeof(lines)/sizeof(float)/6;
            
            r.drawLines(
                0,
                lines,
                sizeof(float)*3,
                count,
                GN_RGBA(255,255,0,255),
                world, view, proj );
        }//*/
    }
};

///
/// GFX module test application
///
class GfxTestApp : public SampleApp
{
    TestScene * mScene;
public:

    GfxTestApp() : mScene(0) {}

    bool onRendererCreate()
    {
        mScene = new TestScene(*this);
        return mScene->init();
    }

    void onRendererDestroy()
    {
        safeDelete(mScene);
    }

    void onUpdate()
    {
        if( gAnimation ) mScene->update();
    }

    void onRender()
    {
        Renderer & r = gRenderer;
        
        r.clearScreen( Vector4f(0,0,0,1) ); // clear to pure black

        // draw scene
        GN_ASSERT( mScene );
        mScene->draw();

        // draw mouse position on screen
        StrW mousePos;
        int x = 1, y = 1;
        gInput.getMousePosition( x, y );
        mousePos.format( L"Mouse Position : %d, %d", x, y );
        getFontRenderer().drawText( mousePos.cptr(), 0, 100 );
    }
};

int main( int argc, const char * argv[] )
{
    GfxTestApp app;
    return app.run( argc, argv );
}

#else

#include "garnet/GNengine.h"
using namespace GN;
using namespace GN::input;
using namespace GN::gfx;
using namespace GN::engine;

bool init( RenderEngine & )
{
    return true;
}

void quit()
{
}

void run( RenderEngine & engine )
{
    __try
    {
        while( 1 )
        {
            gInput.processInputEvents();

            if( gInput.getKeyStatus( KEY_ESCAPE ).down )
            {
                break;
            }
            
            engine.frameBegin();

            engine.clearScreen( Vector4f(0,0,1,0) );

            engine.frameEnd();
        }
    }
    __finally
    {
    }
}

struct InputInitiator
{
    InputInitiator()
    {
        createInputSystem( API_NATIVE );
    }

    ~InputInitiator()
    {
        if( gInputPtr ) delete gInputPtr;
    }
};

int main()
{
    InputInitiator input;

    RenderEngine engine;

    UInt32 MB = 1024 * 1024;
    RenderEngineInitParameters reip = { 32*MB, 32*MB, 4*MB };

    RendererOptions ro;

    if( !engine.init(reip) ) return -1;

    if( !engine.resetRenderer( API_D3D9, ro ) ) return -1;

    run( engine );

    

    // success
    return 0;
}

#endif
