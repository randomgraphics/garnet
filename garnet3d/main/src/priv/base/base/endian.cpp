#include "pch.h"

// ****************************************************************************
// public functions
// ****************************************************************************

// TODO: optimize for SSE and PPC

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
