#ifndef __GN_BASE_CLOCK_H__
#define __GN_BASE_CLOCK_H__
// *****************************************************************************
//! \file    clock.h
//! \brief   high resolution clock system.
//! \author  chenlee (2005.7.1)
// *****************************************************************************

namespace GN
{
    //!
    //! high resolution clock system
    //!
    class Clock : public StdClass
    {
         GN_DECLARE_STDCLASS( Clock, StdClass );

        // ********************************
        //! name  ctor/dtor
        // ********************************

        //@{
    public:
        Clock()          { clear(); }
        virtual ~Clock() { quit(); }
        //@}

        // ********************************
        //! name standard init/quit
        // ********************************

        //@{
    public:
        bool init();
        void quit() { GN_STDCLASS_QUIT(); }
        bool ok() const { return MyParent::ok(); }
    private:
        void clear() {}
        //@}

        // ********************************
        //   public functions
        // ********************************
    public:

        //!
        //! 获得当前时间
        //!
        float getTime() const
        {
            double c1 = static_cast<double>( getCleanCycleCount() );
            double c2 = static_cast<double>( mSystemCycleFrequency );
            return static_cast<float>( c1 / c2 );
        }

        //!
        //! 重置时钟到初始状态
        //!
        void reset();

        //!
        //! 恢复时钟运行
        //!
        void resume();

        //!
        //! 暂停时钟
        //!
        void pause();

        //!
        //! 当前时钟是否暂停
        //!
        bool paused() const { return mPaused; }

        // ********************************
        //   private variables
        // ********************************
    private:

        typedef int64_t CycleType;

        CycleType mSystemCycleFrequency; //!< 系统计时器的频率（每秒钟的cycle数）
        CycleType mResetTime;            //!< 计时器复位时的cycle数
        CycleType mPauseTime;            //!< 计时器暂停时的cycle数
        CycleType mPauseElapsed;         //!< 计时器总计暂停的cycle数
        bool      mPaused;               //!< 计时器是否暂停

        // ********************************
        //   private functions
        // ********************************
    private:

        //!
        //! 得到从上次计时器复位到现在且去除暂停时间后所实际经过的净cycle数
        //!
        CycleType getCleanCycleCount() const
        {
            if (mPaused)
                return mPauseTime;
            else
                return getSystemCycleCount() - mResetTime - mPauseElapsed;
        }

        //!
        //! 一个高精度的计时函数
        //!
        //! 返回从开机到现在计时器经过的cycle数，
        //! 用返回值除以计时器的主频，就可以换算成秒。
        //!
        CycleType getSystemCycleCount() const;

        //!
        //! 获得计数器的主频
        //!
        CycleType getSystemCycleFrequency() const;
    };
}


// *****************************************************************************
//                           End of clock.h
// *****************************************************************************
#endif // __GN_BASE_CLOCK_H__

