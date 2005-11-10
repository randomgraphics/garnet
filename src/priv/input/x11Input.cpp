#include "pch.h"
#include "x11Input.h"

#if GN_POSIX

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

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
        int mask = KeyPressMask | KeyReleaseMask
                 | ButtonPressMask | ButtonReleaseMask ;
        if( XCheckWindowEvent( mDisplay, mWindow, mask, &e ) )
        {
            switch( e.type )
            {
                case KeyPress:
                    GNINPUT_INFO( "keypress: code(%d)", e.xkey.keycode );
                    //triggerKeyPress( 0, true );
                    break;

                case KeyRelease:
                    GNINPUT_INFO( "keyrelease: code(%d)", e.xkey.keycode );
                    //triggerKeyPress( 0, false );
                    break;

                case ButtonPress:
                    GNINPUT_INFO( "buttonpress: code(%d)", e.xbutton.button );
                    break;

                case ButtonRelease:
                    GNINPUT_INFO( "buttonrelease: code(%d)", e.xbutton.button );
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
void GN::input::X11Input::getMousePosition( int &, int & ) const
{
    GN_GUARD;

    GN_UNGUARD;
}

#endif // GN_POSIX
