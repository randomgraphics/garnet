#include "pch.h"

// ****************************************************************************
// public functions
// ****************************************************************************

// TODO: optimize for SSE and PPC

//
//
// ----------------------------------------------------------------------------
void GN::swap8in16( UInt16 * out, const UInt16 * in, size_t count )
{
    union Local
    {
        UInt16 u16;
        struct
        {
            UInt8 a, b;
        };
    };

    Local tmp;

    for( size_t n = 0; n < count; ++n, ++in, ++out )
    {
        Local & i = *(Local*)in;

        tmp.a = i.b;
        tmp.b = i.a;

        *out = tmp.u16;
    }
};

//
//
// ----------------------------------------------------------------------------
void GN::swap8in32( UInt32 * out, const UInt32 * in, size_t count )
{
    union Local
    {
        UInt32 u32;
        struct
        {
            UInt8 a, b, c, d;
        };
    };

    Local tmp;

    for( size_t n = 0; n < count; ++n, ++in, ++out )
    {
        Local & i = *(Local*)in;

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
void GN::swap8in64( UInt64 * out, const UInt64 * in, size_t count )
{
    union Local
    {
        UInt64 u64;
        UInt8  u8[8];
    };

    Local tmp;

    for( size_t n = 0; n < count; ++n, ++in, ++out )
    {
        Local & i = *(Local*)in;

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
void GN::swap16in32( UInt32 * out, const UInt32 * in, size_t count )
{
    union Local
    {
        UInt32 u32;
        struct
        {
            UInt16 a, b;
        };
    };

    Local tmp;

    for( size_t n = 0; n < count; ++n, ++in, ++out )
    {
        Local & i = *(Local*)in;

        tmp.a = i.b;
        tmp.b = i.a;

        *out = tmp.u32;
    }
};
