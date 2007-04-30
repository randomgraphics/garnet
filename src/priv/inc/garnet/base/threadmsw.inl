#if GN_MSWIN

#include <intrin.h>

//
//
// -----------------------------------------------------------------------------
inline void GN::memoryBarrier()
{
    _ReadWriteBarrier();
}

//
//
// -----------------------------------------------------------------------------
inline SInt32 GN::atomGet32( const SInt32 volatile * dest )
{
    GN_ASSERT( dest );
    return *dest;
}

//
//
// -----------------------------------------------------------------------------
inline void GN::atomSet32( SInt32 volatile * dest, SInt32 value )
{
    GN_ASSERT( dest );
    *dest = value;
}

//
//
// -----------------------------------------------------------------------------
inline SInt32 GN::atomInc32( SInt32 volatile * dest )
{
    GN_ASSERT( dest );
    return _InterlockedIncrement( (volatile long *)dest );
}

//
//
// -----------------------------------------------------------------------------
inline SInt32 GN::atomDec32( SInt32 volatile * dest )
{
    GN_ASSERT( dest );
    return _InterlockedDecrement( (volatile long *)dest );
}

//
//
// -----------------------------------------------------------------------------
inline SInt32 GN::atomXchg32( SInt32 volatile * dest, SInt32 xchg )
{
    GN_ASSERT( dest );
    return _InterlockedExchange( (volatile long *)dest, xchg );
}

//
//
// -----------------------------------------------------------------------------
inline SInt32 GN::atomCmpXchg32( SInt32 volatile * dest, SInt32 xchg, SInt32 cmp )
{
    GN_ASSERT( dest );
    return _InterlockedCompareExchange( (volatile long *)dest, xchg, cmp );
}

#endif
