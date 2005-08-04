#include "pch.h"
#include "garnet/base/clock.h"
#if GN_POSIX
#include <pthread.h>
#include <sys/timeb.h>
#endif

#define USE_RTDSC 0

// ****************************************************************************
//                        local functions
// ****************************************************************************

//!
//! 获得计数器的主频
// ----------------------------------------------------------------------------
static GN::Clock::CycleType sGetSystemCycleFrequency()
{
    GN_GUARD;

    GN::Clock::CycleType r;

#if USE_RTDSC

    // 获得CPU的频率
    //
    // FIXME: timer may warp in this second
    //
    GN_INFO( "Clock : Evaluating CPU Frequency ......" );

    GN::Clock::CycleType c1 = getSystemCycleCount();
#if GN_WIN32
    Sleep(100);
    GN::Clock::CycleType c2 = getSystemCycleCount();
    r = (c2 - c1) * 10;
#else
    sleep(1);
    GN::Clock::CycleType c2 = getSystemCycleCount();
    r = (c2 - c1);
#endif

    GN_INFO( "Clock : OK! Current CPU Frequency is %d MHz!",
        (uint32_t)(r / 1000000) ) );

#else // !USE_RTDSC

#if GN_WIN32
    if( !QueryPerformanceFrequency((LARGE_INTEGER*)&r) )
    {
        GN_INFO( "Current system do NOT support high-res "
            "performance counter, use getTickCount()!" );
        r = 1000;  // getTickCount()返回的是毫秒
    }
#else
    r = 1000;
#endif

#endif // USE_RTDSC

    // success
    return r;

    GN_UNGUARD;
}

// ****************************************************************************
//                      interface functions
// ****************************************************************************

GN::Clock::CycleType
GN::Clock::mSystemCycleFrequency = sGetSystemCycleFrequency();

//
// 计时器复位
// ----------------------------------------------------------------------------
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
// ----------------------------------------------------------------------------
void GN::Clock::pause()
{
    if (!mPaused)
    {
        GN_INFO( "Timer pause!" );
        mPauseTime = getSystemCycleCount();
        mPaused = true;
    }
}

//
// 计时器恢复
// ----------------------------------------------------------------------------
void GN::Clock::resume()
{
    if (mPaused)
    {
        GN_INFO( "Timer resume!" );
        mPauseElapsed += getSystemCycleCount() - mPauseTime;
        mPaused = false;
    }
}

// ****************************************************************************
//                      Private Functions
// ****************************************************************************

//
//
// ----------------------------------------------------------------------------
GN::Clock::CycleType GN::Clock::getSystemCycleCount() const
{
    GN_GUARD_SLOW;

#if USE_RTDSC

    // Use the assembly instruction rdtsc, which gets the current
    // cycle count (since the process started) and puts it in edx:eax.
#if GN_MSVC
    volatile union
    {
        int64_t i64;
        struct {
            int32_t lo32;
            int32_t hi32;
        };
    } t;
    __asm
    {
        rdtsc
        mov t.lo32, eax
        mov t.hi32, edx
    }
    return t.i64;
#elif GN_GNUC
    volatile int64_t r;
    __asm__ __volatile__ ("rdtsc" : "=A" (r) );
    return r;
#else
#error Unimplemented!
#endif

#else // !USE_RTDSC

#if GN_WIN32
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

#endif // USE_RTDSC

    GN_UNGUARD_SLOW;
}
