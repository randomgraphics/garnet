#include "pch.h"

#if GN_POSIX

float const GN::INFINITE_TIME = 1e38f;

static GN::Logger * sLogger = GN::GetLogger("GN.base.Sync");

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
bool GN::Mutex::TryLock()
{
    return 0 == pthread_mutex_trylock( (pthread_mutex_t*)mInternal );
}

//
//
// -----------------------------------------------------------------------------
void GN::Mutex::Lock()
{
    pthread_mutex_lock( (pthread_mutex_t*)mInternal );
}

//
//
// -----------------------------------------------------------------------------
void GN::Mutex::Unlock()
{
    pthread_mutex_unlock( (pthread_mutex_t*)mInternal );
}

// *****************************************************************************
// syncevent class
// *****************************************************************************

///
/// sync event on POSIX system.
///
class SyncEventPOSIX : public SyncEvent, public StdClass
{
    GN_DECLARE_STDCLASS( SyncEventPOSIX, StdClass );

    // ********************************
    // ctor/dtor
    // ********************************

    //@{
public:
    SyncEventPOSIX()          { Clear(); }
    virtual ~SyncEventPOSIX() { Quit(); }
    //@}

    // ********************************
    // from StdClass
    // ********************************

    //@{
public:
    bool Init( bool initialSignaled, bool autoreset, const char * name )
    {
        GN_GUARD;

        // standard init procedure
        GN_STDCLASS_INIT( SyncEventPOSIX, () );

        GN_UNIMPL();

        // success
        return Success();

        GN_UNGUARD;
    }
    void Quit()
    {
        GN_GUARD;

        GN_UNIMPL();

        // standard Quit procedure
        GN_STDCLASS_QUIT();

        GN_UNGUARD;
    }
private:
    void Clear() {}
    //@}

    // ********************************
    // from SyncEvent
    // ********************************
public:

    virtual void Signal()
    {
        GN_UNIMPL_WARNING();
    }

    virtual void Unsignal()
    {
        GN_UNIMPL_WARNING();
    }

    virtual bool Wait( float seconds )
    {
        GN_UNIMPL_WARNING();
        return true;
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
    SemaphorePOSIX()          { Clear(); }
    virtual ~SemaphorePOSIX() { Quit(); }
    //@}

    // ********************************
    // from StdClass
    // ********************************

    //@{
public:
    bool Init( size_t maxcount, size_t initialcount, const char * name )
    {
        GN_GUARD;

        // standard init procedure
        GN_STDCLASS_INIT( SemaphorePOSIX, () );

        GN_UNIMPL();

        // success
        return Success();

        GN_UNGUARD;
    }
    void Quit()
    {
        GN_GUARD;

        GN_UNIMPL();

        // standard Quit procedure
        GN_STDCLASS_QUIT();

        GN_UNGUARD;
    }
private:
    void Clear() {}
    //@}

    // ********************************
    // from Semaphore
    // ********************************
public:

    virtual bool Wait( float seconds )
    {
        GN_UNIMPL_WARNING();
        return true;
    }

    virtual void Wake( size_t count )
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
GN::SyncEvent * GN::NewSyncEvent(
    bool initialSignaled,
    bool autoreset,
    const char * name )
{
    GN_GUARD;

    AutoObjPtr<SyncEventPOSIX> s( new SyncEventPOSIX );

    if( !s->Init( initialSignaled, autoreset, name ) ) return 0;

    return s.Detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::Semaphore * GN::NewSemaphore(
    size_t maxcount,
    size_t initialcount,
    const char * name )
{
    GN_GUARD;

    AutoObjPtr<SemaphorePOSIX> s( new SemaphorePOSIX );

    if( !s->Init( maxcount, initialcount, name ) ) return 0;

    return s.Detach();

    GN_UNGUARD;
}

#endif
