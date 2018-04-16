#ifndef __GN_BASE_SYNC_H__
#define __GN_BASE_SYNC_H__
// *****************************************************************************
/// \file
/// \brief   platform independent sync operations
/// \author  chen@@CHENLI-HOMEPC (2007.4.8)
// *****************************************************************************

#include <atomic>
#include <mutex>
#include <thread>
#include <chrono>

namespace GN
{
    /* Wait time definition
    //@{

    /// Define wait time, in 10^(-9) seconds
    typedef uint64 TimeInNanoSecond;

    ///
    /// special TimeInNanoSecond number that indicates infinite time.
    ///
    extern GN_API const TimeInNanoSecond INFINITE_TIME; // = (uint64)(-1);

    ///
    /// One second in unit of nano second
    ///
    extern GN_API const TimeInNanoSecond ONE_SECOND_IN_NS;

    ///
    /// Define wait result
    ///
    struct WaitResult
    {
        enum Enum
        {
            COMPLETED, ///< Wait operation completes successfully
            KILLED,    ///< Wait operation aborted because the primitive is killed.
            TIMEDOUT,  ///< Wait operation is time out.
            FAILED,    ///< Wait failed for unspecified reason.
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
    inline void memoryBarrier(); */

    //@}

    ///
    /// Spinloop lock
    ///
    class SpinLoop : NoCopy
    {
        std::atomic_flag mLock;
    public:
        //@{
        bool tryLock() { return mLock.test_and_set(); }
        void lock()
        {
            size_t i = 0;
            while( !mLock.test_and_set() )
            {
                ++i;
                if( i > 1000000 )
                {
                    std::this_thread::sleep_for( std::chrono::microseconds::min() );
                    i = 0;
                }
            }
        }
        void unlock() { mLock.clear(); }
        //@}
    };

    ///
    /// Mutex lock.
    ///
    class Mutex
    {
        std::recursive_mutex mMutex;

    public:

        //@{
        bool trylock() { return mMutex.try_lock(); }
        void lock()    { return mMutex.lock(); }
        void unlock()  { return mMutex.unlock(); }
        //@}

        //@{
        bool tryEnter() { return trylock(); }
        void enter() { return lock(); }
        void leave() { return unlock(); }
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

    /*
    ///
    /// sync event
    ///
    class GN_API SyncEvent : public NoCopy
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
        ///     WaitResult::TIMEDOUT    : time out before the event is signaled.
        ///     WaitResult::FAILED      : wait operation failed for unspecified reason.
        WaitResult wait( TimeInNanoSecond timeoutTime = INFINITE_TIME ) const;

        //@}

    private:
        class Impl;
        Impl * mImpl;
    };

    ///
    /// Abstract semaphore interface
    ///
    class GN_API Semaphore : public NoCopy
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
    };*/
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_SYNC_H__
