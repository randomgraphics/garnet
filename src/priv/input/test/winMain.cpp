#include "pch.h"
#include "garnet/base/NTWindow.h"

#if GN_MSWIN

//!
//! input module test application
//!
class InputTest
{
    GN::NTWindow mWin;
    GN::SharedLib mLib;
    GN::input::CreateInputFunc mCreator;
    GN::AutoObjPtr<GN::input::Input> mInput;

    bool mDone;

    bool createWindow()
    {
        GN::NTWindow::CreateParam cp;
        mWin.setWindowProcedure( GN::makeFunctor(this,&InputTest::winProc) );
        if( !mWin.create(cp) ) return false;
        mWin.showWindow();
        return true;
    }

    bool createInput( const char * api )
    {
        GN::StrA libName = GN::StrA("GNinput") + api;
        if( !mLib.load( libName.cstr() ) ) return false;
        mCreator = (GN::input::CreateInputFunc)mLib.getSymbol( "GNinputCreateInput" );
        if( !mCreator ) return false;

        GN::input::UserOptions uo;
        uo.windowHandle = mWin.getWindow();
        mInput.reset( mCreator(uo) );
        if( !mInput ) return false;

        // connect to input signals
        mInput->sigKeyPress.connect( this, &InputTest::onKeyPress );
        mInput->sigCharPress.connect( this, &InputTest::onCharPress );
        mInput->sigAxisMove.connect( this, &InputTest::onAxisMove );

        return true;
    }

    LRESULT winProc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp )
    {
        switch(msg)
        {
            case WM_CLOSE: mDone = true; break;
            default:; // do nothing
        }
        
        return ::DefWindowProc( hwnd, msg, wp, lp );
    }

    void onKeyPress( GN::input::KeyEvent ke )
    {
        if( !ke.down )
        {
            if( GN::input::KEY_ESCAPE == ke.code ) mDone = true;
        }
    }

    void onCharPress( wchar_t )
    {
    }

    void onAxisMove( GN::input::Axis, int )
    {
    }

public:

    //!
    //! Default constructor
    //!
    InputTest() {}

    //!
    //! Destructor
    //!
    ~InputTest() { quit(); }

    //!
    //! Initialize test application
    //!
    bool init( const char * api )
    {
        if( !createWindow() ||
            !createInput(api) ) return false;

        mDone = false;

        // success
        return true;
    }

    //!
    //! Quit test application
    //!
    void quit()
    {
        mInput.reset();
        mCreator = 0;
        mLib.free();
        mWin.destroy();
    }

    //!
    //! Run test application
    //!
    int run()
    {
        if( !mInput )
        {
            GN_ERROR( "InputTest is not initialized!" );
            return -1;
        }

        while(!mDone)
        {
            GN::win::processMessages( mWin.getWindow() );
            update();
        }

        return 0; 
    }

    //!
    //! Frame update
    //!
    void update()
    {
    }
};

//!
//! Print usage
//!
void usage( const char * appName )
{
    printf(
        "Input module test application.\n"
        "\n"
        "Usage: %s [NT|DI]\n"
        "\n"
        "Note: default module is NT\n",
        appName );
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
        module = "NT";
    }
    else
    {
        module = argv[1];
    }

    InputTest app;
    if( !app.init( module ) ) return -1;
    return app.run();
}

#else // GN_MSWIN
int main() { return 0; }
#endif
