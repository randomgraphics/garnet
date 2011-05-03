#ifndef __GN_BASE_PROFILER_H__
#define __GN_BASE_PROFILER_H__
// *****************************************************************************
/// \file
/// \brief   light-weight profiler
/// \author  chenlee (2005.8.4)
// *****************************************************************************

#include <float.h>


/// \name prof macros
//@{

#if GN_ENABLE_PROFILING

///
/// define a static instance of the timer
///
#define GN_DEFINE_STATIC_PROFILER( name, desc ) static GN::ProfileTimer & GN_JOIN(__GN_profiler_,name) = GN::ProfilerManager::sGetGlobalInstance().getTimer( desc )

///
/// start a previously defined profile timer
///
#define GN_START_PROFILER( name ) if(0) {} else GN_JOIN(__GN_profiler_,name).start()

///
/// stop a previously defined profile timer
///
#define GN_STOP_PROFILER( name ) if(0) {} else GN_JOIN(__GN_profiler_,name).stop()

///
/// define an automatic profiler that evaluate the time of its life-scope.
///
#define GN_SCOPE_PROFILER( name, desc ) GN_DEFINE_STATIC_PROFILER( name, desc ); GN::ScopeTimer GN_JOIN(__GN_scope_profiler,name)( &GN_JOIN(__GN_profiler_,name) )

#else

#define GN_DEFINE_STATIC_PROFILER( name )
#define GN_START_PROFILER( name )
#define GN_STOP_PROFILER( name )
#define GN_SCOPE_PROFILER( name, desc )

#endif
//@}

namespace GN
{
    ///
    /// profile timer
    ///
    class GN_API ProfileTimer
    {
        Clock & clock;

        size_t  count;

        double  timesum, timemin, timemax, timestart;

        void * impl;

        friend class ProfilerManager;

    protected:

        /// ctor
        ProfileTimer( Clock & c );

        /// dtor
        ~ProfileTimer();

    public:

        ///
        /// start the timer
        ///
        void start();

        ///
        /// stop the timer
        ///
        void stop();
    };

    ///
    /// Profiler Manager
    ///
    /// \todo Make it thread-safe.
    ///
    class GN_API ProfilerManager
    {
        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        ProfilerManager() {}
        ~ProfilerManager();
        //@}

        // ********************************
        //   public functions
        // ********************************
    public:

        ///
        /// return global instance of the profiler
        ///
        static ProfilerManager & sGetGlobalInstance();

        ///
        /// reset profiler, clear all timers
        ///
        void reset()
        {
            ScopeMutex<SpinLoop> lock( mMutex );
            mTimers.clear();
            mClock.reset();
        }

        ///
        /// print profile result to string
        ///
        void toString( StrA & ) const;

        ///
        /// print profile result to string
        ///
        StrA toString() { StrA str; toString(str); return str; }

        ///
        /// return a named timer
        ///
        ProfileTimer & getTimer( const StrA & name )
        {
            GN_ASSERT( !name.empty() );

            ScopeMutex<SpinLoop> lock( mMutex );

            ProfilerTimerImpl * p = mTimers.find( name );
            if( NULL != p ) return *p;

            // create new timer
            ProfilerTimerImpl newTimer( mClock );
            return mTimers.insert( name, newTimer )->value;
        }

        ///
        /// start a profile timer
        ///
        void startTimer( const StrA & name )
        {
            getTimer(name).start();
        }

        ///
        /// stop a profile timer
        ///
        void stopTimer( const StrA & name )
        {
            getTimer(name).stop();
        }

        // ********************************
        //   private variables
        // ********************************
    private:

        struct ProfilerTimerImpl : public ProfileTimer
        {
            ProfilerTimerImpl( Clock & c ) : ProfileTimer( c ) {}
            ~ProfilerTimerImpl() {}
        };

        typedef StringMap<char,ProfilerTimerImpl> TimerMap;

        Clock             mClock;
        TimerMap          mTimers;
        mutable SpinLoop  mMutex;

        // ********************************
        //   private functions
        // ********************************
    private:
    };

    ///
    /// scope timer
    ///
    class ScopeTimer
    {
        ProfileTimer * mTimer;

    public :

        ///
        /// start the timer
        ///
        ScopeTimer( const char * name ) : mTimer( &ProfilerManager::sGetGlobalInstance().getTimer(name) )
        {
            mTimer->start();
        }

        ///
        /// start the user-defined timer
        ///
        ScopeTimer( ProfileTimer * timer ) : mTimer( timer )
        {
            mTimer->start();
        }

        ///
        /// end the mTimer
        ///
        ~ScopeTimer()
        {
            stop();
        }

        ///
        /// end the mTimer manually
        ///
        void stop()
        {
            if( mTimer )
            {
                mTimer->stop();
                mTimer = 0;
            }
        }
    };
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_PROFILER_H__
