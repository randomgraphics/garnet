#include "pch.h"
#include "garnet/gfx/effect.h"

using namespace GN;
using namespace GN::gfx;

class Scene
{
    AutoRef<Shader> ps1, ps2;

    uint32_t tex0;

    effect::Effect eff0;

    Matrix44f world, view, proj;

    bool loadEffect()
    {
        GN_GUARD;

        // create effect 0
        {
            effect::EffectDesc desc;

            // define effect parameters
            desc.uniforms["color"];
            desc.uniforms["pvw"].hasDefaultValue = true;
            desc.uniforms["pvw"].defaultValue.setM( Matrix44f(
                2,  0,  0, -1,
                0, -2,  0,  1,
                0,  0,  1,  0,
                0,  0,  0,  1 ) );

            // set render states
            desc.rsb.rs[RS_CULL_MODE] = RSV_CULL_NONE;
            desc.rsb.rs[RS_DEPTH_WRITE] = RSV_FALSE;

            // init vs0
            desc.shaders["vs.1.1"].type = VERTEX_SHADER;
            desc.shaders["vs.1.1"].lang = LANG_D3D_ASM;
            desc.shaders["vs.1.1"].code =
                "vs.1.1 \n"
                "dcl_position v0 \n"
                "m4x4 oPos, v0, c0";
            desc.shaders["vs.1.1"].uniforms["c0"] = "pvw";

            // init vs1
            desc.shaders["arbvp1"].type = VERTEX_SHADER;
            desc.shaders["arbvp1"].lang = LANG_OGL_ARB;
            desc.shaders["arbvp1"].code =
                "!!ARBvp1.0 \n"
                "PARAM pvw[4] = { state.matrix.program[0] }; \n"
                "DP4 result.position.x, pvw[0], vertex.position; \n"
                "DP4 result.position.y, pvw[1], vertex.position; \n"
                "DP4 result.position.z, pvw[2], vertex.position; \n"
                "DP4 result.position.w, pvw[3], vertex.position; \n"
                "END";
            desc.shaders["arbvp1"].uniforms["m0"] = "pvw";

            // init ps0
            desc.shaders["ps.1.1"].type = PIXEL_SHADER;
            desc.shaders["ps.1.1"].lang = LANG_D3D_ASM;
            desc.shaders["ps.1.1"].code =
                "ps.1.1 \n"
                "mov r0, c0";
            desc.shaders["ps.1.1"].uniforms["c0"] = "color";

            // init ps1
            desc.shaders["arbfp1"].type = PIXEL_SHADER;
            desc.shaders["arbfp1"].lang = LANG_OGL_ARB;
            desc.shaders["arbfp1"].code =
                "!!ARBfp1.0 \n"
                "MOV result.color, program.local[0]; \n"
                "END";
            desc.shaders["arbfp1"].uniforms["l0"] = "color";

            // create tech0
            desc.techniques["t0"].passes.resize(1);
            desc.techniques["t0"].passes[0].shaders[VERTEX_SHADER] = "vs.1.1";
            desc.techniques["t0"].passes[0].shaders[PIXEL_SHADER] = "ps.1.1";

            // create tech1
            desc.techniques["t1"].passes.resize(1);
            desc.techniques["t1"].passes[0].shaders[VERTEX_SHADER] = "arbvp1";
            desc.techniques["t1"].passes[0].shaders[PIXEL_SHADER] = "arbfp1";

            if( !eff0.init( desc ) ) return false;
        }

        // success
        return true;

        GN_UNGUARD;
    }

public:

    Scene() {}

    ~Scene() { quit(); }

    bool init()
    {
        Renderer & r = gRenderer;

        // create pixel shaders
        if( r.supportShader( PIXEL_SHADER, LANG_D3D_ASM ) )
        {
            static const char * code =
                "ps_1_1\n"
                "mov r0, c0";
            ps1.attach( r.createPxlShader( LANG_D3D_ASM, code ) );
            if( !ps1 ) return false;
            ps1->setUniformByNameV( "c0", Vector4f(0,1,0,1) );
        }
        else if( r.supportShader( PIXEL_SHADER, LANG_OGL_ARB ) )
        {
            static const char * code =
                "!!ARBfp1.0 \n"
                "PARAM white = program.local[0]; \n"
                "OUTPUT oClr = result.color; \n"
                "MOV oClr, white; \n"
                "END";
            ps1.attach( r.createPxlShader( LANG_OGL_ARB, code ) );
            if( !ps1 ) return false;
            ps1->setUniformByNameV( "l0", Vector4f(0,1,0,1) );
        }
        if( r.supportShader( PIXEL_SHADER, LANG_D3D_HLSL ) )
        {
            static const char * code =
                "float4 diffuse; \n"
                "float4 psMain() : COLOR0 \n"
                "{ \n"
                "   return diffuse; \n"
                "} \n";
            ps2.attach( r.createPxlShader( LANG_D3D_HLSL, code, "psMain" ) );
            if( !ps2 ) return false;
            ps2->setUniformByNameV( "diffuse", Vector4f(1,0,0,1) );
        }
        else if( r.supportShader( PIXEL_SHADER, LANG_OGL_GLSL ) )
        {
            static const char * code =
                "uniform vec4 diffuse; \n"
                "void main() \n"
                "{ \n"
                "   gl_FragColor = diffuse; \n"
                "} \n";
            ps2.attach( r.createPxlShader( LANG_OGL_GLSL, code ) );
            if( !ps2 ) return false;
            ps2->setUniformByNameV( "diffuse", Vector4f(1,0,0,1) );
        }

        // get texture handle
        tex0 = gTexDict.getResourceHandle( "texture/rabit.png" );

        // create the effect
        if( !loadEffect() ) return false;

        // initialize matrices
        world.identify();
        view.lookAtRh( Vector3f(200,200,200), Vector3f(0,0,0), Vector3f(0,1,0) );
        gRenderer.composePerspectiveMatrix( proj, 1.0f, 4.0f/3.0f, 1.0f, 1000.0f );

        // success
        return true;
    }

    void quit()
    {
        eff0.quit();
        ps1.clear();
		ps2.clear();
    }

    void update()
    {
        // update world matrix
        static float angle = 0.0f;
        angle += deg2rad(0.2f);
        world.rotateY( angle );

        // update color
        static int r = 0; static int rr = 1;
        static int g = 80; static int gg = 1;
        static int b = 160; static int bb = 1;
        r += rr; if( 0 == r || 255 == r ) rr = -rr;
        g += gg; if( 0 == g || 255 == g ) gg = -gg;
        b += bb; if( 0 == b || 255 == b ) bb = -bb;
        GN::gfx::UniformValue u;
        u.setV( GN::Vector4f( r/255.0f, g/255.0f, b/255.0f, 1.0f ) );
        eff0.setUniformByName( "color", u );
    }

    void draw()
    {
        Renderer & r = gRenderer;

        // quad 1
        r.bindTexture( 0, gTexDict.getResource(tex0) );
        r.draw2DTexturedQuad( 0, 0, 0, 0.5, 0.5 );

        // quad 2
        if( ps1 )
        {
            r.bindShaders( 0, ps1 );
            r.draw2DTexturedQuad( DQ_USE_CURRENT_PS, 0.5, 0.0, 1.0, 0.5 );
        }

        // quad 3
        if( ps2 )
        {
            r.bindShaders( 0, ps2 );
            r.draw2DTexturedQuad( DQ_USE_CURRENT_PS, 0.0, 0.5, 0.5, 1.0 );
        }

        // quad 4
        {
            size_t n;
            if( eff0.drawBegin( &n ) )
            {
                for( size_t i = 0; i < n; ++i )
                {
                    eff0.passBegin( i );
                    eff0.commitChanges();
                    r.draw2DTexturedQuad( DQ_USE_CURRENT, 0.5, 0.5, 1.0, 1.0 );
                    eff0.passEnd();
                }
                eff0.drawEnd();
            }
        }

        // the a wireframe box
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
        }
    }
};

//!
//! GFX module test application
//!
class GfxTestApp : public app::SampleApp
{
    Scene * mScene;
public:

    GfxTestApp() : mScene(0) {}

    bool onRendererCreate()
    {
        mScene = new Scene;
        return mScene->init();
    }

    void onRendererDestroy()
    {
        safeDelete(mScene);
    }

    void onKeyPress( input::KeyEvent ke )
    {
        app::SampleApp::onKeyPress( ke );
        if( input::KEY_RETURN == ke.code && ke.status.down && ke.status.altDown() )
        {
            // toggle fullscreen mode
            RendererOptions ro = gRenderer.getOptions();
            ro.fullscreen = !ro.fullscreen;
            if( !gRenderer.changeOptions(ro) ) postExistEvent();
        }
    }

    void onUpdate()
    {
        mScene->update();
    }

    void onRender()
    {
        Renderer & r = gRenderer;
        
        r.clearScreen( Vector4f(0,0,0,1) ); // clear to pure blue.

        // draw scene
        GN_ASSERT( mScene );
        mScene->draw();

        // draw mouse position on screen
        StrA mousePos;
        int x = 1, y = 1;
        gInput.getMousePosition( x, y );
        mousePos.format( "Mouse: %d, %d", x, y );
        r.drawDebugTextA( mousePos.cstr(), 0, 100 );//*/
    }
};

int main( int argc, const char * argv[] )
{
    GfxTestApp app;
    return app.run( argc, argv );
}
