#include "pch.h"
#include "garnet/GNinput.h"
#include "garnet/gfx/effect.h"

//
//
// -----------------------------------------------------------------------------
#if GN_MSWIN && !GN_XENON
static void sProcessMswMessages( GN::HandleType ) {}
#else
static void sProcessMswMessages( GN::HandleType wnd )
{
    GN_GUARD;

    GN_ASSERT( ::IsWindow( (HWND)wnd ) );

    MSG msg;
    while( true )
    {
        if( ::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
        {
            if( WM_QUIT == msg.message )
            {
                return;
            }
            ::TranslateMessage( &msg );
            ::DispatchMessage(&msg);
        }
        else if( ::IsIconic( (HWND)wnd ) ) // Block minimized application
        {
            GN_INFO( "Wait for window messages..." );
            ::WaitMessage();
        }
        else return; // Idle time
    }

    GN_UNGUARD;
}
#endif

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
class GfxTest
{
    GN::Clock mClock;

    bool mDone;

    GN::StrA mFPS;

    Scene * mScene;

public:

    //!
    //! Default constructor
    //!
    GfxTest() : mScene(0) {}

    //!
    //! Destructor
    //!
    ~GfxTest() { quit(); }

    //!
    //! Initialize test application
    //!
    bool init( const char * apiName )
    {
        // create renderer
        GN::gfx::RendererAPI api;
        if( 0 == GN::strCmpI( apiName, "-ogl" ) )
            api = GN::gfx::API_OGL;
        else if( 0 == GN::strCmpI( apiName, "-d3d" ) )
            api = GN::gfx::API_D3D;
        else if( 0 == GN::strCmpI( apiName, "-fake" ) )
            api = GN::gfx::API_FAKE;
        else
        {
            GN_ERROR( "invalid rendering API. Must be [-ogl|-d3d|-fake]." );
            return false;
        }
        GN::gfx::RendererOptions ro;
        GN::gfx::Renderer * r = GN::gfx::createRenderer( api, ro );
        if( 0 == r ) return false;

        // create input
        const GN::gfx::DispDesc & dd = r->getDispDesc();
        GN::input::Input * i = GN::input::createInputSystem();
        if( !i || !i->attachToWindow(dd.displayHandle,dd.windowHandle) ) return false;
        i->sigKeyPress.connect( this, &GfxTest::onKeyPress );

        // create scene
        mScene = new Scene;
        if( !mScene->init() ) return false;

        // success
        mDone = false;
        return true;
    }

    //!
    //! Quit test application
    //!
    void quit()
    {
        GN::safeDelete(mScene);
        delete gInputPtr;
        GN::gfx::deleteRenderer();
    }

    //!
    //! Run test application
    //!
    int run()
    {
        while(!mDone)
        {
            sProcessMswMessages( gRenderer.getDispDesc().windowHandle );
            gInput.processInputEvents();
            update();
            if( gRendererPtr && gRenderer.drawBegin() )
            {
                render();
                gRenderer.drawEnd();
            }
        }

        return 0;
    }


    //!
    //! Key event handler
    //!
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

    //!
    //! Frame update
    //!
    void update()
    {
        const GN::input::KeyStatus * kb = gInput.getKeyboardStatus();
        mDone = kb[GN::input::KEY_ESCAPE].down;
    }

    //!
    //! Frame render
    //!
    void render()
    {
        GN::gfx::Renderer & r = gRenderer;
        
        r.clearScreen( GN::Vector4f(0,0,0,1) ); // clear to pure blue.

        // draw scene
        GN_ASSERT( mScene );
        mScene->draw();

        // draw FPS
        static size_t frames = 0;
        static double lastTime = mClock.getTimeD();
        double currentTime = mClock.getTimeD();
        ++frames;
        if( currentTime - lastTime >= 0.5 )
        {
            mFPS.format( "FPS: %.2f", frames/(currentTime - lastTime) );
            lastTime = currentTime;
            frames = 0;
        }
        r.drawDebugTextA( mFPS.cstr(), 0, 0 );

        // draw mouse position on screen
        GN::StrA mousePos;
        int x = 1, y = 1;
        gInput.getMousePosition( x, y );
        mousePos.format( "Mouse: %d, %d", x, y );
        r.drawDebugTextA( mousePos.cstr(), 0, 100 );//*/
    }
};

#if GN_MSWIN
#define DEFAULT_MODULE "-D3D"
#else
#define DEFAULT_MODULE "-OGL"
#endif

//!
//! Print usage
//!
void usage()
{
    printf(
        "GFX module test application.\n"
        "\n"
        "Usage: GNgfxTest [-d3d|-ogl|-fake]\n"
        "\n"
        "Note: default module is %s\n",
        DEFAULT_MODULE );
}

//!
//! Main entry point
//!
int main( int argc, const char * argv[] )
{
    GN_GUARD_ALWAYS;

    const char * module;

    if( argc < 2 )
    {
        usage();
        module = DEFAULT_MODULE;
    }
    else
    {
        module = argv[1];
    }

    GfxTest app;
    if( !app.init( module ) ) return -1;
    return app.run();

    GN_UNGUARD_ALWAYS_NO_THROW;
    return -1;
}
