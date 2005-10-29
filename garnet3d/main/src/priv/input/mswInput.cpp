#include "pch.h"
#include "mswInput.h"

#if GN_MSWIN

//
//
// -----------------------------------------------------------------------------
GN::input::MswInput::MswInput()
{
    // clear all fields to KEY_NONE
    memset( mKeyMap, KEY_NONE, sizeof(mKeyMap) );

    // setup key map
    #define GNINPUT_DEFINE_KEYCODE( name, dikey, scancode, vkeycode ) \
        if( vkeycode > 0 ) mKeyMap[vkeycode] = name;
    #include "garnet/input/keyCodeMeta.h"
    #undef  GNINPUT_DEFINE_KEYCODE
}

//
//
// -----------------------------------------------------------------------------
void GN::input::MswInput::msgHandler( UINT msg, WPARAM wp, LPARAM lp )
{
    switch( msg )
    {
        // keyboard messages
        case WM_KEYDOWN :
        case WM_KEYUP :
        case WM_SYSKEYDOWN :
        case WM_SYSKEYUP :
            if( KEY_NONE != mKeyMap[wp] )
            {
                // 最高位表示该键是否按下（0：按下，1：抬起）
                triggerKeyPress( mKeyMap[wp], !(lp&0x80000000) );
            }
            break;

            // trigger mouse button events
        case WM_LBUTTONDOWN :
            triggerKeyPress( KEY_MOUSEBTN_0, true );
            break;
        case WM_LBUTTONUP   :
            triggerKeyPress( KEY_MOUSEBTN_0, false );
            break;
        case WM_RBUTTONDOWN :
            triggerKeyPress( KEY_MOUSEBTN_1, true );
            break;
        case WM_RBUTTONUP   :
            triggerKeyPress( KEY_MOUSEBTN_1, false );
            break;
        case WM_MBUTTONDOWN :
            triggerKeyPress( KEY_MOUSEBTN_2, true );
            break;
        case WM_MBUTTONUP   :
            triggerKeyPress( KEY_MOUSEBTN_2, false );
            break;
    }

    // call parent's method
    BasicMswInput::msgHandler( msg, wp, lp );
}

#endif // GN_MSWIN
