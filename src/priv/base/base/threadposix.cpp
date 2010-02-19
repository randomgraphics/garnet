#include "pch.h"

#if GN_POSIX

static GN::Logger * sLogger = GN::GetLogger("GN.base.Thread");

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
    ThreadX11()          { Clear(); }
    virtual ~ThreadX11() { Quit(); }
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
            return Failure();
        }

        GN_UNIMPL_WARNING();

        // success
        mAttached = false;
        return Success();

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
        return Success();

        GN_UNGUARD;
    }

    void Quit()
    {
        GN_GUARD;

        GN_UNIMPL_WARNING();

        if( !mAttached /*&& mHandle*/ )
        {
            // wait for thread termination
            WaitForTermination( INFINITE_TIME, 0 );

            // TODO: close thread handle
        }

        // standard Quit procedure
        GN_STDCLASS_QUIT();

        GN_UNGUARD;
    }

private:
    void Clear()
    {
        mAttached = false;
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

        GN_UNIMPL_WARNING();

        mPriority = p;
    }

    virtual void SetAffinity( UInt32 hardwareThread )
    {
        GN_UNIMPL_WARNING();
    }

    bool IsCurrentThread() const
    {
        GN_UNIMPL_WARNING();
        return true;
    }

    virtual void Suspend()
    {
        GN_UNIMPL_WARNING();
    }

    virtual void Resume()
    {
        GN_UNIMPL_WARNING();
    }

    virtual bool WaitForTermination( float seconds, UInt32 * threadProcReturnValue )
    {
        // can't wait for self termination
        GN_ASSERT( !IsCurrentThread() );

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
GN::NewThread(
    const ThreadProcedure & proc,
    void                  * param,
    ThreadPriority          priority,
    bool                    initialSuspended,
    const char            * name )
{
    GN_GUARD;

    AutoObjPtr<ThreadX11> s( new ThreadX11 );

    if( !s->create( proc, param, priority, initialSuspended, name ) ) return 0;

    return s.Detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::SleepCurrentThread( float seconds )
{
    GN_UNIMPL_WARNING();
}

//
//
// -----------------------------------------------------------------------------
Thread * GN::GenerateCurrentThreadObject()
{
    GN_GUARD;

    AutoObjPtr<ThreadX11> s( new ThreadX11 );

    if( !s->Attach() ) return 0;

    return s.Detach();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
SInt32 GN::GetCurrentThreadIdentifier()
{
    GN_UNIMPL_WARNING();
    return 0;
}

#endif
