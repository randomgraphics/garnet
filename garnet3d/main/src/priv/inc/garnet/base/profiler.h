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

#if GN_BUILD_PROFILE

///
/// define a static instance of the timer
///
#define GN_DEFINE_STATIC_PROFILER( name, desc ) static GN::ProfileTimer & GN_JOIN(__GN_profiler_,name) = GN::ProfilerManager::sGetGlobalInstance().GetTimer( desc )

///
/// start a previously defined profile timer
///
#define GN_START_PROFILER( name ) if(0) {} else GN_JOIN(__GN_profiler_,name).Start()

///
/// stop a previously defined profile timer
///
#define GN_STOP_PROFILER( name ) if(0) {} else GN_JOIN(__GN_profiler_,name).Stop()

///
/// define an automatic profiler that evaluate the time of its life-scope.
///
#define GN_SCOPE_PROFILER( name, desc ) GN_DEFINE_STATIC_PROFILER( name, desc ); GN::ScopeTimer GN_JOIN(__GN_scope_profiler,name)( &GN_JOIN(__GN_profiler_,name) )

#else

#define GN_DEFINE_STATIC_PROFILER( name, desc )
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
    class ProfileTimer
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
        /// Start the timer
        ///
        void Start();

        ///
        /// Stop the timer
        ///
        void Stop();
    };

    ///
    /// Profiler Manager
    ///
    /// \todo Make it thread-safe.
    ///
    class ProfilerManager
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
        static GN_PUBLIC ProfilerManager & sGetGlobalInstance();

        ///
        /// reset profiler, clear all timers
        ///
        void Reset()
        {
            ScopeMutex<SpinLoop> lock( mMutex );
            mTimers.Clear();
            mClock.Reset();
        }

        ///
        /// print profile result to string
        ///
        void ToString( StrA & ) const;

        ///
        /// print profile result to string
        ///
        StrA ToString() { StrA str; ToString(str); return str; }

        ///
        /// return a named timer
        ///
        ProfileTimer & GetTimer( const StrA & name )
        {
            GN_ASSERT( !name.Empty() );

            ScopeMutex<SpinLoop> lock( mMutex );

            ProfilerTimerImpl * impl = mTimers.Find( name );
            if( NULL != impl ) return *impl;

            // create new timer
            ProfilerTimerImpl newTimer( mClock );

            return mTimers.Insert( name, newTimer )->value;
        }

        ///
        /// start a profile timer
        ///
        void StartTimer( const StrA & name )
        {
            GetTimer(name).Start();
        }

        ///
        /// stop a profile timer
        ///
        void StopTimer( const StrA & name )
        {
            GetTimer(name).Stop();
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
        ScopeTimer( const char * name ) : mTimer( &ProfilerManager::sGetGlobalInstance().GetTimer(name) )
        {
            mTimer->Start();
        }

        ///
        /// start the user-defined timer
        ///
        ScopeTimer( ProfileTimer * timer ) : mTimer( timer )
        {
            mTimer->Start();
        }

        ///
        /// end the mTimer
        ///
        ~ScopeTimer()
        {
            Stop();
        }

        ///
        /// end the mTimer manually
        ///
        void Stop()
        {
            if( mTimer )
            {
                mTimer->Stop();
                mTimer = 0;
            }
        }
    };
}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_PROFILER_H__
