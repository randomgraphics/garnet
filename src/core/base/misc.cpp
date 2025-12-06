#include "pch.h"

//
//
// -----------------------------------------------------------------------------
GN_API void GN::putEnv(const char * name, const char * value) {
    static GN::Logger * sLogger = GN::getLogger("GN.base.putEnv");

#if GN_XBOX2 || GN_XBOX3
    // Xenon does not support putenv()
    GN_UNUSED_PARAM(name);
    GN_UNUSED_PARAM(value);
#else
    if (str::empty(name)) {
        GN_ERROR(sLogger)("Environment variable name can't be empty!");
        return;
    }

    if (0 == value) value = "";

    #if GN_POSIX
    if (0 != ::setenv(name, value, 1)) { GN_ERROR(sLogger)("fail to set environment '{}={}'.", name, value); }
    #else
    StrA s;
    if (str::empty(value)) {
        s.formatInplace("{}=", name);
    } else {
        s.formatInplace("{}={}", name, value);
    }

    if (0 != _putenv(const_cast<char *>(s.data()))) { GN_ERROR(sLogger)("fail to set environment '{}'.", s.data()); }
    #endif
#endif
}

//
//
// -----------------------------------------------------------------------------
GN_API void GN::getEnv(StrA & result, const char * name) {
#if GN_XBOX2 || GN_XBOX3
    // Xbox does not support getenv()
    GN_UNUSED_PARAM(name);
    result.clear();
#else
    if (str::empty(name)) {
        result.clear();
    } else {
    #if GN_MSVC
        char * value;
        size_t sz;
        if (0 == _dupenv_s(&value, &sz, name)) {
            result.assign(value, sz);
            ::free(value);
        }
    #else
        result.assign(::getenv(name));
    #endif
    }
#endif
}

//
//
// -----------------------------------------------------------------------------
GN_API const char * GN::Guid::toStr() const {
#if GN_CYGWIN
    return "<ERROR: mingw does not support TLS>";
#else
    // GUID as string: {xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx}
    static GN_TLS char str[1 + 8 + 1 + 4 + 1 + 4 + 1 + 4 + 1 + 12 + 1 + 1];

    str::formatTo(str, "{{{:08x}-{:04x}-{:04x}-{:02x}{:02x}-{:02x}{:02x}{:02x}{:02x}{:02x}{:02x}}}", data1, data2, data3, data4[0], data4[1], data4[2],
                  data4[3], data4[4], data4[5], data4[6], data4[7]);

    return str;
#endif
}
