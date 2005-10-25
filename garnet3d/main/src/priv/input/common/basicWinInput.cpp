#include "pch.h"
#include "basicWinInput.h"

#if GN_WINNT

// *****************************************************************************
//    init / quit functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::input::BasicWinInput::init( const UserOptions & uo )
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::input::BasicWinInput, () );

    // setup windows hooks
    if( !setupWindowHooks() ) { quit(); return selfOK(); }

    // get current mouse position
    ::GetCursorPos( &mMousePosition );

    // call changeUserOptions()
    if( !changeUserOptions(uo) ) { quit(); return selfOK(); }

    // success
    return selfOK();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::input::BasicWinInput::quit()
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
bool GN::input::BasicWinInput::changeUserOptions( const UserOptions & uo )
{
    GN_GUARD;

    // check window handle
    HWND hwnd = (HWND)uo.windowHandle;
    if( !::IsWindow(hwnd) ) return false;

    // reset basic input class
    resetInputStates();

    // (re)capture mouse
    if( mMouseCapture )
    {
        ::ReleaseCapture();
        ::SetCapture( hwnd );
    }

    // store user options
    mUserOptions = uo;

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::input::BasicWinInput::getMousePosition( int & x, int & y ) const
{
    GN_GUARD;

    RECT  rc;
    POINT pt = mMousePosition;
    ::GetClientRect( (HWND)mUserOptions.windowHandle, &rc );
    ::ScreenToClient( (HWND)mUserOptions.windowHandle, &pt );
    x = pt.x - rc.left;
    y = pt.y - rc.top;

    GN_UNGUARD;
}

// *****************************************************************************
//                           protected functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::input::BasicWinInput::processMouseMove()
{
    GN_GUARD;

    POINT oldpt = mMousePosition;
    ::GetCursorPos( &mMousePosition );
    if( mMousePosition.x != oldpt.x )
        triggerAxisMove( AXIS_0, mMousePosition.x - oldpt.x );
    if( mMousePosition.y != oldpt.y )
        triggerAxisMove( AXIS_1, mMousePosition.y - oldpt.y );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::input::BasicWinInput::msgHandler( UINT message, WPARAM wParam, LPARAM /*lParam*/ )
{
    GN_GUARD;

    switch(message)
    {
        // process WM_CHAR message
        case WM_CHAR :
            triggerCharPress( static_cast<char>(wParam) );
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
            processMouseMove();
            break;

        // mouse wheel
        case WM_MOUSEWHEEL :
            triggerAxisMove( AXIS_2, (short)HIWORD(wParam)/10 );
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
bool GN::input::BasicWinInput::setupWindowHooks()
{
    GN_GUARD;

    // remove previous hook
    removeWindowHooks();

    // setup hooks
    mMsgHook = ::SetWindowsHookEx(
        WH_GETMESSAGE, sMsgHookProc,
        ::GetModuleHandle(0),
        ::GetCurrentThreadId() );
    mCwpHook = ::SetWindowsHookEx(
        WH_CALLWNDPROC, sCwpHookProc,
        ::GetModuleHandle(0),
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
void GN::input::BasicWinInput::removeWindowHooks()
{
    GN_GUARD;

    if( mMsgHook )
    {
        GN_WIN_CHECK( ::UnhookWindowsHookEx( mMsgHook ) );
        mMsgHook = 0;
    }

    if( mCwpHook )
    {
        GN_WIN_CHECK( ::UnhookWindowsHookEx( mCwpHook ) );
        mCwpHook = 0;
    }

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::input::BasicWinInput::captureMouse()
{
    GN_GUARD;

    if( mMouseCapture ) return;
    ::SetCapture( (HWND)mUserOptions.windowHandle );
    ::SetCursor( ::LoadCursor(NULL, IDC_ARROW) );
    mMouseCapture = true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::input::BasicWinInput::releaesMouse()
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
GN::input::BasicWinInput::sMsgHookProc( int nCode, WPARAM wParam, LPARAM lParam )
{
    GN_GUARD;

    BasicWinInput * inst = safeCast<BasicWinInput*>( BasicWinInput::getInstancePtr() );
    GN_ASSERT( inst );

    const MSG * p = (const MSG*)lParam;
    if( nCode >= 0 && p->hwnd == inst->mUserOptions.windowHandle )
    {
        inst->msgHandler( p->message, p->wParam, p->lParam );
    }

    return ::CallNextHookEx( inst->mMsgHook, nCode, wParam, lParam );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
LRESULT CALLBACK
GN::input::BasicWinInput::sCwpHookProc( int nCode, WPARAM wParam, LPARAM lParam )
{
    GN_GUARD;

    BasicWinInput * inst = safeCast<BasicWinInput*>( BasicWinInput::getInstancePtr() );
    GN_ASSERT( inst );

    const CWPSTRUCT * p = (const CWPSTRUCT*)lParam;
    if( nCode >= 0 && p->hwnd == inst->mUserOptions.windowHandle )
    {
        inst->msgHandler( p->message, p->wParam, p->lParam );
    }

    return ::CallNextHookEx( inst->mCwpHook, nCode, wParam, lParam );

    GN_UNGUARD;
}

#endif // GN_WINNT
