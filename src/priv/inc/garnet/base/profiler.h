#ifndef __GN_BASE_PROFILER_H__
#define __GN_BASE_PROFILER_H__
// *****************************************************************************
/// \file    profiler.h
/// \brief   light-weight profiler
/// \author  chenlee (2005.8.4)
// *****************************************************************************

#include <float.h>
#include <map>


/// \name prof macros
//@{

#define GN_PROFILE_ENABLED GN_PROFILE_BUILD || GN_DEBUG_BUILD

#if GN_PROFILE_ENABLED

///
/// define a static instance of the timer
///
#define GN_DEFINE_STATIC_PROFILER(name) static GN::ProfileTimer & GN_JOIN(__GN_profiler_,name) = GN::ProfilerManager::sGetGlobalInstance().getTimer( #name )

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
#define GN_SCOPE_PROFILER( name ) GN_DEFINE_STATIC_PROFILER(name); GN::ScopeTimer GN_JOIN(__GN_scope_profiler,name)( &GN_JOIN(__GN_profiler_,name) )

#else

#define GN_DEFINE_STATIC_PROFILER( name )
#define GN_START_PROFILER( name )
#define GN_STOP_PROFILER( name )
#define GN_SCOPE_PROFILER( name )

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
        size_t count;
        double timesum, timemin, timemax, timestart;

        friend class ProfilerManager;

    protected:

        /// ctor
        ProfileTimer( Clock & c )
            : clock(c)
            , count(0)
            , timesum(0)
            , timemin( DBL_MAX )
            , timemax( 0 )
            , timestart(0)
        {}

        /// dtor
        virtual ~ProfileTimer() {}

    public:

        ///
        /// start the timer
        ///
        void start()
        {
            GN_ASSERT( 0 == timestart );
            timestart = clock.getTimeD();
        }

        ///
        /// stop the timer
        ///
        void stop()
        {
            GN_ASSERT( 0 != timestart );
            double t = clock.getTimeD() - timestart;
            if( t < timemin ) timemin = t;
            if( t > timemax ) timemax = t;
            timesum += t;
            timestart = 0;
            ++count;
        }
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

            std::map<StrA,ProfilerTimerImpl>::iterator i = mTimers.find( name );
            if( mTimers.end() != i ) return i->second;

            // create new timer
            ProfilerTimerImpl newTimer( mClock );
            return mTimers.insert( std::make_pair( name, newTimer ) ).first->second;
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

        Clock                            mClock;
        std::map<StrA,ProfilerTimerImpl> mTimers;
        mutable SpinLoop                 mMutex;

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
//                           End of profiler.h
// *****************************************************************************
#endif // __GN_BASE_PROFILER_H__
