#include "pch.h"
#include "garnet/GNapp.h"

float GN::app::SampleApp::UPDATE_INTERVAL = 1.0f/60.0f;

// *****************************************************************************
// public functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
int GN::app::SampleApp::run( int argc, const char * const argv[] )
{
    GN_GUARD_ALWAYS;

    if( !init( argc, argv ) ) return -1;

    mDone = false;

    double elapsedUpdateTime;
    double lastUpdateTime = mFps.getCurrentTime();

    while( !mDone )
    {
        // process user input
        GN::win::processWindowMessages( gRenderer.getDispDesc().windowHandle, true );
        gInput.processInputEvents();

        // update time stuff
        mFps.onFrame();

        // call update in fixed interval
        elapsedUpdateTime = mFps.getCurrentTime() - lastUpdateTime;
        if( elapsedUpdateTime > UPDATE_INTERVAL )
        {
            int count = (int)( elapsedUpdateTime / UPDATE_INTERVAL );
            for( int i = 0; i < count; ++i ) onUpdate();
            lastUpdateTime += UPDATE_INTERVAL * count;
        }

        // do render
        if( gRenderer.drawBegin() )
        {
            mLastFrameTime = 1.0 / mFps.getFps();
            mTimeSinceLastUpdate = mFps.getCurrentTime() - lastUpdateTime;
            onRender();
            drawHUD();
            gRenderer.drawEnd();
        }
    }

    // success
    quit();
    return 0;

    // failed
    GN_UNGUARD_ALWAYS_NO_THROW;
    quit();
    return -1;
}

//
//
// -----------------------------------------------------------------------------
void GN::app::SampleApp::onKeyPress( input::KeyEvent ke )
{
    if( input::KEY_ESCAPE == ke.code && !ke.status.down ) mDone = true;
    if( input::KEY_R == ke.code && !ke.status.down ) reloadGfxResources();
}

//
//
// -----------------------------------------------------------------------------
void GN::app::SampleApp::reloadGfxResources()
{
    GN_GUARD;

    gTexDict.disposeAll();
    gShaderDict.disposeAll();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::app::SampleApp::switchRenderer()
{
    GN_GUARD;

    mInitParam.rapi = (gfx::RendererAPI)((mInitParam.rapi+1)%gfx::NUM_RENDERER_API);
    return recreateRenderer();

    GN_UNGUARD;
}

// *****************************************************************************
// private functions
// *****************************************************************************

//
//
// -----------------------------------------------------------------------------
bool GN::app::SampleApp::init( int argc, const char * const argv[] )
{
    GN_GUARD_ALWAYS;

    if( !checkCmdLine(argc,argv) ) { quit(); return false; }
    if( !initApp() ) { quit(); return false; }
    if( !initRenderer() ) { quit(); return false; }
    if( !initInput() ) { quit(); return false; }

    // success
    return true;

    // failed
    GN_UNGUARD_ALWAYS_NO_THROW;
    quit(); return false;
}

//
//
// -----------------------------------------------------------------------------
void GN::app::SampleApp::quit()
{
    GN_GUARD_ALWAYS;

    quitRenderer();
    quitInput();
    quitApp();

    GN_UNGUARD_ALWAYS_NO_THROW;
}

//
//
// -----------------------------------------------------------------------------
bool GN::app::SampleApp::checkCmdLine( int argc, const char * const argv[] )
{
    GN_GUARD;

#if GN_XENON
    GN_UNUSED_PARAM( argc );
    GN_UNUSED_PARAM( argv );
    mInitParam.rapi = gfx::API_D3D;
    mInitParam.ro = gfx::RendererOptions();
    mInitParam.iapi = input::API_NATIVE;
#else
    // setup default parameters
    mInitParam.rapi = gfx::API_AUTO;
    mInitParam.ro = gfx::RendererOptions();
    mInitParam.ro.windowedWidth = 800; // default is 800x600
    mInitParam.ro.windowedHeight = 600;
    mInitParam.iapi = input::API_NATIVE;

    for( int i = 1; i < argc; ++i )
    {
        const char * a = argv[i];
        if( 0 == strCmpI( a, "-?" ) || 0 == strCmpI( a, "-h" ) )
        {
            GN_INFO( "\n"
                "Usage : %s [options]\n\n"
                "Options : (options are case-insensitive)\n"
                "    -h, -?             : Show help screen.\n"
                "    -ref               : Use reference device.\n"
                "    -d3d, -ogl, -fake  : Select rendering API.\n"
                "    -msaa              : Enable MSAA/FSAA.\n",
                GN::path::baseName(argv[0]).cstr() );
            return false;
        }
        else if( 0 == strCmpI( a, "-msaa") ) mInitParam.ro.msaa = GN::gfx::MSAA_ULTRA;
        else if( 0 == strCmpI( a, "-ref" ) ) mInitParam.ro.reference = true;
        else if( 0 == strCmpI( a, "-d3d" ) ) mInitParam.rapi = gfx::API_D3D;
        else if( 0 == strCmpI( a, "-ogl" ) ) mInitParam.rapi = gfx::API_OGL;
        else if( 0 == strCmpI( a, "-fake" ) ) mInitParam.rapi = gfx::API_FAKE;
    }
#endif

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::app::SampleApp::initApp()
{
    GN_GUARD;

    if( !onAppInit() ) return false;

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::app::SampleApp::quitApp()
{
    GN_GUARD;

    onAppQuit();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::app::SampleApp::initRenderer()
{
    GN_GUARD;

    // connect to renderer signals
    GN::gfx::Renderer::sSigDeviceCreate.connect( this, &SampleApp::onRendererCreate );
    GN::gfx::Renderer::sSigDeviceRestore.connect( this, &SampleApp::onRendererRestore );
    GN::gfx::Renderer::sSigDeviceDispose.connect( this, &SampleApp::onRendererDispose );
    GN::gfx::Renderer::sSigDeviceDestroy.connect( this, &SampleApp::onRendererDestroy );

    // create renderer
    return recreateRenderer();

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::app::SampleApp::quitRenderer()
{
    GN_GUARD;

    // delete Renderer
    GN::gfx::deleteRenderer();

    // disconnect to renderer signals
    GN::gfx::Renderer::sSigDeviceDestroy.disconnect( this );
    GN::gfx::Renderer::sSigDeviceDispose.disconnect( this );
    GN::gfx::Renderer::sSigDeviceRestore.disconnect( this );
    GN::gfx::Renderer::sSigDeviceCreate.disconnect( this );

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::app::SampleApp::recreateRenderer()
{
    GN_GUARD;

    // (re)create renderer
    GN::gfx::Renderer * r = gfx::createRenderer( mInitParam.ro, mInitParam.rapi );
    if( NULL == r ) return false;

    // reattach input window
    const GN::gfx::DispDesc & dd = r->getDispDesc();
    if( gInputPtr && !gInput.attachToWindow( dd.displayHandle,dd.windowHandle ) )
    {
        return false;
    }

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
bool GN::app::SampleApp::initInput()
{
    GN_GUARD;

    // release old input system
    quitInput();

    // create INPUT system
    GN::input::Input * input = GN::input::createInputSystem( mInitParam.iapi );
    if( 0 == input ) return false;

    if( gRendererPtr )
    {
        const GN::gfx::DispDesc & dd = gRenderer.getDispDesc();
        if( !input->attachToWindow(dd.displayHandle,dd.windowHandle) ) return false;
    }

    // connect to input signals
    input->sigKeyPress.connect( this, &SampleApp::onKeyPress );
    input->sigCharPress.connect( this, &SampleApp::onCharPress );
    input->sigAxisMove.connect( this, &SampleApp::onAxisMove );

    // success
    return true;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::app::SampleApp::quitInput()
{
    GN_GUARD;

    if( gInputPtr ) delete gInputPtr;

    GN_UNGUARD;
}

//
//
// -----------------------------------------------------------------------------
void GN::app::SampleApp::drawHUD()
{
    GN_GUARD_SLOW;

    if( mShowFps ) gRenderer.drawDebugTextA( mFps.getFpsString(), 0, 0 );

    GN_UNGUARD_SLOW;
}
