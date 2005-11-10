#include "pch.h"
#include "diInput.h"

#if GN_MSWIN

#if GN_MSVC
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxerr9.lib" )
#endif

#define IDX_INPUT_BUFFER_SIZE  128

// *****************************************************************************
// Initialize and shutdown
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::input::DIInput::init()
{
    GN_GUARD;

    // standard init procedure
    GN_STDCLASS_INIT( GN::input::DIInput, () );

    // init dinput stuff
    if( !diInit() ) { quit(); return selfOK(); }

    // success
    return selfOK();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::input::DIInput::quit()
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
bool GN::input::DIInput::attachToWindow( HandleType disp, HandleType window )
{
    GN_GUARD;

    mAttached = false;

    if( !MyParent::attachToWindow(disp,window) ) return false;

    GN_ASSERT( ::IsWindow( (HWND)window ) );

    // set keyboard cooperative level
    DI_CHECK_RV(
        mKeyboard->SetCooperativeLevel(
            (HWND)window,
            DISCL_NONEXCLUSIVE | DISCL_FOREGROUND ),
        false );

    // set mouse cooperative level
    DI_CHECK_RV(
        mMouse->SetCooperativeLevel(
            (HWND)window,
            DISCL_NONEXCLUSIVE | DISCL_FOREGROUND ),
        false );

    // success
    mAttached = true;
    return true;

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
void GN::input::DIInput::processInputEvents()
{
    GN_GUARD_SLOW;

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
void GN::input::DIInput::msgHandler( UINT msg, WPARAM wp, LPARAM lp )
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
bool GN::input::DIInput::acquire()
{
    GN_GUARD;

    GN_ASSERT( selfOK() );

    mAcquired = false;

    DI_CHECK_RV( mKeyboard->Acquire(), 0 );
    DI_CHECK_RV( mMouse->Acquire(), 0 );

    // success
    GNINPUT_INFO( "Acquire DInput devices" );
    mAcquired = true;
    return true;

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
bool GN::input::DIInput::unacquire()
{
    GN_GUARD;

    mAcquired = false;

    GNINPUT_INFO( "Unacquire DInput devices" );
    HRESULT rval;
    if( mKeyboard )
    {
        rval = mKeyboard->Unacquire();
        if (rval != DI_OK && rval != DI_NOEFFECT)
        {
            GNINPUT_ERROR( "Fail to unacquire keyboard, %s!", DXGetErrorString9A(rval) );
            return false;
        }
    }
    if( mMouse )
    {
        rval = mMouse->Unacquire();
        if (rval != DI_OK && rval != DI_NOEFFECT)
        {
            GNINPUT_ERROR( "Fail to unacquire mouse, %s!", DXGetErrorString9A(rval) );
            return false;
        }
    }

    return true;

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
void GN::input::DIInput::pollKeyboard()
{
    GN_GUARD_SLOW;

    // retrieve keyboard data
    DIDEVICEOBJECTDATA od[IDX_INPUT_BUFFER_SIZE];
    uint32_t elementCount = IDX_INPUT_BUFFER_SIZE;
    DI_CHECK_DO(
        mKeyboard->GetDeviceData( sizeof(DIDEVICEOBJECTDATA), od, (LPDWORD)&elementCount, 0),
        mLost = (DIERR_INPUTLOST==rr||DIERR_NOTACQUIRED==rr); return; );

    for( uint32_t i = 0; i < elementCount; ++i )
    {
        uint8_t dikeycode = static_cast<uint8_t>(od[i].dwOfs);
        uint32_t dikeydata = od[i].dwData;

        // do nothing with unrecognized keys
        if( KEY_NONE != mKeyMap[dikeycode] )
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
void GN::input::DIInput::pollMouse()
{
    GN_GUARD_SLOW;

    //NOTE: mouse movement has already be processed by basicMsgInput

    // process mouse action
    uint32_t i;
    DIDEVICEOBJECTDATA od[IDX_INPUT_BUFFER_SIZE];
    uint32_t elementCount = IDX_INPUT_BUFFER_SIZE;
    DI_CHECK_DO(
        mMouse->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), od, (LPDWORD)&elementCount, 0),
        mLost = (DIERR_INPUTLOST==rr||DIERR_NOTACQUIRED==rr); return; );

    for( i = 0; i < elementCount; ++i )
    {
        int offset = od[i].dwOfs;
        switch(offset)
        {
            case DIMOFS_BUTTON0:
                triggerKeyPress( KEY_MOUSEBTN_0, !!(od[i].dwData & 0x80) );
                break;
            case DIMOFS_BUTTON1:
                triggerKeyPress( KEY_MOUSEBTN_1, !!(od[i].dwData & 0x80) );
                break;
            case DIMOFS_BUTTON2:
                triggerKeyPress( KEY_MOUSEBTN_2, !!(od[i].dwData & 0x80) );
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
void GN::input::DIInput::buildKeyMap()
{
    GN_GUARD;

    // clear all field to KEY_NONE
    memset( mKeyMap, KEY_NONE, sizeof(mKeyMap) );

    // setup key map
    #define GNINPUT_DEFINE_KEYCODE( name, dikey, scancode, vkeycode ) \
        if( dikey > 0 ) mKeyMap[dikey] = name;
    #include "garnet/input/keyCodeMeta.h"
    #undef  GNINPUT_DEFINE_KEYCODE

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
bool GN::input::DIInput::diInit()
{
    GN_GUARD;

    // Load dinput library
    GN_MSW_CHECK( ( mLibrary = ::LoadLibraryA( "dinput8.dll" ) ) );

    // Load symbol
    typedef HRESULT (WINAPI*DICreator)(HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID *ppvOut, LPUNKNOWN punkOuter);
    DICreator dc;
    GN_MSW_CHECK( ( dc = (DICreator)::GetProcAddress( mLibrary, "DirectInput8Create" ) ) );

    // create dinput
    DI_CHECK_RV(
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
void GN::input::DIInput::diQuit()
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
bool GN::input::DIInput::kbInit()
{
    GN_GUARD;

    // create keyboard device
    DI_CHECK_RV( mDInput->CreateDevice(GUID_SysKeyboard, &mKeyboard, 0), 0 );

    // set keyboard data format
    DI_CHECK_RV( mKeyboard->SetDataFormat(&c_dfDIKeyboard), 0 );

    // set to buffer mode
    DIPROPDWORD dipdw;
    dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj        = 0;
    dipdw.diph.dwHow        = DIPH_DEVICE;
    dipdw.dwData            = IDX_INPUT_BUFFER_SIZE;
    DI_CHECK_RV( mKeyboard->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph), 0 );

    // success
    return true;

    GN_UNGUARD;
}

//
//
// ----------------------------------------------------------------------------
bool GN::input::DIInput::mouseInit()
{
    GN_GUARD;

    // create mouse device
    DI_CHECK_RV( mDInput->CreateDevice(GUID_SysMouse, &mMouse, 0), false );

    // set mouse data format
    DI_CHECK_RV( mMouse->SetDataFormat(&c_dfDIMouse), false );

//* // set to buffer mode
    DIPROPDWORD dipdw;
    dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj        = 0;
    dipdw.diph.dwHow        = DIPH_DEVICE;
    dipdw.dwData            = IDX_INPUT_BUFFER_SIZE;
    DI_CHECK_RV( mMouse->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph), false );
/**/

    // success
    return true;

    GN_UNGUARD;
}

#endif // GN_MSWIN
