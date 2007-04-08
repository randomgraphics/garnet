#ifndef __GN_BASE_THREAD_H__
#define __GN_BASE_THREAD_H__
// *****************************************************************************
//! \file    base/thread.h
//! \brief   platform independent thread wrapper
//! \author  chen@@CHENLI-HOMEPC (2007.4.8)
// *****************************************************************************

namespace GN
{
    ///
    /// special float number that indicates infinite time.
    ///
    extern const float INFINITE_TIME; // = 1.0e38f;

    ///
    /// Mutex lock, the lightest sync object.
    ///
    class Mutex
    {
        UInt32 mInternal[8]; ///< store platform dependata data

    public:

        //@{
        Mutex();
        ~Mutex();
        //@}

        //@{
        bool trylock();
        void lock();
        void unlock();
        //@}
    };

    ///
    /// Semaphore
    ///
    class Semaphore
    {
    };

    ///
    /// thread priority
    ///
    enum ThreadPriority
    {
        TP_REALTIME, ///< The highest priority, for time-critical task only.
        TP_HIGH,     ///< high priority
        TP_NORMAL,   ///< normal priority, suitable for most of application.
        TP_LOW,      ///< low priority
    };

    ///
    /// thread interface
    ///
    struct Thread : public NoCopy
    {
        /// \name thread properties
        //@{
        virtual ThreadPriority getPriority() const = 0;
        virtual void           setPriority( ThreadPriority ) = 0;
        //@}

        /// \name thread operations
        //@{
        virtual void suspend() = 0;
        virtual void sleep( float seconds ) = 0;
        virtual void resume() = 0; //< resume suspended and sleeping thread.

        ///
        /// wait for termination of the thread.
        ///
        virtual bool waitForTermination( float seconds = INFINITE_TIME ) = 0;

        ///
        /// signal of thread termination
        ///
        Signal0<void> sigTermination;

        //@}
    };

    ///
    /// thread procedure functor
    ///
    typedef Delegate1<void,void*> ThreadProcedure;

    ///
    /// create a new thread
    ///
    Thread * createThread(
        ThreadPriority priority,
        const ThreadProcedure & proc );
}

// *****************************************************************************
//                           End of thread.h
// *****************************************************************************
#endif // __GN_BASE_THREAD_H__
