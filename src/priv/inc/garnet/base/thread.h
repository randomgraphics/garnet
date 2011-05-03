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
    /// Threading API.
    ///
    /// TODO: instead of a class with static methods, could this be just a namespace?
    ///
    struct Thread
    {
        //@{

        /// Thread ID.
        ///
        /// Unless we have run out of all identifier values (which is very unlikly),
        /// thread id is never reused in one OS life time. And you can always use
        /// the id to query the status of the thread you want, without worrying about
        /// it being redirected to another thread suddenly.
        typedef uint64 Identifier;

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
        typedef Delegate1<void,void*> Procedure;

        //@}

        /// \name thread operations
        //@{

        /// Create a new thread. Return 0 on failure.
        ///
        /// \note
        ///     All resources associated with the thread will
        ///     be released automatically when the thread exits normally.
        ///     So there's no need to explictly "destroy" a thread ID.
        GN_API static Identifier sCreate(
            const Procedure & proc,
            void            * param,
            const char      * name = 0 );

        /// Block calling thread for a fixed amount of time.
        GN_API static void sSleepCurrentThread( TimeInNanoSecond sleepTime );

        /// Wait for termination of the thread (join operation).
        /// COMPLETED: the thread is terminated, either normally or being killed.
        /// TIMEOUT  : time out before the thread is either terminated or killed; or the caller is current thread.
        /// FAILED   : wait operation failed for unspecified reason.
        GN_API static WaitResult sWaitForTermination(
            Identifier       thread,
            TimeInNanoSecond timeoutTime = INFINITE_TIME );

        /// Alias for sWaitForTermination
        static inline WaitResult sJoin(
            Identifier       thread,
            TimeInNanoSecond timeoutTime = INFINITE_TIME )
        {
            return sWaitForTermination( thread, timeoutTime );
        }

        //@}

        /// \name thread properties
        //@{
        GN_API static Identifier sGetCurrentThread();
        GN_API static bool       sIsCurrentThread( Identifier );
        GN_API static Priority   sGetPriority( Identifier ); ///< Return NORMAL if the thread ID is invalid.
        GN_API static void       sSetPriority( Identifier, Priority );
        GN_API static void       sSetAffinity( Identifier, uint32 hardwareThread ); //< Meaning of hardwareThread is platform specific.
        //@}
    };
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_THREAD_H__
