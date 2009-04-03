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
        UInt16 u16;
        struct
        {
            UInt8 a, b;
        };
    };

    Local tmp;

    UInt16 * out = (UInt16*)outvoid;
    const UInt16 * in = (const UInt16*)invoid;

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
        UInt32 u32;
        struct
        {
            UInt8 a, b, c, d;
        };
    };

    Local tmp;

    UInt32 * out = (UInt32*)outvoid;
    const UInt32 * in = (const UInt32*)invoid;

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
        UInt64 u64;
        UInt8  u8[8];
    };

    Local tmp;

    UInt64 * out = (UInt64*)outvoid;
    const UInt64 * in = (const UInt64*)invoid;

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
        UInt32 u32;
        struct
        {
            UInt16 a, b;
        };
    };

    Local tmp;

    UInt32 * out = (UInt32*)outvoid;
    const UInt32 * in = (const UInt32*)invoid;

    for( size_t n = 0; n < count; ++n, ++in, ++out )
    {
        Local & i = *(Local*)in;

        tmp.a = i.b;
        tmp.b = i.a;

        *out = tmp.u32;
    }
};
