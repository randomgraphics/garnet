#include "pch.h"

// ****************************************************************************
// public functions
// ****************************************************************************

// TODO: optimize for SSE and PPC

//
//
// ----------------------------------------------------------------------------
GN_API void GN::swap8in16(void * outvoid, const void * invoid, size_t count) {
    union Local {
        uint16_t u16;
        struct {
            uint8_t a, b;
        };
    };

    Local tmp;

    uint16_t *       out = (uint16_t *) outvoid;
    const uint16_t * in  = (const uint16_t *) invoid;

    for (size_t n = 0; n < count; ++n, ++in, ++out) {
        Local & i = *(Local *) in;

        tmp.a = i.b;
        tmp.b = i.a;

        *out = tmp.u16;
    }
};

//
//
// ----------------------------------------------------------------------------
GN_API void GN::swap8in32(void * outvoid, const void * invoid, size_t count) {
    union Local {
        uint32_t u32;
        struct {
            uint8_t a, b, c, d;
        };
    };

    Local tmp;

    uint32_t *       out = (uint32_t *) outvoid;
    const uint32_t * in  = (const uint32_t *) invoid;

    for (size_t n = 0; n < count; ++n, ++in, ++out) {
        Local & i = *(Local *) in;

        tmp.a = i.d;
        tmp.b = i.c;
        tmp.c = i.b;
        tmp.d = i.a;

        *out = tmp.u32;
    }
};

//
//
// ----------------------------------------------------------------------------
GN_API void GN::swap8in64(void * outvoid, const void * invoid, size_t count) {
    union Local {
        uint64_t u64;
        uint8_t  u8[8];
    };

    Local tmp;

    uint64_t *       out = (uint64_t *) outvoid;
    const uint64_t * in  = (const uint64_t *) invoid;

    for (size_t n = 0; n < count; ++n, ++in, ++out) {
        Local & i = *(Local *) in;

        tmp.u8[0] = i.u8[7];
        tmp.u8[1] = i.u8[6];
        tmp.u8[2] = i.u8[5];
        tmp.u8[3] = i.u8[4];
        tmp.u8[4] = i.u8[3];
        tmp.u8[5] = i.u8[2];
        tmp.u8[6] = i.u8[1];
        tmp.u8[7] = i.u8[0];

        *out = tmp.u64;
    }
}

//
//
// ----------------------------------------------------------------------------
GN_API void GN::swap16in32(void * outvoid, const void * invoid, size_t count) {
    union Local {
        uint32_t u32;
        struct {
            uint16_t a, b;
        };
    };

    Local tmp;

    uint32_t *       out = (uint32_t *) outvoid;
    const uint32_t * in  = (const uint32_t *) invoid;

    for (size_t n = 0; n < count; ++n, ++in, ++out) {
        Local & i = *(Local *) in;

        tmp.a = i.b;
        tmp.b = i.a;

        *out = tmp.u32;
    }
};
