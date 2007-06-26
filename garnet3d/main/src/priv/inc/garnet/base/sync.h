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

    inline SInt32 atomGet32( const SInt32 volatile * ); ///< get 32bit integer value.
    inline void   atomSet32( SInt32 volatile *, SInt32 ); ///< set 32bit integer value.
    inline SInt32 atomInc32( SInt32 volatile * ); ///< return incremented value
    inline SInt32 atomDec32( SInt32 volatile * ); ///< return decremented value
    inline SInt32 atomXchg32( SInt32 volatile * dest, SInt32 xchg ); ///< return initial value of the destination.

    ///
    /// if initial value of "dest" equals "cmp", then do exchange; else, do nothing.
    ///
    /// \return
    ///     Always return initial value of "dest".
    ///
    inline SInt32 atomCmpXchg32( SInt32 volatile * dest, SInt32 xchg, SInt32 cmp );

    ///
    /// memory barrier. currently implemented on MS Windows platform only.
    ///
    inline void memoryBarrier();

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
        bool trylock() { return 0 == atomCmpXchg32( &mLock, 1, 0 ); }
        void lock() { while( 0 != atomCmpXchg32( &mLock, 1, 0 ) ); }
        void unlock() { atomSet32( &mLock, 0 ); }
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
    struct SyncEvent : public NoCopy
    {
        //@{
        virtual void signal() = 0;   ///< signal the event, wake one thread that is waiting for it.
        virtual void unsignal() = 0; ///< unsignal the event, block any threads that wait for it.
        virtual bool wait( float seconds = INFINITE_TIME ) = 0; ///< return true means the event is signaled; return false means timeout.
        //@}
    };

    ///
    /// sync event group.
    ///
    /// Represents at most 32 single sync events, indexed from 0 to 31.
    /// Every one of them can be signaled/unsignel/waited independently.
    ///
    struct SyncEventGroup : public NoCopy
    {
        //@{
        virtual int  count() const = 0;     ///< return event count in the group. Should be [1..32];
        virtual bool autoreset() const = 0; ///< true means that wait() function will unsignal the event automatically before returning.
        //@}

        //@{
        virtual void signal( int index ) = 0;
        virtual void unsignal( int index ) = 0;
        virtual void wait( int index, float seconds = INFINITE_TIME ) = 0;
        virtual int  waitAny( float seconds = INFINITE_TIME ) = 0; ///< return index of the event that is signaled, or -1 when timeout.
        //@}
    };

    ///
    /// Abstract semaphore interface
    ///    
    struct Semaphore : public NoCopy
    {
        virtual bool wait( float seconds = INFINITE_TIME ) = 0; ///< block calling thread, until the semaphore is available. return false means timeout.
        virtual void wake( size_t count = 1 ) = 0; ///< wake up specified number of threads that is waiting for this semaphore.

        /// \name aliases for P/V operations
        //@{
        bool P( float seconds = INFINITE_TIME ) { return wait( seconds ); }
        void V() { return wake(); }
        //@}
    };

    //@{

    SyncEventGroup * createSyncEventGroup( int count, bool initialSignaled, bool autoreset, const char * name = 0 );

    SyncEvent * createSyncEvent( bool initialSignaled, bool autoreset, const char * name = 0 );

    Semaphore * createSemaphore( size_t maxcount, size_t initialcount, const char * name = 0 );

    //@}
}

#if GN_MSWIN
#include "syncmsw.inl"
#else
#error unimpl
#endif

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_SYNC_H__
