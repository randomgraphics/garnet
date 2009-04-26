#include "pch.h"

#if GN_POSIX

float const GN::INFINITE_TIME = 1e38f;

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
/// sync event on MS Windows.
///
class SyncEventX11 : public SyncEvent, public StdClass
{
    GN_DECLARE_STDCLASS( SyncEventX11, StdClass );

    // ********************************
    // ctor/dtor
    // ********************************

    //@{
public:
    SyncEventX11()          { clear(); }
    virtual ~SyncEventX11() { quit(); }
    //@}

    // ********************************
    // from StdClass
    // ********************************

    //@{
public:
    bool init( bool initialSignaled, bool autoreset, const char * name )
    {
        GN_GUARD;

        // standard init procedure
        GN_STDCLASS_INIT( SyncEventX11, () );

        GN_UNIMPL_WARNING();

        // success
        return success();

        GN_UNGUARD;
    }
    void quit()
    {
        GN_GUARD;

        GN_UNIMPL_WARNING();

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

    virtual void signal()
    {
        GN_UNIMPL_WARNING();
    }

    virtual void unsignal()
    {
        GN_UNIMPL_WARNING();
    }

    virtual bool wait( float seconds )
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
/// semaphore on MS Windows.
///
class SemaphoreX11 : public Semaphore, public StdClass
{
    GN_DECLARE_STDCLASS( SemaphoreX11, StdClass );

    // ********************************
    // ctor/dtor
    // ********************************

    //@{
public:
    SemaphoreX11()          { clear(); }
    virtual ~SemaphoreX11() { quit(); }
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
        GN_STDCLASS_INIT( SemaphoreX11, () );

        GN_UNIMPL_WARNING();

        // success
        return success();

        GN_UNGUARD;
    }
    void quit()
    {
        GN_GUARD;

        GN_UNIMPL_WARNING();

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

    virtual bool wait( float seconds )
    {
        GN_UNIMPL_WARNING();
        return true;
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
GN::SyncEvent * GN::createSyncEvent(
    bool initialSignaled,
    bool autoreset,
    const char * name )
{
    GN_GUARD;

    AutoObjPtr<SyncEventX11> s( new SyncEventX11 );

    if( !s->init( initialSignaled, autoreset, name ) ) return 0;

    return s.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::Semaphore * GN::createSemaphore(
    size_t maxcount,
    size_t initialcount,
    const char * name )
{
    GN_GUARD;

    AutoObjPtr<SemaphoreX11> s( new SemaphoreX11 );

    if( !s->init( maxcount, initialcount, name ) ) return 0;

    return s.detach();

    GN_UNGUARD;
}

#endif
