#include "pch.h"

static GN::Logger * sLogger = GN::GetLogger("GN.gfx.test.input");

///
/// input module test application
///
class InputTest
{
    GN::AutoObjPtr<GN::win::Window> mWin;

    bool mDone;

    GN::input::KeyEvent mLastKeyEvent;

    bool CreateWindow()
    {
        mWin.Attach( GN::win::NewWindow( GN::win::WCP_WINDOWED_RENDER_WINDOW ) );
        if( mWin.Empty() ) return false;
        mWin->Show();
        return true;
    }

    bool CreateInput( const char * api )
    {
        if( !GN::input::InitializeInputSystem( 0 == GN::StringCompare("DI",api) ? GN::input::InputAPI::DINPUT : GN::input::InputAPI::NATIVE ) )
            return false;
        if( !gInputPtr->AttachToWindow( 0, mWin->GetWindowHandle() ) )
            return false;

        // connect to input signals
        gInput.sigKeyPress.Connect( this, &InputTest::OnKeyPress );
        gInput.sigCharPress.Connect( this, &InputTest::OnCharPress );
        gInput.sigAxisMove.Connect( this, &InputTest::OnAxisMove );

        return true;
    }

    /*LRESULT WinProc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp )
    {
        switch(msg)
        {
            case WM_PAINT:
                {
                    PAINTSTRUCT ps;
                    HDC dc = BeginPaint( hwnd, &ps );

                    GN::StrA txt;
                    txt.format(
                        "%s%s%s%s %s",
                        mLastKeyEvent.status.CtrlDown()?"CTRL-":"",
                        mLastKeyEvent.status.ShiftDown()?"SHIFT-":"",
                        mLastKeyEvent.status.AltDown()?"ALT-":"",
                        GN::input::KeyCode2String(mLastKeyEvent.code),
                        mLastKeyEvent.status.down?"DOWN":"UP" );
                    TextOutA( dc, 0, 0, txt.ToRawPtr(), (INT)txt.Size() );

                    if( gInputPtr )
                    {
                        int x, y;
                        gInputPtr->GetMousePosition( x, y );
                        txt.format( "Mouse: %d, %d", x, y );
                        TextOutA( dc, 0, 20, txt.ToRawPtr(), (INT)txt.Size() );
                    }

                    EndPaint( hwnd, &ps );
                }
                break;

            case WM_CLOSE: mDone = true; break;
            default:; // do nothing
        }

        return ::DefWindowProc( hwnd, msg, wp, lp );
    }*/

    void OnKeyPress( GN::input::KeyEvent ke )
    {
        mLastKeyEvent = ke;
        mWin->Repaint();
        if( !ke.status.down )
        {
            if( GN::input::KeyCode::ESCAPE == ke.code ) mDone = true;
        }
    }

    void OnCharPress( wchar_t )
    {
    }

    void OnAxisMove( GN::input::Axis, int  )
    {
        mWin->Repaint();
    }

public:

    ///
    /// Default constructor
    ///
    InputTest() {}

    ///
    /// Destructor
    ///
    ~InputTest() { Quit(); }

    ///
    /// Initialize test application
    ///
    bool Init( const char * api )
    {
        if( !CreateWindow() || !CreateInput(api) ) return false;

        mDone = false;

        // success
        return true;
    }

    ///
    /// Quit test application
    ///
    void Quit()
    {
        GN::input::ShutdownInputSystem();
        mWin.Clear();
    }

    ///
    /// Run test application
    ///
    int Run()
    {
        if( !gInputPtr )
        {
            GN_ERROR(sLogger)( "InputTest is not initialized!" );
            return -1;
        }

        while(!mDone)
        {
            mWin->RunUntilNoNewEvents();
            gInputPtr->ProcessInputEvents();
        }

        return 0;
    }
};

///
/// Print usage
///
void Usage( const char * appName )
{
    printf(
        "Input module test application.\n"
        "\n"
        "Usage: %s [MSW|DI]\n"
        "\n"
        "Note: default module is NT\n",
        appName );
}

///
/// Main entry point
///
int main( int argc, const char * argv[] )
{
    const char * module;

    if( argc < 2 )
    {
        Usage( argv[0] );
        module = "MSW";
    }
    else
    {
        module = argv[1];
    }

    InputTest app;
    if( !app.Init( module ) ) return -1;
    return app.Run();
}
