#include "pch.h"
#include "garnet/base/profiler.h"

static GN::StrA sTime2Str( double time )
{
    using namespace GN;

    if( time < 0.000001 )
    {
        return str::format( "%fus", time * 1000000 );
    }
    else if( time < 0.001 )
    {
        return str::format( "%fms", time * 1000 );
    }
    else
    {
        return str::format( "%fs", time );
    }
}

// *****************************************************************************
// Profile Timer
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN_API GN::ProfileTimer::ProfileTimer( Clock & c )
    : clock(c)
    , count(0)
    , timesum(0)
    , timemin( DBL_MAX )
    , timemax( 0 )
    , timestart(0)
    , impl(NULL)
{
}

//
//
// -----------------------------------------------------------------------------
GN_API GN::ProfileTimer::~ProfileTimer()
{
}

//
//
// -----------------------------------------------------------------------------
GN_API void GN::ProfileTimer::start()
{
    timestart = clock.getTimeD();
}

//
//
// -----------------------------------------------------------------------------
GN_API void GN::ProfileTimer::stop()
{
    double t = clock.getTimeD() - timestart;
    if( t < timemin ) timemin = t;
    if( t > timemax ) timemax = t;
    timesum += t;
    timestart = 0;
    ++count;
}

// *****************************************************************************
// Profile Manager
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN_API GN::ProfilerManager::~ProfilerManager()
{
#if GN_ENABLE_PROFILING
    // print profile result
    StrA s;
    toString( s );
    printf( "%s\n", s.rawptr() );
#endif
}

//
//
// -----------------------------------------------------------------------------
GN_API void GN::ProfilerManager::toString( GN::StrA & rval ) const
{
    ScopeMutex<SpinLoop> lock( mMutex );

    if( mTimers.empty() ) { rval = ""; return; }

    rval =
        "\n"
        "=====================================================================\n"
        "                         profile result\n"
        "---------------------------------------------------------------------\n"
        "\n";
    const StringMap<char,ProfilerTimerImpl>::KeyValuePair * i;
    for( i = mTimers.first(); i != NULL; i = mTimers.next( i ) )
    {
        const ProfilerTimerImpl & t = i->value;
        rval += GN::str::format(
            "    %s :\n"
            "        count(%d), sum(%s), ave(%s), min(%s), max(%s)\n"
            "\n",
            i->key,
            t.count,
            sTime2Str( t.timesum ).rawptr(),
            sTime2Str( 0 == t.count ? 0 : ( t.timesum / t.count ) ).rawptr(),
            sTime2Str( 0 == t.count ? 0 : t.timemin ).rawptr(),
            sTime2Str( 0 == t.count ? 0 : t.timemax ).rawptr() );
    }
    rval +=
        "=====================================================================\n"
        "\n";
}
