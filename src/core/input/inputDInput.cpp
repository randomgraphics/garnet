#include "pch.h"
#include "inputDInput.h"

#ifdef HAS_DINPUT

#if GN_MSVC
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")
#endif

#define IDX_INPUT_BUFFER_SIZE  128

static GN::Logger * sLogger = GN::getLogger("GN.input.DI");

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::input::InputDInput::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT();

    // init dinput stuff
    if( !diInit() ) return failure();

    // success
    return success();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::input::InputDInput::quit()
{
    GN_GUARD;

    unacquire();
    diQuit();

    // standard quit procedure
    GN_STDCLASS_QUIT();

    GN_UNGUARD;
}

// ****************************************************************************
// inherited from parents
// ****************************************************************************

//
//
// ----------------------------------------------------------------------------
bool GN::input::InputDInput::attachToWindow( intptr_t disp, intptr_t window )
{
    GN_GUARD;

    mAttached = false;

    if( !MyParent::attachToWindow(disp,window) ) return false;

    GN_ASSERT( ::IsWindow( (HWND)window ) );

    // set keyboard cooperative level
    GN_DX_CHECK_RETURN(
        mKeyboard->SetCooperativeLevel(
            (HWND)window,
            DISCL_NONEXCLUSIVE | DISCL_FOREGROUND ),
        false );

    // set mouse cooperative level
    GN_DX_CHECK_RETURN(
        mMouse->SetCooperativeLevel(
            (HWND)window,
            DISCL_NONEXCLUSIVE | DISCL_FOREGROUND ),
        false );

    if( !acquire() ) return false;

    // success
    mAttached = true;
    return true;

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
void GN::input::InputDInput::processInputEvents()
{
    GN_GUARD_SLOW;

    MyParent::processInputEvents();

    if( mAttached && mAcquired )
    {
        if( mLost && !acquire() ) return;
        pollKeyboard();
        pollMouse();
    }

    GN_UNGUARD_SLOW;
}

//
//
// ----------------------------------------------------------------------------
void GN::input::InputDInput::msgHandler( UINT msg, WPARAM wp, LPARAM lp )
{
    GN_GUARD_SLOW;

    switch( msg )
    {
        case WM_ACTIVATE :
            if( WA_INACTIVE == wp )
                unacquire();
            else
                acquire();
            break;

        case WM_ENTERMENULOOP:
        case WM_ENTERSIZEMOVE:
            unacquire();
            break;

        case WM_EXITMENULOOP:
        case WM_EXITSIZEMOVE:
            acquire();
            break;

        default:; // do nothing
    }
    MyParent::msgHandler( msg, wp, lp );

    GN_UNGUARD_SLOW;
}

/*****************************************************************************\
                              private functions
\*****************************************************************************/

//
//
// ----------------------------------------------------------------------------
bool GN::input::InputDInput::acquire()
{
    GN_GUARD;

    GN_ASSERT( ok() );

    mAcquired = false;

    GN_DX_CHECK_RETURN( mKeyboard->Acquire(), 0 );
    GN_DX_CHECK_RETURN( mMouse->Acquire(), 0 );

    // success
    GN_TRACE(sLogger)( "Acquire DInput devices" );
    mAcquired = true;
    return true;

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
bool GN::input::InputDInput::unacquire()
{
    GN_GUARD;

    mAcquired = false;

    GN_TRACE(sLogger)( "Unacquire DInput devices" );
    HRESULT rval;
    if( mKeyboard )
    {
        rval = mKeyboard->Unacquire();
        if (rval != DI_OK && rval != DI_NOEFFECT)
        {
            GN_ERROR(sLogger)( "Fail to unacquire keyboard, %s!", getDXErrorInfo(rval) );
            return false;
        }
    }
    if( mMouse )
    {
        rval = mMouse->Unacquire();
        if (rval != DI_OK && rval != DI_NOEFFECT)
        {
            GN_ERROR(sLogger)( "Fail to unacquire mouse, %s!", getDXErrorInfo(rval) );
            return false;
        }
    }

    return true;

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
void GN::input::InputDInput::pollKeyboard()
{
    GN_GUARD_SLOW;

    // retrieve keyboard data
    DIDEVICEOBJECTDATA od[IDX_INPUT_BUFFER_SIZE];
    uint32 elementCount = IDX_INPUT_BUFFER_SIZE;
    GN_DX_CHECK_DO(
        mKeyboard->GetDeviceData( sizeof(DIDEVICEOBJECTDATA), od, (LPDWORD)&elementCount, 0),
        mLost = (DIERR_INPUTLOST==hr||DIERR_NOTACQUIRED==hr); return; );

    for( uint32 i = 0; i < elementCount; ++i )
    {
        uint8 dikeycode = static_cast<uint8>(od[i].dwOfs);
        uint32 dikeydata = od[i].dwData;

        // do nothing with unrecognized keys
        if( KeyCode::NONE != mKeyMap[dikeycode] )
        {
            // trigger key_press signal
            triggerKeyPress( mKeyMap[dikeycode], !!(dikeydata & 0x80) );
        }
    }

    GN_UNGUARD_SLOW;
}

//
//
// ----------------------------------------------------------------------------
void GN::input::InputDInput::pollMouse()
{
    GN_GUARD_SLOW;

    //NOTE: mouse movement has already be processed by basicMsgInput

    // process mouse action
    uint32 i;
    DIDEVICEOBJECTDATA od[IDX_INPUT_BUFFER_SIZE];
    uint32 elementCount = IDX_INPUT_BUFFER_SIZE;
    GN_DX_CHECK_DO(
        mMouse->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), od, (LPDWORD)&elementCount, 0),
        mLost = (DIERR_INPUTLOST==hr||DIERR_NOTACQUIRED==hr); return; );

    for( i = 0; i < elementCount; ++i )
    {
        int offset = od[i].dwOfs;
        switch(offset)
        {
            case DIMOFS_BUTTON0:
                triggerKeyPress( KeyCode::MOUSEBTN_0, !!(od[i].dwData & 0x80) );
                break;
            case DIMOFS_BUTTON1:
                triggerKeyPress( KeyCode::MOUSEBTN_1, !!(od[i].dwData & 0x80) );
                break;
            case DIMOFS_BUTTON2:
                triggerKeyPress( KeyCode::MOUSEBTN_2, !!(od[i].dwData & 0x80) );
                break;
            //case DIMOFS_X:
            //    triggerAxisMove( MOUSE_AXIS_0, od[i].dwData );
            //    break;
            //case DIMOFS_Y:
            //    triggerAxisMove( MOUSE_AXIS_1, od[i].dwData );
            //    break;
            //case DIMOFS_Z:
            //    triggerAxisMove( MOUSE_AXIS_2, od[i].dwData );
            //    break;
        }
    }

    GN_UNGUARD_SLOW;
}

//
//
// ----------------------------------------------------------------------------
void GN::input::InputDInput::buildKeyMap()
{
    // clear all field to KeyCode::NONE
    memset( mKeyMap, KeyCode::NONE, sizeof(mKeyMap) );

    // setup key map
    #define GNINPUT_DEFINE_KEYCODE( name, dikey, scancode, vkeycode, xkeysym ) \
        if( dikey > 0 ) mKeyMap[dikey] = KeyCode::name;
    #include "garnet/input/keyCodeMeta.h"
    #undef  GNINPUT_DEFINE_KEYCODE
}

//
//
// ----------------------------------------------------------------------------
bool GN::input::InputDInput::diInit()
{
    GN_GUARD;

    // Load dinput library
    GN_MSW_CHECK( ( mLibrary = ::LoadLibraryA( "dinput8.dll" ) ) );

    // Load symbol
    typedef HRESULT (WINAPI*DICreator)(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID *ppvOut, LPUNKNOWN punkOuter);
    DICreator dc;
    GN_MSW_CHECK( ( dc = (DICreator)::GetProcAddress( mLibrary, "DirectInput8Create" ) ) );

    // create dinput
    GN_DX_CHECK_RETURN(
        dc( (HINSTANCE)GetModuleHandle(0), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&mDInput, 0 ),
        false );

    // init keyboard
    if( !kbInit() ) return false;

    // init mouse
    if( !mouseInit() ) return false;

    // success
    return true;

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
void GN::input::InputDInput::diQuit()
{
    GN_GUARD;

    // release dinput stuff
    safeRelease(mMouse);
    safeRelease(mKeyboard);
    safeRelease(mDInput);
    if( mLibrary ) ::FreeLibrary( mLibrary ), mLibrary = 0;

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
bool GN::input::InputDInput::kbInit()
{
    GN_GUARD;

    // create keyboard device
    GN_DX_CHECK_RETURN( mDInput->CreateDevice(GUID_SysKeyboard, &mKeyboard, 0), 0 );

    // set keyboard data format
    GN_DX_CHECK_RETURN( mKeyboard->SetDataFormat(&c_dfDIKeyboard), 0 );

    // set to buffer mode
    DIPROPDWORD dipdw;
    dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj        = 0;
    dipdw.diph.dwHow        = DIPH_DEVICE;
    dipdw.dwData            = IDX_INPUT_BUFFER_SIZE;
    GN_DX_CHECK_RETURN( mKeyboard->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph), 0 );

    // success
    return true;

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
bool GN::input::InputDInput::mouseInit()
{
    GN_GUARD;

    // create mouse device
    GN_DX_CHECK_RETURN( mDInput->CreateDevice(GUID_SysMouse, &mMouse, 0), false );

    // set mouse data format
    GN_DX_CHECK_RETURN( mMouse->SetDataFormat(&c_dfDIMouse), false );

//* // set to buffer mode
    DIPROPDWORD dipdw;
    dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj        = 0;
    dipdw.diph.dwHow        = DIPH_DEVICE;
    dipdw.dwData            = IDX_INPUT_BUFFER_SIZE;
    GN_DX_CHECK_RETURN( mMouse->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph), false );
/**/

    // success
    return true;

    GN_UNGUARD;
}

#endif // HAS_DINPUT
