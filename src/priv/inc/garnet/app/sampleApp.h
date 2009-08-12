#ifndef __GN_APP_SAMPLEAPP_H__
#define __GN_APP_SAMPLEAPP_H__
// *****************************************************************************
/// \file
/// \brief   Sample application framework
/// \author  chenlee (2006.1.7)
// *****************************************************************************

#include "garnet/GNgfx.h"
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
            gfx::GpuOptions ro;                     ///< renderer options
            bool                 useMultithreadGpu; ///< use multithread renderer or not.
            input::InputAPI      iapi;                   ///< input API
            util::FontFaceDesc   ffd;                    ///< default font face descriptor
        };

        static float UPDATE_INTERVAL; ///< Time interval for calling onUpdate(), in seconds.

        virtual int  run( int argc, const char * const argv[] );
        virtual bool onCheckExtraCmdlineArguments( int argc, const char * const argv[] ); // only non-standard/unknown arguments are passed to this function.
        virtual void onPrintHelpScreen( const char * executableName );
        virtual bool onPreInit( InitParam & ) { return true; }
        virtual bool onPostInit() = 0;
        virtual void onQuit() = 0;
        virtual void onUpdate() = 0;
        virtual void onRender() = 0;
        virtual void onRenderWindowResize( HandleType /*window*/, UInt32 /*width*/, UInt32 /*height*/ ) {}
        virtual void onKeyPress( input::KeyEvent );
        virtual void onCharPress( wchar_t ) {}
        virtual void onAxisMove( input::Axis, int ) {}

        ///
        /// Draw X/Y/Z coordinate axes onto screen
        ///
        void drawXYZCoordinateAxes( const Matrix44f & projViewWorld );

        ///
        /// print standard command line options to console screen
        ///
        void printStandardCommandLineOptions();

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
        /// post exit event. Application will exit at the beginning of next frame.
        ///
        void postExitEvent() { mDone = true; }

		///
		/// show HUD or not?
		///
		void showHUD( bool show ) { mShowHUD = show; }

        ///
        /// get the FPS
        ///
        float getFps() const { return mFps.getFps(); }

        ///
        /// get reference to renderer
        ///
        gfx::Gpu & getGpu() { GN_ASSERT(mGpu); return *mGpu; }

        ///
        /// get font renderer
        ///
        util::BitmapFont & getFont() { return mFont; }

        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        InitParam             mInitParam;

        gfx::Gpu       * mGpu;
        gfx::SpriteRenderer * mSpriteRenderer;
        gfx::LineRenderer   * mLineRenderer;
        util::BitmapFont      mFont;

        // time stuff
        util::FpsCalculator   mFps;
        double                mLastFrameTime;
        double                mTimeSinceLastUpdate;

        StrW                  mHelpText;
        bool                  mShowHUD;  ///< show simple HUD over game scene
        bool                  mShowHelp; ///< show detailed help text in HUD. No effect if mShowHUD is false.
        bool                  mDone;     ///< Set to true to quit the application.

        // ********************************
        // private functions
        // ********************************
    private:

        bool init( int argc, const char *  const argv[] );
        void quit();
        bool checkCmdLine( int argc, const char * const argv[] );
        bool initGpu();
        void quitGpu();
        bool recreateGpu();
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
