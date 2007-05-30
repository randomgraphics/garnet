#include "pch.h"
#include "garnet/base/profiler.h"

static GN::StrA sTime2Str( double time )
{
    using namespace GN;

    if( time < 0.000001 )
    {
        return strFormat( "%fus", time * 1000000 );
    }
    else if( time < 0.001 )
    {
        return strFormat( "%fms", time * 1000 );
    }
    else
    {
        return strFormat( "%fs", time );
    }
}

//
//
// -----------------------------------------------------------------------------
GN::ProfilerManager::~ProfilerManager()
{
#if GN_PROFILE_BUILD
    // print profile result
    StrA s;
    toString( s );
    printf( "%s\n", s.cptr() );
#endif
}

//
//
// -----------------------------------------------------------------------------
void GN::ProfilerManager::toString( GN::StrA & rval ) const
{
    ScopeMutex<SpinLoop> lock( mMutex );

    if( mTimers.empty() ) { rval = ""; return; }

    rval =
        "\n"
        "=====================================================================\n"
        "                         profile result\n"
        "---------------------------------------------------------------------\n"
        "\n";
    std::map<StrA,ProfilerTimerImpl>::const_iterator i, e = mTimers.end();
    for( i = mTimers.begin(); i != e; ++i )
    {
        const ProfilerTimerImpl & t = i->second;
        rval += GN::strFormat(
            "    %s :\n"
            "        count(%d), sum(%s), ave(%s), min(%s), max(%s)\n"
            "\n",
            i->first.cptr(),
            t.count,
            sTime2Str( t.timesum ).cptr(),
            sTime2Str( t.timesum / t.count ).cptr(),
            sTime2Str( t.timemin ).cptr(),
            sTime2Str( t.timemax ).cptr() );
    }
    rval +=
        "=====================================================================\n"
        "\n";
}

