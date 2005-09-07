#ifndef __GN_BASE_PROFILER_H__
#define __GN_BASE_PROFILER_H__
// *****************************************************************************
//! \file    profiler.h
//! \brief   light-weight profiler
//! \author  chenlee (2005.8.4)
// *****************************************************************************

#include <limits>
#include <map>

//! \name prof macros
//@{
#ifdef GN_ENABLE_PROFILE
//!
//! start a profile timer
//!
#define GN_PROF(name) ::GN::core::ScopeTimer GN_JOIN(__prof_,name)( #name )
//!
//! stop a previously defined profile timer
//!
//! Normally, you don't need to use this macro, because profile timer
//! will stop automatically when out of its life scope.
//!
#define GN_PROF_END(name) GN_JOIN(__prof_,name).end()
#else
#define GN_PROF(name)
#define GN_PROF_END(name)
#endif
//@}

namespace GN
{
    //!
    //! Profiler Manager
    //!
    class ProfilerManager : public Singleton<ProfilerManager>
    {
        // ********************************
        //! \name  ctor/dtor
        // ********************************

        //@{
    public:
        ProfilerManager() {}
        ~ProfilerManager() {}
        //@}

        // ********************************
        //   public functions
        // ********************************
    public:

        //!
        //! reset profiler, clear all timers
        //!
        void reset() { mTimers.clear(); mClock.reset(); }

        //!
        //! print profile result to string
        //!
        void toString( StrA & ) const;

        //!
        //! print profile result to string
        //!
        StrA toString() { StrA str; toString(str); return str; }

        //!
        //! start a profile timer
        //!
        void startTimer( const char * name )
        {
            GN_GUARD_SLOW;
            mTimers[name].start( mClock );
            GN_UNGUARD_SLOW;
        }

        //!
        //! stop a profile timer
        //!
        void stopTimer( const char * name )
        {
            GN_GUARD_SLOW;
            mTimers[name].stop( mClock );
            GN_UNGUARD_SLOW;
        }

        // ********************************
        //   private variables
        // ********************************
    private:

        //!
        //! private timer structure
        //!
        struct TimerDesc
        {
            double count, timesum, timemin, timemax, timestart;

            TimerDesc()
                : count(0)
                , timesum(0)
                , timemin( std::numeric_limits<double>::max() )
                , timemax( 0 )
                , timestart(0)
            {}

            void start( Clock & c )
            {
                GN_ASSERT( 0 == timestart );
                timestart = c.getTimeD();
            }

            void stop( Clock & c )
            {
                GN_ASSERT( 0 != timestart );
                double t = c.getTimeD() - timestart;
                if( t < timemin ) timemin = t;
                if( t > timemax ) timemax = t;
                timesum += t;
                timestart = 0;
                ++count;
            }
        };

        Clock                            mClock;
        std::map<const char *,TimerDesc> mTimers;

        // ********************************
        //   private functions
        // ********************************
    private:
    };

    //!
    //! scope timer
    //!
    class ScopeTimer
    {
        const char * mName;

    public :

        //!
        //! start the timer
        //!
        ScopeTimer( const char * name ) : mName(name)
        {
            GN_ASSERT( name );
            ProfilerManager::getInstance().startTimer(name);
        }

        //!
        //! end the timer
        //!
        ~ScopeTimer()
        {
            stop();
        }

        //!
        //! end the timer manually
        //!
        void stop()
        {
            if( mName )
            {
                ProfilerManager::getInstance().stopTimer(mName);
                mName = 0;
            }
        }
    };
}

// *****************************************************************************
//                           End of profiler.h
// *****************************************************************************
#endif // __GN_BASE_PROFILER_H__
