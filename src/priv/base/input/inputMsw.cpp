#include "pch.h"
#include "inputMsw.h"

#if GN_MSWIN && !GN_XENON

//
//
// -----------------------------------------------------------------------------
GN::input::InputMsw::InputMsw()
{
    // clear all fields to KEY_NONE
    memset( mKeyMap, KEY_NONE, sizeof(mKeyMap) );

    // setup key map
    #define GNINPUT_DEFINE_KEYCODE( name, dikey, scancode, vkeycode, xkeysym ) \
        if( vkeycode > 0 ) mKeyMap[(unsigned int)vkeycode] = name;
    #include "garnet/input/keyCodeMeta.h"
    #undef  GNINPUT_DEFINE_KEYCODE
}

//
//
// -----------------------------------------------------------------------------
void GN::input::InputMsw::msgHandler( UINT msg, WPARAM wp, LPARAM lp )
{
    switch( msg )
    {
        // keyboard messages
        case WM_KEYDOWN :
        case WM_SYSKEYDOWN :
        case WM_KEYUP :
        case WM_SYSKEYUP :
            {
                bool down = !(lp&0x80000000); // 最高位表示该键是否按下（0：按下，1：抬起）
                if( VK_MENU == wp )
                {
                    pushKeyPress( KEY_LALT, down );
                    pushKeyPress( KEY_RALT, down );
                }
                else if( VK_CONTROL == wp )
                {
                    pushKeyPress( KEY_LCTRL, down );
                    pushKeyPress( KEY_RCTRL, down );
                }
                else if( VK_SHIFT == wp )
                {
                    pushKeyPress( KEY_LSHIFT, down );
                    pushKeyPress( KEY_RSHIFT, down );
                }
                else if( KEY_NONE != mKeyMap[wp] )
                {
                    pushKeyPress( mKeyMap[wp], down );
                }
            }
            break;

            // push mouse button events
        case WM_LBUTTONDOWN :
            pushKeyPress( KEY_MOUSEBTN_0, true );
            break;
        case WM_LBUTTONUP   :
            pushKeyPress( KEY_MOUSEBTN_0, false );
            break;
        case WM_RBUTTONDOWN :
            pushKeyPress( KEY_MOUSEBTN_1, true );
            break;
        case WM_RBUTTONUP   :
            pushKeyPress( KEY_MOUSEBTN_1, false );
            break;
        case WM_MBUTTONDOWN :
            pushKeyPress( KEY_MOUSEBTN_2, true );
            break;
        case WM_MBUTTONUP   :
            pushKeyPress( KEY_MOUSEBTN_2, false );
            break;
    }

    // call parent's method
    BasicInputMsw::msgHandler( msg, wp, lp );
}

#endif // GN_MSWIN
