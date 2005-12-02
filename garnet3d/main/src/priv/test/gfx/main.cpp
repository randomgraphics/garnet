#include "pch.h"
#include "garnet/GNwin.h"
#include "garnet/GNinput.h"

class Scene
{
    GN::AutoRef<GN::gfx::Shader> ps1;

public:

    Scene() {}

    ~Scene() { quit(); }

    bool init( GN::gfx::Renderer & r )
    {
        // create pixel shader
        if( r.supportShader( GN::gfx::PIXEL_SHADER, GN::gfx::LANG_D3D_ASM ) )
        {
            static const char * ps1Code =
                "ps_1_1\n"
                "mov r0, c0";
            ps1.attach( r.createPixelShader( GN::gfx::LANG_D3D_ASM, ps1Code ) );
            if( !ps1 ) return false;
            ps1->setUniformByName( "c0", GN::Vector4f(1,1,1,1) );
        }

        // success
        return true;
    }

    void quit()
    {
        ps1.reset();
    }

    void draw( GN::gfx::Renderer & r )
    {
        //r.setRenderState( GN::gfx::RS_CULL_MODE, GN::gfx::RSV_CULL_NONE );
        r.bindShaders( 0, ps1 );
        r.drawQuad( 0, 0, 0.5, 0.5, 0, 0, 0, 1, GN::gfx::DQ_USE_CURRENT_PS );
    }
};

//!
//! GFX module test application
//!
class GfxTest
{
    GN::SharedLib mLib;
    GN::gfx::CreateRendererFunc mCreator;
    GN::AutoObjPtr<GN::gfx::Renderer> mRenderer;
    GN::AutoObjPtr<GN::input::Input> mInput;
    GN::Clock mClock;

    bool mDone;

    GN::StrA mFPS;

    Scene mScene;

public:

    //!
    //! Default constructor
    //!
    GfxTest() {}

    //!
    //! Destructor
    //!
    ~GfxTest() { quit(); }

    //!
    //! Initialize test application
    //!
    bool init( const char * api )
    {
#if GN_STATIC
        if( 0 == GN::strCmp( api, "OGL" ) )
            mCreator = &GN::gfx::createOGLRenderer;
        else
            mCreator = &GN::gfx::createD3DRenderer;
#else
        GN::StrA libName = GN::StrA("GNgfx") + api;
        if( !mLib.load( libName.cstr() ) ) return false;
        mCreator = (GN::gfx::CreateRendererFunc)mLib.getSymbol( "GNgfxCreateRenderer" );
        if( !mCreator ) return false;
#endif
        GN::gfx::RendererOptions ro;
        mRenderer.attach( mCreator(ro) );
        if( !mRenderer ) return false;

        const GN::gfx::DispDesc & dd = mRenderer->getDispDesc();

        mInput.attach( GN::input::createInputSystem() );
        if( !mInput || !mInput->attachToWindow(dd.displayHandle,dd.windowHandle) ) return false;

        mInput->sigKeyPress.connect( this, &GfxTest::onKeyPress );

        mDone = false;

        if( !mScene.init( *mRenderer ) ) return false;

        // success
        return true;
    }

    //!
    //! Quit test application
    //!
    void quit()
    {
        mScene.quit();
        if( mInput ) mInput->sigKeyPress.disconnect(this);
        mInput.clear();
        mRenderer.clear();
        mCreator = 0;
        mLib.free();
    }

    //!
    //! Run test application
    //!
    int run()
    {
        if( !mRenderer )
        {
            GN_ERROR( "GfxTest is not initialized!" );
            return -1;
        }

        while(!mDone)
        {
#if GN_MSWIN
            GN::win::processMswMessages( mRenderer->getDispDesc().windowHandle );
#endif
            mInput->processInputEvents();
            update();
            if( mRenderer->drawBegin() )
            {
                render();
                mRenderer->drawEnd();
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
            GN::gfx::RendererOptions ro = mRenderer->getOptions();
            ro.fullscreen = !ro.fullscreen;
            if( !mRenderer->changeOptions(ro) ) mDone = true;
        }
    }

    //!
    //! Frame update
    //!
    void update()
    {
        const GN::input::KeyStatus * kb = mInput->getKeyboardStatus();
        mDone = kb[GN::input::KEY_ESCAPE].down;
    }

    //!
    //! Frame render
    //!
    void render()
    {
        mRenderer->clearScreen( GN::Vector4f(0,0,0,1) ); // clear to pure blue.

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
        mRenderer->drawTextA( mFPS.cstr(), 0, 0 );

        // draw mouse position on screen
        GN::StrA mousePos;
        int x = 1, y = 1;
        mInput->getMousePosition( x, y );
        mousePos.format( "Mouse: %d, %d", x, y );
        mRenderer->drawTextA( mousePos.cstr(), 0, 100 );

        // draw scene
        mScene.draw( *mRenderer );
    }
};

#if GN_MSWIN
#define DEFAULT_MODULE "D3D"
#else
#define DEFAULT_MODULE "OGL"
#endif

//!
//! Print usage
//!
void usage( const char * appName )
{
    printf(
        "GFX module test application.\n"
        "\n"
        "Usage: %s [D3D|OGL]\n"
        "\n"
        "Note: default module is %s\n",
        appName,
        DEFAULT_MODULE );
}

//!
//! Main entry point
//!
int main( int argc, const char * argv[] )
{
    const char * module;

    if( argc < 2 )
    {
        usage( argv[0] );
        module = DEFAULT_MODULE;
    }
    else
    {
        module = argv[1];
    }

    GfxTest app;
    if( !app.init( module ) ) return -1;
    return app.run();
}
