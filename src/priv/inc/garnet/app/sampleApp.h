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
            gfx::RendererOptions ro;                     ///< renderer options
            bool                 useMultithreadRenderer; ///< use multithread renderer or not.
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
        virtual void onKeyPress( input::KeyEvent );
        virtual void onCharPress( wchar_t ) {}
        virtual void onAxisMove( input::Axis, int ) {}

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
        gfx::Renderer & getRenderer() { GN_ASSERT(mRenderer); return *mRenderer; }

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

        gfx::Renderer       * mRenderer;
        gfx::SpriteRenderer * mSpriteRenderer;
        util::BitmapFont      mFont;

        // time stuff
        util::FpsCalculator   mFps;
        double                mLastFrameTime;
        double                mTimeSinceLastUpdate;

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
        bool initRenderer();
        void quitRenderer();
        bool recreateRenderer();
        bool initInput();
        void quitInput();
        bool initFont();
        void quitFont();
        void drawHUD();
    };

    ///
    /// Draw X/Y/Z coordinate axes onto screen
    ///
    inline void drawXYZCoordinateAxes(
        gfx::Renderer   & rndr,
        const Matrix44f & world,
        const Matrix44f & view,
        const Matrix44f & proj )
    {
        static const float X[] = { 0.0f, 0.0f, 0.0f, 10000.0f, 0.0f, 0.0f };
        static const float Y[] = { 0.0f, 0.0f, 0.0f, 0.0f, 10000.0f, 0.0f };
        static const float Z[] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 10000.0f };

        rndr.drawLines( 0, X, 3*sizeof(float), 2, GN_RGBA32(255,0,0,255), world, view, proj );
        rndr.drawLines( 0, Y, 3*sizeof(float), 2, GN_RGBA32(0,255,0,255), world, view, proj );
        rndr.drawLines( 0, Z, 3*sizeof(float), 2, GN_RGBA32(0,0,255,255), world, view, proj );
    }
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_APP_SAMPLEAPP_H__
