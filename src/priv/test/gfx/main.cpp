#include "pch.h"
#include "garnet/gfx/effect.h"

class Scene
{
    GN::AutoRef<GN::gfx::Shader> ps1, ps2;

    uint32_t tex0;

    GN::gfx::effect::Effect eff0;

    bool loadEffect()
    {
        GN_GUARD;

        // create effect 0
        {
            GN::gfx::effect::EffectDesc desc;
            GN::gfx::effect::ShaderDesc & vs = desc.shaders["vs"];
            GN::gfx::effect::ShaderDesc & ps = desc.shaders["ps"];
            vs.type = GN::gfx::VERTEX_SHADER;
            ps.type = GN::gfx::PIXEL_SHADER;
            GN::gfx::effect::TechniqueDesc & tech = desc.techniques["ffp"];
            tech.passes.resize(1);
            GN::gfx::effect::PassDesc & pass0 = tech.passes[0];
            pass0.shaders[GN::gfx::VERTEX_SHADER] = "vs";
            pass0.shaders[GN::gfx::PIXEL_SHADER] = "ps";
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
        GN::gfx::Renderer & r = gRenderer;

        // create pixel shaders
        if( r.supportShader( GN::gfx::PIXEL_SHADER, GN::gfx::LANG_D3D_ASM ) )
        {
            static const char * code =
                "ps_1_1\n"
                "mov r0, c0";
            ps1.attach( r.createPxlShader( GN::gfx::LANG_D3D_ASM, code ) );
            if( !ps1 ) return false;
            ps1->setUniformByNameV( "c0", GN::Vector4f(0,1,0,1) );
        }
        else if( r.supportShader( GN::gfx::PIXEL_SHADER, GN::gfx::LANG_OGL_ARB ) )
        {
            static const char * code =
                "!!ARBfp1.0 \n"
                "PARAM white = program.local[0]; \n"
                "OUTPUT oClr = result.color; \n"
                "MOV oClr, white; \n"
                "END";
            ps1.attach( r.createPxlShader( GN::gfx::LANG_OGL_ARB, code ) );
            if( !ps1 ) return false;
            ps1->setUniformByNameV( "l0", GN::Vector4f(0,1,0,1) );
        }
        if( r.supportShader( GN::gfx::PIXEL_SHADER, GN::gfx::LANG_D3D_HLSL ) )
        {
            static const char * code =
                "float4 diffuse; \n"
                "float4 psMain() : COLOR0 \n"
                "{ \n"
                "   return diffuse; \n"
                "} \n";
            ps2.attach( r.createPxlShader( GN::gfx::LANG_D3D_HLSL, code, "psMain" ) );
            if( !ps2 ) return false;
            ps2->setUniformByNameV( "diffuse", GN::Vector4f(1,0,0,1) );
        }
        else if( r.supportShader( GN::gfx::PIXEL_SHADER, GN::gfx::LANG_OGL_GLSL ) )
        {
            static const char * code =
                "uniform vec4 diffuse; \n"
                "void main() \n"
                "{ \n"
                "   gl_FragColor = diffuse; \n"
                "} \n";
            ps2.attach( r.createPxlShader( GN::gfx::LANG_OGL_GLSL, code ) );
            if( !ps2 ) return false;
            ps2->setUniformByNameV( "diffuse", GN::Vector4f(1,0,0,1) );
        }

        // get texture handle
        tex0 = gTexDict.getResourceHandle( "texture/rabit.png" );

        // create the effect
        if( !loadEffect() ) return false;

        // success
        return true;
    }

    void quit()
    {
        eff0.quit();
        ps1.reset();
		ps2.reset();
    }

    void draw()
    {
        GN::gfx::Renderer & r = gRenderer;

        r.bindTexture( 0, gTexDict.getResource(tex0) );
        r.draw2DQuad( 0, 0, 0, 0.5, 0.5 );
        if( ps1 )
        {
            r.bindShaders( 0, ps1 );
            r.draw2DQuad( GN::gfx::DQ_USE_CURRENT_PS, 0.5, 0.0, 1.0, 0.5 );
        }
        if( ps2 )
        {
            r.bindShaders( 0, ps2 );
            r.draw2DQuad( GN::gfx::DQ_USE_CURRENT_PS, 0.0, 0.5, 0.5, 1.0 );
        }
    }
};

//!
//! GFX module test application
//!
class GfxTestApp : public GN::app::SampleApp
{
    bool mDone;

    GN::app::FpsCounter mFps;

    Scene * mScene;

public:

    GfxTestApp() : mScene(0) {}

    bool onRendererCreate()
    {
        mFps.reset();
        mScene = new Scene;
        return mScene->init();
    }

    void onRendererDestroy()
    {
        GN::safeDelete(mScene);
    }

    void onKeyPress( GN::input::KeyEvent ke )
    {
        if( GN::input::KEY_RETURN == ke.code && ke.status.down && ke.status.altDown() )
        {
            // toggle fullscreen mode
            GN::gfx::RendererOptions ro = gRenderer.getOptions();
            ro.fullscreen = !ro.fullscreen;
            if( !gRenderer.changeOptions(ro) ) mDone = true;
        }
    }

    void onUpdate()
    {
        const GN::input::KeyStatus * kb = gInput.getKeyboardStatus();
        mDone = kb[GN::input::KEY_ESCAPE].down;
    }

    void onRender()
    {
        GN::gfx::Renderer & r = gRenderer;
        
        r.clearScreen( GN::Vector4f(0,0,0,1) ); // clear to pure blue.

        // draw scene
        GN_ASSERT( mScene );
        mScene->draw();

        // draw FPS
        mFps.onFrame();
        r.drawDebugTextA( mFps.fpsString(), 0, 0 );

        // draw mouse position on screen
        GN::StrA mousePos;
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
