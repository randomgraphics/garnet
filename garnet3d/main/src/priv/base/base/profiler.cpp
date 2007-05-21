#include "pch.h"
#include "garnet/base/profiler.h"


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
            "        count(%d), sum(%f), ave(%f), min(%f), max(%f)\n"
            "\n",
            i->first.cptr(),
            t.count,
            t.timesum,
            t.timesum / t.count,
            t.timemin,
            t.timemax );
    }
    rval +=
        "=====================================================================\n"
        "\n";
}

