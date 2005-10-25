#include "pch.h"
#include "ntInput.h"

// *****************************************************************************
// Global functions
// *****************************************************************************

#if GN_STATIC
GN::input::Input *
GN::input::createNTInput( const UserOptions & uo )
#else
extern "C" GN_EXPORT GN::input::Input *
GNinputCreateInput( const GN::input::UserOptions & uo )
#endif
{
    GN_GUARD;

    GN::AutoObjPtr<GN::input::NTInput> p( new GN::input::NTInput );
    if( !p->init(uo) ) return 0;
    return p.detatch();

    GN_UNGUARD;
}

// *****************************************************************************
// class NTInput
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::input::NTInput::NTInput()
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
// ----------------------------------------------------------------------------
void GN::input::NTInput::msgHandler( UINT msg, WPARAM wp, LPARAM lp )
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
    BasicWinInput::msgHandler( msg, wp, lp );
}
