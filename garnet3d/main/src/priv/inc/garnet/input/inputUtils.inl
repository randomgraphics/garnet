namespace GN { namespace input
{
    //
    //
    // ----------------------------------------------------------------------------
    inline KeyCode str2kc( const char * name )
    {
        if( GN::strEmpty(name) ) return KEY_NONE;

        static const char * sTable[]=
        {
            "KEY_NONE",
            #define GNINPUT_DEFINE_KEYCODE( name, dikey, scancode, vkeycode, xkeysym ) #name,
            #include "keyCodeMeta.h"
            #undef  GNINPUT_DEFINE_KEYCODE
        };

        for( size_t i = 0; i < sizeof(sTable)/sizeof(sTable[0]); ++i )
        {
            if( 0 == strCmp( name, sTable[i] ) ) return (KeyCode)i;
        }

        return KEY_NONE;
    }

    //
    //
    // ----------------------------------------------------------------------------
    inline const char * kc2str( int code )
    {
        static const char * sTable[]=
        {
            "KEY_NONE",
            #define GNINPUT_DEFINE_KEYCODE( name, dikey, scancode, vkeycode, xkeysym ) #name,
            #include "keyCodeMeta.h"
            #undef  GNINPUT_DEFINE_KEYCODE
        };

        if( KEY_NONE <= code && code < KEY_NUM_OF_KEYS )
            return sTable[code];
        else
            return "";
    }
}}
