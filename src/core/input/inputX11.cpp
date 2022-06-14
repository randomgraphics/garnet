#include "pch.h"
#include "inputX11.h"

#if HAS_X11

static const int INTERESTED_EVENTS = PointerMotionMask | ButtonMotionMask // | PointerMotionHintMask
                                     | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask;

static const int UNINTERESTED_EVENTS = PointerMotionHintMask;

static GN::Logger * sLogger = GN::getLogger("GN.input.X11");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::input::InputX11::InputX11() {
    clear();

    // clear all fields to KeyCode::NONE
    memset(mKeyMap, KeyCode::NONE, sizeof(mKeyMap));

    // setup key map
    #define GNINPUT_DEFINE_KEYCODE(name, dikey, scancode, vkeycode, xkeysym) \
        GN_ASSERT((scancode + 8) < MAX_SCAN_CODE);                           \
        if (scancode > 0) mKeyMap[scancode + 8] = KeyCode::name;
    #include "garnet/input/keyCodeMeta.h"
    #undef GNINPUT_DEFINE_KEYCODE
}

//
//
// -----------------------------------------------------------------------------
bool GN::input::InputX11::init() {
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT();

    // Do custom init here

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::input::InputX11::quit() {
    GN_GUARD;

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// *****************************************************************************
// from Input
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::input::InputX11::attachToWindow(intptr_t disp, intptr_t win) {
    GN_GUARD;

    if (!disp || !win) {
        GN_ERROR(sLogger)("Invalid display or window handle!");
        return false;
    }

    // TODO: Verify that the window can receive keyboard and mouse events.

    mDisplay = (Display *) disp;
    mWindow  = (Window) win;

    // get current input selection
    XWindowAttributes wa;
    GN_X_CHECK_RETURN(XGetWindowAttributes(mDisplay, mWindow, &wa), false);

    int mask = wa.your_event_mask;

    // modify event mask
    mask |= INTERESTED_EVENTS;
    mask &= ~UNINTERESTED_EVENTS;

    // update window event selection
    GN_X_CHECK(XSelectInput(mDisplay, mWindow, mask));

    // update mouse position
    int x, y;
    getMousePosition(x, y);
    updateMousePosition(x, y, false);

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::input::InputX11::processInputEvents() {
    GN_GUARD_SLOW;

    if (!mDisplay || !mWindow) {
        GN_ERROR(sLogger)("InputX11 is not initialized");
        return;
    }

    XEvent e;
    for (;;) {
        if (XCheckWindowEvent(mDisplay, mWindow, INTERESTED_EVENTS, &e)) {
            KeyCode kc;

            switch (e.type) {
            case MotionNotify:
                // GN_TRACE( "MotionNotify: x(%d), y(%d)", e.xmotion.x, e.xmotion.y );
                updateMousePosition(e.xmotion.x, e.xmotion.y);
                break;

            case KeyPress:
                // GN_TRACE( "KeyPress: keycode(%d)", e.xkey.keycode );
                kc = mKeyMap[e.xkey.keycode];
                if (KeyCode::NONE != kc) triggerKeyPress(kc, true);
                break;

            case KeyRelease:
                // GN_TRACE( "KeyRelease: keycode(%d)", e.xkey.keycode );
                kc = mKeyMap[e.xkey.keycode];
                if (KeyCode::NONE != kc) triggerKeyPress(kc, false);
                break;

            case ButtonPress:
                // GN_TRACE( "ButtonPress: button(%d)", e.xbutton.button );
                if (4 == e.xbutton.button) {
                    // mouse wheel up
                    triggerAxisMove(Axis::MOUSE_WHEEL_0, 10);
                } else if (5 == e.xbutton.button) {
                    // mouse wheel down
                    triggerAxisMove(Axis::MOUSE_WHEEL_0, -10);
                } else {
                    kc = (KeyCode) (KeyCode::MOUSEBTN_0 + e.xbutton.button - 1);
                    if (KeyCode::FIRST_MOUSE_BUTTON <= kc && kc <= KeyCode::LAST_MOUSE_BUTTON) triggerKeyPress(kc, true);
                }
                break;

            case ButtonRelease:
                // GN_TRACE( "ButtonRelease: button(%d)", e.xbutton.button );
                kc = (KeyCode) (KeyCode::MOUSEBTN_0 + e.xbutton.button - 1);
                if (KeyCode::FIRST_MOUSE_BUTTON <= kc && kc <= KeyCode::LAST_MOUSE_BUTTON) triggerKeyPress(kc, false);
                break;

            default:; // do nothing
            }
        } else
            return; // Idle time
    }

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::input::InputX11::getMousePosition(int & x, int & y) const {
    GN_GUARD;

    if (!mDisplay || !mWindow) {
        GN_ERROR(sLogger)("input system is not attached to a window!");
        return;
    }

    Window       root;
    Window       child;
    int          rootX, rootY, winX, winY;
    unsigned int mask;
    if (XQueryPointer(mDisplay, mWindow, &root, &child, &rootX, &rootY, &winX, &winY, &mask)) {
        x = winX;
        y = winY;
    }

    GN_UNGUARD;
}

#endif // GN_POSIX
