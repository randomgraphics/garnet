#include "pch.h"
#include "garnet/base/clock.h"
#if GN_POSIX
#include <pthread.h>
#include <sys/timeb.h>
#endif

// *****************************************************************************
//                        local functions
// *****************************************************************************

static GN::Logger * sLogger = GN::getLogger("GN.base.Clock");

///
/// 获得计数器的主频
// -----------------------------------------------------------------------------
static GN::Clock::CycleType sGetSystemCycleFrequency()
{
    GN_GUARD;

    GN::Clock::CycleType r;

#if GN_MSWIN
    if( !QueryPerformanceFrequency((LARGE_INTEGER*)&r) )
    {
        GN_TRACE(sLogger)( "Current system do NOT support high-res "
            "performance counter, use getTickCount()!" );
        r = 1000;  // getTickCount()返回的是毫秒
    }
#else
    r = 1000;
#endif

    // success
    return r;

    GN_UNGUARD;
}

// *****************************************************************************
//                      interface functions
// *****************************************************************************

GN::Clock::CycleType
GN::Clock::mSystemCycleFrequency = sGetSystemCycleFrequency();

//
// 计时器复位
// -----------------------------------------------------------------------------
void GN::Clock::reset()
{
    // reset timer variables
    mResetTime = getSystemCycleCount();
    mPaused = 0;
    mPauseTime = 0;
    mPauseElapsed = 0;
}

//
// 计时器暂停
// -----------------------------------------------------------------------------
void GN::Clock::pause()
{
    if (!mPaused)
    {
        GN_TRACE(sLogger)( "Timer pause!" );
        mPauseTime = getSystemCycleCount();
        mPaused = true;
    }
}

//
// 计时器恢复
// -----------------------------------------------------------------------------
void GN::Clock::resume()
{
    if (mPaused)
    {
        GN_TRACE(sLogger)( "Timer resume!" );
        mPauseElapsed += getSystemCycleCount() - mPauseTime;
        mPaused = false;
    }
}

// *****************************************************************************
//                      Private Functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
GN::Clock::CycleType GN::Clock::getSystemCycleCount() const
{
    GN_GUARD_SLOW;

#if GN_MSWIN
    CycleType r;
    if( 1000 != mSystemCycleFrequency )
    {
        QueryPerformanceCounter( (LARGE_INTEGER*)&r );
    }
    else
    {
        r = GetTickCount();
    }
    return r;
#elif GN_POSIX
    timeb tp;
    ftime (&tp);
    return CycleType(tp.time * 1000 + tp.millitm);
#else
#error Unknown platform!
#endif

    GN_UNGUARD_SLOW;
}
