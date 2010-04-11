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
inline SInt32 GN::atomGet32( const SInt32 volatile * dest )
{
    return *dest;
}

//
//
// -----------------------------------------------------------------------------
inline void GN::atomSet32( SInt32 volatile * dest, SInt32 value )
{
    *dest = value;
}

//
//
// -----------------------------------------------------------------------------
inline SInt32 GN::atomInc32( SInt32 volatile * dest )
{
    return __sync_add_and_fetch( dest, 1 );
}

//
//
// -----------------------------------------------------------------------------
inline SInt32 GN::atomDec32( SInt32 volatile * dest )
{
    return __sync_sub_and_fetch( dest, 1 );
}

//
//
// -----------------------------------------------------------------------------
inline SInt32 GN::atomXchg32( SInt32 volatile * dest, SInt32 xchg )
{
    return __sync_lock_test_and_set( dest, xchg );
}

//
//
// -----------------------------------------------------------------------------
inline SInt32 GN::atomCmpXchg32( SInt32 volatile * dest, SInt32 xchg, SInt32 cmp )
{
    return __sync_val_compare_and_swap( dest, cmp, xchg );
}

#endif
