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
    struct Thread
    {
        //@{

        /// Thread ID
        typedef void * ThreadID;

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

        /// \name thread operations
        //@{

        /// Create a new thread. Return 0 on failure.
        ///
        /// Note that All resources associated with the thread will
        /// be released automatically when the thread exits normally.
        /// So there's no need to explictly "destroy" a thread ID.
        static ThreadID sCreate(
            const Procedure & proc,
            void            * param,
            const char      * name = 0 );

        /// Terminate a thread by force.
        ///
        /// Note that terminating a thread does not ensure that all resources allocated
        /// by the tread are released. So use this function with extreme caution.
        static void sKill( ThreadID );

        static void sSleepCurrentThread( TimeInNanoSecond sleepTime );

        /// Wait for termination of the thread (join operation).
        /// COMPLETED: the thread is terminated, either normally or being killed.
        /// TIMEOUT  : time out before the thread is either terminated or killed; or the caller is current thread.
        /// FAILED   : wait operation failed for unspecified reason.
        static WaitResult sWaitForTermination(
            ThreadID         thread,
            TimeInNanoSecond timeoutTime = INFINITE_TIME,
            uint32         * threadProcReturnValue = 0 );

        //@}

        /// \name thread properties
        //@{
        static ThreadID sGetCurrentThread();
        static bool     sIsCurrentThread( ThreadID );
        static Priority sGetPriority( ThreadID ); ///< Return NORMAL if the thread ID is invalid.
        static void     sSetPriority( ThreadID, Priority );
        static void     sSetAffinity( ThreadID, uint32 hardwareThread ); //< Meaning of hardwareThread is platform specific.
        //@}
    };
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_THREAD_H__
