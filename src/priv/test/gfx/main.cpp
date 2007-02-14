#include "pch.h"
#include "garnet/gfx/effect.h"
#include "garnet/gfx/renderable.h"

using namespace GN;
using namespace GN::gfx;
using namespace GN::app;
using namespace GN::util;

bool gAnimation = true;

class Scene
{
    SampleApp & app;
    
    AutoRef<Shader> ps1, ps2;

    UInt32 tex0;

    UInt32 eff0;

    UInt32 box;

    Matrix44f world, view, proj;

    ArcBall arcball;

public:

    Scene( SampleApp & a ) : app(a) {}

    ~Scene() { quit(); }

    bool init()
    {
        Renderer & r = gRenderer;

        // create pixel shaders
        if( r.supportShader( "ps_1_1" ) )
        {
            static const char * code =
                "ps_1_1\n"
                "mov r0, c0";
            ps1.attach( r.createPS( LANG_D3D_ASM, code ) );
            if( !ps1 ) return false;
            ps1->setUniformByNameV( "c0", Vector4f(0,1,0,1) );
        }
        else if( r.supportShader( "arbfp1" ) )
        {
            static const char * code =
                "!!ARBfp1.0 \n"
                "MOV result.color, program.local[0]; \n"
                "END";
            ps1.attach( r.createPS( LANG_OGL_ARB, code ) );
            if( !ps1 ) return false;
            ps1->setUniformByNameV( "l0", Vector4f(0,1,0,1) );
        }
        if( r.supportShader( "ps_1_1" ) )
        {
            static const char * code =
                "float4 diffuse; \n"
                "float4 psMain() : COLOR0 \n"
                "{ \n"
                "   return diffuse; \n"
                "} \n";
            ps2.attach( r.createPS( LANG_D3D_HLSL, code, "entry=psMain sm30=false" ) );
            if( !ps2 ) return false;
            ps2->setUniformByNameV( "diffuse", Vector4f(1,0,0,1) );
        }
        else if( r.supportShader( "glslps" ) )
        {
            static const char * code =
                "uniform vec4 diffuse; \n"
                "void main() \n"
                "{ \n"
                "   gl_FragColor = diffuse; \n"
                "} \n";
            ps2.attach( r.createPS( LANG_OGL_GLSL, code ) );
            if( !ps2 ) return false;
            ps2->setUniformByNameV( "diffuse", Vector4f(1,0,0,1) );
        }
        else if( r.supportShader( "arbfp1" ) )
        {
            static const char * code =
                "!!ARBfp1.0 \n"
                "MOV result.color, program.local[0]; \n"
                "END";
            ps2.attach( r.createPS( LANG_OGL_ARB, code ) );
            if( !ps2 ) return false;
            ps2->setUniformByNameV( "l0", Vector4f(1,0,0,1) );
        }

        SampleResourceManager & rm = app.getResMgr();

        // load box
        box = rm.renderables.getResourceHandle( "media::renderable/cube1.xml" );

        // load texture
        tex0 = rm.textures.getResourceHandle( "media::texture/rabit.png" );

        // load effect
        eff0 = rm.effects.getResourceHandle( "media::effect/sprite.xml" );

        // initialize matrices
        world.identity();
        view.lookAtRh( Vector3f(3,3,3), Vector3f(0,0,0), Vector3f(0,1,0) );
        gRenderer.composePerspectiveMatrixRh( proj, 1.0f, 4.0f/3.0f, 1.0f, 10.0f );

        // initialize arcball.
        arcball.setHandness( util::ArcBall::RIGHT_HAND );
        arcball.setViewMatrix( view );
        const DispDesc & dd = gRenderer.getDispDesc();
        arcball.setMouseMoveWindow( 0, 0, (int)dd.width, (int)dd.height );

        // success
        return true;
    }

    void quit()
    {
        ps1.clear();
		ps2.clear();
    }

    void onKeyPress( input::KeyEvent key )
    {
        if( input::KEY_MOUSEBTN_0 == key.code )
        {
            if( key.status.down )
            {
                int x, y;
                gInput.getMousePosition( x, y );
                arcball.onMouseButtonDown( x, y );
            }
            else
            {
                arcball.onMouseButtonUp();
            }
        }
    }

    void onAxisMove( input::Axis, int )
    {
        int x, y;
        gInput.getMousePosition( x, y );
        arcball.onMouseMove( x, y );
    }

    void update()
    {
        // update box matrix
        Renderable * rbox = app.getResMgr().renderables.getResource( box );
        GN_ASSERT( rbox );

        world = arcball.getRotationMatrix();
        Matrix44f pvw = proj * view * world;

        if( !rbox->subsets.empty() )
        {
            rbox->subsets[0].effect->setUniformByName( "pvw", pvw );
        }

        // update color
        static int r = 0; static int rr = 1;
        static int g = 80; static int gg = 1;
        static int b = 160; static int bb = 1;
        r += rr; if( 0 == r || 255 == r ) rr = -rr;
        g += gg; if( 0 == g || 255 == g ) gg = -gg;
        b += bb; if( 0 == b || 255 == b ) bb = -bb;
        GN::gfx::UniformValue u;
        u.setV( GN::Vector4f( r/255.0f, g/255.0f, b/255.0f, 1.0f ) );
        app.getResMgr().effects.getResource(eff0)->setUniformByName( "color", u );
    }

    void draw()
    {
        Renderer & r = gRenderer;

        // quad 1
        r.setTexture( 0, app.getResMgr().textures.getResource(tex0) );
        r.draw2DTexturedQuad( DQ_UPDATE_DEPTH, 0, 0, 0, 0.5, 0.5 );

        // quad 2
        if( ps1 )
        {
            r.setShaders( 0, ps1, 0 );
            r.draw2DTexturedQuad( DQ_USE_CURRENT_PS, 0, 0.5, 0.0, 1.0, 0.5 );
        }

        // quad 3
        if( ps2 )
        {
            r.setShaders( 0, ps2, 0 );
            r.draw2DTexturedQuad( DQ_USE_CURRENT_PS, 0, 0.0, 0.5, 0.5, 1.0 );
        }

        // quad 4
        Effect * eff = app.getResMgr().effects.getResource( eff0 );
        for( size_t i = 0; i < eff->getNumPasses(); ++i )
        {
            eff->passBegin( i );
            eff->commitChanges();
            r.draw2DTexturedQuad( DQ_USE_CURRENT, 0, 0.5, 0.5, 1.0, 1.0 );
            eff->passEnd();
        }

        // draw box
        app.getResMgr().renderables.getResource(box)->draw();

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
                ubyte4ToBGRA32(255,255,0,255),
                world, view, proj );
        }//*/
    }
};

///
/// GFX module test application
///
class GfxTestApp : public SampleApp
{
    Scene * mScene;
public:

    GfxTestApp() : mScene(0) {}

    bool onRendererCreate()
    {
        mScene = new Scene(*this);
        return mScene->init();
    }

    void onRendererDestroy()
    {
        safeDelete(mScene);
    }

    void onKeyPress( input::KeyEvent ke )
    {
        SampleApp::onKeyPress( ke );
        mScene->onKeyPress( ke );
    }

    void onAxisMove( input::Axis a, int d )
    {
        mScene->onAxisMove( a, d );
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
        StrA mousePos;
        int x = 1, y = 1;
        gInput.getMousePosition( x, y );
        mousePos.format( "Mouse: %d, %d", x, y );
        r.drawDebugText( mousePos.cptr(), 0, 100 );//*/
    }
};

int main( int argc, const char * argv[] )
{
    GfxTestApp app;
    return app.run( argc, argv );
}
