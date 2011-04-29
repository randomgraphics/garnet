#ifndef __GN_BASE_SYNC_H__
#define __GN_BASE_SYNC_H__
// *****************************************************************************
/// \file
/// \brief   platform independent sync operations
/// \author  chen@@CHENLI-HOMEPC (2007.4.8)
// *****************************************************************************

namespace GN
{
    // Wait time definition
    //@{

    /// Define wait time, in 10^(-9) seconds
    typedef uint64 TimeInNanoSecond;

    ///
    /// special TimeInNanoSecond number that indicates infinite time.
    ///
    extern const TimeInNanoSecond INFINITE_TIME; // = (uint64)(-1);

    ///
    /// Define wait result
    ///
    struct WaitResult
    {
        enum Enum
        {
            COMPLETED, ///< Wait operation completes successfully
            KILLED,    ///< Wait operation aborted because the primitive is killed.
            TIMEOUT,   ///< Wait operation is time out.
        };

        GN_DEFINE_ENUM_CLASS_HELPERS( WaitResult, Enum );
    };

    //@}

    /// \name atomic operations
    //@{

    inline sint32 atomGet32( const sint32 volatile * ); ///< get 32bit integer value.
    inline void   atomSet32( sint32 volatile *, sint32 ); ///< set 32bit integer value.
    inline sint32 atomInc32( sint32 volatile * ); ///< return incremented value
    inline sint32 atomDec32( sint32 volatile * ); ///< return decremented value
    inline sint32 atomXchg32( sint32 volatile * dest, sint32 xchg ); ///< return initial value of the destination.

    ///
    /// if initial value of "dest" equals "cmp", then do exchange; else, do nothing.
    ///
    /// \return
    ///     Always return initial value of "dest".
    ///
    inline sint32 atomCmpXchg32( sint32 volatile * dest, sint32 xchg, sint32 cmp );

    ///
    /// Full memory barrier. currently implemented on MS Windows platform only.
    ///
    inline void memoryBarrier();

    //@}

    ///
    /// Spinloop lock
    ///
    class SpinLoop
    {
        volatile sint32 mLock;

    public:

        ///
        /// volatile type traits
        ///
        template <typename T>
        struct VolatileType
        {
            typedef volatile T type; ///< ...
        };

        //@{
        SpinLoop() : mLock(0) {}
        ~SpinLoop() {}
        //@}

        //@{
        bool trylock() { return 0 == atomCmpXchg32( &mLock, 1, 0 ); }
        void lock();
        void unlock() { atomSet32( &mLock, 0 ); }
        //@}
    };

    ///
    /// Mutex lock.
    ///
    class Mutex
    {
        uint32 mInternal[16]; ///< store platform dependata data here, no runtime heap allocation

    public:

        ///
        /// volatile type traits
        ///
        template <typename T>
        struct VolatileType
        {
            typedef volatile T type; ///< ...
        };

        //@{
        Mutex();
        ~Mutex();
        //@}

        //@{
        bool trylock();
        void lock();
        void unlock();
        //@}
    };

    ///
    /// this is a "fake" mutex to ease using mutex in template
    ///
    struct SingleThreadMutex
    {
        ///
        /// volatile type traits
        ///
        template <typename T>
        struct VolatileType
        {
            typedef T type; ///< ...
        };

        //@{
        bool trylock() { return true; }
        void lock()    {}
        void unlock()  {}
        //@}
    };

    ///
    /// auto mutex
    ///
    template< class M = Mutex >
    class ScopeMutex
    {
        M & mMutex;
    public:
        //@{
        ScopeMutex( M & m ) : mMutex(m) { mMutex.lock(); }
        ~ScopeMutex() { mMutex.unlock(); }
        //@}
    };

    ///
    /// sync event
    ///
    class SyncEvent : public NoCopy
    {
    public:

        //@{

        enum InitialState
        {
            SIGNALED,
            UNSIGNALED,
        };

        enum ResetMode
        {
            AUTO_RESET,
            MANUAL_RESET,
        };

        //@}

        //@{
        SyncEvent();
        ~SyncEvent();
        //@}

        //@{

        bool create(SyncEvent::InitialState initialState, SyncEvent::ResetMode resetMode, const char * name = 0);

        void destroy();

        void signal();   ///< signal the event, wake one thread that is waiting for it.

        void unsignal(); ///< unsignal the event, block any threads that wait for it.

        /// Returns:
        ///     WaitResult::COMPLETED   : the event is signaled
        ///     WaitResult::KILLED      : the event is destroied before signaled.
        ///     WaitResult::TIMEOUT     : time out before the event is signaled.
        WaitResult wait( TimeInNanoSecond timeoutTime = INFINITE_TIME ) const;

        //@}

    private:
        class Impl;
        Impl * mImpl;
    };

    ///
    /// Abstract semaphore interface
    ///
    class Semaphore : public NoCopy
    {
        //@{

        Semaphore();
        ~Semaphore();

        //@}

        //@{

        bool create( size_t maxcount, size_t initialcount, const char * name = 0 );
        void destroy();

        WaitResult wait( TimeInNanoSecond timeoutTime = INFINITE_TIME ); ///< block calling thread, until the semaphore is available. return false means timeout.
        void       wake( size_t count ); ///< wake up specified number of threads that is waiting for this semaphore.

        // aliases for P/V operations
        WaitResult P( TimeInNanoSecond timeoutTime = INFINITE_TIME ) { return wait( timeoutTime ); }
        void       V() { return wake( 1 ); }

        //@}

    private:
        class Impl;
        Impl * mImpl;
    };
}

#if GN_MSWIN || GN_XENON
#include "syncmsw.inl"
#elif GN_POSIX
#include "syncposix.inl"
#else
#error unsupport platform
#endif

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_SYNC_H__
