/*
 * This file use to define key-codes
 *
 * NOTE : internelly used by input module, do NOT include it directly in your
 *        own header or source files.
 *
 * syntax : GNINPUT_DEFINE_KEYCODE(
 *                  keyname, dinput-keycode, scan-code, virtual-key-code, x11-KeySym )
 *
 */

//                      按键名称               DInput键码          扫描码        Windows虚拟键码      X11 KeySym
//                      key name               dinput keycode      scan code     windows vkey code

// 控制键
GNINPUT_DEFINE_KEYCODE( ESCAPE               , DIK_ESCAPE        , 0x01         , VK_ESCAPE         , NoSymbol )
GNINPUT_DEFINE_KEYCODE( BACKSPACE            , DIK_BACK          , 0x0E         , VK_BACK           , NoSymbol )
GNINPUT_DEFINE_KEYCODE( TAB                  , DIK_TAB           , 0x0F         , VK_TAB            , NoSymbol )
GNINPUT_DEFINE_KEYCODE( CAPSLOCK             , DIK_CAPSLOCK      , 0x3A         , VK_CAPITAL        , NoSymbol )
GNINPUT_DEFINE_KEYCODE( RETURN               , DIK_RETURN        , 0x1C         , VK_RETURN         , NoSymbol )
GNINPUT_DEFINE_KEYCODE( SPACEBAR             , DIK_SPACE         , 0x39         , VK_SPACE          , NoSymbol )

GNINPUT_DEFINE_KEYCODE( LSHIFT               , DIK_LSHIFT        , 0x2A         , VK_LSHIFT         , NoSymbol )
GNINPUT_DEFINE_KEYCODE( RSHIFT               , DIK_RSHIFT        , 0x36         , VK_RSHIFT         , NoSymbol )
GNINPUT_DEFINE_KEYCODE( LCTRL                , DIK_LCONTROL      , 0x1D         , VK_LCONTROL       , NoSymbol )
GNINPUT_DEFINE_KEYCODE( RCTRL                , DIK_RCONTROL      , 0x11D        , VK_RCONTROL       , NoSymbol )
GNINPUT_DEFINE_KEYCODE( LALT                 , DIK_LMENU         , 0x38         , VK_LMENU          , NoSymbol )
GNINPUT_DEFINE_KEYCODE( RALT                 , DIK_RMENU         , 0x138        , VK_RMENU          , NoSymbol )

GNINPUT_DEFINE_KEYCODE( PAGEUP               , DIK_PGUP          , 0x149        , VK_PRIOR          , NoSymbol )
GNINPUT_DEFINE_KEYCODE( PAGEDOWN             , DIK_PGDN          , 0x151        , VK_NEXT           , NoSymbol )
GNINPUT_DEFINE_KEYCODE( END                  , DIK_END           , 0x14F        , VK_END            , NoSymbol )
GNINPUT_DEFINE_KEYCODE( HOME                 , DIK_HOME          , 0x147        , VK_HOME           , NoSymbol )
GNINPUT_DEFINE_KEYCODE( INSERT               , DIK_INSERT        , 0x152        , VK_INSERT         , NoSymbol )

// Note: DELETE is defined as a macro in MAPIWin.h
GNINPUT_DEFINE_KEYCODE( _DELETE              , DIK_DELETE        , 0x153        , VK_DELETE         , NoSymbol )

GNINPUT_DEFINE_KEYCODE( LEFT                 , DIK_LEFT          , 0x14B        , VK_LEFT           , NoSymbol )
GNINPUT_DEFINE_KEYCODE( UP                   , DIK_UP            , 0x148        , VK_UP             , NoSymbol )
GNINPUT_DEFINE_KEYCODE( RIGHT                , DIK_RIGHT         , 0x14D        , VK_RIGHT          , NoSymbol )
GNINPUT_DEFINE_KEYCODE( DOWN                 , DIK_DOWN          , 0x150        , VK_DOWN           , NoSymbol )

// 数字键
GNINPUT_DEFINE_KEYCODE( _0                   , DIK_0             , 0x2          , '0'               , NoSymbol )
GNINPUT_DEFINE_KEYCODE( _1                   , DIK_1             , 0x3          , '1'               , NoSymbol )
GNINPUT_DEFINE_KEYCODE( _2                   , DIK_2             , 0x4          , '2'               , NoSymbol )
GNINPUT_DEFINE_KEYCODE( _3                   , DIK_3             , 0x5          , '3'               , NoSymbol )
GNINPUT_DEFINE_KEYCODE( _4                   , DIK_4             , 0x6          , '4'               , NoSymbol )
GNINPUT_DEFINE_KEYCODE( _5                   , DIK_5             , 0x7          , '5'               , NoSymbol )
GNINPUT_DEFINE_KEYCODE( _6                   , DIK_6             , 0x8          , '6'               , NoSymbol )
GNINPUT_DEFINE_KEYCODE( _7                   , DIK_7             , 0x9          , '7'               , NoSymbol )
GNINPUT_DEFINE_KEYCODE( _8                   , DIK_8             , 0xA          , '8'               , NoSymbol )
GNINPUT_DEFINE_KEYCODE( _9                   , DIK_9             , 0xB          , '9'               , NoSymbol )

// 字母键
GNINPUT_DEFINE_KEYCODE( A                    , DIK_A             , 0x1E         , 'A'               , NoSymbol )
GNINPUT_DEFINE_KEYCODE( B                    , DIK_B             , 0x30         , 'B'               , NoSymbol )
GNINPUT_DEFINE_KEYCODE( C                    , DIK_C             , 0x2E         , 'C'               , NoSymbol )
GNINPUT_DEFINE_KEYCODE( D                    , DIK_D             , 0x20         , 'D'               , NoSymbol )
GNINPUT_DEFINE_KEYCODE( E                    , DIK_E             , 0x12         , 'E'               , NoSymbol )
GNINPUT_DEFINE_KEYCODE( F                    , DIK_F             , 0x21         , 'F'               , NoSymbol )
GNINPUT_DEFINE_KEYCODE( G                    , DIK_G             , 0x22         , 'G'               , NoSymbol )
GNINPUT_DEFINE_KEYCODE( H                    , DIK_H             , 0x23         , 'H'               , NoSymbol )
GNINPUT_DEFINE_KEYCODE( I                    , DIK_I             , 0x17         , 'I'               , NoSymbol )
GNINPUT_DEFINE_KEYCODE( J                    , DIK_J             , 0x24         , 'J'               , NoSymbol )
GNINPUT_DEFINE_KEYCODE( K                    , DIK_K             , 0x25         , 'K'               , NoSymbol )
GNINPUT_DEFINE_KEYCODE( L                    , DIK_L             , 0x26         , 'L'               , NoSymbol )
GNINPUT_DEFINE_KEYCODE( M                    , DIK_M             , 0x32         , 'M'               , NoSymbol )
GNINPUT_DEFINE_KEYCODE( N                    , DIK_N             , 0x31         , 'N'               , NoSymbol )
GNINPUT_DEFINE_KEYCODE( O                    , DIK_O             , 0x18         , 'O'               , NoSymbol )
GNINPUT_DEFINE_KEYCODE( P                    , DIK_P             , 0x19         , 'P'               , NoSymbol )
GNINPUT_DEFINE_KEYCODE( Q                    , DIK_Q             , 0x10         , 'Q'               , NoSymbol )
GNINPUT_DEFINE_KEYCODE( R                    , DIK_R             , 0x13         , 'R'               , NoSymbol )
GNINPUT_DEFINE_KEYCODE( S                    , DIK_S             , 0x1F         , 'S'               , NoSymbol )
GNINPUT_DEFINE_KEYCODE( T                    , DIK_T             , 0x14         , 'T'               , NoSymbol )
GNINPUT_DEFINE_KEYCODE( U                    , DIK_U             , 0x16         , 'U'               , NoSymbol )
GNINPUT_DEFINE_KEYCODE( V                    , DIK_V             , 0x2F         , 'V'               , NoSymbol )
GNINPUT_DEFINE_KEYCODE( W                    , DIK_W             , 0x11         , 'W'               , NoSymbol )
GNINPUT_DEFINE_KEYCODE( X                    , DIK_X             , 0x2D         , 'X'               , NoSymbol )
GNINPUT_DEFINE_KEYCODE( Y                    , DIK_Y             , 0x15         , 'Y'               , NoSymbol )
GNINPUT_DEFINE_KEYCODE( Z                    , DIK_Z             , 0x2C         , 'Z'               , NoSymbol )

// 标点和符号键
GNINPUT_DEFINE_KEYCODE( MINUS                , DIK_MINUS         , 0x0C         , VK_OEM_MINUS      , NoSymbol ) // -
GNINPUT_DEFINE_KEYCODE( EQUALS               , DIK_EQUALS        , 0x0D         , VK_OEM_PLUS       , NoSymbol ) // =
GNINPUT_DEFINE_KEYCODE( LBRACKET             , DIK_LBRACKET      , 0x1A         , VK_OEM_4          , NoSymbol ) // [
GNINPUT_DEFINE_KEYCODE( RBRACKET             , DIK_RBRACKET      , 0x1B         , VK_OEM_6          , NoSymbol ) // ]
GNINPUT_DEFINE_KEYCODE( SEMICOLON            , DIK_SEMICOLON     , 0x27         , VK_OEM_1          , NoSymbol ) // ;
GNINPUT_DEFINE_KEYCODE( APOSTROPHE           , DIK_APOSTROPHE    , 0x28         , VK_OEM_7          , NoSymbol ) // '
GNINPUT_DEFINE_KEYCODE( GRAVE                , DIK_GRAVE         , 0x29         , VK_OEM_3          , NoSymbol ) // `
GNINPUT_DEFINE_KEYCODE( BACKSLASH            , DIK_BACKSLASH     , 0x2B         , VK_OEM_5          , NoSymbol ) // \ (反斜线)
GNINPUT_DEFINE_KEYCODE( COMMA                , DIK_COMMA         , 0x33         , VK_OEM_COMMA      , NoSymbol ) // ,
GNINPUT_DEFINE_KEYCODE( PERIOD               , DIK_PERIOD        , 0x34         , VK_OEM_PERIOD     , NoSymbol ) // .
GNINPUT_DEFINE_KEYCODE( SLASH                , DIK_SLASH         , 0x35         , VK_OEM_2          , NoSymbol ) // / (正斜线)

// 数字小键盘
GNINPUT_DEFINE_KEYCODE( NUMPAD_0             , DIK_NUMPAD0       , 0x52         , VK_NUMPAD0        , NoSymbol )
GNINPUT_DEFINE_KEYCODE( NUMPAD_1             , DIK_NUMPAD1       , 0x4F         , VK_NUMPAD1        , NoSymbol )
GNINPUT_DEFINE_KEYCODE( NUMPAD_2             , DIK_NUMPAD2       , 0x50         , VK_NUMPAD2        , NoSymbol )
GNINPUT_DEFINE_KEYCODE( NUMPAD_3             , DIK_NUMPAD3       , 0x51         , VK_NUMPAD3        , NoSymbol )
GNINPUT_DEFINE_KEYCODE( NUMPAD_4             , DIK_NUMPAD4       , 0x4B         , VK_NUMPAD4        , NoSymbol )
GNINPUT_DEFINE_KEYCODE( NUMPAD_5             , DIK_NUMPAD5       , 0x4C         , VK_NUMPAD5        , NoSymbol )
GNINPUT_DEFINE_KEYCODE( NUMPAD_6             , DIK_NUMPAD6       , 0x4D         , VK_NUMPAD6        , NoSymbol )
GNINPUT_DEFINE_KEYCODE( NUMPAD_7             , DIK_NUMPAD7       , 0x47         , VK_NUMPAD7        , NoSymbol )
GNINPUT_DEFINE_KEYCODE( NUMPAD_8             , DIK_NUMPAD8       , 0x48         , VK_NUMPAD8        , NoSymbol )
GNINPUT_DEFINE_KEYCODE( NUMPAD_9             , DIK_NUMPAD9       , 0x49         , VK_NUMPAD9        , NoSymbol )
GNINPUT_DEFINE_KEYCODE( NUMPAD_MULTIPLY      , DIK_MULTIPLY      , 0x37         , VK_MULTIPLY       , NoSymbol ) // *
GNINPUT_DEFINE_KEYCODE( NUMPAD_ADD           , DIK_ADD           , 0x4E         , VK_ADD            , NoSymbol ) // +
GNINPUT_DEFINE_KEYCODE( NUMPAD_SUBTRACT      , DIK_SUBTRACT      , 0x4A         , VK_SUBTRACT       , NoSymbol ) // -
GNINPUT_DEFINE_KEYCODE( NUMPAD_DECIMAL       , DIK_DECIMAL       , 0x53         , VK_DECIMAL        , NoSymbol ) // .
GNINPUT_DEFINE_KEYCODE( NUMPAD_DIVIDE        , DIK_DIVIDE        , 0x135        , VK_DIVIDE         , NoSymbol ) // / (正斜线)
GNINPUT_DEFINE_KEYCODE( NUMPAD_ENTER         , DIK_NUMPADENTER   , 0x11C        , VK_SEPARATOR      , NoSymbol )
GNINPUT_DEFINE_KEYCODE( NUMPAD_NUMLOCK       , DIK_NUMLOCK       , 0x145        , VK_NUMLOCK        , NoSymbol )

// 功能键
GNINPUT_DEFINE_KEYCODE( F1                   , DIK_F1            , 0x3B         , VK_F1             , NoSymbol )
GNINPUT_DEFINE_KEYCODE( F2                   , DIK_F2            , 0x3C         , VK_F2             , NoSymbol )
GNINPUT_DEFINE_KEYCODE( F3                   , DIK_F3            , 0x3D         , VK_F3             , NoSymbol )
GNINPUT_DEFINE_KEYCODE( F4                   , DIK_F4            , 0x3E         , VK_F4             , NoSymbol )
GNINPUT_DEFINE_KEYCODE( F5                   , DIK_F5            , 0x3F         , VK_F5             , NoSymbol )
GNINPUT_DEFINE_KEYCODE( F6                   , DIK_F6            , 0x40         , VK_F6             , NoSymbol )
GNINPUT_DEFINE_KEYCODE( F7                   , DIK_F7            , 0x41         , VK_F7             , NoSymbol )
GNINPUT_DEFINE_KEYCODE( F8                   , DIK_F8            , 0x42         , VK_F8             , NoSymbol )
GNINPUT_DEFINE_KEYCODE( F9                   , DIK_F9            , 0x43         , VK_F9             , NoSymbol )
GNINPUT_DEFINE_KEYCODE( F10                  , DIK_F10           , 0x44         , VK_F10            , NoSymbol )
GNINPUT_DEFINE_KEYCODE( F11                  , DIK_F11           , 0x57         , VK_F11            , NoSymbol )
GNINPUT_DEFINE_KEYCODE( F12                  , DIK_F12           , 0x58         , VK_F12            , NoSymbol )

// 其他
GNINPUT_DEFINE_KEYCODE( SYSRQ                , DIK_SYSRQ         , 0x137        , VK_PRINT          , NoSymbol )
GNINPUT_DEFINE_KEYCODE( SCROLL               , DIK_SCROLL        , 0x46         , VK_SCROLL         , NoSymbol )
GNINPUT_DEFINE_KEYCODE( PAUSE                , DIK_PAUSE         , 0x45         , VK_PAUSE          , NoSymbol )

// 鼠标键
GNINPUT_DEFINE_KEYCODE( MOUSEBTN_0           , 0                 , 0            , 0                 , NoSymbol )
GNINPUT_DEFINE_KEYCODE( MOUSEBTN_1           , 0                 , 0            , 0                 , NoSymbol )
GNINPUT_DEFINE_KEYCODE( MOUSEBTN_2           , 0                 , 0            , 0                 , NoSymbol )
GNINPUT_DEFINE_KEYCODE( MOUSEBTN_3           , 0                 , 0            , 0                 , NoSymbol )
GNINPUT_DEFINE_KEYCODE( MOUSEBTN_4           , 0                 , 0            , 0                 , NoSymbol )
GNINPUT_DEFINE_KEYCODE( MOUSEBTN_5           , 0                 , 0            , 0                 , NoSymbol )
GNINPUT_DEFINE_KEYCODE( MOUSEBTN_6           , 0                 , 0            , 0                 , NoSymbol )
GNINPUT_DEFINE_KEYCODE( MOUSEBTN_7           , 0                 , 0            , 0                 , NoSymbol )

// Xbox360游戏控制器
GNINPUT_DEFINE_KEYCODE( XB360_UP             , 0                 , 0            , 0                 , NoSymbol )
GNINPUT_DEFINE_KEYCODE( XB360_DOWN           , 0                 , 0            , 0                 , NoSymbol )
GNINPUT_DEFINE_KEYCODE( XB360_LEFT           , 0                 , 0            , 0                 , NoSymbol )
GNINPUT_DEFINE_KEYCODE( XB360_RIGHT          , 0                 , 0            , 0                 , NoSymbol )
GNINPUT_DEFINE_KEYCODE( XB360_START          , 0                 , 0            , 0                 , NoSymbol )
GNINPUT_DEFINE_KEYCODE( XB360_BACK           , 0                 , 0            , 0                 , NoSymbol )
GNINPUT_DEFINE_KEYCODE( XB360_LEFT_THUMB     , 0                 , 0            , 0                 , NoSymbol )
GNINPUT_DEFINE_KEYCODE( XB360_RIGHT_THUMB    , 0                 , 0            , 0                 , NoSymbol )
GNINPUT_DEFINE_KEYCODE( XB360_LEFT_SHOULDER  , 0                 , 0            , 0                 , NoSymbol )
GNINPUT_DEFINE_KEYCODE( XB360_RIGHT_SHOULDER , 0                 , 0            , 0                 , NoSymbol )
GNINPUT_DEFINE_KEYCODE( XB360_A              , 0                 , 0            , 0                 , NoSymbol )
GNINPUT_DEFINE_KEYCODE( XB360_B              , 0                 , 0            , 0                 , NoSymbol )
GNINPUT_DEFINE_KEYCODE( XB360_X              , 0                 , 0            , 0                 , NoSymbol )
GNINPUT_DEFINE_KEYCODE( XB360_Y              , 0                 , 0            , 0                 , NoSymbol )
