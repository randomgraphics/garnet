#ifndef __GN_BASE_SYNC_H__
#define __GN_BASE_SYNC_H__
// *****************************************************************************
/// \file
/// \brief   platform independent sync operations
/// \author  chen@@CHENLI-HOMEPC (2007.4.8)
// *****************************************************************************

namespace GN
{
    ///
    /// special float number that indicates infinite time.
    ///
    extern const float INFINITE_TIME; // = 1.0e38f;

    /// \name atomic operations
    //@{

    inline SInt32 AtomGet32( const SInt32 volatile * ); ///< get 32bit integer value.
    inline void   AtomSet32( SInt32 volatile *, SInt32 ); ///< set 32bit integer value.
    inline SInt32 AtomInc32( SInt32 volatile * ); ///< return incremented value
    inline SInt32 AtomDec32( SInt32 volatile * ); ///< return decremented value
    inline SInt32 AtomXchg32( SInt32 volatile * dest, SInt32 xchg ); ///< return initial value of the destination.

    ///
    /// if initial value of "dest" equals "cmp", then do exchange; else, do nothing.
    ///
    /// \return
    ///     Always return initial value of "dest".
    ///
    inline SInt32 AtomCmpXchg32( SInt32 volatile * dest, SInt32 xchg, SInt32 cmp );

    ///
    /// memory barrier. currently implemented on MS Windows platform only.
    ///
    inline void MemoryBarrier();

    //@}

    ///
    /// Spinloop lock
    ///
    class SpinLoop
    {
        volatile SInt32 mLock;

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
        bool TryLock() { return 0 == AtomCmpXchg32( &mLock, 1, 0 ); }
        void Lock();
        void Unlock() { AtomSet32( &mLock, 0 ); }
        //@}
    };

    ///
    /// Mutex lock.
    ///
    class Mutex
    {
        UInt32 mInternal[16]; ///< store platform dependata data here, no runtime heap allocation

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
        bool TryLock();
        void Lock();
        void Unlock();
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
        bool TryLock() { return true; }
        void Lock()    {}
        void Unlock()  {}
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
        ScopeMutex( M & m ) : mMutex(m) { mMutex.Lock(); }
        ~ScopeMutex() { mMutex.Unlock(); }
        //@}
    };

    ///
    /// sync event
    ///
    struct SyncEvent : public NoCopy
    {
        //@{
        virtual void Signal() = 0;   ///< signal the event, wake one thread that is waiting for it.
        virtual void Unsignal() = 0; ///< unsignal the event, block any threads that Wait for it.
        virtual bool Wait( float seconds = INFINITE_TIME ) = 0; ///< return true means the event is signaled; return false means timeout.
        //@}
    };

    ///
    /// Abstract semaphore interface
    ///
    struct Semaphore : public NoCopy
    {
        virtual bool Wait( float seconds = INFINITE_TIME ) = 0; ///< block calling thread, until the semaphore is available. return false means timeout.
        virtual void Wake( size_t count = 1 ) = 0; ///< Wake up specified number of threads that is waiting for this semaphore.

        /// \name aliases for P/V operations
        //@{
        bool P( float seconds = INFINITE_TIME ) { return Wait( seconds ); }
        void V() { return Wake(); }
        //@}
    };

    //@{

    SyncEvent * NewSyncEvent( bool initialSignaled, bool autoreset, const char * name = 0 );

    Semaphore * NewSemaphore( size_t maxcount, size_t initialcount, const char * name = 0 );

    //@}
}

#if GN_MSWIN
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
