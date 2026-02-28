#include "pch.h"
#include <atomic>

#if GN_DARWIN
    #include <uuid/uuid.h>
#endif

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
GN_API GN::Guid GN::Guid::createRandom() {
#if GN_DARWIN
    // Use Darwin API generator
    Guid g = {};
    if (KERN_SUCCESS == ::uuid_create(&g)) { return g; }
    // fallback - continue below
#elif GN_POSIX
    // Try read kernal uuid file on Linux and Android
    FILE * f = ::fopen("/proc/sys/kernel/random/uuid", "rb");
    if (f) {
        char   uuid[37];
        size_t got = ::fread(uuid, 1, 36, f);
        ::fclose(f);
        int data1, data2, data3, data4, data5, data6, data7, data8, data9, data10, data11;
        if (got == 36 && 36 == ::sscanf(uuid, "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x", &data1, &data2, &data3, &data4, &data5, &data6, &data7,
                                        &data8, &data9, &data10, &data11)) {
            Guid g;
            g.data1    = (uint32_t) data1;
            g.data2    = (uint16_t) data2;
            g.data3    = (uint16_t) data3;
            g.data4[0] = (uint8_t) data4;
            g.data4[1] = (uint8_t) data5;
            g.data4[2] = (uint8_t) data6;
            g.data4[3] = (uint8_t) data7;
            g.data4[4] = (uint8_t) data8;
            g.data4[5] = (uint8_t) data9;
            g.data4[6] = (uint8_t) data10;
            g.data4[7] = (uint8_t) data11;
            return g;
        }
    }
    // fallback - continue below
#elif defined(_WIN32)
    // Use Windows API generator
    Guid g = {};
    GUID win_guid;
    if (CoCreateGuid(&win_guid) == S_OK) {
        // Map fields
        g.data1 = win_guid.Data1;
        g.data2 = win_guid.Data2;
        g.data3 = win_guid.Data3;
        for (int i = 0; i < 8; ++i) g.data4[i] = win_guid.Data4[i];
        // Set RFC 4122 version and variant (CoCreateGuid is not RFC 4122, but sets bits close enough in practice)
        g.data3    = (g.data3 & 0x0FFF) | (4 << 12);
        g.data4[0] = (g.data4[0] & 0x3F) | 0x80;
        return g;
    }
    // fallback - continue below
#endif

    // Portable C++11-thread-safe fallback
    Guid g;
    // Thread-safe entropy: rand() is only weakly random. Use address, time, and rand, mix with a static counter.
    static std::atomic<uint32_t> counter {0};
    uint64_t                     ts   = static_cast<uint64_t>(time(nullptr));
    uintptr_t                    addr = reinterpret_cast<uintptr_t>(&g);
    uint32_t                     cnt  = ++counter;
    // Seed rand using a combination of address, ts, and counter to reduce collision
    uint32_t seed = static_cast<uint32_t>(addr ^ (ts << 16) ^ cnt);
    // Fallback deterministic PRNG (LCG)
    uint32_t x        = seed;
    auto     nextRand = [&]() -> uint32_t {
        x = 1664525 * x + 1013904223;
        return x;
    };
    g.data1 = nextRand();
    g.data2 = static_cast<uint16_t>(nextRand());
    g.data3 = static_cast<uint16_t>(nextRand());
    for (int i = 0; i < 8; ++i) { g.data4[i] = static_cast<uint8_t>(nextRand()); }
    // Set RFC 4122 bits
    g.data3    = (g.data3 & 0x0FFF) | (4 << 12); // Version 4
    g.data4[0] = (g.data4[0] & 0x3F) | 0x80;     // Variant specified in RFC
    return g;
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
