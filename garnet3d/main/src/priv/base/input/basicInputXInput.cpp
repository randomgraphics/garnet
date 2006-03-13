#include "pch.h"
#include "basicInputXInput.h"

#if GN_MSWIN



#if GN_XENON

// Xenon platform always has XInput.
#ifndef HAS_XINPUT
#define HAS_XINPUT
#endif

#elif defined(HAS_XINPUT)

#include <XInput.h>
#if GN_MSVC
#pragma comment( lib, "XInput.lib" )
#endif

#endif // GN_XENON


//
//
// -----------------------------------------------------------------------------
void GN::input::BasicXInput::processInputEvents()
{
#ifdef HAS_XINPUT
    GN_GUARD;

    // process xinput event
    XINPUT_STATE state;
    if( ERROR_SUCCESS == XInputGetState( 0, &state ) &&
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
#endif
}

#endif // GN_MSWIN
