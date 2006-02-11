#include "pch.h"
#include "basicMswInput.h"

#if GN_MSWIN

// *****************************************************************************
//    init / quit functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::input::BasicMswInput::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::input::BasicMswInput, () );

    // setup windows hooks
    if( !setupWindowHooks() ) { quit(); return selfOK(); }

    // initialize internal mouse position
    POINT pos;
    GN_MSW_CHECK_DO( ::GetCursorPos( &pos ), quit(); return selfOK(); );
    updateMousePosition( pos.x, pos.y, false );

    // success
    return selfOK();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::input::BasicMswInput::quit()
{
    GN_GUARD;

    // remove windows hook
    removeWindowHooks();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}


// *****************************************************************************
//                           interface functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::input::BasicMswInput::attachToWindow( HandleType, HandleType window )
{
    GN_GUARD;

    HWND hwnd = (HWND)window;

    // check window handle
    if( !::IsWindow(hwnd) )
    {
        GN_ERROR( "Window handle is not valid!" );
        return false;
    }

    // reset input states
    resetInputStates();

    // (re)capture mouse
    if( mMouseCapture )
    {
        ::ReleaseCapture();
        ::SetCapture( hwnd );
    }

    // success
    mWindow = hwnd;
    GN_TRACE( "Attach to window 0x%X", mWindow );
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::input::BasicMswInput::getMousePosition( int & x, int & y ) const
{
    GN_GUARD_SLOW;

    RECT  rc;
    POINT pt;
    ::GetCursorPos( &pt );
    ::GetClientRect( mWindow, &rc );
    ::ScreenToClient( mWindow, &pt );
    x = pt.x - rc.left;
    y = pt.y - rc.top;

    GN_UNGUARD_SLOW;
}

// *****************************************************************************
//                           protected functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::input::BasicMswInput::msgHandler( UINT message, WPARAM wp, LPARAM )
{
    GN_GUARD;

    switch(message)
    {
        // process WM_CHAR message
        case WM_CHAR :
            triggerCharPress( (char)wp );
            break;

        // capture mouse when mouse-button pressed
        case WM_LBUTTONDOWN :
            captureMouse();
            break;
        case WM_LBUTTONUP   :
            releaesMouse();
            break;
        case WM_RBUTTONDOWN :
            captureMouse();
            break;
        case WM_RBUTTONUP   :
            releaesMouse();
            break;
        case WM_MBUTTONDOWN :
            captureMouse();
            break;
        case WM_MBUTTONUP   :
            releaesMouse();
            break;

        // mouse move
        case WM_MOUSEMOVE :
            {
                POINT pos;
                GN_MSW_CHECK( ::GetCursorPos( &pos ) );
                updateMousePosition( pos.x, pos.y );
            }
            break;

        // mouse wheel
        case WM_MOUSEWHEEL :
            triggerAxisMove( AXIS_2, (short)HIWORD(wp)/10 );
            break;
    }

    GN_UNGUARD;
}

// *****************************************************************************
//                             private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::input::BasicMswInput::setupWindowHooks()
{
    GN_GUARD;

    // remove previous hook
    removeWindowHooks();

    // setup hooks
    mMsgHook = ::SetWindowsHookEx(
        WH_GETMESSAGE, sMsgHookProc,
        0,
        ::GetCurrentThreadId() );
    mCwpHook = ::SetWindowsHookEx(
        WH_CALLWNDPROC, sCwpHookProc,
        0,
        ::GetCurrentThreadId() );
    if( 0 == mMsgHook || 0 == mCwpHook )
    {
        GN_ERROR( getOSErrorInfo() );
        return false;
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::input::BasicMswInput::removeWindowHooks()
{
    GN_GUARD;

    if( mMsgHook )
    {
        GN_MSW_CHECK( ::UnhookWindowsHookEx( mMsgHook ) );
        mMsgHook = 0;
    }

    if( mCwpHook )
    {
        GN_MSW_CHECK( ::UnhookWindowsHookEx( mCwpHook ) );
        mCwpHook = 0;
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::input::BasicMswInput::captureMouse()
{
    GN_GUARD;

    if( mMouseCapture ) return;
    ::SetCapture( mWindow );
    ::SetCursor( ::LoadCursor(NULL, IDC_ARROW) );
    mMouseCapture = true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::input::BasicMswInput::releaesMouse()
{
    GN_GUARD;

    if( !mMouseCapture ) return;
    ::ReleaseCapture();
    mMouseCapture = false;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
LRESULT CALLBACK
GN::input::BasicMswInput::sMsgHookProc( int nCode, WPARAM wp, LPARAM lParam )
{
    GN_GUARD;

    BasicMswInput * inst = safeCast<BasicMswInput*>(gInputPtr);
    GN_ASSERT( inst );

    const MSG * p = (const MSG*)lParam;
    if( nCode >= 0 && p->hwnd == inst->mWindow )
    {
        inst->msgHandler( p->message, p->wParam, p->lParam );
    }

    return ::CallNextHookEx( inst->mMsgHook, nCode, wp, lParam );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
LRESULT CALLBACK
GN::input::BasicMswInput::sCwpHookProc( int nCode, WPARAM wp, LPARAM lParam )
{
    GN_GUARD;

    BasicMswInput * inst = safeCast<BasicMswInput*>(gInputPtr);
    GN_ASSERT( inst );

    const CWPSTRUCT * p = (const CWPSTRUCT*)lParam;
    //GN_TRACE( "CWP hook: hwnd(0x%X), msg(%s)", p->hwnd, GN::win::msg2str(p->message) );
    if( nCode >= 0 && p->hwnd == inst->mWindow )
    {
        inst->msgHandler( p->message, p->wParam, p->lParam );
    }

    return ::CallNextHookEx( inst->mCwpHook, nCode, wp, lParam );

    GN_UNGUARD;
}

#endif // GN_MSWIN
