#include "pch.h"
#include "garnet/base/profiler.h"

//
//
// -----------------------------------------------------------------------------
void GN::ProfilerManager::toString( GN::StrA & rval ) const
{
    if( mTimers.empty() ) { rval = ""; return; }

    rval =
        "\n"
        "=======================================\n"
        "            profile result\n"
        "---------------------------------------\n";
    std::map<const char *,TimerDesc>::const_iterator i, e = mTimers.end();
    for( i = mTimers.begin(); i != e; ++i )
    {
        const TimerDesc & t = i->second;
        rval += GN::strFormat(
            "    %s : count(%.0f), sum(%.0f), ave(%.0f), min(%.0f), max(%.0f)\n",
            i->first,
            t.count,
            t.timesum,
            t.timesum / t.count,
            t.timemin,
            t.timemax );
    }
    rval +=
        "=======================================\n"
        "\n";
}

