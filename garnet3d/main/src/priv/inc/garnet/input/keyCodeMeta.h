/*
 * This file use to define key-codes
 *
 * NOTE : internelly used by input module, do NOT include it directly in your
 *        own header or source files.
 *
 * syntax : GNINPUT_DEFINE_KEYCODE(
 *                  keyname, dinput-keycode, scan-code, virtual-key-code )
 *
 */

//                      按键名称                 DInput键码          扫描码        Windows虚拟键码
//                      key name                 dinput keycode      scan code     windows vkey code

// 控制键
GNINPUT_DEFINE_KEYCODE( KEY_ESCAPE             , DIK_ESCAPE        , 0x01         , VK_ESCAPE    )
GNINPUT_DEFINE_KEYCODE( KEY_BACKSPACE          , DIK_BACK          , 0x0E         , VK_BACK      )
GNINPUT_DEFINE_KEYCODE( KEY_TAB                , DIK_TAB           , 0x0F         , VK_TAB       )
GNINPUT_DEFINE_KEYCODE( KEY_CAPSLOCK           , DIK_CAPSLOCK      , 0x3A         , VK_CAPITAL   )
GNINPUT_DEFINE_KEYCODE( KEY_RETURN             , DIK_RETURN        , 0x1C         , VK_RETURN    )
GNINPUT_DEFINE_KEYCODE( KEY_SPACEBAR           , DIK_SPACE         , 0x39         , VK_SPACE     )

GNINPUT_DEFINE_KEYCODE( KEY_LSHIFT             , DIK_LSHIFT        , 0x2A         , VK_LSHIFT    )
GNINPUT_DEFINE_KEYCODE( KEY_RSHIFT             , DIK_RSHIFT        , 0x36         , VK_RSHIFT    )
GNINPUT_DEFINE_KEYCODE( KEY_LCTRL              , DIK_LCONTROL      , 0x1D         , VK_LCONTROL  )
GNINPUT_DEFINE_KEYCODE( KEY_RCTRL              , DIK_RCONTROL      , 0x11D        , VK_RCONTROL  )
GNINPUT_DEFINE_KEYCODE( KEY_LALT               , DIK_LMENU         , 0x38         , VK_LMENU     )
GNINPUT_DEFINE_KEYCODE( KEY_RALT               , DIK_RMENU         , 0x138        , VK_RMENU     )

GNINPUT_DEFINE_KEYCODE( KEY_PAGEUP             , DIK_PGUP          , 0x149        , VK_PRIOR     )
GNINPUT_DEFINE_KEYCODE( KEY_PAGEDOWN           , DIK_PGDN          , 0x151        , VK_NEXT      )
GNINPUT_DEFINE_KEYCODE( KEY_END                , DIK_END           , 0x14F        , VK_END       )
GNINPUT_DEFINE_KEYCODE( KEY_HOME               , DIK_HOME          , 0x147        , VK_HOME      )
GNINPUT_DEFINE_KEYCODE( KEY_INSERT             , DIK_INSERT        , 0x152        , VK_INSERT    )
GNINPUT_DEFINE_KEYCODE( KEY_DELETE             , DIK_DELETE        , 0x153        , VK_DELETE    )

GNINPUT_DEFINE_KEYCODE( KEY_LEFT               , DIK_LEFT          , 0x14B        , VK_LEFT      )
GNINPUT_DEFINE_KEYCODE( KEY_UP                 , DIK_UP            , 0x148        , VK_UP        )
GNINPUT_DEFINE_KEYCODE( KEY_RIGHT              , DIK_RIGHT         , 0x14D        , VK_RIGHT     )
GNINPUT_DEFINE_KEYCODE( KEY_DOWN               , DIK_DOWN          , 0x150        , VK_DOWN      )

// 数字键
GNINPUT_DEFINE_KEYCODE( KEY_0                  , DIK_0             , 0x2          , '0'          )
GNINPUT_DEFINE_KEYCODE( KEY_1                  , DIK_1             , 0x3          , '1'          )
GNINPUT_DEFINE_KEYCODE( KEY_2                  , DIK_2             , 0x4          , '2'          )
GNINPUT_DEFINE_KEYCODE( KEY_3                  , DIK_3             , 0x5          , '3'          )
GNINPUT_DEFINE_KEYCODE( KEY_4                  , DIK_4             , 0x6          , '4'          )
GNINPUT_DEFINE_KEYCODE( KEY_5                  , DIK_5             , 0x7          , '5'          )
GNINPUT_DEFINE_KEYCODE( KEY_6                  , DIK_6             , 0x8          , '6'          )
GNINPUT_DEFINE_KEYCODE( KEY_7                  , DIK_7             , 0x9          , '7'          )
GNINPUT_DEFINE_KEYCODE( KEY_8                  , DIK_8             , 0xA          , '8'          )
GNINPUT_DEFINE_KEYCODE( KEY_9                  , DIK_9             , 0xB          , '9'          )

// 字母键
GNINPUT_DEFINE_KEYCODE( KEY_A                  , DIK_A             , 0x1E         , 'A'          )
GNINPUT_DEFINE_KEYCODE( KEY_B                  , DIK_B             , 0x30         , 'B'          )
GNINPUT_DEFINE_KEYCODE( KEY_C                  , DIK_C             , 0x2E         , 'C'          )
GNINPUT_DEFINE_KEYCODE( KEY_D                  , DIK_D             , 0x20         , 'D'          )
GNINPUT_DEFINE_KEYCODE( KEY_E                  , DIK_E             , 0x12         , 'E'          )
GNINPUT_DEFINE_KEYCODE( KEY_F                  , DIK_F             , 0x21         , 'F'          )
GNINPUT_DEFINE_KEYCODE( KEY_G                  , DIK_G             , 0x22         , 'G'          )
GNINPUT_DEFINE_KEYCODE( KEY_H                  , DIK_H             , 0x23         , 'H'          )
GNINPUT_DEFINE_KEYCODE( KEY_I                  , DIK_I             , 0x17         , 'I'          )
GNINPUT_DEFINE_KEYCODE( KEY_J                  , DIK_J             , 0x24         , 'J'          )
GNINPUT_DEFINE_KEYCODE( KEY_K                  , DIK_K             , 0x25         , 'K'          )
GNINPUT_DEFINE_KEYCODE( KEY_L                  , DIK_L             , 0x26         , 'L'          )
GNINPUT_DEFINE_KEYCODE( KEY_M                  , DIK_M             , 0x32         , 'M'          )
GNINPUT_DEFINE_KEYCODE( KEY_N                  , DIK_N             , 0x31         , 'N'          )
GNINPUT_DEFINE_KEYCODE( KEY_O                  , DIK_O             , 0x18         , 'O'          )
GNINPUT_DEFINE_KEYCODE( KEY_P                  , DIK_P             , 0x19         , 'P'          )
GNINPUT_DEFINE_KEYCODE( KEY_Q                  , DIK_Q             , 0x10         , 'Q'          )
GNINPUT_DEFINE_KEYCODE( KEY_R                  , DIK_R             , 0x13         , 'R'          )
GNINPUT_DEFINE_KEYCODE( KEY_S                  , DIK_S             , 0x1F         , 'S'          )
GNINPUT_DEFINE_KEYCODE( KEY_T                  , DIK_T             , 0x14         , 'T'          )
GNINPUT_DEFINE_KEYCODE( KEY_U                  , DIK_U             , 0x16         , 'U'          )
GNINPUT_DEFINE_KEYCODE( KEY_V                  , DIK_V             , 0x2F         , 'V'          )
GNINPUT_DEFINE_KEYCODE( KEY_W                  , DIK_W             , 0x11         , 'W'          )
GNINPUT_DEFINE_KEYCODE( KEY_X                  , DIK_X             , 0x2D         , 'X'          )
GNINPUT_DEFINE_KEYCODE( KEY_Y                  , DIK_Y             , 0x15         , 'Y'          )
GNINPUT_DEFINE_KEYCODE( KEY_Z                  , DIK_Z             , 0x2C         , 'Z'          )

// 标点和符号键
GNINPUT_DEFINE_KEYCODE( KEY_MINUS              , DIK_MINUS         , 0x0C         , VK_OEM_MINUS ) // -
GNINPUT_DEFINE_KEYCODE( KEY_EQUALS             , DIK_EQUALS        , 0x0D         , VK_OEM_PLUS  ) // =
GNINPUT_DEFINE_KEYCODE( KEY_LBRACKET           , DIK_LBRACKET      , 0x1A         , VK_OEM_4     ) // [
GNINPUT_DEFINE_KEYCODE( KEY_RBRACKET           , DIK_RBRACKET      , 0x1B         , VK_OEM_6     ) // ]
GNINPUT_DEFINE_KEYCODE( KEY_SEMICOLON          , DIK_SEMICOLON     , 0x27         , VK_OEM_1     ) // ;
GNINPUT_DEFINE_KEYCODE( KEY_APOSTROPHE         , DIK_APOSTROPHE    , 0x28         , VK_OEM_7     ) // '
GNINPUT_DEFINE_KEYCODE( KEY_GRAVE              , DIK_GRAVE         , 0x29         , VK_OEM_3     ) // `
GNINPUT_DEFINE_KEYCODE( KEY_BACKSLASH          , DIK_BACKSLASH     , 0x2B         , VK_OEM_5     ) // \ (反斜线)
GNINPUT_DEFINE_KEYCODE( KEY_COMMA              , DIK_COMMA         , 0x33         , VK_OEM_COMMA ) // ,
GNINPUT_DEFINE_KEYCODE( KEY_PERIOD             , DIK_PERIOD        , 0x34         , VK_OEM_PERIOD) // .
GNINPUT_DEFINE_KEYCODE( KEY_SLASH              , DIK_SLASH         , 0x35         , VK_OEM_2     ) // / (正斜线)

// 数字小键盘
GNINPUT_DEFINE_KEYCODE( KEY_NUMPAD_0           , DIK_NUMPAD0       , 0x52         , VK_NUMPAD0   )
GNINPUT_DEFINE_KEYCODE( KEY_NUMPAD_1           , DIK_NUMPAD1       , 0x4F         , VK_NUMPAD1   )
GNINPUT_DEFINE_KEYCODE( KEY_NUMPAD_2           , DIK_NUMPAD2       , 0x50         , VK_NUMPAD2   )
GNINPUT_DEFINE_KEYCODE( KEY_NUMPAD_3           , DIK_NUMPAD3       , 0x51         , VK_NUMPAD3   )
GNINPUT_DEFINE_KEYCODE( KEY_NUMPAD_4           , DIK_NUMPAD4       , 0x4B         , VK_NUMPAD4   )
GNINPUT_DEFINE_KEYCODE( KEY_NUMPAD_5           , DIK_NUMPAD5       , 0x4C         , VK_NUMPAD5   )
GNINPUT_DEFINE_KEYCODE( KEY_NUMPAD_6           , DIK_NUMPAD6       , 0x4D         , VK_NUMPAD6   )
GNINPUT_DEFINE_KEYCODE( KEY_NUMPAD_7           , DIK_NUMPAD7       , 0x47         , VK_NUMPAD7   )
GNINPUT_DEFINE_KEYCODE( KEY_NUMPAD_8           , DIK_NUMPAD8       , 0x48         , VK_NUMPAD8   )
GNINPUT_DEFINE_KEYCODE( KEY_NUMPAD_9           , DIK_NUMPAD9       , 0x49         , VK_NUMPAD9   )
GNINPUT_DEFINE_KEYCODE( KEY_NUMPAD_MULTIPLY    , DIK_MULTIPLY      , 0x37         , VK_MULTIPLY  ) // *
GNINPUT_DEFINE_KEYCODE( KEY_NUMPAD_ADD         , DIK_ADD           , 0x4E         , VK_ADD       ) // +
GNINPUT_DEFINE_KEYCODE( KEY_NUMPAD_SUBTRACT    , DIK_SUBTRACT      , 0x4A         , VK_SUBTRACT  ) // -
GNINPUT_DEFINE_KEYCODE( KEY_NUMPAD_DECIMAL     , DIK_DECIMAL       , 0x53         , VK_DECIMAL   ) // .
GNINPUT_DEFINE_KEYCODE( KEY_NUMPAD_DIVIDE      , DIK_DIVIDE        , 0x135        , VK_DIVIDE    ) // / (正斜线)
GNINPUT_DEFINE_KEYCODE( KEY_NUMPAD_ENTER       , DIK_NUMPADENTER   , 0x11C        , VK_SEPARATOR )
GNINPUT_DEFINE_KEYCODE( KEY_NUMPAD_NUMLOCK     , DIK_NUMLOCK       , 0x145        , VK_NUMLOCK   )

// 功能键
GNINPUT_DEFINE_KEYCODE( KEY_F1                 , DIK_F1            , 0x3B         , VK_F1        )
GNINPUT_DEFINE_KEYCODE( KEY_F2                 , DIK_F2            , 0x3C         , VK_F2        )
GNINPUT_DEFINE_KEYCODE( KEY_F3                 , DIK_F3            , 0x3D         , VK_F3        )
GNINPUT_DEFINE_KEYCODE( KEY_F4                 , DIK_F4            , 0x3E         , VK_F4        )
GNINPUT_DEFINE_KEYCODE( KEY_F5                 , DIK_F5            , 0x3F         , VK_F5        )
GNINPUT_DEFINE_KEYCODE( KEY_F6                 , DIK_F6            , 0x40         , VK_F6        )
GNINPUT_DEFINE_KEYCODE( KEY_F7                 , DIK_F7            , 0x41         , VK_F7        )
GNINPUT_DEFINE_KEYCODE( KEY_F8                 , DIK_F8            , 0x42         , VK_F8        )
GNINPUT_DEFINE_KEYCODE( KEY_F9                 , DIK_F9            , 0x43         , VK_F9        )
GNINPUT_DEFINE_KEYCODE( KEY_F10                , DIK_F10           , 0x44         , VK_F10       )
GNINPUT_DEFINE_KEYCODE( KEY_F11                , DIK_F11           , 0x57         , VK_F11       )
GNINPUT_DEFINE_KEYCODE( KEY_F12                , DIK_F12           , 0x58         , VK_F12       )

// 其他
GNINPUT_DEFINE_KEYCODE( KEY_SYSRQ              , DIK_SYSRQ         , 0x137        , VK_PRINT     )
GNINPUT_DEFINE_KEYCODE( KEY_SCROLL             , DIK_SCROLL        , 0x46         , VK_SCROLL    )
GNINPUT_DEFINE_KEYCODE( KEY_PAUSE              , DIK_PAUSE         , 0x45         , VK_PAUSE     )

// 鼠标键
GNINPUT_DEFINE_KEYCODE( KEY_MOUSEBTN_0         , 0                 , 0            , 0            )
GNINPUT_DEFINE_KEYCODE( KEY_MOUSEBTN_1         , 0                 , 0            , 0            )
GNINPUT_DEFINE_KEYCODE( KEY_MOUSEBTN_2         , 0                 , 0            , 0            )
GNINPUT_DEFINE_KEYCODE( KEY_MOUSEBTN_3         , 0                 , 0            , 0            )
GNINPUT_DEFINE_KEYCODE( KEY_MOUSEBTN_4         , 0                 , 0            , 0            )
GNINPUT_DEFINE_KEYCODE( KEY_MOUSEBTN_5         , 0                 , 0            , 0            )
GNINPUT_DEFINE_KEYCODE( KEY_MOUSEBTN_6         , 0                 , 0            , 0            )
GNINPUT_DEFINE_KEYCODE( KEY_MOUSEBTN_7         , 0                 , 0            , 0            )
