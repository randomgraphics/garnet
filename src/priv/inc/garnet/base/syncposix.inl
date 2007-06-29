#if GN_POSIX

//
//
// -----------------------------------------------------------------------------
inline void GN::memoryBarrier()
{
    GN_UNIMPL();
}

//
//
// -----------------------------------------------------------------------------
inline SInt32 GN::atomGet32( const SInt32 volatile * dest )
{
    GN_UNIMPL();
    GN_ASSERT( dest );
    return *dest;
}

//
//
// -----------------------------------------------------------------------------
inline void GN::atomSet32( SInt32 volatile * dest, SInt32 value )
{
    GN_UNIMPL();
    GN_ASSERT( dest );
    *dest = value;
}

//
//
// -----------------------------------------------------------------------------
inline SInt32 GN::atomInc32( SInt32 volatile * dest )
{
    GN_ASSERT( dest );
    GN_UNIMPL();
    return 0;
}

//
//
// -----------------------------------------------------------------------------
inline SInt32 GN::atomDec32( SInt32 volatile * dest )
{
    GN_ASSERT( dest );
    GN_UNIMPL();
    return 0;
}

//
//
// -----------------------------------------------------------------------------
inline SInt32 GN::atomXchg32( SInt32 volatile * dest, SInt32 xchg )
{
    GN_ASSERT( dest );
    GN_UNIMPL();
    return 0;
}

//
//
// -----------------------------------------------------------------------------
inline SInt32 GN::atomCmpXchg32( SInt32 volatile * dest, SInt32 xchg, SInt32 cmp )
{
    GN_ASSERT( dest );
    GN_UNIMPL();
    return 0;
}

#endif
