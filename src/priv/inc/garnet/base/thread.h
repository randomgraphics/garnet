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
    typedef Delegate1<UInt32,void*> ThreadProcedure;

    ///
    /// abstract thread interface
    ///
    struct Thread : public NoCopy
    {
        /// \name thread properties
        //@{
        virtual ThreadPriority GetPriority() const = 0;
        virtual void           SetPriority( ThreadPriority ) = 0;
        virtual void           SetAffinity( UInt32 hardwareThread ) = 0;
        virtual bool           IsCurrentThread() const = 0;
        //@}

        /// \name thread operations
        //@{
        virtual void Suspend() = 0;
        virtual void Resume() = 0; //< Resume suspended and sleeping thread.

        ///
        /// wait for termination of the thread.
        ///
        virtual bool WaitForTermination( float seconds = INFINITE_TIME, UInt32 * threadProcReturnValue = 0 ) = 0;

        //@}
    };

    //@{

    Thread * NewThread(
        const ThreadProcedure & proc,
        void * param,
        ThreadPriority priority,
        bool initialSuspended = false,
        const char * name = 0 );

    void SleepCurrentThread( float seconds );

    ///
    /// Generate a thread object that represents the calling thread.
    /// Note that delete the pointer returned by this function won't stop running of the thread.
    ///
    Thread * GenerateCurrentThreadObject();

    ///
    /// Returns a platform dependent identifier of the current thread.
    ///
    /// Note that this function is much fater then GenerateCurrentThreadObject().
    ///
    SInt32 GetCurrentThreadIdentifier();

    //@}
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_THREAD_H__
