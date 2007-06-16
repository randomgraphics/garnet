#include "pch.h"
#include "basicInput.h"
#if !GN_ENABLE_INLINE
#include "basicInput.inl"
#endif
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

    //GN_TRACE( "Key press: %s %s", kc2str(code), keydown?"down":"up" );

    // ����״̬���ı�־
    mKeyFlags.down = keydown;
    if     ( KEY_LCTRL  == code ) mKeyFlags.lctrl  = keydown;
    else if( KEY_RCTRL  == code ) mKeyFlags.rctrl  = keydown;
    else if( KEY_LALT   == code ) mKeyFlags.lalt   = keydown;
    else if( KEY_RALT   == code ) mKeyFlags.ralt   = keydown;
    else if( KEY_LSHIFT == code ) mKeyFlags.lshift = keydown;
    else if( KEY_RSHIFT == code ) mKeyFlags.rshift = keydown;

    KeyEvent k( code, mKeyFlags );

    // ���¼���״̬����
    mKeyboardStatus[code] = k.status;

    //update last key event
    mKeyEventQueueMutex.lock();
    if( mKeyEventQueue.size() >= 32 ) // buffer 32 key events
    {
        mKeyEventQueue.pop();
    }
    mKeyEventQueue.push( k );
    mKeyEventQueueMutex.unlock();

    // ���������ź�
    gSigKeyPress( k );

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
        //GN_TRACE( "Char press: %s", StrA(&ch,1).cptr() );
        gSigCharPress( ch );
    }
    else if( mHalfWideChar )
    {
        mHalfBytes[1] = ch;
        wchar_t wch[2];
        mbs2wcs( wch, 2, mHalfBytes, 2 );

        //GN_TRACE( "Char press: %s", StrA(mHalfBytes,2).cptr() );
        gSigCharPress( wch[0] );

        // ��������ַ�����־
        mHalfWideChar = false;
    }
    else
    {
        mHalfWideChar = true;
        mHalfBytes[0] = ch;
    }

    GN_UNGUARD;
}