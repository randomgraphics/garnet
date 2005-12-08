#include "pch.h"
#include "x11Input.h"

#if GN_POSIX

static const int INTERESTED_EVENTS = PointerMotionMask | ButtonMotionMask // | PointerMotionHintMask
                                   | KeyPressMask | KeyReleaseMask
                                   | ButtonPressMask | ButtonReleaseMask ;

static const int UNINTERESTED_EVENTS = PointerMotionHintMask;

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::input::X11Input::X11Input()
{
    clear();

    // clear all fields to KEY_NONE
    memset( mKeyMap, KEY_NONE, sizeof(mKeyMap) );

    // setup key map
    #define GNINPUT_DEFINE_KEYCODE( name, dikey, scancode, vkeycode, xkeysym ) \
        GN_ASSERT( (scancode + 8) < MAX_SCAN_CODE ); \
        if( scancode > 0 ) mKeyMap[scancode+8] = name;
    #include "garnet/input/keyCodeMeta.h"
    #undef  GNINPUT_DEFINE_KEYCODE
}

//
//
// -----------------------------------------------------------------------------
bool GN::input::X11Input::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::input::X11Input, () );

    // Do custom init here

    // success
    return selfOK();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::input::X11Input::quit()
{
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
bool GN::input::X11Input::attachToWindow( HandleType disp, HandleType win )
{
    GN_GUARD;

    if( !disp || !win )
    {
        GNINPUT_ERROR( "Invalid display or window handle!" );
        return false;
    }

    // TODO: Verify that the window can receive keyboard and mouse events.

    mDisplay = (Display*)disp;
    mWindow = (Window)win;

    // get current input selection
    XWindowAttributes wa;
    GN_X_CHECK_RV( XGetWindowAttributes( mDisplay, mWindow, &wa ), false );

    int mask = wa.your_event_mask;

    // modify event mask
    mask |= INTERESTED_EVENTS;
    mask &= ~UNINTERESTED_EVENTS;

    // update window event selection
    GN_X_CHECK( XSelectInput( mDisplay, mWindow, mask ) );

    // update mouse position
    int x, y;
    getMousePosition( x, y );
    updateMousePosition( x, y, false );

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::input::X11Input::processInputEvents()
{
    GN_GUARD_SLOW;

    if( !mDisplay || !mWindow )
    {
        GNINPUT_ERROR( "X11Input is not initialized" );
        return;
    }

    XEvent e;
    for(;;)
    {
        if( XCheckWindowEvent( mDisplay, mWindow, INTERESTED_EVENTS, &e ) )
        {
            KeyCode kc;

            switch( e.type )
            {
                case MotionNotify:
                    //GNINPUT_TRACE( "MotionNotify: x(%d), y(%d)", e.xmotion.x, e.xmotion.y );
                    updateMousePosition( e.xmotion.x, e.xmotion.y );
                    break;

                case KeyPress:
                    //GNINPUT_TRACE( "KeyPress: keycode(%d)", e.xkey.keycode );
                    kc = mKeyMap[e.xkey.keycode];
                    if( KEY_NONE != kc ) triggerKeyPress( kc, true );
                    break;

                case KeyRelease:
                    //GNINPUT_TRACE( "KeyRelease: keycode(%d)", e.xkey.keycode );
                    kc = mKeyMap[e.xkey.keycode];
                    if( KEY_NONE != kc ) triggerKeyPress( kc, false );
                    break;

                case ButtonPress:
                    //GNINPUT_TRACE( "ButtonPress: button(%d)", e.xbutton.button );
                    kc = (KeyCode)( KEY_MOUSEBTN_0 + e.xbutton.button );
                    if( KEY_MOUSEBTN_FIRST <= kc && kc <= KEY_MOUSEBTN_LAST )
                    triggerKeyPress( kc, true );
                    break;

                case ButtonRelease:
                    //GNINPUT_TRACE( "ButtonRelease: button(%d)", e.xbutton.button );
                    kc = (KeyCode)( KEY_MOUSEBTN_0 + e.xbutton.button );
                    if( KEY_MOUSEBTN_FIRST <= kc && kc <= KEY_MOUSEBTN_LAST )
                    triggerKeyPress( kc, false );
                    break;

                default: ; // do nothing
            }
        }
        else return; // Idle time
    }

    GN_UNGUARD_SLOW;
}

//
//
// -----------------------------------------------------------------------------
void GN::input::X11Input::getMousePosition( int & x, int & y ) const
{
    GN_GUARD;

    if( !mDisplay || !mWindow )
    {
        GNINPUT_ERROR( "input system is not attached to a window!" );
        return;
    }

    Window root;
    Window child;
    int rootX, rootY, winX, winY;
    unsigned int mask;
    if( XQueryPointer( mDisplay, mWindow, &root, &child, &rootX, &rootY, &winX, &winY, &mask ) )
    {
        x = winX;
        y = winY;
    }

    GN_UNGUARD;
}

#endif // GN_POSIX
