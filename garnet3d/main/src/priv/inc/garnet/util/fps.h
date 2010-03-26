#ifndef __GN_UTIL_FPS_H__
#define __GN_UTIL_FPS_H__
// *****************************************************************************
/// \file
/// \brief   FPS calculator
/// \author  chenli@@REDMOND (2009.1.6)
// *****************************************************************************

namespace GN { namespace util
{
    ///
    /// Simple FPS calculator
    ///
    class FpsCalculator
    {
        Clock     mClock;
        float     mFpsValue;
        StrW      mFormatString;
        StrW      mFpsString;
        size_t    mFrameCounter;
        double    mLastCheckPoint;
        double    mLastFrameElapsed;
        double    mLastFrameTime;
        double    mCurrentTime;
        bool      mBeforeFirstUpdate;

    public:

        ///
        /// Constructor
        ///
        FpsCalculator( const wchar_t * format = L"FPS: %.2f" ) : mFormatString(format) { Reset(); }

        ///
        /// Get time
        ///
        double getCurrentTime() const { return mCurrentTime; }

        ///
        /// reset the counter
        ///
        void Reset()
        {
            mCurrentTime = mClock.GetTimeD();
            mFpsValue = 60.0f; // ensure non-zero FPS for the very first frame.
            mFpsString.Format( mFormatString.ToRawPtr(), 0 );
            mFrameCounter = 0;
            mLastFrameElapsed = 1.0f / mFpsValue;
            mLastFrameTime = mCurrentTime - mLastFrameElapsed;
            mLastCheckPoint = mLastFrameTime - 1.0f / mFpsValue;
            mBeforeFirstUpdate = true;
        }

        ///
        /// Update the counter
        ///
        void onFrame()
        {
            mCurrentTime = mClock.GetTimeD();

            ++mFrameCounter;

            mLastFrameElapsed = mCurrentTime - mLastFrameTime;
            mLastFrameTime = mCurrentTime;

            double timeSinceLastCheckPoint = mCurrentTime - mLastCheckPoint;
            if( timeSinceLastCheckPoint >= 1.0f )
            {
                mBeforeFirstUpdate = false;
                mFpsValue = (float)( mFrameCounter / timeSinceLastCheckPoint );
                mFpsString.Format( mFormatString.ToRawPtr(), mFpsValue );
                mLastCheckPoint = mCurrentTime;
                mFrameCounter = 0;
                static Logger * sLogger = GetLogger("GN.util.fps");
                GN_VERBOSE(sLogger)( mFpsString.ToRawPtr() );
            }
            else if( mBeforeFirstUpdate )
            {
                mFpsValue = (float)( (mCurrentTime - mLastCheckPoint) / mFrameCounter );
                mFpsString.Format( mFormatString.ToRawPtr(), mFpsValue );
            }
        }

        ///
        /// Get elapsed time of last frame
        ///
        double getLastFrameElasped() const { return mLastFrameElapsed; }

        ///
        /// Get average FPS value
        ///
        float getFps() const { return mFpsValue; }

        ///
        /// Get FPS string
        ///
        const StrW & getFpsString() const { return mFpsString; }
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_UTIL_FPS_H__
