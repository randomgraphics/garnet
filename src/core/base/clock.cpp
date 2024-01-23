#include "pch.h"
#include "garnet/base/clock.h"
#if GN_POSIX
    #include <pthread.h>
#endif

// *****************************************************************************
//                        local functions
// *****************************************************************************

static GN::Logger * sLogger = GN::getLogger("GN.base.Clock");

// *****************************************************************************
//                      interface functions
// *****************************************************************************

//
// 计时器复位
// -----------------------------------------------------------------------------
GN_API void GN::Clock::reset() {
    // reset timer variables
    mResetTime    = SystemClock::now();
    mPaused       = false;
    mPauseElapsed = Duration {0};
}

//
// 计时器暂停
// -----------------------------------------------------------------------------
GN_API void GN::Clock::pause() {
    if (!mPaused) {
        GN_TRACE(sLogger)("Timer pause!");
        mPauseTime = SystemClock::now();
        mPaused    = true;
    }
}

//
// 计时器恢复
// -----------------------------------------------------------------------------
GN_API void GN::Clock::resume() {
    if (mPaused) {
        GN_TRACE(sLogger)("Timer resume!");
        mPauseElapsed += SystemClock::now() - mPauseTime;
        mPaused = false;
    }
}
