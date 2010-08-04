#include "pch.h"

#if GN_POSIX

GN::TimeInNanoSecond const GN::INFINITE_TIME = (GN::TimeInNanoSecond)-1;

static GN::Logger * sLogger = GN::getLogger("GN.base.Sync");

using namespace GN;

// *****************************************************************************
// mutex class
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::Mutex::Mutex()
{
    // initiialize a recursive mutex (same behavior as mutex on MSWIN)
    GN_CASSERT( sizeof(pthread_mutex_t) <= sizeof(mInternal) );
    pthread_mutexattr_t mta;
    pthread_mutexattr_init(&mta);
    pthread_mutexattr_settype( &mta, PTHREAD_MUTEX_RECURSIVE );
    pthread_mutex_init( (pthread_mutex_t*)mInternal, &mta );
    pthread_mutexattr_destroy(&mta);
}

//
//
// -----------------------------------------------------------------------------
GN::Mutex::~Mutex()
{
    pthread_mutex_destroy( (pthread_mutex_t*)mInternal );
}

//
//
// -----------------------------------------------------------------------------
bool GN::Mutex::trylock()
{
    return 0 == pthread_mutex_trylock( (pthread_mutex_t*)mInternal );
}

//
//
// -----------------------------------------------------------------------------
void GN::Mutex::lock()
{
    pthread_mutex_lock( (pthread_mutex_t*)mInternal );
}

//
//
// -----------------------------------------------------------------------------
void GN::Mutex::unlock()
{
    pthread_mutex_unlock( (pthread_mutex_t*)mInternal );
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
    Impl()          { clear(); }
    virtual ~Impl() { quit(); }
    //@}

    // ********************************
    // from StdClass
    // ********************************

    //@{
public:
    bool init( SyncEvent::InitialState initialState, SyncEvent::ResetMode resetMode, const char * name )
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
    // from SyncEvent
    // ********************************
public:

    void signal()
    {
        GN_UNIMPL_WARNING();
    }

    void unsignal()
    {
        GN_UNIMPL_WARNING();
    }

    WaitResult wait( TimeInNanoSecond timeoutTime )
    {
        GN_UNIMPL_WARNING();
        return WaitResult::KILLED;
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
class SemaphorePOSIX : public Semaphore, public StdClass
{
    GN_DECLARE_STDCLASS( SemaphorePOSIX, StdClass );

    // ********************************
    // ctor/dtor
    // ********************************

    //@{
public:
    SemaphorePOSIX()          { clear(); }
    virtual ~SemaphorePOSIX() { quit(); }
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
        GN_STDCLASS_INIT( SemaphorePOSIX, () );

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

    virtual WaitResult wait( TimeInNanoSecond timeoutTime )
    {
        GN_UNIMPL_WARNING();
        return WaitResult::COMPLETED;
    }

    virtual void wake( size_t count )
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

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::Semaphore * GN::createSemaphore(
    size_t maxcount,
    size_t initialcount,
    const char * name )
{
    GN_GUARD;

    AutoObjPtr<SemaphorePOSIX> s( new SemaphorePOSIX );

    if( !s->init( maxcount, initialcount, name ) ) return 0;

    return s.detach();

    GN_UNGUARD;
}

#endif
