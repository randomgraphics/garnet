#include "pch.h"

#if GN_MSWIN

float const GN::INFINITE_TIME = 1e38f;

static GN::Logger * sLogger = GN::GetLogger("GN.base.Sync");

using namespace GN;

// *****************************************************************************
// local class and functions
// *****************************************************************************


///
/// convert seconds to milliseconds on MS windows platform
///
UInt32 sec2usec( float time )
{
    if( INFINITE_TIME == time ) return INFINITE;
    else return (UInt32)( time * 1000.0f );
}

// *****************************************************************************
// mutex class
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::Mutex::Mutex()
{
    GN_CASSERT( sizeof(CRITICAL_SECTION) <= sizeof(mInternal) );

    // try spin loop 5000 times before sleeping.
    InitializeCriticalSectionAndSpinCount( (CRITICAL_SECTION*)mInternal, 5000 );
}

//
//
// -----------------------------------------------------------------------------
GN::Mutex::~Mutex()
{
    DeleteCriticalSection( (CRITICAL_SECTION*)mInternal );
}

//
//
// -----------------------------------------------------------------------------
bool GN::Mutex::TryLock()
{
    return !!TryEnterCriticalSection( (CRITICAL_SECTION*)mInternal );
}

//
//
// -----------------------------------------------------------------------------
void GN::Mutex::Lock()
{
    EnterCriticalSection( (CRITICAL_SECTION*)mInternal );
}

//
//
// -----------------------------------------------------------------------------
void GN::Mutex::Unlock()
{
    LeaveCriticalSection( (CRITICAL_SECTION*)mInternal );
}

// *****************************************************************************
// syncevent class
// *****************************************************************************

///
/// sync event on MS Windows.
///
class SyncEventMsw : public SyncEvent, public StdClass
{
    GN_DECLARE_STDCLASS( SyncEventMsw, StdClass );

    // ********************************
    // ctor/dtor
    // ********************************

    //@{
public:
    SyncEventMsw()          { Clear(); }
    virtual ~SyncEventMsw() { Quit(); }
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
        GN_STDCLASS_INIT( SyncEventMsw, () );

        GN_MSW_CHECK_RETURN(
            mHandle = CreateEventA( 0, !autoreset, initialSignaled, name ),
            Failure() );

        // success
        return Success();

        GN_UNGUARD;
    }
    void Quit()
    {
        GN_GUARD;

        if( mHandle ) CloseHandle( mHandle );

        // standard Quit procedure
        GN_STDCLASS_QUIT();

        GN_UNGUARD;
    }
private:
    void Clear() { mHandle = 0; }
    //@}

    // ********************************
    // from SyncEvent
    // ********************************
public:

    virtual void Signal()
    {
        GN_ASSERT( mHandle );
        GN_MSW_CHECK( SetEvent( mHandle ) );
    }

    virtual void Unsignal()
    {
        GN_ASSERT( mHandle );
        GN_MSW_CHECK( ResetEvent( mHandle ) );
    }

    virtual bool Wait( float seconds )
    {
        return WAIT_OBJECT_0 == WaitForSingleObject( mHandle, sec2usec( seconds ) );
    }

    // ********************************
    // private variables
    // ********************************
private:

    HANDLE mHandle;

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
class SemaphoreMsw : public Semaphore, public StdClass
{
    GN_DECLARE_STDCLASS( SemaphoreMsw, StdClass );

    // ********************************
    // ctor/dtor
    // ********************************

    //@{
public:
    SemaphoreMsw()          { Clear(); }
    virtual ~SemaphoreMsw() { Quit(); }
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
        GN_STDCLASS_INIT( SemaphoreMsw, () );

        GN_MSW_CHECK_RETURN(
            mHandle = CreateSemaphoreA( 0, (LONG)initialcount, (LONG)maxcount, name ),
            Failure() );

        // success
        return Success();

        GN_UNGUARD;
    }
    void Quit()
    {
        GN_GUARD;

        if( mHandle ) CloseHandle( mHandle );

        // standard Quit procedure
        GN_STDCLASS_QUIT();

        GN_UNGUARD;
    }
private:
    void Clear() { mHandle = 0; }
    //@}

    // ********************************
    // from Semaphore
    // ********************************
public:

    virtual bool Wait( float seconds )
    {
        return WAIT_OBJECT_0 == WaitForSingleObject( mHandle, sec2usec( seconds ) );
    }

    virtual void Wake( size_t count )
    {
        GN_ASSERT( mHandle );
        GN_MSW_CHECK( ReleaseSemaphore( mHandle, (LONG)count, 0 ) );
    }

    // ********************************
    // private variables
    // ********************************
private:

    HANDLE mHandle;

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

    AutoObjPtr<SyncEventMsw> s( new SyncEventMsw );

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

    AutoObjPtr<SemaphoreMsw> s( new SemaphoreMsw );

    if( !s->Init( maxcount, initialcount, name ) ) return 0;

    return s.Detach();

    GN_UNGUARD;
}

#endif
