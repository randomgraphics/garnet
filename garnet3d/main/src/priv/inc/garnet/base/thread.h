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
    /// abstract thread interface
    ///
    struct Thread : public NoCopy
    {
        //@{

        /// Thread priority
        enum Priority
        {
            REALTIME,       ///< The highest priority, for time-critical task only.
            HIGH,           ///< high priority
            NORMAL,         ///< normal priority, suitable for most of application.
            LOW,            ///< low priority
            IDLE,           ///< The lowest priority
            NUM_PRIORITIES, ///< number of thread priorities
        };

        ///
        /// thread procedure functor
        ///
        typedef Delegate1<uint32,void*> Procedure;

        //@}

        /// \name thread properties
        //@{
        virtual sint32   getID() const = 0;
        virtual Priority getPriority() const = 0;
        virtual void     setPriority( Priority ) const = 0;
        virtual void     setAffinity( uint32 hardwareThread ) const = 0;
        virtual bool     isCurrentThread() const = 0;
        //@}

        /// \name thread operations
        //@{

        static Thread * sCreateThread(
            const Procedure & proc,
            void            * param,
            Priority          priority,
            bool              initialSuspended = false,
            const char      * name = 0 );

        static Thread * sAttachToCurrentThread();

        static void sSleepCurrentThread( TimeInNanoSecond sleepTime );

        virtual ~Thread() {}

        virtual void suspend() const = 0;
        virtual void resume() const = 0;
        /// Wait for termination of the thread.
        /// COMPLETED: the thread is terminated normally.
        /// KILLED   : the thread has been killed
        /// TIMEOUT  : time out before the thread is either terminated or killed; or the caller is current thread.
        virtual WaitResult waitForTermination(
            TimeInNanoSecond timeoutTime = INFINITE_TIME,
            uint32         * threadProcReturnValue = 0 ) = 0;

        //@}
    };
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_THREAD_H__
