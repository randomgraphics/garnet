#include "pch.h"

#if GN_POSIX

//static GN::Logger * sLogger = GN::GetLogger("GN.base.Thread");

using namespace GN;

/* *****************************************************************************
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
        const Procedure & proc,
        void * param,
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

        // Store user parameters
        if( proc.empty() )
        {
            GN_ERROR(sLogger)( "Null thread procedure." );
            return failure();
        }
        mUserProc = proc;
        mUserParam = param;

        // create thread attribute
        int error = pthread_attr_init( &mAttr );
        if( error )
        {
            GN_ERROR(sLogger)( "pthread_attr_init() failed: %d.", error );
            return failure();
        }
        mAttrOk = true;

        // create thread object.
        error = pthread_create( &mThread, &mAttr, sProcDispatcher, this );
        if( error )
        {
            GN_ERROR(sLogger)( "pthread_create() failed: %d.", error );
            return failure();
        }

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

        mThread = pthread_self();

        // success
        mAttached = true;
        return success();

        GN_UNGUARD;
    }

    void quit()
    {
        GN_GUARD;

        if( mThread )
        {
            // wait for thread termination
            if( !mAttached )
            {
                pthread_join( mThread, NULL );
                mAttached = false;
            }
            mThread = 0;
        }

        if( mAttrOk )
        {
            pthread_attr_destroy( &mAttr );
            mAttrOk = false;
        }

        // standard quit procedure
        GN_STDCLASS_QUIT();

        GN_UNGUARD;
    }

private:
    void clear()
    {
        mAttrOk   = false;
        mThread   = 0;
        mAttached = false;
    }
    //@}

    // ********************************
    // inherited from Thread
    // ********************************
public:

    virtual sint32 getID() const
    {
        return (sint32)mThread;
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
        return mThread && pthread_equal( mThread, pthread_self() );
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

    virtual WaitResult waitForTermination( TimeInNanoSecond timeoutTime, uint32 * threadProcReturnValue )
    {
        // can't wait for self termination
        GN_ASSERT( !isCurrentThread() );

        if( !mThread ) return WaitResult::KILLED;

        int error = pthread_join( mThread, NULL );
        if( error )
        {
            GN_ERROR(sLogger)( "pthread_join() failed: %d.", error );
            return WaitResult::FAILED;
        }

        return WaitResult::COMPLETED;
    }

    // ********************************
    // private variables
    // ********************************
private:

    Procedure        mUserProc;
    void *           mUserParam;
    mutable Priority mPriority;

    pthread_attr_t   mAttr;
    bool             mAttrOk;

    pthread_t        mThread;
    bool             mAttached;

    // ********************************
    // private functions
    // ********************************
private:

    ///
    /// thread procedure dispather
    ///
    static void * sProcDispatcher( void * parameter )
    {
        GN_ASSERT( parameter );

        ThreadPosix * pThis = (ThreadPosix*)parameter;

        GN_ASSERT( pThis );

        int status = pThis->mUserProc( pThis->mUserParam );

        return (void*)status;
    };

}; */

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN_API GN::Thread::Identifier GN::Thread::sCreate(
    const Procedure & proc,
    void            * param,
    const char      * name )
{
    GN_GUARD;

    //AutoObjPtr<ThreadPosix> s( new ThreadPosix );
    //if( !s->create( proc, param, name ) ) return 0;
    //return s.detach();
    GN_UNIMPL_WARNING();
    GN_UNUSED_PARAM( proc );
    GN_UNUSED_PARAM( param );
    GN_UNUSED_PARAM( name );
    return 0;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
GN_API GN::WaitResult GN::Thread::sWaitForTermination(
    Identifier       thread,
    TimeInNanoSecond timeoutTime )
{
    GN_UNIMPL_WARNING();
    GN_UNUSED_PARAM( thread );
    GN_UNUSED_PARAM( timeoutTime );
    return GN::WaitResult::FAILED;
}

//
//
// -----------------------------------------------------------------------------
void GN::Thread::sSleepCurrentThread( TimeInNanoSecond sleepTime )
{
    GN_UNIMPL_WARNING();
    GN_UNUSED_PARAM( sleepTime );
}

//
//
// -----------------------------------------------------------------------------
GN_API GN::Thread::Identifier GN::Thread::sGetCurrentThread()
{
    GN_UNIMPL_WARNING();
    return 0;
}

//
//
// -----------------------------------------------------------------------------
GN_API bool GN::Thread::sIsCurrentThread( Identifier id )
{
    return id == sGetCurrentThread();
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
    GN_UNIMPL_WARNING();
    GN_UNUSED_PARAM( t );
    GN_UNUSED_PARAM( p );
}

//
//
// -----------------------------------------------------------------------------
GN_API void GN::Thread::sSetAffinity( Identifier, uint32 )
{
    GN_UNIMPL_WARNING();
}

#endif
