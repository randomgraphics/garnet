#include "pch.h"
#include "basicInputXInput.h"

#if defined(HAS_XINPUT) || GN_XENON

typedef DWORD (WINAPI*XInputGetStateFuncPtr)(DWORD dwUserIndex,XINPUT_STATE* pState);

//
//
// -----------------------------------------------------------------------------
void GN::input::BasicXInput::processInputEvents()
{
    GN_GUARD;

    if( !mXInputGetState ) return;

    // process gamepad key
    XINPUT_STATE state;
    XINPUT_GAMEPAD & pad = state.Gamepad;
    if( ERROR_SUCCESS == ((XInputGetStateFuncPtr)mXInputGetState)( 0, &state ) &&
        mXInputPacketNumber != state.dwPacketNumber )
    {
        mXInputPacketNumber = state.dwPacketNumber;

        // update keys
        WORD b = pad.wButtons;
        triggerKeyPress( KeyCode::XB360_UP             , !!(b & XINPUT_GAMEPAD_DPAD_UP)        );
        triggerKeyPress( KeyCode::XB360_DOWN           , !!(b & XINPUT_GAMEPAD_DPAD_DOWN)      );
        triggerKeyPress( KeyCode::XB360_LEFT           , !!(b & XINPUT_GAMEPAD_DPAD_LEFT)      );
        triggerKeyPress( KeyCode::XB360_RIGHT          , !!(b & XINPUT_GAMEPAD_DPAD_RIGHT)     );
        triggerKeyPress( KeyCode::XB360_START          , !!(b & XINPUT_GAMEPAD_START)          );
        triggerKeyPress( KeyCode::XB360_BACK           , !!(b & XINPUT_GAMEPAD_BACK)           );
        triggerKeyPress( KeyCode::XB360_LEFT_THUMB     , !!(b & XINPUT_GAMEPAD_LEFT_THUMB)     );
        triggerKeyPress( KeyCode::XB360_RIGHT_THUMB    , !!(b & XINPUT_GAMEPAD_RIGHT_THUMB)    );
        triggerKeyPress( KeyCode::XB360_LEFT_SHOULDER  , !!(b & XINPUT_GAMEPAD_LEFT_SHOULDER)  );
        triggerKeyPress( KeyCode::XB360_RIGHT_SHOULDER , !!(b & XINPUT_GAMEPAD_RIGHT_SHOULDER) );
        triggerKeyPress( KeyCode::XB360_A              , !!(b & XINPUT_GAMEPAD_A)              );
        triggerKeyPress( KeyCode::XB360_B              , !!(b & XINPUT_GAMEPAD_B)              );
        triggerKeyPress( KeyCode::XB360_X              , !!(b & XINPUT_GAMEPAD_X)              );
        triggerKeyPress( KeyCode::XB360_Y              , !!(b & XINPUT_GAMEPAD_Y)              );

        // update axises
        triggerAxisMoveAbs( Axis::XB360_LEFT_TRIGGER  , (int)pad.bLeftTrigger, XINPUT_GAMEPAD_TRIGGER_THRESHOLD );
        triggerAxisMoveAbs( Axis::XB360_RIGHT_TRIGGER , (int)pad.bRightTrigger, XINPUT_GAMEPAD_TRIGGER_THRESHOLD );
        triggerAxisMoveAbs( Axis::XB360_THUMB_LX      , (int)pad.sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE );
        triggerAxisMoveAbs( Axis::XB360_THUMB_LY      , (int)pad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE );
        triggerAxisMoveAbs( Axis::XB360_THUMB_RX      , (int)pad.sThumbRX, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE );
        triggerAxisMoveAbs( Axis::XB360_THUMB_RY      , (int)pad.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE );
    }

    GN_UNGUARD;
}

#else

//
//
// -----------------------------------------------------------------------------
void GN::input::BasicXInput::processInputEvents() {}

#endif
