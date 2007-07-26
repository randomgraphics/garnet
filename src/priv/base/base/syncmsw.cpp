#include "pch.h"

float const GN::INFINITE_TIME = 1e38f;

#if GN_MSWIN

static GN::Logger * sLogger = GN::getLogger("GN.base.Sync");

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
// SyncEventGroup class
// *****************************************************************************

///
/// sync event group on MS Windows.
///
class SyncEventGroupMsw : public SyncEventGroup, public StdClass
{
    GN_DECLARE_STDCLASS( SyncEventGroupMsw, StdClass );

    // ********************************
    // ctor/dtor
    // ********************************

    //@{
public:
    SyncEventGroupMsw()          { clear(); }
    virtual ~SyncEventGroupMsw() { quit(); }
    //@}

    // ********************************
    // from StdClass
    // ********************************

    //@{
public:
    bool init( size_t count, bool initialSignaled, bool autoreset, const char * name )
    {
        GN_GUARD;

        // standard init procedure
        GN_STDCLASS_INIT( SyncEventMsw, () );

        if( count < 1 || count > 32 )
        {
            GN_ERROR(sLogger)( "count must be in range [1,32]." );
            return failure();
        }

        for( size_t i = 0; i < count; ++i )
        {
            GN_MSW_CHECK_RV(
                mHandles[i] = CreateEventA( 0, !autoreset, initialSignaled, name ),
                failure() );
        }

        mCount     = count;
        mAutoReset = autoreset;

        // success
        return success();

        GN_UNGUARD;
    }
    void quit()
    {
        GN_GUARD;

        for( int i = 0; i < GN_ARRAY_COUNT(mHandles); ++i )
        {
            if( mHandles[i] )
            {
                CloseHandle( mHandles[i] );
                mHandles[i] = 0;
            }
        }

        // standard quit procedure
        GN_STDCLASS_QUIT();

        GN_UNGUARD;
    }
private:
    void clear() { memset( mHandles, 0, sizeof(mHandles) ); }
    //@}

    // ********************************
    // from SyncEvent
    // ********************************
public:

    virtual size_t count() const { return mCount; }
    virtual bool   autoreset() const { return mAutoReset; }

    virtual void signal( size_t index )
    {
        if( index >= mCount )
        {
            GN_ERROR(sLogger)( "invalid event index." );
            return;
        }

        GN_ASSERT( mHandles[index] );

        GN_MSW_CHECK( SetEvent( mHandles[index] ) );
    }

    virtual void unsignal( size_t index )
    {
        if( index >= mCount )
        {
            GN_ERROR(sLogger)( "invalid event index." );
            return;
        }

        GN_ASSERT( mHandles[index] );

        GN_MSW_CHECK( ResetEvent( mHandles[index] ) );
    }

    virtual bool wait( size_t index, float seconds )
    {
        if( index >= mCount )
        {
            GN_ERROR(sLogger)( "invalid event index." );
            return false;
        }

        GN_ASSERT( mHandles[index] );

        return WAIT_OBJECT_0 == WaitForSingleObject( mHandles[index], sec2usec( seconds ) );
    }

    virtual int waitAny( float seconds )
    {
        DWORD result = WaitForMultipleObjects( (DWORD)mCount, mHandles, false, sec2usec( seconds ) );

        if( WAIT_OBJECT_0 <= result && result < (WAIT_OBJECT_0+mCount) )
        {
            return result - WAIT_OBJECT_0;
        }
        else if( WAIT_ABANDONED_0 <= result && result < (WAIT_ABANDONED_0+mCount) )
        {
            return result - WAIT_ABANDONED_0;
        }
        else if( WAIT_TIMEOUT == result )
        {
            return -1;
        }
        else
        {
            GN_ERROR(sLogger)( "WaitForMultipleObjects() failed: %s", getOSErrorInfo() );
            return -1;
        }
    }

    // ********************************
    // private variables
    // ********************************
private:

    HANDLE mHandles[32];
    size_t mCount;
    bool   mAutoReset;

    // ********************************
    // private functions
    // ********************************
private:
};

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
    SyncEventMsw()          { clear(); }
    virtual ~SyncEventMsw() { quit(); }
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
        GN_STDCLASS_INIT( SyncEventMsw, () );

        GN_MSW_CHECK_RV(
            mHandle = CreateEventA( 0, !autoreset, initialSignaled, name ),
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

    virtual void signal()
    {
        GN_ASSERT( mHandle );
        GN_MSW_CHECK( SetEvent( mHandle ) );
    }

    virtual void unsignal()
    {
        GN_ASSERT( mHandle );
        GN_MSW_CHECK( ResetEvent( mHandle ) );
    }

    virtual bool wait( float seconds )
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

        GN_MSW_CHECK_RV(
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

    virtual bool wait( float seconds )
    {
        return WAIT_OBJECT_0 == WaitForSingleObject( mHandle, sec2usec( seconds ) );
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
GN::SyncEventGroup * GN::createSyncEventGroup(
    size_t       count,
    bool         initialSignaled,
    bool         autoreset,
    const char * name )
{
    GN_GUARD;

    AutoObjPtr<SyncEventGroupMsw> s( new SyncEventGroupMsw );

    if( !s->init( count, initialSignaled, autoreset, name ) ) return 0;

    return s.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN::SyncEvent * GN::createSyncEvent(
    bool initialSignaled,
    bool autoreset,
    const char * name )
{
    GN_GUARD;

    AutoObjPtr<SyncEventMsw> s( new SyncEventMsw );

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

    AutoObjPtr<SemaphoreMsw> s( new SemaphoreMsw );

    if( !s->init( maxcount, initialcount, name ) ) return 0;

    return s.detach();

    GN_UNGUARD;
}

#endif
