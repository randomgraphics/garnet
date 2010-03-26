#ifndef __GN_UTIL_SAMPLEAPP_H__
#define __GN_UTIL_SAMPLEAPP_H__
// *****************************************************************************
/// \file
/// \brief   Sample application framework
/// \author  chenlee (2006.1.7)
// *****************************************************************************

#include "garnet/GNgfx.h"
#include "garnet/GNinput.h"
#include "garnet/GNutil.h"

namespace GN { namespace util
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
            gfx::GpuOptions            ro;                ///< renderer options
            bool                       useMultithreadGpu; ///< use multithread renderer or not.
            input::InputAPI            iapi;              ///< input API
            util::FontFaceCreationDesc ffc;               ///< default font face creation descriptor
        };

        static float UPDATE_INTERVAL; ///< Time interval for calling OnUpdate(), in seconds.

        virtual int  Run( int argc, const char * const argv[] );
        virtual bool OnCheckExtraCmdlineArguments( int argc, const char * const argv[] ); // only non-standard/unknown arguments are passed to this function.
        virtual void OnPrintHelpScreen( const char * executableName );
        virtual bool OnPreInit( InitParam & ) { return true; }
        virtual bool OnInit() = 0;
        virtual void OnQuit() = 0;
        virtual void OnUpdate() = 0;
        virtual void OnRender() = 0;
        virtual void OnRenderWindowResize( HandleType /*window*/, UInt32 /*width*/, UInt32 /*height*/ ) {}
        virtual void OnKeyPress( input::KeyEvent );
        virtual void OnCharPress( wchar_t ) {}
        virtual void OnAxisMove( input::Axis, int ) {}

        ///
        /// Draw X/Y/Z coordinate axes onto screen
        ///
        void DrawXYZCoordinateAxes( const Matrix44f & projViewWorld );

        ///
        /// print standard command line options to console screen
        ///
        void PrintStandardCommandLineOptions();

        ///
        /// post exit event. Application will exit at the beginning of next frame.
        ///
        void PostExitEvent() { mDone = true; }

		///
		/// show HUD or not?
		///
		void ShowHUD( bool show ) { mShowHUD = show; }

        ///
        /// Return time in seconds since application starts
        ///
        double GetCurrentTime() const { return mFps.GetCurrentTime(); }

        ///
        /// Return time in seconds of the last frame
        ///
        double GetLastFrameTime() const { return mLastFrameTime; }

        ///
        /// Return time in seconds since last call to OnUpdate()
        ///
        double GetTimeSinceLastUpdate() const { return mTimeSinceLastUpdate; }

        ///
        /// get the FPS
        ///
        float GetFps() const { return mFps.GetFps(); }

        ///
        /// get reference to renderer
        ///
        gfx::Gpu & GetGpu() const { GN_ASSERT(mGpu); return *mGpu; }

        ///
        /// get reference to GPU resource database
        ///
        gfx::GpuResourceDatabase & GetGdb() const { GN_ASSERT(mGpuResourceDatabase); return *mGpuResourceDatabase; }

        ///
        /// get refernece to the world
        ///
        util::World & GetWorld() const { GN_ASSERT(mWorld); return *mWorld; }

        ///
        /// get sprite renderer
        ///
        gfx::SpriteRenderer & GetSpriteRenderer() const { GN_ASSERT( mSpriteRenderer ); return *mSpriteRenderer; }

        ///
        /// get font renderer
        ///
        util::BitmapFont & GetFont() { return mFont; }

        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        InitParam                  mInitParam;

        gfx::Gpu                 * mGpu;
        gfx::SpriteRenderer      * mSpriteRenderer;
        gfx::LineRenderer        * mLineRenderer;
        gfx::GpuResourceDatabase * mGpuResourceDatabase;
        util::World              * mWorld;
        util::BitmapFont           mFont;

        // time stuff
        util::FpsCalculator        mFps;
        double                     mLastFrameTime;
        double                     mTimeSinceLastUpdate;

        StrW                       mHelpText;
        bool                       mShowHUD;  ///< show simple HUD over game scene
        bool                       mShowHelp; ///< show detailed help text in HUD. No effect if mShowHUD is false.
        bool                       mDone;     ///< Set to true to quit the application.

        // ********************************
        // private functions
        // ********************************
    private:

        bool Init( int argc, const char *  const argv[] );
        void Quit();
        bool CheckCmdLine( int argc, const char * const argv[] );
        bool InitGpu();
        void QuitGpu();
        bool InitInput();
        void QuitInput();
        bool InitFont();
        void QuitFont();
        void DrawHUD();
    };
}}

// *****************************************************************************
//                                     EOF
// *****************************************************************************
#endif // __GN_UTIL_SAMPLEAPP_H__
