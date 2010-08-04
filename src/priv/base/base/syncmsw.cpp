#include "pch.h"

#if GN_MSWIN || GN_XENON

const GN::TimeInNanoSecond GN::INFINITE_TIME = (UInt64)(-1);

static GN::Logger * sLogger = GN::getLogger("GN.base.Sync");

using namespace GN;

// *****************************************************************************
// local class and functions
// *****************************************************************************


//
// convert time from nanoseconds to milliseconds on MS windows platform
// Note: this function is also used in threadmsw.cpp.
// -----------------------------------------------------------------------------
UInt32 ns2ms( TimeInNanoSecond time )
{
    if( INFINITE_TIME == time ) return INFINITE;
    else return (UInt32)( time / 1000000 );
}

//
// convert Win32 wait result to WaitResult enumeration
// -----------------------------------------------------------------------------
static inline WaitResult sWaitResultFromWin32( DWORD result )
{
    if( WAIT_OBJECT_0 == result )
    {
        return WaitResult::COMPLETED;
    }
    else if( WAIT_TIMEOUT == result )
    {
        return WaitResult::TIMEOUT;
    }
    else
    {
        return WaitResult::KILLED;
    }
}

// *****************************************************************************
// mutex class
// *****************************************************************************

//
//
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
bool GN::Mutex::trylock()
{
    return !!TryEnterCriticalSection( (CRITICAL_SECTION*)mInternal );
}

//
//
// -----------------------------------------------------------------------------
void GN::Mutex::lock()
{
    EnterCriticalSection( (CRITICAL_SECTION*)mInternal );
}

//
//
// -----------------------------------------------------------------------------
void GN::Mutex::unlock()
{
    LeaveCriticalSection( (CRITICAL_SECTION*)mInternal );
}

// *****************************************************************************
// SyncEvent class
// *****************************************************************************

///
/// sync event on MS Windows.
///
class SyncEvent::Impl : public StdClass
{
    GN_DECLARE_STDCLASS( SyncEvent::Impl, StdClass );

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
        GN_STDCLASS_INIT( SyncEvent::Impl, () );

        GN_MSW_CHECK_RETURN(
            mHandle = CreateEventA( 0, MANUAL_RESET == resetMode, SIGNALED == initialState, name ),
            failure() );

        // success
        return success();

        GN_UNGUARD;
    }
    void quit()
    {
        GN_GUARD;

        if( mHandle ) CloseHandle( mHandle );

        // standard quit procedure
        GN_STDCLASS_QUIT();

        GN_UNGUARD;
    }
private:
    void clear() { mHandle = 0; }
    //@}

    // ********************************
    // from SyncEvent
    // ********************************
public:

    void signal()
    {
        GN_MSW_CHECK( SetEvent( mHandle ) );
    }

    void unsignal()
    {
        GN_MSW_CHECK( ResetEvent( mHandle ) );
    }

    WaitResult wait( TimeInNanoSecond timeoutTime ) const
    {
        return sWaitResultFromWin32( WaitForSingleObject( mHandle, ns2ms( timeoutTime ) ) );
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

GN::SyncEvent::SyncEvent() : mImpl(NULL) { mImpl = new Impl(); }
GN::SyncEvent::~SyncEvent() { delete mImpl; }
bool GN::SyncEvent::create(SyncEvent::InitialState initialState, SyncEvent::ResetMode resetMode, const char * name ) { return mImpl->init(initialState, resetMode, name); }
void GN::SyncEvent::destroy() { return mImpl->quit(); }
void GN::SyncEvent::signal() { return mImpl->signal(); }
void GN::SyncEvent::unsignal() { return mImpl->unsignal(); }
GN::WaitResult GN::SyncEvent::wait( TimeInNanoSecond timeoutTime ) const { return mImpl->wait( timeoutTime ); }

/*
//
// -----------------------------------------------------------------------------
GN::SyncEvent * GN::createSyncEvent(
    SyncEvent::InitialState initialState,
    SyncEvent::ResetMode resetMode,
    const char * name )
{
    GN_GUARD;

    AutoObjPtr<SyncEventMsw> s( new SyncEventMsw );

    if( !s->init( initialState, resetMode, name ) ) return 0;

    return s.detach();

    GN_UNGUARD;
}*/

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
    SemaphoreMsw()          { clear(); }
    virtual ~SemaphoreMsw() { quit(); }
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
        GN_STDCLASS_INIT( SemaphoreMsw, () );

        GN_MSW_CHECK_RETURN(
            mHandle = CreateSemaphoreA( 0, (LONG)initialcount, (LONG)maxcount, name ),
            failure() );

        // success
        return success();

        GN_UNGUARD;
    }
    void quit()
    {
        GN_GUARD;

        if( mHandle ) CloseHandle( mHandle );

        // standard quit procedure
        GN_STDCLASS_QUIT();

        GN_UNGUARD;
    }
private:
    void clear() { mHandle = 0; }
    //@}

    // ********************************
    // from Semaphore
    // ********************************
public:

    virtual WaitResult wait( TimeInNanoSecond timeoutTime )
    {
        return sWaitResultFromWin32( WaitForSingleObject( mHandle, ns2ms( timeoutTime ) ) );
    }

    virtual void wake( size_t count )
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
GN::Semaphore * GN::createSemaphore(
    size_t maxcount,
    size_t initialcount,
    const char * name )
{
    GN_GUARD;

    AutoObjPtr<SemaphoreMsw> s( new SemaphoreMsw );

    if( !s->init( maxcount, initialcount, name ) ) return 0;

    return s.detach();

    GN_UNGUARD;
}

#endif
