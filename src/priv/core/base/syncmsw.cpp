#include "pch.h"

#if GN_MSWIN || GN_XENON

const GN::TimeInNanoSecond GN::INFINITE_TIME = (uint64)(-1);
const GN::TimeInNanoSecond GN::ONE_SECOND_IN_NS = 1000000000; // 10^9

static GN::Logger * sLogger = GN::getLogger("GN.base.Sync");

using namespace GN;

// *****************************************************************************
// local class and functions
// *****************************************************************************


//
// convert time from nanoseconds to milliseconds on MS windows platform
// Note: this function is also used in threadmsw.cpp.
// -----------------------------------------------------------------------------
uint32 ns2ms( TimeInNanoSecond time )
{
    if( INFINITE_TIME == time ) return INFINITE;
    else return (uint32)( time / 1000000 );
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
        return WaitResult::TIMEDOUT;
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
// -----------------------------------------------------------------------------
GN_API GN::Mutex::Mutex()
{
    GN_CASSERT( sizeof(CRITICAL_SECTION) <= sizeof(mInternal) );

    // try spin loop 5000 times before sleeping.
    InitializeCriticalSectionAndSpinCount( (CRITICAL_SECTION*)mInternal, 5000 );
}

//
//
// -----------------------------------------------------------------------------
GN_API GN::Mutex::~Mutex()
{
    DeleteCriticalSection( (CRITICAL_SECTION*)mInternal );
}

//
//
// -----------------------------------------------------------------------------
GN_API bool GN::Mutex::trylock()
{
    return !!TryEnterCriticalSection( (CRITICAL_SECTION*)mInternal );
}

//
//
// -----------------------------------------------------------------------------
GN_API void GN::Mutex::lock()
{
    EnterCriticalSection( (CRITICAL_SECTION*)mInternal );
}

//
//
// -----------------------------------------------------------------------------
GN_API void GN::Mutex::unlock()
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

GN_API GN::SyncEvent::SyncEvent() : mImpl(NULL) { mImpl = new Impl(); }
GN_API GN::SyncEvent::~SyncEvent() { delete mImpl; }
GN_API bool GN::SyncEvent::create(SyncEvent::InitialState initialState, SyncEvent::ResetMode resetMode, const char * name ) { return mImpl->init(initialState, resetMode, name); }
GN_API void GN::SyncEvent::destroy() { return mImpl->quit(); }
GN_API void GN::SyncEvent::signal() { return mImpl->signal(); }
GN_API void GN::SyncEvent::unsignal() { return mImpl->unsignal(); }
GN_API GN::WaitResult GN::SyncEvent::wait( TimeInNanoSecond timeoutTime ) const { return mImpl->wait( timeoutTime ); }

// *****************************************************************************
// semaphore class
// *****************************************************************************

///
/// semaphore on MS Windows.
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

    WaitResult wait( TimeInNanoSecond timeoutTime )
    {
        return sWaitResultFromWin32( WaitForSingleObject( mHandle, ns2ms( timeoutTime ) ) );
    }

    void wake( size_t count )
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

GN_API GN::Semaphore::Semaphore() : mImpl(NULL) { mImpl= new Impl(); }
GN_API GN::Semaphore::~Semaphore() { delete mImpl; }
GN_API bool GN::Semaphore::create( size_t maxcount, size_t initialcount, const char * name ) { return mImpl->init( maxcount, initialcount, name ); }
GN_API void GN::Semaphore::destroy() { return mImpl->quit(); }
GN_API GN::WaitResult GN::Semaphore::wait( TimeInNanoSecond timeoutTime ) { return mImpl->wait( timeoutTime ); }
GN_API void GN::Semaphore::wake( size_t count ) { return mImpl->wake( count ); }

#endif
