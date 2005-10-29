#include "pch.h"
#include "basicInput.h"
#include <algorithm>

// *****************************************************************************
//                  protected functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::input::BasicInput::triggerKeyPress( KeyCode code, bool keydown )
{
    GN_GUARD;

    // ignore redundant keyup(s)
    if( keydown == mKeyStates[code] ) return;

    //GNINPUT_TRACE( "Key press: %s %s", kc2str(code), keydown?"down":"up" );

    // 更新状态键的标志
    if     ( KEY_LCTRL  == code ) mKeyFlags.lctrl  = keydown;
    else if( KEY_RCTRL  == code ) mKeyFlags.rctrl  = keydown;
    else if( KEY_LALT   == code ) mKeyFlags.lalt   = keydown;
    else if( KEY_RALT   == code ) mKeyFlags.ralt   = keydown;
    else if( KEY_LSHIFT == code ) mKeyFlags.lshift = keydown;
    else if( KEY_RSHIFT == code ) mKeyFlags.rshift = keydown;

    // 触发按键信号
    KeyEvent k( mKeyFlags );
    k.code = static_cast<uint8_t>(code);
    k.down = keydown;
    sigKeyPress( k );

    // 更新键盘状态数组
    mKeyStates[code] = keydown;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::input::BasicInput::triggerCharPress( char ch )
{
    GN_GUARD;

    if( (unsigned char)ch < 128 )
    {
        // ASCII character
        //GNINPUT_TRACE( "Char press: %s", StrA(&ch,1).cstr() );
        sigCharPress( ch );
    }
    else if( mHalfWideChar )
    {
        mHalfBytes[1] = ch;
        wchar_t wch[2];
        ::mbstowcs( wch, mHalfBytes, 2 );

        //GNINPUT_TRACE( "Char press: %s", StrA(mHalfBytes,2).cstr() );
        sigCharPress( wch[0] );

        // 清除“半字符”标志
        mHalfWideChar = false;
    }
    else
    {
        mHalfWideChar = true;
        mHalfBytes[0] = ch;
    }

    GN_UNGUARD;
}
