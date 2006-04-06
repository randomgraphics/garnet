#include "pch.h"
#include "basicInputXInput.h"

#if defined(HAS_XINPUT) || GN_XENON
#include <XInput.h>

typedef DWORD (WINAPI*XInputGetStateFuncPtr)(DWORD dwUserIndex,XINPUT_STATE* pState);

//
//
// -----------------------------------------------------------------------------
void GN::input::BasicXInput::processInputEvents()
{
    GN_GUARD;

    if( !mXInputGetState ) return;

    // process xinput event
    XINPUT_STATE state;
    if( ERROR_SUCCESS == ((XInputGetStateFuncPtr)mXInputGetState)( 0, &state ) &&
        mXInputPacketNumber != state.dwPacketNumber )
    {
        mXInputPacketNumber = state.dwPacketNumber;
        WORD b = state.Gamepad.wButtons;
        triggerKeyPress( KEY_XB360_UP             , !!(b & XINPUT_GAMEPAD_DPAD_UP)        );
        triggerKeyPress( KEY_XB360_DOWN           , !!(b & XINPUT_GAMEPAD_DPAD_DOWN)      );
        triggerKeyPress( KEY_XB360_LEFT           , !!(b & XINPUT_GAMEPAD_DPAD_LEFT)      );
        triggerKeyPress( KEY_XB360_RIGHT          , !!(b & XINPUT_GAMEPAD_DPAD_RIGHT)     );
        triggerKeyPress( KEY_XB360_START          , !!(b & XINPUT_GAMEPAD_START)          );
        triggerKeyPress( KEY_XB360_BACK           , !!(b & XINPUT_GAMEPAD_BACK)           );
        triggerKeyPress( KEY_XB360_LEFT_THUMB     , !!(b & XINPUT_GAMEPAD_LEFT_THUMB)     );
        triggerKeyPress( KEY_XB360_RIGHT_THUMB    , !!(b & XINPUT_GAMEPAD_RIGHT_THUMB)    );
        triggerKeyPress( KEY_XB360_LEFT_SHOULDER  , !!(b & XINPUT_GAMEPAD_LEFT_SHOULDER)  );
        triggerKeyPress( KEY_XB360_RIGHT_SHOULDER , !!(b & XINPUT_GAMEPAD_RIGHT_SHOULDER) );
        triggerKeyPress( KEY_XB360_A              , !!(b & XINPUT_GAMEPAD_A)              );
        triggerKeyPress( KEY_XB360_B              , !!(b & XINPUT_GAMEPAD_B)              );
        triggerKeyPress( KEY_XB360_X              , !!(b & XINPUT_GAMEPAD_X)              );
        triggerKeyPress( KEY_XB360_Y              , !!(b & XINPUT_GAMEPAD_Y)              );
    }

    GN_UNGUARD;
}

#else

//
//
// -----------------------------------------------------------------------------
void GN::input::BasicXInput::processInputEvents() {}

#endif
