namespace GN {
namespace input {
//
//
// ----------------------------------------------------------------------------
inline KeyCode str2kc(const char * name) {
    if (GN::str::isEmpty(name)) return KeyCode::NONE;

    static const char * sTable[] = {
        "KeyCode::NONE",
#define GNINPUT_DEFINE_KEYCODE(name, dikey, scancode, vkeycode, xkeysym) #name,
#include "keyCodeMeta.h"
#undef GNINPUT_DEFINE_KEYCODE
    };

    for (size_t i = 0; i < sizeof(sTable) / sizeof(sTable[0]); ++i) {
        if (0 == str::compare(name, sTable[i])) return (KeyCode) i;
    }

    return KeyCode::NONE;
}

//
//
// ----------------------------------------------------------------------------
inline const char * keyCode2String(int code) {
    static const char * sTable[] = {
        "KeyCode::NONE",
#define GNINPUT_DEFINE_KEYCODE(name, dikey, scancode, vkeycode, xkeysym) #name,
#include "keyCodeMeta.h"
#undef GNINPUT_DEFINE_KEYCODE
    };

    if (KeyCode::NONE <= code && code < KeyCode::NUM_KEYS)
        return sTable[code];
    else
        return "";
}
} // namespace input
} // namespace GN
