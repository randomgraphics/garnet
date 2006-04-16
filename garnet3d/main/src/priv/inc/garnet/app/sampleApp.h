#ifndef __GN_APP_SAMPLEAPP_H__
#define __GN_APP_SAMPLEAPP_H__
// *****************************************************************************
//! \file    app/sampleApp.h
//! \brief   Sample application framework
//! \author  chenlee (2006.1.7)
// *****************************************************************************

#include "garnet/GNgfx.h"
#include "garnet/GNinput.h"

namespace GN { namespace app
{
    //!
    //! Simple FPS counter
    //!
    class FpsCounter
    {
        GN::Clock mClock;
        float     mFpsValue;
        StrA      mFpsString;
        size_t    mFrameCounter;
        double    mLastFrameTime;
        double    mCurrentTime;
        bool      mBeforeFirstUpdate;

    public:

        //!
        //! Constructor
        //!
        FpsCounter() { reset(); }

        //!
        //! Get time
        //!
        double getCurrentTime() const { return mCurrentTime; }

        //!
        //! reset the counter
        //!
        void reset()
        {
            mFpsValue = 60.0f; // ensure non-zero FPS for the very first frame.
            mFpsString = "FPS: 0.00";
            mFrameCounter = 0;
            mCurrentTime = mClock.getTimeD();
            mLastFrameTime = mCurrentTime - 1.0f/60.0f;
            mBeforeFirstUpdate = true;
        }

        //!
        //! Update the counter
        //!
        void onFrame()
        {
            mCurrentTime = mClock.getTimeD();
            ++mFrameCounter;
            if( mCurrentTime - mLastFrameTime >= 1.0f )
            {
                mBeforeFirstUpdate = false;
                mFpsValue = (float)( mFrameCounter );
                mFpsString.format( "FPS: %.2f", mFpsValue );
                mLastFrameTime = mCurrentTime;
                mFrameCounter = 0;
            }
            else if( mBeforeFirstUpdate )
            {
                mFpsValue = (float)( (mCurrentTime - mLastFrameTime) / mFrameCounter );
                mFpsString.format( "FPS: %.2f", mFpsValue );
            }
        }

        //!
        //! Get FPS value
        //!
        float getFps() const { return mFpsValue; }

        //!
        //! Get FPS string
        //!
        const char * getFpsString() const { return mFpsString.cptr(); }
    };
    
    
    //!
    //! Sample application framework
    //!
    class SampleApp : public SlotBase
    {
        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        SampleApp() : mShowFps(true) { mFps.reset(); }
        virtual ~SampleApp() {}
        //@}

        // ********************************
        // sample framework
        // ********************************
    public:

        //@{

        //!
        //! application initialization structure
        //!
        struct InitParam
        {
            gfx::RendererAPI rapi;   //!< renderer API
            gfx::RendererOptions ro; //!< renderer options
            input::InputApi iapi;    //!< input API
        };

        static float UPDATE_INTERVAL; //!< Time interval for calling onUpdate()

        virtual int  run( int argc, const char * const argv[] );
        virtual bool onAppInit() { return true; }
        virtual void onAppQuit() {}
        virtual void onDetermineInitParam( InitParam & ) {}
        virtual bool onRendererInit() { return true; }
        virtual bool onRendererDeviceCreate() { return true; }
        virtual bool onRendererDeviceRestore() { return true; }
        virtual void onRendererDeviceDispose() {}
        virtual void onRendererDeviceDestroy() {}
        virtual void onRendererQuit() {}
        virtual void onUpdate() {}
        virtual void onRender() {}
        virtual void onKeyPress( input::KeyEvent );
        virtual void onCharPress( wchar_t ) {}
        virtual void onAxisMove( input::Axis, int ) {}

        //!
        //! Return seconds since application starts
        //!
        double getCurrentTime() const { return mFps.getCurrentTime(); }

        //!
        //! Return seconds of last frame
        //!
        double getLastFrameTime() const { return mLastFrameTime; }

        //!
        //! Return time since last update
        //!
        double getTimeSinceLastUpdate() const { return mTimeSinceLastUpdate; }

        void reloadGfxResources();

        void postExistEvent() { mDone = true; }

        bool switchRenderer();

        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        InitParam mInitParam;

        bool mDone;

        // time stuff
        bool mShowFps;
        FpsCounter mFps;
        double mLastFrameTime;
        double mTimeSinceLastUpdate;

        // ********************************
        // private functions
        // ********************************
    private:

        bool init( int argc, const char *  const argv[] );
        void quit();
        bool checkCmdLine( int argc, const char * const argv[] );
        bool initApp();
        void quitApp();
        bool initRenderer();
        void quitRenderer();
        bool recreateRenderer();
        bool initInput();
        void quitInput();
        void drawHUD();
    };
}}

// *****************************************************************************
//                           End of sampleApp.h
// *****************************************************************************
#endif // __GN_APP_SAMPLEAPP_H__
