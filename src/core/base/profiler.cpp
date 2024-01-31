#include "pch.h"
#include "garnet/base/profiler.h"

static std::string sTime2Str(double time) {
    using namespace GN;

    if (time < 0.000001) {
        return fmt::format("%fus", time * 1000000);
    } else if (time < 0.001) {
        return fmt::format("%fms", time * 1000);
    } else {
        return fmt::format("%fs", time);
    }
}

// *****************************************************************************
// Profile Timer
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN_API GN::ProfileTimer::ProfileTimer(Clock & c): clock(c), count(0), timesum(0), timemin(DBL_MAX), timemax(0), timestart(0), impl(NULL) {}

//
//
// -----------------------------------------------------------------------------
GN_API GN::ProfileTimer::~ProfileTimer() {}

//
//
// -----------------------------------------------------------------------------
GN_API void GN::ProfileTimer::start() { timestart = clock.seconds(); }

//
//
// -----------------------------------------------------------------------------
GN_API void GN::ProfileTimer::stop() {
    double t = clock.seconds() - timestart;
    if (t < timemin) timemin = t;
    if (t > timemax) timemax = t;
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
GN_API GN::ProfilerManager::~ProfilerManager() {
#if GN_BUILD_PROFILING_ENABLED
    // print profile result
    std::string s;
    toString(s);
    printf("%s\n", s.data());
#endif
}

//
//
// -----------------------------------------------------------------------------
GN_API void GN::ProfilerManager::toString(std::string & rval) const {
    std::lock_guard<SpinLoop> lock(mMutex);

    if (mTimers.empty()) {
        rval = "";
        return;
    }

    rval = "\n"
           "=====================================================================\n"
           "                         profile result\n"
           "---------------------------------------------------------------------\n"
           "\n";
    const StringMap<char, ProfilerTimerImpl>::KeyValuePair * i;
    for (i = mTimers.first(); i != NULL; i = mTimers.next(i)) {
        const ProfilerTimerImpl & t = i->value;
        rval += GN::fmt::format("    %s :\n"
                                "        count(%d), sum(%s), ave(%s), min(%s), max(%s)\n"
                                "\n",
                                i->key, t.count, sTime2Str(t.timesum).data(), sTime2Str(0 == t.count ? 0 : (t.timesum / t.count)).data(),
                                sTime2Str(0 == t.count ? 0 : t.timemin).data(), sTime2Str(0 == t.count ? 0 : t.timemax).data());
    }
    rval += "=====================================================================\n"
            "\n";
}
