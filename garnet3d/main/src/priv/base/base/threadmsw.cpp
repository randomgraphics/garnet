#include "pch.h"

#if GN_MSWIN

#include <process.h>

static GN::Logger * sLogger = GN::GetLogger("GN.base.Thread");

using namespace GN;

// *****************************************************************************
// local class and functions
// *****************************************************************************


// defined in syncmsw.cpp
extern UInt32 sec2usec( float time );

static int sPriorityTable[] =
{
    THREAD_PRIORITY_TIME_CRITICAL,
    THREAD_PRIORITY_ABOVE_NORMAL,
    THREAD_PRIORITY_NORMAL,
    THREAD_PRIORITY_BELOW_NORMAL,
    THREAD_PRIORITY_IDLE,
};
GN_CASSERT( GN_ARRAY_COUNT(sPriorityTable) == NUM_THREAD_PRIORITIES );

///
/// convert thread priority to WIN32 constant
///
static int sPriority2Msw( ThreadPriority p )
{
    GN_ASSERT( p < NUM_THREAD_PRIORITIES );

    return sPriorityTable[p];
}

// *****************************************************************************
// thread class
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
    ThreadMsw()          { Clear(); }
    virtual ~ThreadMsw() { Quit(); }
    //@}

    // ********************************
    // from StdClass
    // ********************************

    //@{
public:
    bool Create(
        const ThreadProcedure & proc,
        void * param,
        ThreadPriority priority,
        bool initialSuspended,
        const char * )
    {
        GN_GUARD;

        // standard init procedure
        GN_STDCLASS_INIT( ThreadMsw, () );

        // check parameter
        if( priority < 0 || priority >= NUM_THREAD_PRIORITIES )
        {
            GN_ERROR(sLogger)( "invalid thread priority." );
            return Failure();
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
        GN_MSW_CHECK_RETURN( mHandle, Failure() );

        // success
        mAttached = false;
        return Success();

        GN_UNGUARD;
    }

    bool Attach()
    {
        GN_GUARD;

        // standard init procedure
        GN_STDCLASS_INIT( ThreadMsw, () );

        mHandle = ::GetCurrentThread();
        mId = ::GetCurrentThreadId();

        // TODO: get real priority value, then convert to TP_XXX enums.
        mPriority = TP_NORMAL;

        // success
        mAttached = true;
        return Success();

        GN_UNGUARD;
    }

    void Quit()
    {
        GN_GUARD;

        if( !mAttached && mHandle )
        {
            // wait for thread termination
            WaitForTermination( INFINITE_TIME, 0 );

            // close thread handle
            CloseHandle( mHandle );
        }

        // standard Quit procedure
        GN_STDCLASS_QUIT();

        GN_UNGUARD;
    }

private:
    void Clear()
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

    virtual ThreadPriority GetPriority() const
    {
        return mPriority;
    }

    virtual void SetPriority( ThreadPriority p )
    {
        if( p < 0 || p >= NUM_THREAD_PRIORITIES )
        {
            GN_ERROR(sLogger)( "invalid thread priority!" );
            return;
        }

        GN_MSW_CHECK_RETURN_VOID( ::SetThreadPriority( mHandle, sPriority2Msw(p) ) );

        mPriority = p;
    }

    virtual void SetAffinity( UInt32 hardwareThread )
    {
#if GN_XENON
        if( (DWORD)-1 == XSetThreadProcessor( mHandle, hardwareThread ) )
        {
            GN_ERROR(sLogger)( "fail to set thread affinity: %s", GetWin32LastErrorInfo() );
        }
#else
        if( (DWORD)-1 == SetThreadIdealProcessor( mHandle, hardwareThread ) )
        {
            GN_ERROR(sLogger)( "fail to set thread affinity: %s", GetWin32LastErrorInfo() );
        }
#endif
    }

    bool IsCurrentThread() const
    {
        return ::GetCurrentThreadId() == mId;
    }

    virtual void Suspend()
    {
        if( (DWORD)-1 == ::SuspendThread( mHandle ) )
        {
            GN_ERROR(sLogger)( GetWin32LastErrorInfo() );
        }
    }

    virtual void Resume()
    {
        if( (DWORD)-1 == ::ResumeThread( mHandle ) )
        {
            GN_ERROR(sLogger)( GetWin32LastErrorInfo() );
        }
    }

    virtual bool WaitForTermination( float seconds, UInt32 * threadProcReturnValue )
    {
        // can't wait for self termination
        GN_ASSERT( !IsCurrentThread() );

        UInt32 ret = ::WaitForSingleObject( mHandle, sec2usec( seconds ) );

        if( WAIT_TIMEOUT == ret )
        {
            GN_TRACE(sLogger)( "time out!" );
            return false;
        }
        else if( WAIT_OBJECT_0 == ret )
        {
            if( threadProcReturnValue )
            {
                GN_MSW_CHECK( GetExitCodeThread( mHandle, (LPDWORD)threadProcReturnValue ) );
            }
            return true;
        }
        else
        {
            GN_ERROR(sLogger)( GetWin32LastErrorInfo() );
            return false;
        }
    }

    // ********************************
    // private variables
    // ********************************
private:

    struct ThreadParam
    {
        ThreadMsw * instance;
        void      * userparam;
    };

    ThreadProcedure mProc;
    ThreadParam     mParam;
    ThreadPriority  mPriority;

    HANDLE          mHandle;
    DWORD           mId;

    bool            mAttached;

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
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::Thread *
GN::NewThread(
    const ThreadProcedure & proc,
    void * param,
    ThreadPriority priority,
    bool initialSuspended,
    const char * name )
{
    GN_GUARD;

    AutoObjPtr<ThreadMsw> s( new ThreadMsw );

    if( !s->Create( proc, param, priority, initialSuspended, name ) ) return 0;

    return s.Detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::SleepCurrentThread( float seconds )
{
   ::Sleep( sec2usec( seconds ) );
}

//
//
// -----------------------------------------------------------------------------
Thread * GN::GenerateCurrentThreadObject()
{
    GN_GUARD;

    AutoObjPtr<ThreadMsw> s( new ThreadMsw );

    if( !s->Attach() ) return 0;

    return s.Detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
SInt32 GN::GetCurrentThreadIdentifier()
{
    return (SInt32)::GetCurrentThreadId();
}

#endif
