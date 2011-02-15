#include "pch.h"

#if GN_POSIX

static GN::Logger * sLogger = GN::getLogger("GN.base.Thread");

using namespace GN;

// *****************************************************************************
// thread class
// *****************************************************************************

///
/// thread class on POSIX system
///
class ThreadPosix : public Thread, public StdClass
{
    GN_DECLARE_STDCLASS( ThreadPosix, StdClass );

    // ********************************
    // ctor/dtor
    // ********************************

    //@{
public:
    ThreadPosix()          { clear(); }
    virtual ~ThreadPosix() { quit(); }
    //@}

    // ********************************
    // from StdClass
    // ********************************

    //@{
public:
    bool create(
        const ThreadProcedure & proc,
        void * param,
        ThreadPriority priority,
        bool initialSuspended,
        const char * )
    {
        GN_GUARD;

        // standard init procedure
        GN_STDCLASS_INIT( ThreadPosix, () );

        // check parameter
        if( priority < 0 || priority >= NUM_PRIORITIES )
        {
            GN_ERROR(sLogger)( "invalid thread priority." );
            return failure();
        }

        GN_UNIMPL_WARNING();

        // success
        mAttached = false;
        return success();

        GN_UNGUARD;
    }

    bool attach()
    {
        GN_GUARD;

        GN_UNIMPL_WARNING();

        // standard init procedure
        GN_STDCLASS_INIT( ThreadPosix, () );

        // success
        mAttached = true;
        return success();

        GN_UNGUARD;
    }

    void quit()
    {
        GN_GUARD;

        GN_UNIMPL_WARNING();

        if( !mAttached /*&& mHandle*/ )
        {
            // wait for thread termination
            waitForTermination( INFINITE_TIME, 0 );

            // TODO: close thread handle
        }

        // standard quit procedure
        GN_STDCLASS_QUIT();

        GN_UNGUARD;
    }

private:
    void clear()
    {
        mAttached = false;
    }
    //@}

    // ********************************
    // inherited from Thread
    // ********************************
public:

    virtual sint32 getID() const
    {
        GN_UNIMPL_WARNING();
        return 0;
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

        GN_UNIMPL_WARNING();

        mPriority = p;
    }

    virtual void setAffinity( uint32 hardwareThread ) const
    {
        GN_UNIMPL_WARNING();
    }

    bool isCurrentThread() const
    {
        GN_UNIMPL_WARNING();
        return true;
    }

    virtual void suspend() const
    {
        GN_UNIMPL_WARNING();
    }

    virtual void resume() const
    {
        GN_UNIMPL_WARNING();
    }

    virtual void kill()
    {
        GN_UNIMPL_WARNING();
    }

    virtual WaitResult waitForTermination( TimeInNanoSecond timeoutTime, uint32 * threadProcReturnValue ) const
    {
        // can't wait for self termination
        GN_ASSERT( !isCurrentThread() );

        GN_UNIMPL_WARNING();

        return WaitResult::KILLED;
    }

    // ********************************
    // private variables
    // ********************************
private:

    struct ThreadParam
    {
        ThreadPosix * instance;
        void        * userparam;
    };

    Procedure        mProc;
    ThreadParam      mParam;
    mutable Priority mPriority;

    bool             mAttached;

    // ********************************
    // private functions
    // ********************************
private:

    ///
    /// thread procedure dispather
    ///
    static unsigned int sProcDispatcher( void * parameter )
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
GN::Thread::sCreateThread(
    const Procedure & proc,
    void            * param,
    Priority          priority,
    bool              initialSuspended,
    const char      * name )
{
    GN_GUARD;

    AutoObjPtr<ThreadPosix> s( new ThreadPosix );

    if( !s->create( proc, param, priority, initialSuspended, name ) ) return 0;

    return s.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
Thread * GN::Thread::sAttachToCurrentThread()
{
    GN_GUARD;

    AutoObjPtr<ThreadPosix> s( new ThreadPosix );

    if( !s->attach() ) return 0;

    return s.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::sSleepCurrentThread( TimeInNanoSecond sleepTime )
{
    GN_UNIMPL_WARNING();
}

//
//
// -----------------------------------------------------------------------------
sint32 GN::getCurrentThreadId()
{
    GN_UNIMPL_WARNING();
    return 0;
}

#endif
