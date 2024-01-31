#include "pch.h"

struct EmptyString {
    size_t  caps = 0;
    size_t  size = 0;
    int64_t eos  = 0;
};

// *****************************************************************************
//
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN_API size_t GN::str::toSignedInteger(sint64 & result, int bits, int base, const char * s) {
    // check invalid parameters
    if (bits < 2 || bits > 64) return 0;
    if (base < 2) return 0;
    if (empty(s)) return 0;

    errno = 0;

    char * e;

#if GN_POSIX
    sint64 s64 = strtoll(s, &e, base);
#else
    sint64 s64 = _strtoi64(s, &e, base);
#endif

    if (0 != errno || (0 == s64 && s == e)) return 0;

    // check for overflow
    sint64 maxval = (1LL << (bits - 1)) - 1;
    sint64 minval = ~maxval;
    if (s64 < minval || s64 > maxval) return 0;

    // success
    result = s64;
    return e - s;
}

//
//
// -----------------------------------------------------------------------------
GN_API size_t GN::str::toUnsignedInteger(uint64 & result, int bits, int base, const char * s) {
    // check invalid parameters
    if (bits < 2 || bits > 64) return 0;
    if (base < 2) return 0;
    if (empty(s)) return 0;

    errno = 0;

    char * e;
#if GN_POSIX
    uint64 u64 = strtoull(s, &e, base);
#else
    uint64 u64 = _strtoui64(s, &e, base);
#endif

    if (0 != errno || (0 == u64 && s == e)) return 0;

    // Note: _strtoui64 has bug that parses -1 as max unsigned integer.
    const char * ptr = s;
    while (ptr < e && (' ' == *ptr || '\t' == *ptr)) ++ptr;
    if (ptr < e && *ptr == '-') return 0;

    // check for overflow
    uint64 maxval = (((uint64) -1) << (64 - bits)) >> (64 - bits);
    if (u64 > maxval) return 0;

    // success
    result = u64;
    return e - s;
}

//
//
// -----------------------------------------------------------------------------
GN_API size_t GN::str::toFloat(float & i, const char * s) {
    double d;
    size_t n = toDouble(d, s);

    if (0 == n) return 0;

    if (d < -FLT_MAX)
        i = -FLT_MAX;
    else if (d > FLT_MAX)
        i = FLT_MAX;
    else
        i = (float) d;

    return n;
}

//
//
// -----------------------------------------------------------------------------
GN_API size_t GN::str::toDouble(double & i, const char * s) {
    if (empty(s)) return 0;

    char * e;
    double d = strtod(s, &e);

    if (0 != errno || (0 == d && s == e)) {
        return 0;
    } else {
        i = d;
        return e - s;
    }
}

//
//
// -----------------------------------------------------------------------------
GN_API size_t GN::str::toFloatArray(float * buffer, size_t maxCount, const char * str, size_t length) {
    if (NULL == buffer) return 0;
    if (empty(str)) return 0;

    if (0 == length) length = strlen(str);

    float *      bufbegin = buffer;
    float *      bufend   = buffer + maxCount;
    const char * strend   = str + length;

    while (buffer < bufend && str < strend) {
        size_t n = toFloat(*buffer, str);

        if (0 == n) break;

        // next float
        str += n;
        ++buffer;

        // skip float separators
        while ('\n' == *str || '\t' == *str || ' ' == *str || ',' == *str) ++str;
    }

    return buffer - bufbegin;
}
