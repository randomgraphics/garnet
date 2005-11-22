#include "pch.h"
#include "garnet/GNwin.h"
#include "garnet/GNinput.h"

class Scene
{
    GN::AutoRef<GN::gfx::VtxBuf> vbuf1;
    GN::AutoRef<GN::gfx::IdxBuf> ibuf1;
    uint32_t                     vbind;


public:

    Scene() {}

    ~Scene() { quit(); }

    bool init( GN::gfx::Renderer & r )
    {
        // create vertex binding
        GN::gfx::VtxFmtDesc fmt;
        fmt.addAttrib( 0, 0, GN::gfx::VTXSEM_COORD, GN::gfx::FMT_FLOAT3 );
        vbind = r.createVtxBinding( fmt );
        if( 0 == vbind ) return false;

        // create index buffer
        ibuf1.attach( r.createIdxBuf( 4 ) );
        if( ibuf1.empty() ) return false;
        uint16_t * ibData = ibuf1->lock( 0, 0, 0 );
        if( 0 == ibData ) return false;
        ibData[0] = 0; ibData[1] = 1; ibData[2] = 2; ibData[3] = 3;
        ibuf1->unlock();

        // create vertex buffer
        vbuf1.attach( r.createVtxBuf( fmt.streams[0].stride * 4 ) );
        if( vbuf1.empty() ) return false;
        GN::Vector3f * p = (GN::Vector3f*)vbuf1->lock( 0, 0, 0 );
        if( 0 == p ) return false;
        p[0].set( 0, 0, 0 );
        p[1].set( 0, 1, 0 );
        p[2].set( 1, 0, 0 );
        p[2].set( 1, 1, 0 );
        vbuf1->unlock();

        // success
        return true;
    }

    void quit()
    {
        vbuf1.reset();
        ibuf1.reset();
        vbind = 0;
    }

    void draw( GN::gfx::Renderer & r )
    {
        r.bindVtxBinding( vbind );
        r.bindVtxBufs( (const GN::gfx::VtxBuf**)&vbuf1, 0, 1 );
        //r.bindIdxBuf( ibuf1 );
        //r.drawIndexed( GN::gfx::TRIANGLE_STRIP, 2, 0, 0, 4, 0 );
        r.draw( GN::gfx::TRIANGLE_LIST, 1, 0 );
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
