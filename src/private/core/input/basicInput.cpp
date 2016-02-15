#include "pch.h"
#include "basicInput.h"
#include <algorithm>

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::input::KeyEvent GN::input::BasicInput::popLastKeyEvent()
{
    mKeyEventQueueMutex.lock();

    KeyEvent k;

    if( mKeyEventQueue.empty() )
    {
        k.u16 = 0;
    }
    else
    {
        k = mKeyEventQueue.front();
        mKeyEventQueue.pop();
    }

    mKeyEventQueueMutex.unlock();

    return k;
}

// *****************************************************************************
// protected functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
void GN::input::BasicInput::triggerKeyPress( KeyCode code, bool keydown )
{
    GN_GUARD;

    // ignore redundant keyup(s)
    if( keydown == mKeyboardStatus[code].down ) return;

    //GN_TRACE( "Key press: %s %s", keyCode2String(code), keydown?"down":"up" );

    // 更新状态键的标志
    mKeyFlags.down = keydown;
    if     ( KeyCode::LCTRL  == code ) mKeyFlags.lctrl  = keydown;
    else if( KeyCode::RCTRL  == code ) mKeyFlags.rctrl  = keydown;
    else if( KeyCode::LALT   == code ) mKeyFlags.lalt   = keydown;
    else if( KeyCode::RALT   == code ) mKeyFlags.ralt   = keydown;
    else if( KeyCode::LSHIFT == code ) mKeyFlags.lshift = keydown;
    else if( KeyCode::RSHIFT == code ) mKeyFlags.rshift = keydown;

    KeyEvent k( code, mKeyFlags );

    // 更新键盘状态数组
    mKeyboardStatus[code] = k.status;

    //update last key event
    mKeyEventQueueMutex.lock();
    if( mKeyEventQueue.size() >= 32 ) // buffer 32 key events
    {
        mKeyEventQueue.pop();
    }
    mKeyEventQueue.push( k );
    mKeyEventQueueMutex.unlock();

    // 触发按键信号
    sigKeyPress( k );

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
        //GN_TRACE( "Char press: %s", StrA(&ch,1).rawptr() );
        sigCharPress( ch );
    }
    else if( mHalfWideChar )
    {
        mHalfBytes[1] = ch;
        wchar_t wch[2];
        mbs2wcs( wch, 2, mHalfBytes, 2 );

        //GN_TRACE( "Char press: %s", StrA(mHalfBytes,2).rawptr() );
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
