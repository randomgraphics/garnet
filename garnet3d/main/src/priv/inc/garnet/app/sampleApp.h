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

    public:

        //!
        //! reset the counter
        //!
        void reset()
        {
            mFpsValue = .0f;
            mFpsString = "FPS: 0.00";
            mFrameCounter = 0;
            mLastFrameTime = mClock.getTimeD();
        }

        //!
        //! Update the counter
        //!
        void onFrame()
        {
            double currentTime = mClock.getTimeD();
            ++mFrameCounter;
            if( currentTime - mLastFrameTime >= 0.5 )
            {
                mFpsValue = (float)( mFrameCounter/(currentTime - mLastFrameTime) );
                mFpsString.format( "FPS: %.2f", mFpsValue );
                mLastFrameTime = currentTime;
                mFrameCounter = 0;
            }
        }

        //!
        //! Get FPS value
        //!
        float fpsValue() const { return mFpsValue; }

        //!
        //! Get FPS string
        //!
        const char * fpsString() const { return mFpsString.cstr(); }
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

        virtual int  run( int argc, const char * const argv[] );
        virtual bool onAppInit() { return true; }
        virtual void onAppQuit() {}
        virtual bool onRendererCreate() { return true; }
        virtual bool onRendererRestore() { return true; }
        virtual void onRendererDispose() {}
        virtual void onRendererDestroy() {}
        virtual void onUpdate() {}
        virtual void onRender() {}
        virtual void onKeyPress( input::KeyEvent );
        virtual void onCharPress( wchar_t ) {}
        virtual void onAxisMove( input::Axis, int ) {}

        void reloadGfxResources();

        void postExistEvent() { mDone = true; }

        bool switchRenderer();

        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        struct InitParam
        {
            gfx::RendererAPI rapi;
            gfx::RendererOptions ro;
            input::InputApi iapi;
        };

        InitParam mInitParam;

        bool mDone;

        // fps stuff
        bool mShowFps;
        FpsCounter mFps;

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
