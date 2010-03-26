#ifndef __GN_BASE_CLOCK_H__
#define __GN_BASE_CLOCK_H__
// *****************************************************************************
/// \file
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
        Clock()  { Reset(); }
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
        /// 一个高精度的计时函数
        ///
        /// 返回从开机到现在计时器经过的cycle数，
        /// 用返回值除以计时器的主频，就可以换算成秒。
        ///
        static CycleType sGetSystemCycleCount();

        ///
        /// 获得当前时间计数
        ///
        CycleType GetCycleCount() const
        {
            return GetCleanCycleCount();
        }

        ///
        /// 获得当前时间, in seconds
        ///
        double GetTimeD() const
        {
            double c1 = static_cast<double>( GetCleanCycleCount() );
            double c2 = static_cast<double>( mSystemCycleFrequency );
            return c1 / c2;
        }

        ///
        /// 获得当前时间, in seconds
        ///
        float GetTimef() const
        {
            return (float)GetTimeD();
        }

        ///
        /// 重置时钟到初始状态
        ///
        void Reset();

        ///
        /// 恢复时钟运行
        ///
        void Resume();

        ///
        /// 暂停时钟
        ///
        void Pause();

        ///
        /// 当前时钟是否暂停
        ///
        bool Paused() const { return mPaused; }

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
        CycleType GetCleanCycleCount() const
        {
            if (mPaused)
                return mPauseTime;
            else
                return sGetSystemCycleCount() - mResetTime - mPauseElapsed;
        }
    };
}


// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_BASE_CLOCK_H__

