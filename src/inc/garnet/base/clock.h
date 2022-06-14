#ifndef __GN_BASE_CLOCK_H__
#define __GN_BASE_CLOCK_H__
// *****************************************************************************
/// \file
/// \brief   high resolution clock system.
/// \author  chenlee (2005.7.1)
// *****************************************************************************

#include <chrono>

namespace GN {
///
/// high resolution clock system
///
class GN_API Clock {
    // ********************************
    /// name  ctor/dtor
    // ********************************

    //@{
public:
    Clock() { reset(); }
    ~Clock() {}
    //@}

    // ********************************
    //   public interface
    // ********************************
public:
    using SystemClock = std::chrono::high_resolution_clock;
    using TimePoint   = std::chrono::high_resolution_clock::time_point;
    using Duration    = std::chrono::high_resolution_clock::duration;

    ///
    /// 返回从上次reset到当前的时间，刨除暂停的部分
    ///
    Duration now() { return SystemClock::now() - mResetTime - mPauseElapsed; }

    ///
    /// return time in seconds
    ///
    double seconds() { return (double) std::chrono::duration_cast<std::chrono::milliseconds>(now()).count() / 1000.0; }

    ///
    /// 重置时钟到初始状态
    ///
    void reset();

    ///
    /// 恢复时钟运行
    ///
    void resume();

    ///
    /// 暂停时钟
    ///
    void pause();

    ///
    /// 当前时钟是否暂停
    ///
    bool paused() const { return mPaused; }

    // ********************************
    //   private variables
    // ********************************
private:
    TimePoint mResetTime;    ///< 计时器复位时的cycle数
    TimePoint mPauseTime;    ///< 计时器暂停时的cycle数
    Duration  mPauseElapsed; ///< 计时器总计暂停的cycle数
    bool      mPaused;       ///< 计时器是否暂停
};

} // namespace GN

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_CLOCK_H__
