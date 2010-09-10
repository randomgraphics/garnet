#include "pch.h"

// ****************************************************************************
// public functions
// ****************************************************************************

// TODO: optimize for SSE and PPC

//
//
// ----------------------------------------------------------------------------
void GN::swap8in16( void * outvoid, const void * invoid, size_t count )
{
    union Local
    {
        uint16 u16;
        struct
        {
            uint8 a, b;
        };
    };

    Local tmp;

    uint16 * out = (uint16*)outvoid;
    const uint16 * in = (const uint16*)invoid;

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
void GN::swap8in32( void * outvoid, const void * invoid, size_t count )
{
    union Local
    {
        uint32 u32;
        struct
        {
            uint8 a, b, c, d;
        };
    };

    Local tmp;

    uint32 * out = (uint32*)outvoid;
    const uint32 * in = (const uint32*)invoid;

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
void GN::swap8in64( void * outvoid, const void * invoid, size_t count )
{
    union Local
    {
        uint64 u64;
        uint8  u8[8];
    };

    Local tmp;

    uint64 * out = (uint64*)outvoid;
    const uint64 * in = (const uint64*)invoid;

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
void GN::swap16in32( void * outvoid, const void * invoid, size_t count )
{
    union Local
    {
        uint32 u32;
        struct
        {
            uint16 a, b;
        };
    };

    Local tmp;

    uint32 * out = (uint32*)outvoid;
    const uint32 * in = (const uint32*)invoid;

    for( size_t n = 0; n < count; ++n, ++in, ++out )
    {
        Local & i = *(Local*)in;

        tmp.a = i.b;
        tmp.b = i.a;

        *out = tmp.u32;
    }
};
