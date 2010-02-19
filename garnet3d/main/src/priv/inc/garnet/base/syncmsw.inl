#if GN_MSWIN

#if GN_XENON
#include <xtl.h>
#else
#include <intrin.h>
#endif

//
//
// -----------------------------------------------------------------------------
inline void GN::MemoryBarrier()
{
#if GN_XENON
    __lwsync();
#else
    _ReadWriteBarrier();
#endif
}

//
//
// -----------------------------------------------------------------------------
inline SInt32 GN::AtomGet32( const SInt32 volatile * dest )
{
    GN_ASSERT( dest );
    return *dest;
}

//
//
// -----------------------------------------------------------------------------
inline void GN::AtomSet32( SInt32 volatile * dest, SInt32 value )
{
    GN_ASSERT( dest );
    *dest = value;
}

//
//
// -----------------------------------------------------------------------------
inline SInt32 GN::AtomInc32( SInt32 volatile * dest )
{
    GN_ASSERT( dest );
    return _InterlockedIncrement( (volatile long *)dest );
}

//
//
// -----------------------------------------------------------------------------
inline SInt32 GN::AtomDec32( SInt32 volatile * dest )
{
    GN_ASSERT( dest );
    return _InterlockedDecrement( (volatile long *)dest );
}

//
//
// -----------------------------------------------------------------------------
inline SInt32 GN::AtomXchg32( SInt32 volatile * dest, SInt32 xchg )
{
    GN_ASSERT( dest );
    return _InterlockedExchange( (volatile long *)dest, xchg );
}

//
//
// -----------------------------------------------------------------------------
inline SInt32 GN::AtomCmpXchg32( SInt32 volatile * dest, SInt32 xchg, SInt32 cmp )
{
    GN_ASSERT( dest );
    return _InterlockedCompareExchange( (volatile long *)dest, xchg, cmp );
}

#endif
