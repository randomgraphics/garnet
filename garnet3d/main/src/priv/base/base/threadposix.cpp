#include "pch.h"

#if GN_POSIX

static GN::Logger * sLogger = GN::getLogger("GN.base.Thread");

using namespace GN;

// *****************************************************************************
// thread class
// *****************************************************************************

///
/// thread class on MS Windows
///
class ThreadX11 : public Thread, public StdClass
{
    GN_DECLARE_STDCLASS( ThreadX11, StdClass );

    // ********************************
    // ctor/dtor
    // ********************************

    //@{
public:
    ThreadX11()          { clear(); }
    virtual ~ThreadX11() { quit(); }
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
        GN_STDCLASS_INIT( ThreadX11, () );

        // check parameter
        if( priority < 0 || priority >= NUM_THREAD_PRIORITIES )
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
        GN_STDCLASS_INIT( ThreadX11, () );

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

    virtual ThreadPriority getPriority() const
    {
        return mPriority;
    }

    virtual void setPriority( ThreadPriority p )
    {
        if( p < 0 || p >= NUM_THREAD_PRIORITIES )
        {
            GN_ERROR(sLogger)( "invalid thread priority!" );
            return;
        }

        GN_UNIMPL_WARNING();

        mPriority = p;
    }

    virtual void setAffinity( UInt32 hardwareThread )
    {
        GN_UNIMPL_WARNING();
    }

    bool isCurrentThread() const
    {
        GN_UNIMPL_WARNING();
        return true;
    }

    virtual void suspend()
    {
        GN_UNIMPL_WARNING();
    }

    virtual void resume()
    {
        GN_UNIMPL_WARNING();
    }

    virtual bool waitForTermination( TimeInNanoSecond timeoutTime, UInt32 * threadProcReturnValue )
    {
        // can't wait for self termination
        GN_ASSERT( !isCurrentThread() );

        GN_UNIMPL_WARNING();

        return true;
    }

    // ********************************
    // private variables
    // ********************************
private:

    struct ThreadParam
    {
        ThreadX11 * instance;
        void      * userparam;
    };

    ThreadProcedure mProc;
    ThreadParam     mParam;
    ThreadPriority  mPriority;

    bool            mAttached;

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
GN::createThread(
    const ThreadProcedure & proc,
    void                  * param,
    ThreadPriority          priority,
    bool                    initialSuspended,
    const char            * name )
{
    GN_GUARD;

    AutoObjPtr<ThreadX11> s( new ThreadX11 );

    if( !s->create( proc, param, priority, initialSuspended, name ) ) return 0;

    return s.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::sleepCurrentThread( TimeInNanoSecond sleepTime )
{
    GN_UNIMPL_WARNING();
}

//
//
// -----------------------------------------------------------------------------
Thread * GN::generateCurrentThreadObject()
{
    GN_GUARD;

    AutoObjPtr<ThreadX11> s( new ThreadX11 );

    if( !s->attach() ) return 0;

    return s.detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
SInt32 GN::getCurrentThreadId()
{
    GN_UNIMPL_WARNING();
    return 0;
}

#endif
