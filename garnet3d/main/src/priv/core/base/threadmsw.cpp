#include "pch.h"

#if GN_MSWIN || GN_XENON

#include <process.h>

static GN::Logger * sLogger = GN::getLogger("GN.base.Thread");

using namespace GN;

// *****************************************************************************
// local class and functions
// *****************************************************************************


// defined in syncmsw.cpp
extern uint32 ns2ms( TimeInNanoSecond ns );

static int sPriorityTable[] =
{
    THREAD_PRIORITY_TIME_CRITICAL,
    THREAD_PRIORITY_ABOVE_NORMAL,
    THREAD_PRIORITY_NORMAL,
    THREAD_PRIORITY_BELOW_NORMAL,
    THREAD_PRIORITY_IDLE,
};
GN_CASSERT( GN_ARRAY_COUNT(sPriorityTable) == Thread::NUM_PRIORITIES );

///
/// convert thread priority to WIN32 constant
///
static int sPriority2Msw( Thread::Priority p )
{
    GN_ASSERT( p < Thread::NUM_PRIORITIES );

    return sPriorityTable[p];
}

// *****************************************************************************
// Thread::ThreadMsw class
// *****************************************************************************

/*
/// thread class on MS Windows
///
class ThreadMsw : public Thread, public StdClass
{
    GN_DECLARE_STDCLASS( ThreadMsw, StdClass );

    // ********************************
    // ctor/dtor
    // ********************************

    //@{
public:
    ThreadMsw()          { clear(); }
    virtual ~ThreadMsw() { quit(); }
    //@}

    // ********************************
    // from StdClass
    // ********************************

    //@{
public:
    bool create(
        const Procedure & proc,
        void * param,
        Priority priority,
        const char * )
    {
        GN_GUARD;

        // standard init procedure
        GN_STDCLASS_INIT( ThreadMsw, () );

        // check parameter
        if( proc.empty() )
        {
            GN_ERROR(sLogger)( "Null thread procedure." );
            return failure();
        }
        if( priority < 0 || priority >= NUM_PRIORITIES )
        {
            GN_ERROR(sLogger)( "invalid thread priority." );
            return failure();
        }

        mProc = proc;
        mParam.instance = this;
        mParam.userparam = param;
        mPriority = priority;

        mHandle = (HANDLE)_beginthreadex(
            0, // security
            0, // default stack size
            &sProcDispatcher,
            &mParam,
            0,
            (unsigned int*)&mId );
        GN_MSW_CHECK_RETURN( mHandle, failure() );

        // success
        mAttached = false;
        return success();

        GN_UNGUARD;
    }

    bool attach()
    {
        GN_GUARD;

        // standard init procedure
        GN_STDCLASS_INIT( ThreadMsw, () );

        mHandle = GetCurrentThread();
        mId     = GetCurrentThreadId();

        // TODO: get real priority value, then convert to TP_XXX enums.
        mPriority = NORMAL;

        // success
        mAttached = true;
        return success();

        GN_UNGUARD;
    }

    void quit()
    {
        GN_GUARD;

        if( !mAttached && mHandle )
        {
            // Kill the thread.
            TerminateThread( mHandle, (DWORD)-1 );

            // close thread handle
            CloseHandle( mHandle );
        }

        // standard quit procedure
        GN_STDCLASS_QUIT();

        GN_UNGUARD;
    }

private:
    void clear()
    {
        mAttached = false;
        mHandle = 0;
        mId = 0;
    }
    //@}

    // ********************************
    // inherited from Thread
    // ********************************
public:

    virtual sint32 getID() const
    {
        return mId;
    }

    virtual Priority getPriority() const
    {
        return mPriority;
    }

    virtual void setPriority( Priority p ) const
    {
        if( p < 0 || p >= NUM_PRIORITIES )
        {
            GN_ERROR(sLogger)( "invalid thread priority!" );
            return;
        }

        GN_MSW_CHECK_RETURN_VOID( ::SetThreadPriority( mHandle, sPriority2Msw(p) ) );

        mPriority = p;
    }

    virtual void setAffinity( uint32 hardwareThread ) const
    {
#if GN_XENON
        if( (DWORD)-1 == XSetThreadProcessor( mHandle, hardwareThread ) )
        {
            GN_ERROR(sLogger)( "fail to set thread affinity: %s", getWin32LastErrorInfo() );
        }
#else
        if( (DWORD)-1 == SetThreadIdealProcessor( mHandle, hardwareThread ) )
        {
            GN_ERROR(sLogger)( "fail to set thread affinity: %s", getWin32LastErrorInfo() );
        }
#endif
    }

    virtual bool isCurrentThread() const
    {
        return ::GetCurrentThreadId() == mId;
    }

    virtual WaitResult waitForTermination( TimeInNanoSecond timeoutTime, uint32 * threadProcReturnValue )
    {
        // can't wait for self termination
        if( isCurrentThread() )
        {
            GN_ERROR(sLogger)("Can't wait for termination of the current thread." );
            return WaitResult::TIMEDOUT;
        }

        uint32 ret = ::WaitForSingleObject( mHandle, ns2ms( timeoutTime ) );

        if( WAIT_TIMEOUT == ret )
        {
            GN_TRACE(sLogger)( "timed out!" );
            return WaitResult::TIMEDOUT;
        }
        else if( WAIT_OBJECT_0 == ret )
        {
            if( threadProcReturnValue )
            {
                GN_MSW_CHECK( GetExitCodeThread( mHandle, (LPDWORD)threadProcReturnValue ) );
            }
            return WaitResult::COMPLETED;
        }
        else
        {
            // The thread is killed before the thread procedure returns.
            GN_ERROR(sLogger)( getWin32LastErrorInfo() );
            return WaitResult::KILLED;
        }
    }

    // ********************************
    // private variables
    // ********************************
private:

    struct ThreadParam
    {
        ThreadMsw * instance;
        void * userparam;
    };

    Procedure        mProc;
    ThreadParam      mParam;
    mutable Priority mPriority;

    HANDLE           mHandle;
    DWORD            mId;

    bool             mAttached;

    // ********************************
    // private functions
    // ********************************
private:

    ///
    /// thread procedure dispather
    ///
    static unsigned int __stdcall sProcDispatcher( void * parameter )
    {
        GN_ASSERT( parameter );

        ThreadParam * p = (ThreadParam*)parameter;

        GN_ASSERT( p->instance );

        return p->instance->mProc( p->userparam );
    };

};*/

//
// Thread object
// -----------------------------------------------------------------------------
struct ThreadMsw : public DoubleLink
{
    typedef GN::Thread::Identifier Identifier;

    static Identifier sCreate(
        const Thread::Procedure & proc,
        void                    * param,
        const char              * name )
    {
        // check parameters
        if( proc.empty() )
        {
            GN_ERROR(sLogger)( "Null thread procedure." );
            return 0;
        }

        // create a new instance.
        ThreadMsw * thread = new ThreadMsw;
        if( NULL == thread )
        {
            GN_ERROR(sLogger)( "Out of memory" );
            return 0;
        }

        // store user specified properties
        thread->mUserProc = proc;
        thread->mUserParam = param;
        thread->mName = name;

        // create init events (auto-reset, initially unsignaled)
        for( size_t i = 0; i < GN_ARRAY_COUNT(thread->mInitEvents); ++i )
        {
            thread->mInitEvents[i] = ::CreateEvent( NULL, FALSE, FALSE, NULL );
            if( !thread->mInitEvents[i] )
            {
                delete thread;
                GN_ERROR(sLogger)( "Fail to create init event for the thread: %s.", getWin32LastErrorInfo() );
                return 0;
            }
        }

        // Create native thread.
        // Note that the new thread, if created, will be blocked by the init event #0.
        // That's why we can safely access mNativeHandle and mNativeID here.
        thread->mNativeHandle = (HANDLE)_beginthreadex(
            0, // default security
            0, // default stack size
            sMainThreadProc,
            thread,
            0, // default flag,
            &thread->mNativeID );
        if( 0 == thread->mNativeHandle )
        {
            delete thread;
            GN_ERROR(sLogger)( "_beginthreadex() failed: %s.", getWin32LastErrorInfo() );
            return 0;
        }

        // The first step of initialization is done.
        // Now its time to resume the newly created thread.
        SetEvent( thread->mInitEvents[0] );

        // Now wait for the new tread to be fully initialized.
        ::WaitForSingleObject( thread->mInitEvents[1], INFINITE );

        // done
        return thread->mID;
    }

    static Thread::Identifier sAttachToCurrentThread();

    Identifier getID() const { return mID; }

    DWORD getNativeID() const { return mNativeID; }

    HANDLE getNativeHandle() const { return mNativeHandle; }

    ~ThreadMsw();

private:

    Thread::Identifier    mID;
    Thread::Procedure     mUserProc;
    void                * mUserParam;
    StrA                  mName;
    HANDLE                mInitEvents[2];
    unsigned int          mNativeID;
    HANDLE                mNativeHandle;

private:

    ThreadMsw();
    static unsigned int __stdcall sMainThreadProc( void * parameter );
    static void sUtilThreadProc( void * parameter );
};

//
// Thread table
// -----------------------------------------------------------------------------
class ThreadTable
{
    mutable CRITICAL_SECTION   mLock;
    DoubleLink                 mThreads;
    Thread::Identifier         mNextIdentifier;

    void enter() const
    {
        EnterCriticalSection( &mLock );
    }

    void leave() const
    {
        LeaveCriticalSection( &mLock );
    }

public:

    ThreadTable() : mNextIdentifier(1)
    {
        InitializeCriticalSection( &mLock );
    }

    ~ThreadTable()
    {
        // Close remaining thread objects, but do not terminate them by force.
        for(;;)
        {
            // Remove the tread from thread table first.
            enter();
            ThreadMsw * thread = (ThreadMsw*)mThreads.next;
            if( thread ) thread->detach();
            leave();

            if( thread )
                // Delete the thread object.
                delete thread;
            else
                // No more threads in the table. Break out of the loop.
                break;
        }

        DeleteCriticalSection( &mLock );
    }

    Thread::Identifier newThreadId()
    {
        enter();

        Thread::Identifier id = mNextIdentifier++;

        leave();

        return id;
    }

    bool isValidThreadId( Thread::Identifier id ) const
    {
        enter();

        bool valid = id > 0 && id < mNextIdentifier;

        leave();

        return valid;
    }

    void insert( ThreadMsw * thread )
    {
        GN_ASSERT( thread );

        enter();

        if( mThreads.next )
        {
            GN_VERIFY( thread->linkBefore( mThreads.next ) );
        }
        else
        {
            GN_VERIFY( thread->linkAfter( &mThreads ) );
        }

        // Always insert "after" mThreads.
        GN_ASSERT( NULL == mThreads.prev && NULL != mThreads.next );

        leave();
    }

    bool remove( ThreadMsw * thread )
    {
        bool removed = false;

        enter();

        for( ThreadMsw * t = (ThreadMsw*)mThreads.next; t != NULL; t = (ThreadMsw*)t->next )
        {
            if( t == thread )
            {
                // We found it. Now remove it from the list.
                t->detach();
                removed = true;
            }
        }

        leave();

        return removed;
    }

    Thread::Identifier getCurrentThread()
    {
        DWORD currentThread = GetCurrentThreadId();

        Thread::Identifier id = 0;

        enter();
        for( ThreadMsw * t = (ThreadMsw*)mThreads.next; t != NULL; t = (ThreadMsw*)t->next )
        {
            if( t->getNativeID() == currentThread )
            {
                // We found it.
                id = t->getID();
                break;
            }
        }
        leave();

        if( 0 == id )
        {
            id = ThreadMsw::sAttachToCurrentThread();
        }

        return id;
    }

    /// Be cautious that by the time the function returns, the thread might have ended
    /// and self destructed.
    HANDLE getNativeHandle( Thread::Identifier id )
    {
        HANDLE h = 0;

        enter();

        for( ThreadMsw * t = (ThreadMsw*)mThreads.next; t != NULL; t = (ThreadMsw*)t->next )
        {
            if( t->getID() == id )
            {
                h = t->getNativeHandle();
                break;
            }
        }

        leave();

        return h;
    }
};
static ThreadTable s_ThreadTable;

//
//
// -----------------------------------------------------------------------------
ThreadMsw::ThreadMsw()
    : mID( s_ThreadTable.newThreadId() )
    , mNativeID(0)
    , mNativeHandle(0)
{
    //InitializeCriticalSection( &mHoldingLock );
    memset( mInitEvents, 0, sizeof(mInitEvents) );
}

//
//
// -----------------------------------------------------------------------------
ThreadMsw::~ThreadMsw()
{
    for( size_t i = 0; i < GN_ARRAY_COUNT(mInitEvents); ++i )
    {
        if( 0 != mInitEvents[i] )
        {
            CloseHandle( mInitEvents[i] );
        }
    }

    if( mNativeHandle )
    {
        CloseHandle( mNativeHandle );
    }
}

//
//
// -----------------------------------------------------------------------------
Thread::Identifier ThreadMsw::sAttachToCurrentThread()
{
    // create a new instance.
    ThreadMsw * thread = new ThreadMsw;
    if( NULL == thread )
    {
        GN_ERROR(sLogger)( "Out of memory" );
        return 0;
    }

    // get native thread ID.
    thread->mNativeID = GetCurrentThreadId();

    // Create one init event.
    thread->mInitEvents[0] = ::CreateEvent( NULL, FALSE, FALSE, NULL );
    if( !thread->mInitEvents[0] )
    {
        GN_ERROR(sLogger)( "Fail to create init event for the thread: %s.", getWin32LastErrorInfo() );
        delete thread;
        return 0;
    }

    // Get native handle of the thread.
    thread->mNativeHandle = OpenThread( SYNCHRONIZE | THREAD_TERMINATE, FALSE, thread->mNativeID );
    if( !thread->mNativeHandle )
    {
        GN_ERROR(sLogger)( "Fail to open thread %d: %s", thread->mNativeID, getWin32LastErrorInfo() );
        delete thread;
        return 0;
    }

    // create a utility thread that monitors termination of the current thread.
    if( !_beginthread(
        sUtilThreadProc,
        0, // default stack size
        thread ) )
    {
        GN_ERROR(sLogger)( "Fail to create utility thread: %s.", getWin32LastErrorInfo() );
        delete thread;
        return 0;
    }

    // Wait until init event is signaled.
    ::WaitForSingleObject( thread->mInitEvents[0], INFINITE );

    // done
    return thread->mID;
}

//
//
// -----------------------------------------------------------------------------
unsigned int __stdcall ThreadMsw::sMainThreadProc( void * parameter )
{
    GN_ASSERT( parameter );
    ThreadMsw * pThis = (ThreadMsw*)parameter;

    // Wait for init event 0.
    ::WaitForSingleObject( pThis->mInitEvents[0], INFINITE );

    // Add thread object to the global thread table.
    s_ThreadTable.insert( pThis );

    // Now signal the creation thread that initialization is done.
    ::SetEvent( pThis->mInitEvents[1] );

    // call user's thread proc
    pThis->mUserProc( pThis->mUserParam );

    // remove self from thread table, atomically
    if( s_ThreadTable.remove( pThis ) )
    {
        // self destruct.
        delete pThis;
    }
    else
    {
        // this means that the thread is being killed, or has been killed.
    }

    return 0;
}

//
//
// -----------------------------------------------------------------------------
void ThreadMsw::sUtilThreadProc( void * parameter )
{
    GN_ASSERT( parameter );

    ThreadMsw * pThis = (ThreadMsw*)parameter;

    // Add self to the thread table.
    s_ThreadTable.insert( pThis );

    // Now signal the creation thread that initialization is done.
    ::SetEvent( pThis->mInitEvents[0] );

    // Wait for termination of the native thread.
    ::WaitForSingleObject( pThis->mNativeHandle, INFINITE );

    // remove self from thread table, atomically
    if( s_ThreadTable.remove( pThis ) )
    {
        // self destruct.
        delete pThis;
    }
    else
    {
        // this means that the thread is being killed, or has been killed.
    }
}


// *****************************************************************************
// Public thread functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN_API GN::Thread::Identifier GN::Thread::sCreate(
    const Procedure & proc,
    void            * param,
    const char      * name )
{
    return ThreadMsw::sCreate( proc, param, name );
}

//
//
// -----------------------------------------------------------------------------
GN_API GN::WaitResult GN::Thread::sWaitForTermination(
    Identifier       thread,
    TimeInNanoSecond timeoutTime )
{
    if( !s_ThreadTable.isValidThreadId( thread ) )
    {
        GN_ERROR(sLogger)("Invalid thread ID.", thread );
        return WaitResult::FAILED;
    }

    // can't wait for self termination
    if( sIsCurrentThread(thread) )
    {
        GN_ERROR(sLogger)("Can't wait for termination of the calling thread." );
        return WaitResult::TIMEDOUT;
    }

    HANDLE nativeHandle = s_ThreadTable.getNativeHandle( thread );
    if( 0 == nativeHandle )
    {
        // The thread has died already.
        return WaitResult::KILLED;
    }

    uint32 ret = ::WaitForSingleObject( nativeHandle, ns2ms( timeoutTime ) );

    if( WAIT_TIMEOUT == ret )
    {
        GN_TRACE(sLogger)( "timed out!" );
        return WaitResult::TIMEDOUT;
    }
    else if( WAIT_OBJECT_0 == ret )
    {
        return WaitResult::COMPLETED;
    }
    else
    {
        GN_ERROR(sLogger)( getWin32LastErrorInfo() );
        return WaitResult::FAILED;
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::Thread::sSleepCurrentThread( TimeInNanoSecond sleepTime )
{
    ::Sleep( ns2ms( sleepTime ) );
}

//
//
// -----------------------------------------------------------------------------
GN_API GN::Thread::Identifier GN::Thread::sGetCurrentThread()
{
    return s_ThreadTable.getCurrentThread();
}

//
//
// -----------------------------------------------------------------------------
GN_API bool GN::Thread::sIsCurrentThread( Identifier id )
{
    return id == s_ThreadTable.getCurrentThread();
}

//
//
// -----------------------------------------------------------------------------
GN_API GN::Thread::Priority GN::Thread::sGetPriority( Identifier )
{
    GN_UNIMPL_WARNING();
    return Thread::NORMAL;
}

//
//
// -----------------------------------------------------------------------------
GN_API void GN::Thread::sSetPriority( Identifier t, Priority p )
{
    if( p < 0 || p >= NUM_PRIORITIES )
    {
        GN_ERROR(sLogger)( "invalid thread priority!" );
        return;
    }

    int nativePriority = sPriority2Msw(p);

    HANDLE nativeHandle = s_ThreadTable.getNativeHandle( t );
    if( 0 == nativeHandle )
    {
        GN_ERROR(sLogger)( "Thread %t is either invalid or has terminated." );
        return;
    }

    GN_MSW_CHECK( ::SetThreadPriority( nativeHandle, nativePriority ) );
}

//
//
// -----------------------------------------------------------------------------
GN_API void GN::Thread::sSetAffinity( Identifier, uint32 )
{
    GN_UNIMPL_WARNING();
}

#endif
