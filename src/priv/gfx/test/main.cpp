#include "pch.h"

//!
//! GFX module test application
//!
class GfxTest
{
    GN::SharedLib mLib;
    GN::gfx::CreateRendererFunc mCreator;
    GN::AutoObjPtr<GN::gfx::Renderer> mRenderer;

    bool mDone;

    //!
    //! Check if specific key is pressed
    //!
    bool keyDown( int keycode )
    {
#if GN_WINNT
        return !!( 0x8000 & ::GetKeyState(keycode) );
#else
        return false;
#endif
    }

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
        GN::StrA libName = GN::StrA("GNgfx") + api;
        if( !mLib.load( libName.cstr() ) ) return false;
        mCreator = (GN::gfx::CreateRendererFunc)mLib.getSymbol( "GNgfxCreateRenderer" );
        if( !mCreator ) return false;

        GN::gfx::DeviceSettings ds;
        mRenderer.reset( mCreator(ds) );
        if( !mRenderer ) return false;

        mDone = false;

        // success
        return true;
    }

    //!
    //! Quit test application
    //!
    void quit()
    {
        mRenderer.reset();
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
            GN::processWindowMessages( &mDone );
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
    //! Frame update
    //!
    void update()
    {
#if GN_WINNT
        mDone = keyDown(VK_ESCAPE);

        if( keyDown(VK_RETURN) && keyDown(VK_MENU) )
        {
            // toggle fullscreen mode
            GN::gfx::DeviceSettings ds;
            ds.fullscreen = !mRenderer->getDispDesc().fullscreen;
            if( !mRenderer->changeDevice(ds) )
                mDone = true;
        }
#endif
    }

    //!
    //! Frame render
    //!
    void render()
    {
        mRenderer->clearScreen( GN::Vector4f(0,0,1,1) ); // clear to pure blue.
    }
};

#if GN_WINNT
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
