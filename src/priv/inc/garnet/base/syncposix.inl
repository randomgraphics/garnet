#if GN_POSIX

//
//
// -----------------------------------------------------------------------------
inline void GN::memoryBarrier()
{
     _GLIBCXX_READ_MEM_BARRIER;
}

//
//
// -----------------------------------------------------------------------------
inline sint32 GN::atomGet32( const sint32 volatile * dest )
{
    return *dest;
}

//
//
// -----------------------------------------------------------------------------
inline void GN::atomSet32( sint32 volatile * dest, sint32 value )
{
    *dest = value;
}

//
//
// -----------------------------------------------------------------------------
inline sint32 GN::atomInc32( sint32 volatile * dest )
{
    return __sync_add_and_fetch( dest, 1 );
}

//
//
// -----------------------------------------------------------------------------
inline sint32 GN::atomDec32( sint32 volatile * dest )
{
    return __sync_sub_and_fetch( dest, 1 );
}

//
//
// -----------------------------------------------------------------------------
inline sint32 GN::atomXchg32( sint32 volatile * dest, sint32 xchg )
{
    return __sync_lock_test_and_set( dest, xchg );
}

//
//
// -----------------------------------------------------------------------------
inline sint32 GN::atomCmpXchg32( sint32 volatile * dest, sint32 xchg, sint32 cmp )
{
    return __sync_val_compare_and_swap( dest, cmp, xchg );
}

#endif
