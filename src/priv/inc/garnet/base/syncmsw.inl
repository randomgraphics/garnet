#if GN_MSWIN

#if GN_XBOX2
#include <xtl.h>
#else
#include <intrin.h>
#endif

//
//
// -----------------------------------------------------------------------------
inline void GN::memoryBarrier()
{
#if GN_XBOX2
    __lwsync();
#else
    _ReadWriteBarrier();
#endif
}

//
//
// -----------------------------------------------------------------------------
inline sint32 GN::atomGet32( const sint32 volatile * dest )
{
    GN_ASSERT( dest );
    return *dest;
}

//
//
// -----------------------------------------------------------------------------
inline void GN::atomSet32( sint32 volatile * dest, sint32 value )
{
    GN_ASSERT( dest );
    *dest = value;
}

//
//
// -----------------------------------------------------------------------------
inline sint32 GN::atomInc32( sint32 volatile * dest )
{
    GN_ASSERT( dest );
    return _InterlockedIncrement( (volatile long *)dest );
}

//
//
// -----------------------------------------------------------------------------
inline sint32 GN::atomDec32( sint32 volatile * dest )
{
    GN_ASSERT( dest );
    return _InterlockedDecrement( (volatile long *)dest );
}

//
//
// -----------------------------------------------------------------------------
inline sint32 GN::atomXchg32( sint32 volatile * dest, sint32 xchg )
{
    GN_ASSERT( dest );
    return _InterlockedExchange( (volatile long *)dest, xchg );
}

//
//
// -----------------------------------------------------------------------------
inline sint32 GN::atomCmpXchg32( sint32 volatile * dest, sint32 xchg, sint32 cmp )
{
    GN_ASSERT( dest );
    return _InterlockedCompareExchange( (volatile long *)dest, xchg, cmp );
}

#endif
