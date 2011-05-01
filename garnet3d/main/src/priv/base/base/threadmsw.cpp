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
    typedef GN::Thread::ThreadID ThreadID;

    static ThreadID sCreate(
        const Thread::Procedure & proc,
        void                    * param,
        const char              * name )
    {
        // check parameters
        if( proc.empty() )
        {
            GN_ERROR(sLogger)( "Null thread procedure." );
            return NULL;
        }

        // create a new instance.
        ThreadMsw * thread = new ThreadMsw;
        if( NULL == thread )
        {
            GN_ERROR(sLogger)( "Out of memory" );
            return NULL;
        }

        // store user specified properties
        thread->mUserProc = proc;
        thread->mUserParam = param;
        thread->mName = name;

        // create thread parameter.
        ThreadParam tp;
        tp.pThis = thread;
        tp.id    = 0;

        // Create native thread.
        if( !_beginthread(
            sThreadProcDispatcher,
            0, // default stack size
            &tp ) )
        {
            thread->destroy();
            GN_ERROR(sLogger)( "_beginthreadex() failed: %s.", getWin32LastErrorInfo() );
            return 0;
        }

        // We can't assume that thread object is still valid, since it
        // may have finished and self deleted. So we have to get the
        // thread ID from tp.id, instead of from thread->mNativeID.
        while( 0 == tp.id )
        {
            ::Sleep(0);
        }

        // done
        return tp.id;
    }

    static void sTerminate( ThreadID id )
    {
        HANDLE h = OpenThread( THREAD_TERMINATE, FALSE, (DWORD)id );
        if( h )
        {
            TerminateThread( h, (DWORD)-1 );
            CloseHandle( h );
        }
    }

    ThreadID getID() const { return (ThreadID)mNativeID; }

    int run();

    /// self destruction
    void destroy();

private:

    struct ThreadParam
    {
        ThreadMsw       * pThis;
        volatile ThreadID id;
    };

    Thread::Procedure     mUserProc;
    void                * mUserParam;
    StrA                  mName;
    HANDLE                mUtilEvent;
    volatile unsigned int mNativeID;

private:

    ThreadMsw()
        : mUtilEvent(0)
        , mNativeID(0)
    {
    }

    ~ThreadMsw()
    {
    }

    static void sThreadProcDispatcher( void * parameter )
    {
        GN_ASSERT( parameter );

        ThreadParam * p = (ThreadParam*)parameter;

        GN_ASSERT( p->pThis );

        p->id = (ThreadID)GetCurrentThreadId();

        p->pThis->run();
    };
};

//
// Thread table
// -----------------------------------------------------------------------------
class ThreadTable
{
    CRITICAL_SECTION mLock;
    DoubleLink       mThreads;

    void enter()
    {
        EnterCriticalSection( &mLock );
    }

    void leave()
    {
        LeaveCriticalSection( &mLock );
    }

public:

    ThreadTable()
    {
        InitializeCriticalSection( &mLock );
    }

    ~ThreadTable()
    {
        // TODO: verify that all threads should have been deleted.
        DeleteCriticalSection( &mLock );
    }

    void insert( ThreadMsw * thread )
    {
        GN_ASSERT( thread );

        enter();

        mThreads.linkBefore( thread );

        leave();
    }

    void remove( ThreadMsw * thread )
    {
        enter();

        for( ThreadMsw * t = (ThreadMsw*)mThreads.next; t != NULL; t = (ThreadMsw*)t->next )
        {
            if( t == thread )
            {
                // We found it. Now remove it from the list.
                thread->detach();
            }
        }

        leave();
    }

    void kill( Thread::ThreadID thread )
    {
        bool found = false;

        enter();

        for( ThreadMsw * t = (ThreadMsw*)mThreads.next; t != NULL; t = (ThreadMsw*)t->next )
        {
            if( t->getID() == thread )
            {
                // We found it. Now destroy it!
                found = true;
                t->destroy();
                break;
            }
        }

        leave();

        if( !found )
        {
            GN_WARN(sLogger)( "Killing a thread that is not created through garnet thread API." );
        }

        ThreadMsw::sTerminate( thread );
    }
};
static ThreadTable s_ThreadTable;

//
//
// -----------------------------------------------------------------------------
int ThreadMsw::run()
{
    mNativeID = GetCurrentThreadId();

    // Add thread object to the global thread table.
    s_ThreadTable.insert( this );

    // call user's thread proc
    int ret = mUserProc( mUserParam );

    destroy();

    return ret;
}

//
//
// -----------------------------------------------------------------------------
void
ThreadMsw::destroy()
{
    // remove self from thread table.
    s_ThreadTable.remove( this );

    // delte the thread object.
    delete this;
}

// *****************************************************************************
// Public thread functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::Thread::ThreadID GN::Thread::sCreate(
    const Procedure & proc,
    void            * param,
    const char      * name )
{
    return ThreadMsw::sCreate( proc, param, name );
}

//
//
// -----------------------------------------------------------------------------
void GN::Thread::sKill( ThreadID id )
{
    s_ThreadTable.kill( id );
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
GN::WaitResult GN::Thread::sWaitForTermination(
    ThreadID         thread,
    TimeInNanoSecond timeoutTime,
    uint32         * threadProcReturnValue )
{
    // can't wait for self termination
    if( sIsCurrentThread(thread) )
    {
        GN_ERROR(sLogger)("Can't wait for termination of the calling thread." );
        return WaitResult::TIMEDOUT;
    }

    HANDLE h = OpenThread( SYNCHRONIZE, FALSE, (DWORD)thread );
    if( 0 == h )
    {
        GN_ERROR(sLogger)("Invalid thread ID or access denied." );
        return WaitResult::FAILED;
    }

    uint32 ret = ::WaitForSingleObject( h, ns2ms( timeoutTime ) );

    if( WAIT_TIMEOUT == ret )
    {
        GN_TRACE(sLogger)( "timed out!" );
        return WaitResult::TIMEDOUT;
    }
    else if( WAIT_OBJECT_0 == ret )
    {
        if( threadProcReturnValue )
        {
            GN_MSW_CHECK( GetExitCodeThread( h, (LPDWORD)threadProcReturnValue ) );
        }
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
GN::Thread::ThreadID GN::Thread::sGetCurrentThread()
{
    return (ThreadID)GetCurrentThreadId();
}

//
//
// -----------------------------------------------------------------------------
bool GN::Thread::sIsCurrentThread( ThreadID id )
{
    ThreadID current = (ThreadID)GetCurrentThreadId();
    return current == id;
}

//
//
// -----------------------------------------------------------------------------
GN::Thread::Priority GN::Thread::sGetPriority( ThreadID )
{
    GN_UNIMPL_WARNING();
    return Thread::NORMAL;
}

//
//
// -----------------------------------------------------------------------------
void GN::Thread::sSetPriority( ThreadID t, Priority p )
{
    if( p < 0 || p >= NUM_PRIORITIES )
    {
        GN_ERROR(sLogger)( "invalid thread priority!" );
        return;
    }

    HANDLE h = OpenThread( THREAD_ALL_ACCESS, FALSE, (DWORD)t );
    if( h )
    {
        GN_MSW_CHECK( ::SetThreadPriority( h, sPriority2Msw(p) ) );
        CloseHandle( h );
    }
}

//
//
// -----------------------------------------------------------------------------
void GN::Thread::sSetAffinity( ThreadID, uint32 )
{
    GN_UNIMPL_WARNING();
}

#endif
