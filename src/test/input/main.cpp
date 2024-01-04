#include "pch.h"

static GN::Logger * sLogger = GN::getLogger("GN.gfx.test.input");

///
/// input module test application
///
class InputTest {
    GN::AutoObjPtr<GN::win::Window> mWin;

    bool mDone;

    GN::input::KeyEvent mLastKeyEvent;

    bool createWindow() {
        mWin.attach(GN::win::createWindow(GN::win::WCP_WINDOWED_RENDER_WINDOW));
        if (mWin.empty()) return false;
        mWin->show();
        return true;
    }

    bool createInput(const char * api) {
        if (!GN::input::initializeInputSystem(0 == GN::str::compare("DI", api) ? GN::input::InputAPI::DINPUT : GN::input::InputAPI::NATIVE)) return false;
        if (!gInputPtr->attachToWindow(0, mWin->getWindowHandle())) return false;

        // connect to input signals
        gInput.sigKeyPress.connect(this, &InputTest::onKeyPress);
        gInput.sigCharPress.connect(this, &InputTest::onCharPress);
        gInput.sigAxisMove.connect(this, &InputTest::onAxisMove);

        return true;
    }

    void RepainWindow() {
        // ::RedrawWindow(
        //     mWindow,
        //     NULL, NULL, // rc, rgn
        //     RDW_ERASE | RDW_FRAME | RDW_INTERNALPAINT | RDW_INVALIDATE | RDW_ALLCHILDREN );
    }

    /*LRESULT winProc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp )
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
                        mLastKeyEvent.status.ctrlDown()?"CTRL-":"",
                        mLastKeyEvent.status.shiftDown()?"SHIFT-":"",
                        mLastKeyEvent.status.altDown()?"ALT-":"",
                        GN::input::keyCode2String(mLastKeyEvent.code),
                        mLastKeyEvent.status.down?"DOWN":"UP" );
                    TextOutA( dc, 0, 0, txt.rawptr(), (INT)txt.size() );

                    if( gInputPtr )
                    {
                        int x, y;
                        gInputPtr->getMousePosition( x, y );
                        txt.format( "Mouse: %d, %d", x, y );
                        TextOutA( dc, 0, 20, txt.rawptr(), (INT)txt.size() );
                    }

                    EndPaint( hwnd, &ps );
                }
                break;

            case WM_CLOSE: mDone = true; break;
            default:; // do nothing
        }

        return ::DefWindowProc( hwnd, msg, wp, lp );
    }*/

    void onKeyPress(GN::input::KeyEvent ke) {
        mLastKeyEvent = ke;
        RepainWindow();
        if (!ke.status.down) {
            if (GN::input::KeyCode::ESCAPE == ke.code()) mDone = true;
        }
    }

    void onCharPress(wchar_t) {}

    void onAxisMove(GN::input::Axis, int) { RepainWindow(); }

public:
    ///
    /// Default constructor
    ///
    InputTest() {}

    ///
    /// Destructor
    ///
    ~InputTest() { quit(); }

    ///
    /// Initialize test application
    ///
    bool init(const char * api) {
        if (!createWindow() || !createInput(api)) return false;

        mDone = false;

        // success
        return true;
    }

    ///
    /// Quit test application
    ///
    void quit() {
        GN::input::shutdownInputSystem();
        mWin.clear();
    }

    ///
    /// Run test application
    ///
    int run() {
        if (!gInputPtr) {
            GN_ERROR(sLogger)("InputTest is not initialized!");
            return -1;
        }

        while (!mDone) {
            mWin->runUntilNoNewEvents();
            gInputPtr->processInputEvents();
        }

        return 0;
    }
};

///
/// Print usage
///
void usage(const char * appName) {
    printf("Input module test application.\n"
           "\n"
           "Usage: %s [MSW|DI]\n"
           "\n"
           "Note: default module is NT\n",
           appName);
}

///
/// Main entry point
///
int main(int argc, const char * argv[]) {
    const char * module;

    if (argc < 2) {
        usage(argv[0]);
        module = "MSW";
    } else {
        module = argv[1];
    }

    InputTest app;
    if (!app.init(module)) return -1;
    return app.run();
}
