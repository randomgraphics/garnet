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

///
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
        bool initialSuspended,
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
            initialSuspended ? CREATE_SUSPENDED : 0,
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
            // wait for thread termination
            waitForTermination( INFINITE_TIME, 0 );

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

    virtual void suspend() const
    {
        if( (DWORD)-1 == ::SuspendThread( mHandle ) )
        {
            GN_ERROR(sLogger)( getWin32LastErrorInfo() );
        }
    }

    virtual void resume() const
    {
        if( (DWORD)-1 == ::ResumeThread( mHandle ) )
        {
            GN_ERROR(sLogger)( getWin32LastErrorInfo() );
        }
    }

    virtual void kill()
    {
        GN_UNIMPL();
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

};

// *****************************************************************************
// Thread class
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
Thread * GN::Thread::sCreateThread(
    const Procedure & proc,
    void            * param,
    Priority          priority,
    bool              initialSuspended,
    const char      * name )
{
    ThreadMsw * t = new ThreadMsw();
    if( NULL != t && !t->create( proc, param, priority, initialSuspended, name ) )
    {
        delete t;
        t = NULL;
    }
    return t;
}

//
//
// -----------------------------------------------------------------------------
Thread * GN::Thread::sAttachToCurrentThread()
{
    ThreadMsw * t = new ThreadMsw();
    if( NULL != t && !t->attach() )
    {
        delete t;
        t = NULL;
    }
    return t;
}

//
//
// -----------------------------------------------------------------------------
void GN::Thread::sSleepCurrentThread( TimeInNanoSecond sleepTime )
{
    ::Sleep( ns2ms( sleepTime ) );
}

#endif
