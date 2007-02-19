#ifndef __GN_APP_SAMPLEAPP_H__
#define __GN_APP_SAMPLEAPP_H__
// *****************************************************************************
/// \file    app/sampleApp.h
/// \brief   Sample application framework
/// \author  chenlee (2006.1.7)
// *****************************************************************************

#include "garnet/GNgfx.h"
#include "garnet/GNinput.h"

namespace GN { namespace app
{
    ///
    /// Simple FPS counter
    ///
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

        ///
        /// Constructor
        ///
        FpsCounter() { reset(); }

        ///
        /// Get time
        ///
        double getCurrentTime() const { return mCurrentTime; }

        ///
        /// reset the counter
        ///
        void reset()
        {
            mFpsValue = 60.0f; // ensure non-zero FPS for the very first frame.
            mFpsString = "FPS: 0.00";
            mFrameCounter = 0;
            mCurrentTime = mClock.getTimeD();
            mLastFrameTime = mCurrentTime - 1.0f/60.0f;
            mBeforeFirstUpdate = true;
        }

        ///
        /// Update the counter
        ///
        void onFrame()
        {
            mCurrentTime = mClock.getTimeD();
            ++mFrameCounter;
            double duration = mCurrentTime - mLastFrameTime;
            if( duration >= 1.0f )
            {
                mBeforeFirstUpdate = false;
                mFpsValue = (float)( mFrameCounter / duration );
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

        ///
        /// Get FPS value
        ///
        float getFps() const { return mFpsValue; }

        ///
        /// Get FPS string
        ///
        const char * getFpsString() const { return mFpsString.cptr(); }
    };

    ///
    /// Represent raw data block
    ///
    struct RawData : public NoCopy
    {
        ///
        /// get data size
        ///
        virtual size_t size() const = 0;

        ///
        /// get data pointer
        ///
        virtual void * data() const = 0;
    };

    ///
    /// Resource manager class used by sample application
    ///
    struct SampleResourceManager
    {
        ResourceManager<gfx::Shader*>     shaders;     ///< shader manager
        ResourceManager<gfx::Texture*>    textures;    ///< texture manager
        ResourceManager<gfx::Effect*>     effects;     ///< effect manager
        ResourceManager<gfx::Mesh*>       meshes;      ///< mesh manager
        ResourceManager<gfx::Renderable*> renderables; ///< renderable manager.
        ResourceManager<RawData*>         rawData;     ///< raw data manager

        ///
        /// return native resource file name. Return empty string if file does not exist.
        ///
        static GN::StrA sGetNativeResourceFileName( const StrA & name );

        ///
        /// Create texture from file
        ///
        static gfx::Texture * sCreateTextureFromFile( const StrA & );

        ///
        /// Create shader from file
        ///
        static gfx::Shader * sCreateShaderFromFile( gfx::ShaderType type, gfx::ShadingLanguage lang, const StrA & name, const StrA & hints="" );

        ///
        /// ctor
        ///
        SampleResourceManager();

        ///
        /// dtor
        ///
        ~SampleResourceManager();

        ///
        /// bind shader handles to renderer
        ///
        void bindShaderHandles( gfx::Renderer & r, UInt32 vs, UInt32 ps, UInt32 gs )
        {
            r.setShaders(
                vs?shaders.getResource(vs):0,
                ps?shaders.getResource(ps):0,
                gs?shaders.getResource(gs):0 );
        }

        ///
        /// bind texture handle to renderer
        ///
        void bindTextureHandle( gfx::Renderer & r, size_t stage, UInt32 tex )
        {
            r.setTexture( stage, tex?textures.getResource(tex):0 );
        }

        ///
        /// dispose all resources
        ///
        void disposeAll()
        {
            shaders.disposeAll();
            textures.disposeAll();
            effects.disposeAll();
            meshes.disposeAll();
            renderables.disposeAll();
            rawData.disposeAll();
        }

    private:
        void onRendererDispose();
        void onRendererDestroy();

        fs::FileSystem * mMediaFileSys;
    };

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
        virtual ~SampleApp() { quit(); }
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
            gfx::RendererAPI rapi;   ///< renderer API
            gfx::RendererOptions ro; ///< renderer options
            input::InputApi iapi;    ///< input API
        };

        static float UPDATE_INTERVAL; ///< Time interval for calling onUpdate(), in seconds.

        virtual int  run( int argc, const char * const argv[] );
        ///
        /// \note
        ///     - argv[0] is always application name
        ///     - only non-standard/unknown argument will be send to this function.
        ///
        virtual bool onCheckCmdLine( int argc, const char * const argv[] );
        virtual bool onAppInit() { return true; }
        virtual void onAppQuit() {}
        virtual void onDetermineInitParam( InitParam & ) {}
        virtual bool onRendererCreate() { return true; }
        virtual bool onRendererRestore() { return true; }
        virtual void onRendererDispose() {}
        virtual void onRendererDestroy() {}
        virtual void onUpdate() {}
        virtual void onRender() {}
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
        /// Force reloading of all resources
        ///
        void reloadResources() { mResMgr.disposeAll(); }

        ///
        /// post exit event. Application will exit at next frame.
        ///
        void postExitEvent() { mDone = true; }

        ///
        /// switch renderer API
        ///
        bool switchRenderer();

		///
		/// show HUD or not?
		///
		void showHUD( bool show ) { mShowHUD = show; }

        ///
        /// get resource managare instance
        ///
        SampleResourceManager & getResMgr() { return mResMgr; }

        ///
        /// get the FPS
        ///
        float getFps() const { return mFps.getFps(); }

        //@}

        // ********************************
        // private variables
        // ********************************
    private:

        InitParam mInitParam;

        SampleResourceManager mResMgr;

        // time stuff
        bool mShowHUD;
        FpsCounter mFps;
        double mLastFrameTime;
        double mTimeSinceLastUpdate;

        bool mDone; // exit flag

        // ********************************
        // private functions
        // ********************************
    private:

        bool init( int argc, const char *  const argv[] );
        void quit();
        bool initApp();
        void quitApp();
        bool checkCmdLine( int argc, const char * const argv[] );
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
