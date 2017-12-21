#include "pch.h"

#if GN_POSIX

#include <mutex>

GN::TimeInNanoSecond const GN::INFINITE_TIME = (GN::TimeInNanoSecond)-1;
GN::TimeInNanoSecond const GN::ONE_SECOND_IN_NS = 1000000000; // 10^9

//static GN::Logger * sLogger = GN::getLogger("GN.base.Sync");

using namespace GN;

// *****************************************************************************
// mutex class
// *****************************************************************************

typedef std::recursive_mutex stdmutex;

//
//
// -----------------------------------------------------------------------------
GN::Mutex::Mutex()
{
    // initiialize a recursive mutex (same behavior as mutex on MSWIN)
    GN_CASSERT( sizeof(stdmutex) <= sizeof(mInternal) );
    stdmutex * p = (stdmutex*)this;
    new (p) stdmutex();
}

//
//
// -----------------------------------------------------------------------------
GN::Mutex::~Mutex()
{
    stdmutex * p = (stdmutex*)this;
    p->stdmutex::~stdmutex();
}

//
//
// -----------------------------------------------------------------------------
bool GN::Mutex::trylock()
{
    stdmutex * p = (stdmutex*)this;
    return p->try_lock();
}

//
//
// -----------------------------------------------------------------------------
void GN::Mutex::lock()
{
    stdmutex * p = (stdmutex*)this;
    p->lock();
}

//
//
// -----------------------------------------------------------------------------
void GN::Mutex::unlock()
{
    stdmutex * p = (stdmutex*)this;
    p->unlock();
}

// *****************************************************************************
// syncevent class
// *****************************************************************************

///
/// sync event on POSIX system.
///
class SyncEvent::Impl : public StdClass
{
    GN_DECLARE_STDCLASS( Impl, StdClass );

    // ********************************
    // ctor/dtor
    // ********************************

    //@{
public:
    Impl()
    {
        pthread_mutex_init( &mMutex, NULL );
        clear();
    }
    virtual ~Impl()
    {
        quit();
        pthread_mutex_destroy( &mMutex );
    }
    //@}

    // ********************************
    // from StdClass
    // ********************************

    //@{
public:
    bool init( SyncEvent::InitialState initialState, SyncEvent::ResetMode resetMode, const char * name )
    {
        Locker lock(mMutex);

        // standard init procedure
        GN_STDCLASS_INIT( Impl, () );

        // Create condition
        int error = pthread_cond_init( &mCondition, NULL );
        if( error ) return failure();

        mSignaled = (SyncEvent::SIGNALED == initialState );
        mAutoReset = (SyncEvent::AUTO_RESET == resetMode );
        mInitialized = true;

        // success
        return success();
    }
    void quit()
    {
        Locker lock(mMutex);

        if( mInitialized )
        {
            mInitialized = false;
            pthread_cond_broadcast( &mCondition );
            pthread_cond_destroy( &mCondition );
        }

        // standard quit procedure
        GN_STDCLASS_QUIT();
    }
private:
    void clear()
    {
        mInitialized = false;
    }
    //@}

    // ********************************
    // from SyncEvent
    // ********************************
public:

    void signal()
    {
        Locker lock(mMutex);

        if( mInitialized )
        {
            mSignaled = true;
            pthread_cond_signal( &mCondition );
        }
    }

    void unsignal()
    {
        Locker lock(mMutex);

        if( mInitialized )
        {
            mSignaled = false;
        }
    }

    WaitResult wait( TimeInNanoSecond timeoutTime )
    {
        Locker lock(mMutex);

        if( !mInitialized )
        {
            return WaitResult::KILLED;
        }

        if( mSignaled )
        {
            if( mAutoReset ) mSignaled = false;
            return WaitResult::COMPLETED;
        }

        int error;
        if( INFINITE_TIME == timeoutTime )
        {
            error = pthread_cond_wait( &mCondition, &mMutex );
        }
        else
        {
            timespec time;
            clock_gettime(CLOCK_REALTIME, &time);
            time.tv_nsec += timeoutTime;
            error = pthread_cond_timedwait( &mCondition, &mMutex, &time );
        }

        if( ETIMEDOUT == error )
        {
            return WaitResult::TIMEDOUT;
        }
        else if( error || !mInitialized )
        {
            return WaitResult::KILLED;
        }
        else
        {
            return WaitResult::COMPLETED;
        }
    }

    // ********************************
    // private variables
    // ********************************
private:

    struct Locker
    {
        pthread_mutex_t & mutex;

        Locker( pthread_mutex_t & m ) : mutex(m)
        {
            pthread_mutex_lock( &mutex );
        }

        ~Locker()
        {
            pthread_mutex_unlock( &mutex );
        }
    };

    pthread_mutex_t mMutex;
    pthread_cond_t  mCondition;
    bool            mSignaled;
    bool            mAutoReset;
    bool            mInitialized;

    // ********************************
    // private functions
    // ********************************
private:
};

GN::SyncEvent::SyncEvent() : mImpl(NULL) { mImpl = new Impl(); }
GN::SyncEvent::~SyncEvent() { delete mImpl; }
bool GN::SyncEvent::create(SyncEvent::InitialState initialState, SyncEvent::ResetMode resetMode, const char * name ) { return mImpl->init(initialState, resetMode, name); }
void GN::SyncEvent::destroy() { return mImpl->quit(); }
void GN::SyncEvent::signal() { return mImpl->signal(); }
void GN::SyncEvent::unsignal() { return mImpl->unsignal(); }
GN::WaitResult GN::SyncEvent::wait( TimeInNanoSecond timeoutTime ) const { return mImpl->wait( timeoutTime ); }

// *****************************************************************************
// semaphore class
// *****************************************************************************

///
/// semaphore on POSIX system.
///
class Semaphore::Impl : public StdClass
{
    GN_DECLARE_STDCLASS( Impl, StdClass );

    // ********************************
    // ctor/dtor
    // ********************************

    //@{
public:
    Impl()          { clear(); }
    virtual ~Impl() { quit(); }
    //@}

    // ********************************
    // from StdClass
    // ********************************

    //@{
public:
    bool init( size_t maxcount, size_t initialcount, const char * name )
    {
        GN_GUARD;

        // standard init procedure
        GN_STDCLASS_INIT( Impl, () );

        GN_UNIMPL();

        // success
        return success();

        GN_UNGUARD;
    }
    void quit()
    {
        GN_GUARD;

        GN_UNIMPL();

        // standard quit procedure
        GN_STDCLASS_QUIT();

        GN_UNGUARD;
    }
private:
    void clear() {}
    //@}

    // ********************************
    // from Semaphore
    // ********************************
public:

    WaitResult wait( TimeInNanoSecond timeoutTime )
    {
        GN_UNIMPL_WARNING();
        return WaitResult::COMPLETED;
    }

    void wake( size_t count )
    {
        GN_UNIMPL_WARNING();
    }

    // ********************************
    // private variables
    // ********************************
private:

    // ********************************
    // private functions
    // ********************************
private:
};

GN::Semaphore::Semaphore() : mImpl(NULL) { mImpl= new Impl(); }
GN::Semaphore::~Semaphore() { delete mImpl; }
bool GN::Semaphore::create( size_t maxcount, size_t initialcount, const char * name ) { return mImpl->init( maxcount, initialcount, name ); }
void GN::Semaphore::destroy() { return mImpl->quit(); }
GN::WaitResult GN::Semaphore::wait( TimeInNanoSecond timeoutTime ) { return mImpl->wait( timeoutTime ); }
void GN::Semaphore::wake( size_t count ) { return mImpl->wake( count ); }

#endif
