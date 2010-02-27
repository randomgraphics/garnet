#if GN_POSIX

//
//
// -----------------------------------------------------------------------------
inline SInt32 GN::AtomGet32( const SInt32 volatile * dest )
{
    return *dest;
}

//
//
// -----------------------------------------------------------------------------
inline void GN::AtomSet32( SInt32 volatile * dest, SInt32 value )
{
    *dest = value;
}

//
//
// -----------------------------------------------------------------------------
inline SInt32 GN::AtomInc32( SInt32 volatile * dest )
{
    return __sync_add_and_fetch( dest, 1 );
}

//
//
// -----------------------------------------------------------------------------
inline SInt32 GN::AtomDec32( SInt32 volatile * dest )
{
    return __sync_sub_and_fetch( dest, 1 );
}

//
//
// -----------------------------------------------------------------------------
inline SInt32 GN::AtomXchg32( SInt32 volatile * dest, SInt32 xchg )
{
    return __sync_lock_test_and_set( dest, xchg );
}

//
//
// -----------------------------------------------------------------------------
inline SInt32 GN::AtomCmpXchg32( SInt32 volatile * dest, SInt32 xchg, SInt32 cmp )
{
    return __sync_val_compare_and_swap( dest, cmp, xchg );
}

#endif
