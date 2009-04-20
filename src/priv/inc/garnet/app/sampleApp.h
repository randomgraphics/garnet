#ifndef __GN_APP_SAMPLEAPP_H__
#define __GN_APP_SAMPLEAPP_H__
// *****************************************************************************
/// \file
/// \brief   Sample application framework
/// \author  chenlee (2006.1.7)
// *****************************************************************************

#include "garnet/GNengine.h"
#include "garnet/GNscene.h"
#include "garnet/GNinput.h"
#include "garnet/GNutil.h"

namespace GN { namespace app
{
    ///
    /// Sample application framework
    ///
    class SampleApp : public SlotBase
    {
        // ********************************
        // ctor/dtor
        // ********************************

        //@{
    public:
        SampleApp();
        virtual ~SampleApp() {}
        //@}

        // ********************************
        // sample framework
        // ********************************
    public:

        //@{

        ///
        /// application initialization structure
        ///
        struct InitParam
        {
            gfx::GraphicsSystemCreationParameter gscp; ///< graphics system creation parameters
            engine::RenderEngineInitParameters  reip; ///< render engine initialization parameters
            input::InputAPI                      iapi; ///< input API
            scene::FontFaceDesc                  ffd;  ///< default font face descriptor
        };

        static float UPDATE_INTERVAL; ///< Time interval for calling onUpdate(), in seconds.

        virtual int  run( int argc, const char * const argv[] );
        ///
        /// \note
        ///     - argv[0] is always application name
        ///     - only non-standard/unknown argument will be send to this function.
        ///
        virtual bool onCheckCmdLine( int argc, const char * const argv[] );
        virtual void onDetermineInitParam( InitParam & ) {}
        virtual bool onInit() = 0;
        virtual void onQuit() = 0;
        virtual void onUpdate() = 0;
        virtual void onRender() = 0;
        virtual void onKeyPress( input::KeyEvent );
        virtual void onCharPress( wchar_t ) {}
        virtual void onAxisMove( input::Axis, int ) {}

        ///
        /// Return seconds since application starts
        ///
        double getCurrentTime() const { return mFps.getCurrentTime(); }

        ///
        /// Return seconds of last frame
        ///
        double getLastFrameTime() const { return mLastFrameTime; }

        ///
        /// Return time since last update
        ///
        double getTimeSinceLastUpdate() const { return mTimeSinceLastUpdate; }

        ///
        /// post exit event. Application will exit at next frame.
        ///
        void postExitEvent() { mDone = true; }

        ///
        /// switch renderer API
        ///
        bool switchGraphicsAPI( const FOURCC & newapi );

		///
		/// show HUD or not?
		///
		void showHUD( bool show ) { mShowHUD = show; }

        ///
        /// get the FPS
        ///
        float getFps() const { return mFps.getFps(); }

        ///
        /// as is
        ///
        engine::RenderEngine & getRenderEngine() { return mRenderEngine; }

        ///
        /// get font renderer
        ///
        scene::BitmapFont & getFont() { return mFont; }

        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        InitParam             mInitParam;

        engine::RenderEngine  mRenderEngine;
        scene::BitmapFont     mFont;

        // time stuff
        util::FpsCalculator   mFps;
        double                mLastFrameTime;
        double                mTimeSinceLastUpdate;

        bool                  mShowHUD;
        bool                  mShowHelp;
        bool                  mDone; // exit flag

        // ********************************
        // private functions
        // ********************************
    private:

        bool init( int argc, const char *  const argv[] );
        void quit();
        bool checkCmdLine( int argc, const char * const argv[] );
        bool initRenderer();
        void quitRenderer();
        bool recreateRenderer();
        bool initInput();
        void quitInput();
        bool initFont();
        void quitFont();
        void drawHUD();
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_APP_SAMPLEAPP_H__
