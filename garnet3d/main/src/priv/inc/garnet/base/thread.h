#ifndef __GN_BASE_THREAD_H__
#define __GN_BASE_THREAD_H__
// *****************************************************************************
/// \file
/// \brief   platform independent thread wrapper
/// \author  chen@@CHENLI-HOMEPC (2007.4.8)
// *****************************************************************************

namespace GN
{
    ///
    /// thread priority
    ///
    enum ThreadPriority
    {
        TP_REALTIME,  ///< The highest priority, for time-critical task only.
        TP_HIGH,      ///< high priority
        TP_NORMAL,    ///< normal priority, suitable for most of application.
        TP_LOW,       ///< low priority
        TP_IDLE,      ///< The lowest priority
        NUM_THREAD_PRIORITIES, ///< number of thread priorities
    };

    ///
    /// thread procedure functor
    ///
    typedef Delegate1<uint32,void*> ThreadProcedure;

    ///
    /// abstract thread interface
    ///
    struct Thread : public NoCopy
    {
        /// \name thread properties
        //@{
        virtual ThreadPriority getPriority() const = 0;
        virtual void           setPriority( ThreadPriority ) = 0;
        virtual void           setAffinity( uint32 hardwareThread ) = 0;
        virtual bool           isCurrentThread() const = 0;
        //@}

        /// \name thread operations
        //@{
        virtual void suspend() = 0;
        virtual void resume() = 0; //< resume suspended and sleeping thread.

        ///
        /// Wait for termination of the thread.
        ///
        /// Note: when this function returns, the thread object should have been deleted.
        ///
        virtual WaitResult waitForTermination( TimeInNanoSecond timeoutTime = INFINITE_TIME, uint32 * threadProcReturnValue = 0 ) = 0;

        //@}
    };

    //@{

    Thread * createThread(
        const ThreadProcedure & proc,
        void * param,
        ThreadPriority priority,
        bool initialSuspended = false,
        const char * name = 0 );

    void sleepCurrentThread( TimeInNanoSecond sleepTime );

    ///
    /// Generate a thread object that represents the calling thread.
    /// Note that delete the pointer returned by this function won't stop running of the thread.
    ///
    Thread * generateCurrentThreadObject();

    ///
    /// Returns a platform dependent identifier of the current thread.
    ///
    /// Note that this function is much fater then generateCurrentThreadObject().
    ///
    sint32 getCurrentThreadId();

    //@}
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_THREAD_H__
