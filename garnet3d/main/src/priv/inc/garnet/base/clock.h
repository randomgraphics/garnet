#ifndef __GN_BASE_CLOCK_H__
#define __GN_BASE_CLOCK_H__
// *****************************************************************************
/// \file    clock.h
/// \brief   high resolution clock system.
/// \author  chenlee (2005.7.1)
// *****************************************************************************

namespace GN
{
    ///
    /// high resolution clock system
    ///
    class Clock
    {
        // ********************************
        /// name  ctor/dtor
        // ********************************

        //@{
    public:
        Clock()  { reset(); }
        ~Clock() {}
        //@}

        // ********************************
        //   public interface
        // ********************************
    public:

        ///
        /// Clock cycle type
        ///
        typedef SInt64 CycleType;

        ///
        /// 获得当前时间计数
        ///
        CycleType getCycleCount() const
        {
            return getCleanCycleCount();
        }

        ///
        /// 获得当前时间, in seconds
        ///
        double getTimeD() const
        {
            double c1 = static_cast<double>( getCleanCycleCount() );
            double c2 = static_cast<double>( mSystemCycleFrequency );
            return c1 / c2;
        }

        ///
        /// 获得当前时间, in seconds
        ///
        float getTimef() const
        {
            return (float)getTimeD();
        }

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

        CycleType mResetTime;    ///< 计时器复位时的cycle数
        CycleType mPauseTime;    ///< 计时器暂停时的cycle数
        CycleType mPauseElapsed; ///< 计时器总计暂停的cycle数
        bool      mPaused;       ///< 计时器是否暂停

        ///< 系统计时器的频率（每秒钟的cycle数）
        static CycleType mSystemCycleFrequency;

        // ********************************
        //   private functions
        // ********************************
    private:

        ///
        /// 得到从上次计时器复位到现在且去除暂停时间后所实际经过的净cycle数
        ///
        CycleType getCleanCycleCount() const
        {
            if (mPaused)
                return mPauseTime;
            else
                return getSystemCycleCount() - mResetTime - mPauseElapsed;
        }

        ///
        /// 一个高精度的计时函数
        ///
        /// 返回从开机到现在计时器经过的cycle数，
        /// 用返回值除以计时器的主频，就可以换算成秒。
        ///
        CycleType getSystemCycleCount() const;
    };
}


// *****************************************************************************
//                           End of clock.h
// *****************************************************************************
#endif // __GN_BASE_CLOCK_H__

