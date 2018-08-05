#include "pch.h"
#include "basicInputMsw.h"

#if GN_WINPC

static GN::Logger * sLogger = GN::getLogger("GN.input.BasicInputMsw");

// *****************************************************************************
//    init / quit functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::input::BasicInputMsw::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::input::BasicInputMsw, () );

    // setup xinput function pointers
    if( !setupXInputFunctionPointers() ) return failure();

    // initialize internal mouse position
    POINT pos;
    GN_MSW_CHECK_RETURN( ::GetCursorPos( &pos ), failure() );
    updateMousePosition( pos.x, pos.y, false );

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::input::BasicInputMsw::quit()
{
    GN_GUARD;

    // remove windows hook
    removeWindowHooks();

    // release xinput library
    if( mXInputLibrary ) ::FreeLibrary( mXInputLibrary ), mXInputLibrary = 0;

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
void GN::input::BasicInputMsw::processInputEvents()
{
    BasicXInput::processInputEvents();

    std::lock_guard<std::mutex> mutex( mEventQueueMutex );

    while( !mInputEvents.empty() )
    {
        const InputEvent & e = mInputEvents.front();

        switch( e.type )
        {
            case 0  : triggerKeyPress( e.key.code, e.key.down ); break;
            case 1  : triggerCharPress( e.ch ); break;
            case 2  : triggerAxisMove( e.axis.a, e.axis.d ); break;
            case 3  : triggerAxisMoveAbs( e.axis.a, e.axis.d, 0 ); break;
            default : GN_UNEXPECTED();
        }

        mInputEvents.pop();
    }
}

//
//
// -----------------------------------------------------------------------------
bool GN::input::BasicInputMsw::attachToWindow( intptr_t /*display*/, intptr_t window )
{
    GN_GUARD;

    HWND hwnd = (HWND)window;

    // check window handle
    if( !::IsWindow(hwnd) )
    {
        GN_ERROR(sLogger)( "Window handle is not valid!" );
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

    // setup windows hooks
    if( !setupWindowHooks( hwnd ) ) return false;

    // success
    mWindow = hwnd;
    GN_VERBOSE(sLogger)( "Attach to window 0x%X", mWindow );
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::input::BasicInputMsw::getMousePosition( int & x, int & y ) const
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
void GN::input::BasicInputMsw::msgHandler( UINT message, WPARAM wp, LPARAM )
{
    GN_GUARD;

    switch(message)
    {
        // process WM_CHAR message
        case WM_CHAR :
            pushCharPress( (char)wp );
            break;

        // capture mouse when mouse-button pressed
        case WM_LBUTTONDOWN :
            captureMouse();
            break;
        case WM_LBUTTONUP   :
            releaseMouse();
            break;
        case WM_RBUTTONDOWN :
            captureMouse();
            break;
        case WM_RBUTTONUP   :
            releaseMouse();
            break;
        case WM_MBUTTONDOWN :
            captureMouse();
            break;
        case WM_MBUTTONUP   :
            releaseMouse();
            break;

        // mouse move
        case WM_MOUSEMOVE :
            {
                POINT pos;
                GN_MSW_CHECK( ::GetCursorPos( &pos ) );
                pushAxisAbs( Axis::MOUSE_X, pos.x );
                pushAxisAbs( Axis::MOUSE_Y, pos.y );
            }
            break;

        // mouse wheel
        case WM_MOUSEWHEEL :
            pushAxisMove( Axis::MOUSE_WHEEL_0, (short)HIWORD(wp)/10 );
            break;

        case WM_DESTROY :
            // detach from the window
            removeWindowHooks();
            // release mouse capture
            releaseMouse();
            break;
    }

    GN_UNGUARD;
}

// *****************************************************************************
//                             private functions
// *****************************************************************************

#define EVENT_QUEUE_SIZE 32

//
//
// -----------------------------------------------------------------------------
void GN::input::BasicInputMsw::pushKeyPress( KeyCode key, bool keydown )
{
    InputEvent e;
    e.type = 0;
    e.key.code = key;
    e.key.down = keydown;

    std::lock_guard<std::mutex> mutex( mEventQueueMutex );
    if( mInputEvents.size() >= EVENT_QUEUE_SIZE )
    {
        mInputEvents.pop();
    }
    mInputEvents.push( e );
}

//
//
// -----------------------------------------------------------------------------
void GN::input::BasicInputMsw::pushCharPress( char ch )
{
    InputEvent e;
    e.type = 1;
    e.ch = ch;

    std::lock_guard<std::mutex> mutex( mEventQueueMutex );
    if( mInputEvents.size() >= EVENT_QUEUE_SIZE )
    {
        mInputEvents.pop();
    }
    mInputEvents.push( e );
}

//
//
// -----------------------------------------------------------------------------
void GN::input::BasicInputMsw::pushAxisMove( Axis axis, int distance )
{
    InputEvent e;
    e.type = 2;
    e.axis.a = axis;
    e.axis.d = distance;

    std::lock_guard<std::mutex> mutex( mEventQueueMutex );
    if( mInputEvents.size() >= EVENT_QUEUE_SIZE )
    {
        mInputEvents.pop();
    }
    mInputEvents.push( e );
}

//
//
// -----------------------------------------------------------------------------
void GN::input::BasicInputMsw::pushAxisAbs( Axis axis, int pos )
{
    InputEvent e;
    e.type = 3;
    e.axis.a = axis;
    e.axis.d = pos;

    std::lock_guard<std::mutex> mutex( mEventQueueMutex );
    if( mInputEvents.size() >= EVENT_QUEUE_SIZE )
    {
        mInputEvents.pop();
    }
    mInputEvents.push( e );
}

// *****************************************************************************
//                             private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::input::BasicInputMsw::setupXInputFunctionPointers()
{
#ifdef HAS_XINPUT
    GN_GUARD;

    GN_ASSERT( !mXInputLibrary && !mXInputGetState );

    mXInputLibrary = ::LoadLibraryA( XINPUT_DLL_A );
    if( 0 == mXInputLibrary ) return true;

    mXInputGetState = ::GetProcAddress( mXInputLibrary, "XInputGetState" );

    // success
    return true;

    GN_UNGUARD;
#else
    return true;
#endif
}

//
//
// -----------------------------------------------------------------------------
bool GN::input::BasicInputMsw::setupWindowHooks( HWND hwnd )
{
    GN_GUARD;

    // remove previous hook
    removeWindowHooks();

    // get thread of the window
    DWORD threadID = GetWindowThreadProcessId( hwnd, 0 );

    // setup hooks
    mMsgHook = ::SetWindowsHookEx(
        WH_GETMESSAGE, sMsgHookProc,
        0,
        threadID );
    mCwpHook = ::SetWindowsHookEx(
        WH_CALLWNDPROC, sCwpHookProc,
        0,
        threadID );
    if( 0 == mMsgHook || 0 == mCwpHook )
    {
        GN_ERROR(sLogger)( getWin32LastErrorInfo() );
        return false;
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::input::BasicInputMsw::removeWindowHooks()
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
void GN::input::BasicInputMsw::captureMouse()
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
void GN::input::BasicInputMsw::releaseMouse()
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
GN::input::BasicInputMsw::sMsgHookProc( int nCode, WPARAM wp, LPARAM lParam )
{
    GN_GUARD;

    BasicInputMsw * inst = safeCastPtr<BasicInputMsw>(gInputPtr);
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
GN::input::BasicInputMsw::sCwpHookProc( int nCode, WPARAM wp, LPARAM lParam )
{
    GN_GUARD;

    BasicInputMsw * inst = safeCastPtr<BasicInputMsw>(gInputPtr);
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

#endif // GN_WINPC
